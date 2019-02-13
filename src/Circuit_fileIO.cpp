
#include "Circuit.h"

#include "../rapidjson/rapidjson.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/istreamwrapper.h"
#include "../rapidjson/prettywriter.h"

#include <iterator>

bool Circuit::saveToFile(QString fileName) {

	// assign each node and component a unique ID
	int ID = 0;
	for (auto &it : nodes) {
		it->ID = ID;
		++ID;
	}

	ID = 0;
	for (auto &it : components) {
		it->ID = ID;
		++ID;
	}

	// initialize an emppty JSON doc
	rapidjson::Document file;
	file.SetObject();
	rapidjson::Document::AllocatorType& allocator = file.GetAllocator();

	// start by saving all NON-COUPLED(!) nodes to file
	rapidjson::Value arrayNodes(rapidjson::kArrayType);
	for (auto &it : nodes) 
		if (!it->isCoupled())
			it->saveToJSON(arrayNodes, allocator);

	file.AddMember("nodes", arrayNodes, allocator);
	
	// save all components to file, together with nodes coupled to each of them
	rapidjson::Value arrayComponents(rapidjson::kArrayType);

	for (auto &it : components) {

		rapidjson::Value valueComponent;
		valueComponent.SetObject();

		valueComponent.AddMember("ID", it->ID, allocator);

		rapidjson::Value name(it->getName().toStdString().c_str(), it->getName().toStdString().size(), allocator); // TODO wtf is with those strings
		valueComponent.AddMember("name", name, allocator);

		rapidjson::Value position(rapidjson::kArrayType);
		position.PushBack(it->getPos().x(), allocator);
		position.PushBack(it->getPos().y(), allocator);

		valueComponent.AddMember("position", position, allocator);

		// save coupled nodes, if there are any
        if (!it->coupledNodes.empty()) {

			rapidjson::Value arrayCoupledNodes(rapidjson::kArrayType);
			for (auto &itn : it->coupledNodes)
				itn->saveToJSON(arrayCoupledNodes, allocator);

			valueComponent.AddMember("coupledNodes", arrayCoupledNodes, allocator);

		}

		// same for properties
        if (!it->properties.empty()) {
		
			rapidjson::Value arrayProperties(rapidjson::kArrayType);
			
			for (auto &itp : it->properties)
				arrayProperties.PushBack(itp.second.value, allocator);

			valueComponent.AddMember("properties", arrayProperties, allocator);

		}

		arrayComponents.PushBack(valueComponent, allocator);

	}

	file.AddMember("components", arrayComponents, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf); // so that the file is somewhat human-readable
	file.Accept(writer);

	std::ofstream out(fileName.toStdString()); 
    out << strbuf.GetString();
    out.close();

	return true;

}

bool Circuit::loadFromFile(QString fileName) {

	// clean up
	nodes.clear();
	components.clear();

	// load and parse file 
	std::ifstream ifs(fileName.toStdString());
	rapidjson::IStreamWrapper isw(ifs);

	rapidjson::Document file;
	file.ParseStream(isw);

	const rapidjson::Value& nodesVal = file["nodes"];

	// first, we need to create all the nodes - only then can we start connecting them together
	for (rapidjson::Value::ConstValueIterator it = nodesVal.Begin(); it != nodesVal.End(); ++it)
		createNode(QPoint((*it)["position"][0].GetInt(), (*it)["position"][1].GetInt()), false);

	const rapidjson::Value& componentsVal = file["components"];

	for (rapidjson::Value::ConstValueIterator it = componentsVal.Begin(); it != componentsVal.End(); ++it) {

		createComponent((*it)["name"].GetString(), QPoint((*it)["position"][0].GetInt(), (*it)["position"][1].GetInt()), false);

		if ((*it).HasMember("properties")) {

			const rapidjson::Value& arrayProperties =(*it)["properties"];

			//for (rapidjson::Value::ConstValueIterator itp = arrayProperties.Begin(); itp != arrayProperties.End(); ++itp)
				//components.back()->properties.set
			//for (int i = 0; i < components[components.size() - 1]->properties.getPropertyCount(); i++) {

				//double val = component["properties"][i].GetDouble();

			//	components[components.size() - 1]->properties.setPropertyValue(i, component["properties"][i].GetDouble());

			//}
		}

	}


	//for (auto it = std::begin(nodes); it != std::end(nodes); ++it) {

		//int i = std::distance(nodes.begin(), it);

		//if (nodesVal[i].HasMember("connectedNodes")) {

		//	for (rapidjson::Value::ConstValueIterator jtr = nodesVal[i]["connectedNodes"].Begin(); jtr != nodesVal[i]["connectedNodes"].End(); ++jtr) {

			//	(*it)->connectedNodes.push_back(this->nodes[(*jtr).GetInt()]);

		//}

	//}

	return true;

}
