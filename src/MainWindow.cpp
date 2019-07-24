#include "MainWindow.h"

#include <qapplication.h>
#include <qpainter.h>
#include <qtoolbar.h>
#include <QMouseEvent>
#include <qfiledialog.h>
#include <QDockWidget.h>

#include "ComponentPropertiesWindow.h"
#include "SimulationResultsWindow.h"
#include "dialogSimulationParameters.h"
#include "SimulationErrors.h"

#include "Circuit.h"
#include "CircuitEditor.h"

#include <windows.h>

#include "../eigen//Dense"

void MainWindow::slot_componentProperties(){
	// TODO you know what
	if (editor->mouseOverComponent != nullptr){
		qDeleteAll(dockComponentProperties->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

		ComponentPropertiesWindow* d = new ComponentPropertiesWindow(*editor->mouseOverComponent);

		dockComponentProperties->setWidget(d);
		dockComponentProperties->show();
	}
}

void MainWindow::slot_fileExit(){
	QApplication::exit();
}

void MainWindow::slot_simulationRun(){
	DialogSimulationParameters* d = new DialogSimulationParameters(editor->circuit);

	if (d->exec() != QDialog::Accepted)
		return;

	switch (d->parameters.mode){
	case SimulationMode::DCOP: {
		try{
			editor->circuit.simulate();

			// display dock with simulation results
			qDeleteAll(dockSimulationResults->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

			SimulationResultsWindow* w = new SimulationResultsWindow(editor->circuit);

			dockSimulationResults->setWidget(w);
			dockSimulationResults->show();

			update();
		}
		catch (SimulationException& e){
			QMessageBox msgBox(this);

			msgBox.setText("Simulation failed: " + e.what());
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setIcon(QMessageBox::Icon::Critical);

			QApplication::beep();
			msgBox.exec();
		}
	}

	break;
	}
}

void MainWindow::slot_fileSaveAs(){
	QString fileName(QFileDialog::getSaveFileName(this, "Save circuit to file", "", "Circuit Schematic Files (*.csf);;All Files (*)"));

	if (!fileName.isEmpty() && !fileName.isNull()){
		editor->circuit.saveToFile(fileName);
		setWindowTitle(fileName.section("/", -1, -1) + " - CircuitSimulator");
	}
}

void MainWindow::slot_editPaste(){
	// read component data and create all components
	const rapidjson::Value& componentsVal = file["components"];
	for (rapidjson::Value::ConstValueIterator it = componentsVal.Begin(); it != componentsVal.End(); ++it){
		editor->circuit.createComponent((*it)["name"].GetString(), QPoint((*it)["position"][0].GetInt(), (*it)["position"][1].GetInt()), false); // disable automatic node creation

		editor->circuit.components.back()->setRotationAngle((*it)["rotationAngle"].GetInt());
	}

	const rapidjson::Value& nodesVal = file["nodes"];

	// first, we need to create all the nodes - only then can we start connecting them together
	for (rapidjson::Value::ConstValueIterator it = nodesVal.Begin(); it != nodesVal.End(); ++it){
		if ((*it).HasMember("coupled")){
			editor->circuit.createNode(QPoint((*it)["position"][0].GetInt(), (*it)["position"][1].GetInt()), editor->circuit.components[(*it)["coupledComponent"].GetInt()]);
			editor->circuit.components[(*it)["coupledComponent"].GetInt()]->coupledNodes.push_back(editor->circuit.nodes.back());
		}
		else{
			editor->circuit.createNode(QPoint((*it)["position"][0].GetInt(), (*it)["position"][1].GetInt()));
		}

		//nodes.back()->ID = (*it)["ID"].GetInt();
	}

	// now that we're sure that every component has as many coupled nodes as required
	for (const auto& it : editor->circuit.components)
		it->updateNodeOffsets();

	// connect nodes together
	for (rapidjson::Value::ConstValueIterator it = nodesVal.Begin(); it != nodesVal.End(); ++it)
		if ((*it).HasMember("connectedNodes")) // TODO it should always have one
			for (rapidjson::Value::ConstValueIterator jt = (*it)["connectedNodes"].Begin(); jt != (*it)["connectedNodes"].End(); ++jt)
				editor->circuit.nodes[(*it)["ID"].GetInt()]->connectTo(editor->circuit.nodes[(*jt).GetInt()]);
}

void MainWindow::slot_editCopy(){
	// assign each node and component a unique ID, which is essentially its index
	int ID = 0;
	for (auto& it : editor->circuit.nodes){
		it->ID = ID;
		ID++;
	}

	ID = 0;
	for (auto& it : editor->circuit.components){
		it->ID = ID;
		ID++;
	}

	// initialize an emppty JSON doc
	file.SetObject();

	//file.SetObject();
	rapidjson::Document::AllocatorType& allocator = file.GetAllocator();

	// start by saving all nodes to file
	rapidjson::Value arrayNodes(rapidjson::kArrayType);
	for (auto& it : editor->circuit.nodes)
		if (it->selected){
			rapidjson::Value valueNode;
			valueNode.SetObject();

			valueNode.AddMember("ID", ID, allocator);

			rapidjson::Value position(rapidjson::kArrayType);
			position.PushBack(it->pos().x(), allocator);
			position.PushBack(it->pos().y(), allocator);

			valueNode.AddMember("position", position, allocator);

			if (!std::empty(it->connectedNodes)){
				rapidjson::Value connectedNodesArray(rapidjson::kArrayType);

				for (const auto& jt : it->connectedNodes)
					if (jt->selected)
						connectedNodesArray.PushBack(jt->ID, allocator);

				valueNode.AddMember("connectedNodes", connectedNodesArray, allocator);
			}

			if (it->isCoupled()){
				valueNode.AddMember("coupled", true, allocator);
				valueNode.AddMember("coupledComponent", it->getCoupledComponent()->ID, allocator);
			}

			arrayNodes.PushBack(valueNode, allocator);
		}

	file.AddMember("nodes", arrayNodes, allocator);

	// save all components to file
	rapidjson::Value arrayComponents(rapidjson::kArrayType);
	for (auto& it : editor->circuit.components)
		if (it->selected){
			rapidjson::Value valueComponent;
			valueComponent.SetObject();

			valueComponent.AddMember("ID", ID, allocator);

			rapidjson::Value name(it->getName().toUtf8(), it->getName().size(), allocator);
			valueComponent.AddMember("name", name, allocator);

			rapidjson::Value position(rapidjson::kArrayType);
			position.PushBack(it->pos().x(), allocator);
			position.PushBack(it->pos().y(), allocator);
			valueComponent.AddMember("position", position, allocator);

			valueComponent.AddMember("rotationAngle", it->getRotationAngle(), allocator);
		}

	file.AddMember("components", arrayComponents, allocator);
}

void MainWindow::slot_fileOpen(){
	QString fileName(QFileDialog::getOpenFileName(this, "Load circuit from file", "", "Circuit Schematic Files (*.csf);;All Files (*)"));

	if (!fileName.isEmpty() && !fileName.isNull()){
		editor->circuit.loadFromFile(fileName);
		setWindowTitle(fileName.section("/", -1, -1) + " - CircuitSimulator");
	}

	editor->update();
}

//
//	constructor for MainWindow
//
MainWindow::MainWindow(){
	editor = new CircuitEditor();
	setCentralWidget(editor);

	editor->show();

	createInterface();
}

void MainWindow::slot_schematicPlaceComponent(QAction* action){
	editor->setCurrentComponent(action->data().toString());
	editor->setMode(EditorMode::componentCreation);
}

void MainWindow::slot_aboutQt(){
	QApplication::aboutQt();
}
