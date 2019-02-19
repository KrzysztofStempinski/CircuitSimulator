/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Component.cpp
// 
//  ---------------------------------------------

#include <iomanip>
#include <string>

#include "Component.h"

// TODO error handling!
void Component::_loadComponentStamps() {

	_stamps.clear();

	SimpleParser::Interpreter _interpreter;
	_interpreter.varTable = &_varTableIndices;

	if (!_componentData.HasMember("DCOP")) {
		logWindow->log("component " + _name + " has no DCOP entry in its definition file.", LogEntryType::Error);
		return;
	}

	// NOTE the value of "ignore" is always assumed to be true 
	if (_componentData["DCOP"].HasMember("ignore"))
		return;
	
	// iterate through element stamps and evaluate/apply each of them
	const rapidjson::Value& stamps = _componentData["DCOP"]["componentStamps"];
	for (rapidjson::Value::ConstValueIterator it = stamps.Begin(); it != stamps.End(); ++it) {

		ComponentStamp newStamp;
		
		std::string matrixString = (*it)["matrix"].GetString();
		
		if (matrixString == "A") {

			newStamp.matrix = MNAmatrix::A;

			std::string columnEntry = (*it)["column"].GetString(); // currentIndex or rowIndex

			//if (!parserIndices.compile()) {
			//	logWindow->exprTkError(parserIndices);
			//	return;
			//}

			// we need to substract 1 from both row/column number, as voltageIndex is numbered from 1 to n (and C++ uses, well, 0 to n - 1)
			// (and each matrix coord is either voltageIndex, or voltageCount + currentIndex)
			newStamp.columnNumber = _interpreter.evaluate(columnEntry) - 1;

			// we may as well discard the stamp right now, not going to apply it anyway
			if (newStamp.columnNumber < 0) 
				continue;

		}
		
		if (matrixString == "B") 
			newStamp.matrix = MNAmatrix::B;

		std::string rowEntryString = (*it)["row"].GetString();

	//	if (!parserIndices.compile(rowEntryString, expressionIndices)) {
		//	logWindow->exprTkError(parserIndices);
			//return;
		//}
		
		newStamp.rowNumber = _interpreter.evaluate(rowEntryString) - 1;

		if (newStamp.rowNumber < 0) 
			continue;

		// now, compute the value of the entry
		exprtk::parser<double> parserValue;
		exprtk::expression<double> expressionValue;
		expressionValue.register_symbol_table(_symbolTableProperties);

		std::string valueString = (*it)["value"].GetString();

		if (!parserValue.compile(valueString, expressionValue)) {
			throw "exprtk error placeholder!"; // TODO error messages
			//logWindow->exprTkError(parserValue);
			return;
		}

		newStamp.value = expressionValue.value();
			
		 //Cave Johnson, we're done here
		_stamps.push_back(newStamp);

	}
	
}

void Component::_loadSimulationVariables(int voltageCount) {

	// indices
	_varTableIndices.clear();

	for (int i = 0; i < coupledNodes.size(); ++i)
		_varTableIndices.addVariable("this.node" + std::to_string(i) + ".voltageIndex", coupledNodes[i]->voltageIndex);

	_varTableIndices.addVariable("voltageCount", voltageCount);
	_varTableIndices.addVariable("this.currentIndex", currentIndex);

	// properties
	_symbolTableProperties.clear();

	for (auto& it : properties)
		_symbolTableProperties.add_variable("this.properties." + it.first.toStdString(), it.second.value, true);

}

void Component::prepareForSimulation(int voltageCount) {

	_loadSimulationVariables(voltageCount);
	_loadComponentStamps();

}

bool Component::requireCurrentEntry() {
	// NOTE if calculateCurrent is present should always be set to true
	return _componentData["DCOP"].HasMember("calculateCurrent"); 
}

QString Component::getName() {
	return _name;
}

QString Component::getDisplayNameBase() {
	return _componentData["displayNameBase"].GetString();
}

QString Component::getLetterIdentifierBase() {
	return _componentData["letterIdentifierBase"].GetString();
}

void Component::applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount, LogWindow* logWindow){

	for (auto &it : _stamps) {
		if (it.matrix == MNAmatrix::A) 
			matrixA(it.rowNumber, it.columnNumber) += it.value;
		else
			matrixB(it.rowNumber) += it.value;
	}

}

int Component::getNumberOfNodes() {

	const rapidjson::Value& nodes = _componentData["nodes"];

	return  nodes.Size();

}

Component::Component(QString name) {

	_name = name;
	_rotationAngle = 0;
	isSelected = false;
	currentValue = double(0);
	currentIndex = -1;

	QString fileName = "data\\components\\" + name + ".json"; // TODO allow user to look for components in different directory?

	// TODO!!!!!!!!! error handling
	// load and parse description file
	std::ifstream ifs(fileName.toStdString());
	rapidjson::IStreamWrapper isw(ifs);

	_componentData.ParseStream(isw);

	// initialize property table with default values
	if (_componentData.HasMember("properties")) {

		const rapidjson::Value& propertiesData = _componentData["properties"];

		for (rapidjson::Value::ConstValueIterator itr = propertiesData.Begin(); itr != propertiesData.End(); itr++) {

			properties.insert(std::pair<QString, Property>((*itr)["name"].GetString(), 
												Property((*itr)["displayName"].GetString(), 
												(*itr)["unit"].GetString(), 
												(*itr)["defaultValue"].GetDouble())));

		}
	
	}

	// parse geometry data
	const rapidjson::Value& displayMembers = _componentData["display"];
	loadGeometryFromJSON(displayMembers, _geometryObjects);

	// bounding rect
	const rapidjson::Value& boundingRect = _componentData["boundingRect"];

	_boundingRect.setCoords(boundingRect["topLeft"][0].GetInt(),
							boundingRect["topLeft"][1].GetInt(),
							boundingRect["bottomRight"][0].GetInt(),
							boundingRect["bottomRight"][1].GetInt());

	_boundingRect = _boundingRect.normalized(); 

}

void Component::setPos(const QPoint& newPos){

	_pos = newPos;

	for (const auto& it : coupledNodes)
		it->updatePos();

};

bool Component::isMouseOver(const QPoint& mousePos) const {

	return (abs(mousePos.x() - _pos.x()) <= _boundingRect.width() / 2 && abs(mousePos.y() - _pos.y()) <= _boundingRect.height() / 2);

}

bool Component::isWithinRectangle(const QRect& rect) const {

	return _boundingRect.translated(_pos).intersects(rect);

}

void Component::draw(QPainter& painter) {
	
	for (const auto& it : _geometryObjects)
		it->draw(painter, _pos, _rotationAngle);

	// draw text
	// TODO figure out a better way to do this
	QPoint pos(_componentData["nameposition"][0].GetInt(), _componentData["nameposition"][1].GetInt());
	painter.drawText(rotatePoint(pos + _pos, _pos, _rotationAngle % 180), getLetterIdentifierBase() + QString::number(serialNumber));

}

const QPoint Component::pos() const {

	return _pos;

}

int Component::getRotationAngle() const {

	return this->_rotationAngle;

}

void Component::setRotationAngle(const int angle) {

	// division by zero is apparently bad
	if (angle == 0)
		_rotationAngle = 0;
	else 
		_rotationAngle = angle % (sign(angle) * 360);

	// NOTE we assume that the only rotation angles will be 0, +-90, +=180, +-270, +-360
	if (_rotationAngle % 180 != 0)
		_boundingRect = _boundingRect.transposed();

	//  update node positions
	setPos(_pos);

}

// TODO temporary, will figure out decent solution *later*
bool Component::hasSimulationResult() {
	return _componentData["DCOP"].HasMember("simulationResult");
}

std::tuple<QString, QString, double> Component::getSimulationResult() {

	QString type = _componentData["DCOP"]["simulationResult"]["type"].GetString();

	// TODO refactor, you know what do I mean
	exprtk::symbol_table<double> symbolTable;

	// initialize all available variables
	std::vector<std::pair<std::string, double>> variables;

	for (auto it = std::begin(coupledNodes); it != std::end(coupledNodes); ++it) {

		double value = (*it)->voltageValue;
		std::string name = "this.node" + std::to_string(std::distance(std::begin(coupledNodes), it)) + ".voltageValue";

		variables.push_back(std::make_pair(name, value));

	}

	variables.push_back(std::make_pair("this.currentValue", currentValue));
	// 
	for (auto& it : variables)
		symbolTable.add_variable(it.first, it.second, true);

	for (auto &it : properties)
		symbolTable.add_variable("this.properties." + it.first.toStdString(), it.second.value);

	exprtk::expression<double> expression;
	expression.register_symbol_table(symbolTable);

	exprtk::parser<double> parser;

	std::string expr = _componentData["DCOP"]["simulationResult"]["value"].GetString();
	
	parser.compile(expr, expression);

	return std::make_tuple(getLetterIdentifierBase() + QString::number(serialNumber), type, expression.value());

}

void Component::updateNodeOffsets() {

	const rapidjson::Value& nodes = _componentData["nodes"];
	for (rapidjson::Value::ConstValueIterator it = nodes.Begin(); it != nodes.End(); ++it) {

		QPoint nodeOffset((*it)["position"][0].GetInt(), (*it)["position"][1].GetInt());

		coupledNodes[(*it)["ID"].GetInt()]->setOffset(nodeOffset);

	}

}

void Component::saveToJSON(rapidjson::Value& arrayComponents, rapidjson::Document::AllocatorType& allocator) {

	rapidjson::Value valueComponent;
	valueComponent.SetObject();

	valueComponent.AddMember("ID", ID, allocator);

	rapidjson::Value name(_name.toStdString().c_str(), _name.toStdString().size(), allocator); // TODO wtf is with those strings
	valueComponent.AddMember("name", name, allocator);

	rapidjson::Value position(rapidjson::kArrayType);
	position.PushBack(_pos.x(), allocator);
	position.PushBack(_pos.y(), allocator);
	valueComponent.AddMember("position", position, allocator);

	valueComponent.AddMember("rotationAngle", _rotationAngle, allocator);

	if (!properties.empty()) {

		rapidjson::Value arrayProperties(rapidjson::kArrayType);

		for (auto &it : properties)
			arrayProperties.PushBack(it.second.value, allocator);

		valueComponent.AddMember("properties", arrayProperties, allocator);

	}

	arrayComponents.PushBack(valueComponent, allocator);

}
