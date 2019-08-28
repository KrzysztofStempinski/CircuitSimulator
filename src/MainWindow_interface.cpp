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

#include "SimulationResultsWindow.h"
#include "Version.h"

const QString ICON_PATH = "data\\icons\\";
const std::string COMPONENT_PATH = "data\\components\\"; // TODO move to Settings or something like that

void MainWindow::createInterface() {
	setWindowIcon(QIcon(ICON_PATH + "appicon.ico"));
	setWindowTitle("untitled.esf - CircuitSimulator v." + VersionInfo::getVersionString());
	resize(QSize(MAINWINDOW_DEF_WIDTH, MAINWINDOW_DEF_HEIGHT));

	createDockWidgets();
	createMenu();
	createToolBar();
	populateComponents();
}

void MainWindow::createDockWidgets() {
	// log dock
	logWindow = new LogWindow();

	dockLog = new QDockWidget("Log", this);
	dockLog->setWidget(logWindow);
	dockLog->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, dockLog);

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
	auto createAction = [this](QString const& title, const char* slot, QString const& shortcut = "", QString const& iconName = "") -> QAction * {
		QAction* action = new QAction(title);

		if (!iconName.isEmpty())
			action->setIcon(QIcon(ICON_PATH + iconName));

		if (!shortcut.isEmpty())
			action->setShortcut(QKeySequence(shortcut));

		QObject::connect(action, SIGNAL(triggered()), this, slot);

		return action;
	};

	action_componentProperties = createAction("Component properties", SLOT(slot_componentProperties()), "P", "properties.ico");
	addAction(action_componentProperties);

	// file actions
	action_fileNew = createAction("New", SLOT(slot_fileNew()), "Ctrl+N", "fileNew.ico");
	addAction(action_fileNew);

	action_fileOpen = createAction("Open", SLOT(slot_fileOpen()), "Ctrl+O", "fileOpen.ico");
	addAction(action_fileOpen);

	action_fileSave = createAction("Save", SLOT(slot_fileSave()), "Ctrl+S", "fileSave.ico");
	addAction(action_fileSave);

	action_fileSaveAs = createAction("Save As...", SLOT(slot_fileSaveAs()), "Ctrl+Shift+S", "fileSaveAs.ico");
	addAction(action_fileSaveAs);

	action_fileExit = createAction("Exit", SLOT(slot_fileExit()), "Alt+F4");
	addAction(action_fileExit);

	// edit actions
	action_editUndo = createAction("Undo", SLOT(slot_editUndo()), "Ctrl+Z", "editUndo.ico");
	addAction(action_editUndo);

	action_editRedo = createAction("Redo", SLOT(slot_editRedo()), "Ctrl+Y", "editRedo.ico");
	addAction(action_editRedo);

	action_editCopy = createAction("Copy", SLOT(slot_editCopy()), "Ctrl+C", "fileCopy.ico");
	addAction(action_editCopy);

	action_editPaste = createAction("Paste", SLOT(slot_editPaste()), "Ctrl+P", "filePaste.ico");
	addAction(action_editPaste);

	// simulation actions
	action_simulationRun = createAction("Run", SLOT(slot_simulationRun()), "F5", "simulationRun.ico");
	action_simulationRun->setToolTip("Run simulation");
	addAction(action_simulationRun);

	// help actions
	action_aboutQt = createAction("About Qt", SLOT(slot_aboutQt()));

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

		auto tempComponent = new Component(name);
		tempComponent->setPos(QPoint(18, 18));

		pixmap->fill(Qt::transparent);
		tempComponent->draw(*painter);

		QAction* action = new QAction(QString::fromStdString(tempComponent->displayNameBase()));
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