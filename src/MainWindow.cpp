#include "MainWindow.h"

#include <qapplication.h>
#include <qpainter.h>		
#include <qtoolbar.h>
#include <QMouseEvent>
#include <qfiledialog.h>
#include <QDockWidget.h>

#include "ComponentPropertiesWindow.h"
#include "SimulationResultsWindow.h"
#include "LogWindow.h"
#include "SimulationErrors.h"

#include "Geometry.h"
#include "Circuit.h"
#include "CircuitEditor.h"
#include "Simulation.h"
#include "Version.h"

#include "../eigen//Dense"

#include <chrono>

void MainWindow::slot_componentProperties() {

	// TODO you know what
	if (editor->mouseOverComponent != nullptr) {

		qDeleteAll(dockComponentProperties->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

		ComponentPropertiesWindow *d = new ComponentPropertiesWindow(*editor->mouseOverComponent);

		dockComponentProperties->setWidget(d);
		dockComponentProperties->show();

	}

}

void MainWindow::slot_createMidpointNode() {
	editor->createMidpointNode();
}

void MainWindow::slot_fileExit() {
	QApplication::exit();
}

void MainWindow::slot_simulationRun() {



	Eigen::VectorXd solutions;
	solutions.fill(0);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	SimulationResult result = simulate(editor->circuit, SimulationMode::DCOP, solutions, logWindow);

	if (result == SimulationResult::Success) {

		qDeleteAll(dockSimulationResults->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

		SimulationResultsWindow* w = new SimulationResultsWindow(editor->circuit, solutions, editor->circuit.voltageCount);

		dockSimulationResults->setWidget(w);
		dockSimulationResults->show();

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		logWindow->log("Elapsed time: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) + " ms");

	} else {
		logWindow->log("Simulation failed - " + getSimulationErrorMessage(result) + ".", LogEntryType::Error);
	}

	update();

}


void MainWindow::slot_fileSaveAs() {

	QString fileName = QFileDialog::getSaveFileName(this, "Save circuit to file", "", "Circuit Schematic Files (*.csf);;All Files (*)");

	if (!fileName.isEmpty() && !fileName.isNull()) 
		editor->circuit.saveToFile(fileName);

}

void MainWindow::slot_fileOpen() {

	QString fileName = QFileDialog::getOpenFileName(this, "Load circuit from file", "", "Circuit Schematic Files (*.csf);;All Files (*)");

	if (!fileName.isEmpty() && !fileName.isNull())
		editor->circuit.loadFromFile(fileName);

}

// 
//	constructor for MainWindow
//
MainWindow::MainWindow() {

	setWindowIcon(QIcon("data\\icons\\appicon.ico"));
	setWindowTitle("untitled.esf - Schematic Editor | CircuitSimulator v." + getVersionString());

	resize(QSize(MAINWINDOW_DEF_WIDTH, MAINWINDOW_DEF_HEIGHT));

	editor = new CircuitEditor();
	setCentralWidget(editor);
	editor->logWindow = logWindow;
	editor->show();

	createDockWidgets();
	createMenu();
	createToolBar();

	logWindow->log("CircuitSimulator v." + getVersionString() + "\ncopyright (C) 2018 by Krzysztof Stempinski", LogEntryType::Info);
	logWindow->log("This is free software, but comes with ABSOLUTELY NO WARRANTY. You are welcome to redistribute it, subject to certain conditions. See license.md for more details.");

}

void MainWindow::slot_schematicPlaceComponent(QAction *action) {

	// chop 5 chars to get resistor to resistor.json
	// TODO make it less retarded
	editor->setCurrentComponent(action->data().toString().chopped(5));

	editor->setMode(EditorMode::componentCreation);

}


void MainWindow::slot_aboutQt() {
	QApplication::aboutQt();
}