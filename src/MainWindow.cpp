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
#include "LogWindow.h"
#include "SimulationErrors.h"

#include "Circuit.h"
#include "CircuitEditor.h"
#include "Version.h"

#include <windows.h>

#include "../eigen//Dense"

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

	DialogSimulationParameters* d = new DialogSimulationParameters(editor->circuit);

	if (d->exec() != QDialog::Accepted)
		return;

	switch (d->parameters.mode) {
	
		case SimulationMode::DCOP: {

			Eigen::VectorXd solutions;
			solutions.fill(0);

			SimulationResult result = editor->circuit.simulate();

			if (result != SimulationResult::Success) {
				logWindow->log("Simulation failed - " + getSimulationErrorMessage(result) + ".", LogEntryType::Error);
				return;
			}

			qDeleteAll(dockSimulationResults->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

			SimulationResultsWindow* w = new SimulationResultsWindow(editor->circuit, solutions, editor->circuit.voltageCount);

			dockSimulationResults->setWidget(w);
			dockSimulationResults->show();

			update();

		}

		break;

		case SimulationMode::DCSweep: {

			int MAX = (d->parameters.end - d->parameters.start) / d->parameters.delta;

			int i = 0;

			while (i <= MAX) {

				double currval = d->parameters.start + i * d->parameters.delta;

				d->parameters.componentToSweep->properties[d->parameters.propertyToSweep].value = currval;

				Eigen::VectorXd solutions;
				solutions.fill(0);

				SimulationResult result = editor->circuit.simulate();

				if (result != SimulationResult::Success) {
					logWindow->log("Simulation failed - " + getSimulationErrorMessage(result) + ".", LogEntryType::Error);
					return;
				}

				//qDeleteAll(dockSimulationResults->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

				//SimulationResultsWindow* w = new SimulationResultsWindow(editor->circuit, solutions, editor->circuit.voltageCount);

			//	dockSimulationResults->setWidget(w);
				//dockSimulationResults->show();

			//	update();

				++i;

				OutputDebugStringA((std::to_string(d->parameters.componentToSweep->currentValue) + "\n").c_str());
			
			
			}

			
		
		
		}
		
	
	}
	
	

}


void MainWindow::slot_fileSaveAs() {

	QString fileName(QFileDialog::getSaveFileName(this, "Save circuit to file", "", "Circuit Schematic Files (*.csf);;All Files (*)"));

	if (!fileName.isEmpty() && !fileName.isNull()) 
		editor->circuit.saveToFile(fileName);

}

void MainWindow::slot_fileOpen() {

	QString fileName(QFileDialog::getOpenFileName(this, "Load circuit from file", "", "Circuit Schematic Files (*.csf);;All Files (*)"));

	if (!fileName.isEmpty() && !fileName.isNull())
		editor->circuit.loadFromFile(fileName);

	editor->update();

}

// 
//	constructor for MainWindow
//
MainWindow::MainWindow() {

	editor = new CircuitEditor();
	setCentralWidget(editor);
	editor->logWindow = logWindow;
	editor->show();

	createInterface();

	logWindow->log("CircuitSimulator v." + VersionInfo::getVersionString() + "\ncopyright (C) 2018 by Krzysztof Stempinski", LogEntryType::Info);
	logWindow->log("This is free software, but comes with ABSOLUTELY NO WARRANTY. You are welcome to redistribute it, subject to certain conditions. See license.md for more details.");

	editor->circuit.logWindow = logWindow;

}

void MainWindow::slot_schematicPlaceComponent(QAction* action) {

	editor->setCurrentComponent(action->data().toString());

	editor->setMode(EditorMode::componentCreation);

}


void MainWindow::slot_aboutQt() {
	QApplication::aboutQt();
}