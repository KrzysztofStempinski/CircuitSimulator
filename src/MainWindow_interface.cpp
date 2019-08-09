/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	MainWindow_interface.h
//
//  ---------------------------------------------

#include "MainWindow.h"

#include <qdockwidget.h>
#include <qmenubar.h>
#include <qstatusbar.h>

#include "Config.h"
#include "ComponentList.h"
#include "SimulationResultsWindow.h"
#include "util/QActionHelper.h"

void MainWindow::createInterface() {
	setWindowIcon(QIcon(Config::Paths::Icons + "appicon.ico"));
	setWindowTitle("untitled.esf - CircuitSimulator");
	resize(QSize(MAINWINDOW_DEF_WIDTH, MAINWINDOW_DEF_HEIGHT));

	createDockWidgets();
	createMenu();
	createToolBar();
	populateComponents();
}

void MainWindow::createDockWidgets() {

	// component properties dock
	dockComponentProperties = new QDockWidget("Component properties", this);
	dockComponentProperties->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, dockComponentProperties);
	dockComponentProperties->hide();

	// simulation results dock
	dockSimulationResults = new QDockWidget("Simulation results", this);
	dockSimulationResults->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, dockSimulationResults);
	dockSimulationResults->hide();
}

//
// MainWindow::createMenus()
//
void MainWindow::createMenu() {

	//TODO connecte to slots!!!!!
	action_componentProperties = createAction("Component properties", "P", "properties.ico");
	QAction::connect(action_componentProperties, &QAction::triggered, this, &MainWindow::slot_componentProperties);
	addAction(action_componentProperties);

	// file actions
	//TODO wire up
	action_fileNew = createAction("New", "Ctrl+N", "fileNew.ico");
	addAction(action_fileNew);

	action_fileOpen = createAction("Open", "Ctrl+O", "fileOpen.ico");
	QAction::connect(action_fileOpen, &QAction::triggered, this, &MainWindow::slot_fileOpen);
	addAction(action_fileOpen);

	//TODO wire up
	action_fileSave = createAction("Save", "Ctrl+S", "fileSave.ico");
	addAction(action_fileSave);

	action_fileSaveAs = createAction("Save As...", "Ctrl+Shift+S", "fileSaveAs.ico");
	QAction::connect(action_fileSaveAs, &QAction::triggered, this, &MainWindow::slot_fileSaveAs);
	addAction(action_fileSaveAs);

	//TODO wire up
	action_fileExit = createAction("Exit", SLOT(slot_fileExit()), "Alt+F4");
	addAction(action_fileExit);

	// edit actions
	action_editUndo = createAction("Undo", "Ctrl+Z", "editUndo.ico");
	addAction(action_editUndo);

	action_editRedo = createAction("Redo", "Ctrl+Y", "editRedo.ico");
	addAction(action_editRedo);

	action_editCopy = createAction("Copy", "Ctrl+C", "fileCopy.ico");
	addAction(action_editCopy);

	action_editPaste = createAction("Paste", "Ctrl+P", "filePaste.ico");
	addAction(action_editPaste);

	// simulation actions
	action_simulationRun = createAction("Run", "F5", "simulationRun.ico");
	QAction::connect(action_simulationRun, &QAction::triggered, this, &MainWindow::slot_simulationRun);
	action_simulationRun->setToolTip("Run simulation");
	addAction(action_simulationRun);

	// help actions
	action_aboutQt = createAction("About Qt", SLOT(slot_aboutQt()));
	QAction::connect(action_aboutQt, &QAction::triggered, this, &MainWindow::slot_aboutQt);

	QMenu* menuFile = menuBar()->addMenu("File");
	menuFile->addAction(action_fileNew);
	menuFile->addAction(action_fileOpen);
	menuFile->addSeparator();
	menuFile->addAction(action_fileSave);
	menuFile->addAction(action_fileSaveAs);
	menuFile->addSeparator();
	menuFile->addAction(action_fileExit);

	QMenu* menuEdit = menuBar()->addMenu("Edit");
	menuEdit->addAction(action_editUndo);
	menuEdit->addAction(action_editRedo);
	menuEdit->addSeparator();
	menuEdit->addAction(editor->action_delete);
	menuEdit->addSeparator();
	menuEdit->addAction(editor->action_componentRotateRight);
	menuEdit->addAction(editor->action_componentRotateLeft);
	menuEdit->addSeparator();
	menuEdit->addAction(action_editSelectAll);

	QMenu* menuSchematic = menuBar()->addMenu("Schematic");
	menuComponents = menuSchematic->addMenu("Place component");

	QMenu* menuSimulation = menuBar()->addMenu("Simulation");
	menuSimulation->addAction(action_simulationRun);
	menuSimulation->addAction("Pause");
	menuSimulation->addAction("Stop");
	menuSimulation->addSeparator();
	menuSimulation->addAction("Settings");

	QMenu* menuTools = menuBar()->addMenu("Tools");
	menuTools->addAction("Editor settings");

	QMenu* menuWindow = menuBar()->addMenu("Window");
	menuWindow->addAction("Plot area");
	menuWindow->addAction("Simulation results");
	menuWindow->addAction("Simulation log");
	menuWindow->addSeparator();
	menuWindow->addAction("Debug log");

	QMenu* menuHelp = menuBar()->addMenu("Help");
	menuHelp->addAction(action_aboutQt);
	menuHelp->addAction("About Circuit Simulator");
}

void MainWindow::populateComponents() {
	toolbarComponents = new QToolBar();
	toolbarComponents->setIconSize(QSize(40, 40));
	toolbarComponents->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	addToolBar(Qt::LeftToolBarArea, toolbarComponents);

	QPixmap* pixmap = new QPixmap(36, 36);
	pixmap->fill(Qt::transparent);

	QPainter* painter = new QPainter(pixmap);
	painter->setPen(Qt::cyan);

	// lambda magic
	auto createComponentAction = [pixmap, painter](QString const& name) -> QAction * {
		Component* tempComponent = getComponentFromName(name);
		tempComponent->setPos(QPoint(18, 18));

		pixmap->fill(Qt::transparent);
		tempComponent->draw(*painter);

		QAction* action = new QAction(tempComponent->displayNameBase());
		action->setData(name);
		action->setIcon(QIcon(*pixmap));

		delete tempComponent;

		return action;
	};

	for (const auto& it : COMPONENT_LIST) {
		QAction* act = createComponentAction(it);
		menuComponents->addAction(act);
		toolbarComponents->addAction(act);
	}

	connect(menuComponents, SIGNAL(triggered(QAction*)), this, SLOT(slot_schematicPlaceComponent(QAction*)));
}

void MainWindow::createToolBar() {
	QToolBar* toolbar = new QToolBar();

	toolbar->setWindowTitle("Toolbar");

	addToolBar(toolbar);

	toolbar->addAction(action_fileNew);
	toolbar->addAction(action_fileOpen);

	toolbar->addSeparator();

	toolbar->addAction(action_fileSave);
	toolbar->addAction(action_fileSaveAs);

	toolbar->addSeparator();

	toolbar->addAction(action_editUndo);
	toolbar->addAction(action_editRedo);;

	toolbar->addSeparator();

	toolbar->addAction(action_simulationRun);

	toolbar->addSeparator();
}

void MainWindow::createStatusBar() {
	QStatusBar* statusbar = new QStatusBar(this);
	setStatusBar(statusbar);
	statusbar->show();
}