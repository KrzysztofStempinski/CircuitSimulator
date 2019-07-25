#include "MainWindow.h"

#include <qapplication.h>
#include <qpainter.h>
#include <qtoolbar.h>
#include <QMouseEvent>
#include <qfiledialog.h>
#include <QDockWidget.h>

#include "WidgetComponentProperties.h"
#include "SimulationResultsWindow.h"
#include "dialogSimulationParameters.h"
#include "SimulationErrors.h"

#include "Circuit.h"
#include "CircuitEditor.h"

#include <windows.h>

#include "../eigen//Dense"

void MainWindow::slot_componentProperties(){
	// TODO you know what
	if (editor->mouseOverComponent != nullptr) {
		qDeleteAll(dockComponentProperties->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

		auto d = new WidgetComponentProperties(*editor->mouseOverComponent);
		QObject::connect(d, &WidgetComponentProperties::finished, this, [&]() { dockComponentProperties->hide();});

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
}

void MainWindow::slot_editCopy(){
}

void MainWindow::slot_fileOpen(){
	const QString fileName = QFileDialog::getOpenFileName(this, "Load circuit from file", "", "Circuit Schematic Files (*.csf);;All Files (*)");

	if (!fileName.isNull() && !fileName.isEmpty()){
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
