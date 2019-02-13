#include "MainWindow.h"

#include "SimulationResultsWindow.h"

#include <filesystem>

#include <qdockwidget.h>
#include <qcombobox.h>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <qstatusbar.h>

#include "../rapidjson/rapidjson.h"
#include "../rapidjson/istreamwrapper.h"

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

	//TODO TEMP
	action_componentProperties = new QAction(tr("Component properties\tP"), this);
	action_componentProperties->setShortcut(QKeySequence(tr("P")));
	connect(action_componentProperties, SIGNAL(triggered()), this, SLOT(slot_componentProperties()));
	addAction(action_componentProperties);

	action_createMidpointNode = new QAction(tr("Create midpoint node\tM"), this);
	action_createMidpointNode->setShortcut(QKeySequence(tr("M")));
	connect(action_createMidpointNode, SIGNAL(triggered()), this, SLOT(slot_createMidpointNode()));
	addAction(action_createMidpointNode);

	QMenu *menuFile = this->menuBar()->addMenu(tr("&File"));

	action_fileNew = new QAction(tr("&New\t Ctrl+N"), this);
	action_fileNew->setIcon(QIcon("data\\icons\\fileNew.ico"));
	menuFile->addAction(action_fileNew);
	//	connect(menuFile, SIGNAL(triggered(QAction*)), this, SLOT(action_saveCircuitToFile(QAction*)));

	action_fileOpen = new QAction(tr("&Open\t Ctrl+O"), this);
	action_fileOpen->setIcon(QIcon("data\\icons\\fileOpen.ico"));
	menuFile->addAction(action_fileOpen);
	connect(action_fileOpen, SIGNAL(triggered()), this, SLOT(slot_fileOpen()));

	menuFile->addSeparator();

	action_fileSave = new QAction(tr("&Save\t Ctrl+S"), this);
	action_fileSave->setIcon(QIcon("data\\icons\\fileSave.ico"));
	menuFile->addAction(action_fileSave);

	action_fileSaveAs = new QAction(tr("&Save As...\t Ctrl+Shift+S"), this);
	action_fileSaveAs->setIcon(QIcon("data\\icons\\fileSaveAs.ico"));
	menuFile->addAction(action_fileSaveAs);
	connect(action_fileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileSaveAs()));

	menuFile->addSeparator();

	menuFile->addAction("Print preview");
	menuFile->addAction("Print");

	menuFile->addSeparator();
	
	action_fileExit = new QAction(tr("&Exit\t Alt+F4"), this);
	action_fileExit->setShortcut(QKeySequence(Qt::Key_Alt + Qt::Key_F4));
	connect(action_fileExit, SIGNAL(triggered()), this, SLOT(slot_fileExit()));
	menuFile->addAction(action_fileExit);


	QMenu *menuEdit = this->menuBar()->addMenu(tr("&Edit"));

	action_editUndo = new QAction(tr("Undo\t Ctrl+Z"), this);
	action_editUndo->setIcon(QIcon("data\\icons\\editUndo.ico"));
	action_editUndo->setShortcut(QKeySequence(Qt::Key_Control + Qt::Key_Z));
	connect(action_editUndo, SIGNAL(triggered()), this, SLOT(slot_editUndo()));
	menuEdit->addAction(action_editUndo);

	action_editRedo = new QAction(tr("Redo\t Ctrl+Y"), this);
	action_editRedo->setIcon(QIcon("data\\icons\\editRedo.ico"));
	action_editRedo->setShortcut(QKeySequence(Qt::Key_Control + Qt::Key_Y));
	connect(action_editRedo, SIGNAL(triggered()), this, SLOT(slot_editRedo()));
	menuEdit->addAction(action_editRedo);

	menuEdit->addSeparator();
	menuEdit->addAction("Cut");
	menuEdit->addAction("Copy");
	menuEdit->addAction("Paste");

	menuEdit->addSeparator();

	menuEdit->addAction(editor->action_delete);

	menuEdit->addSeparator();

	menuEdit->addAction(editor->action_componentRotateRight);
	menuEdit->addAction(editor->action_componentRotateLeft);

	menuEdit->addSeparator();

	action_editSelectAll = new QAction(tr("&Select All\t Ctrl+A"), this);
	action_editSelectAll->setIcon(QIcon("data\\icons\\editSelectAll.ico"));
	action_editSelectAll->setShortcut(QKeySequence(tr("Ctrl+A")));
	connect(action_editSelectAll, SIGNAL(triggered()), this, SLOT(slot_editSelectAll()));
	menuEdit->addAction(action_editSelectAll);

	QMenu *menuView = this->menuBar()->addMenu(tr("&View"));
	menuView->addAction("Zoom in");
	menuView->addAction("Zoom out");
	menuView->addAction("Zoom to fit");
	menuView->addAction("Reset zoom");
	menuView->addSeparator();
	menuView->addAction("Show grid");
	menuView->addSeparator();
	menuView->addAction("Show toolbar");
	menuView->addAction("Show statusbar");

	// create all the menus
	QMenu *menuSchematic = this->menuBar()->addMenu(tr("&Schematic"));

	QMenu *menuComponentList = menuSchematic->addMenu(tr("Place component"));

	bool foundFile = false;
	// TODO 1) make path a settings entry or something like that
	// TODO !!!! 2) if there's no "data" folder, a nasty exception appears
	// TODO 3) use Qt's built-in directory listing function
	std::string path = "data\\components\\";

	for (auto &p : std::filesystem::directory_iterator(path)) {
		if (p.path().extension() == L".json") {

			std::ifstream ifs(p.path());
			rapidjson::IStreamWrapper isw(ifs);

			rapidjson::Document file;

			file.ParseStream(isw);

			QAction *action = new QAction(file["displayNameBase"].GetString());

			action->setData(QString(p.path().filename().string().c_str())); // TODO what in the name of the unholy fuck

			menuComponentList->addAction(action);

			foundFile = true;

		}
	}

	if (foundFile) {
		connect(menuComponentList, SIGNAL(triggered(QAction*)), this, SLOT(slot_schematicPlaceComponent(QAction*)));
	} else {
		QAction* temp = new QAction("No available components");
		temp->setEnabled(false);
		menuComponentList->addAction(temp);
	}

	menuSchematic->addSeparator();
	menuSchematic->addAction(tr("Add comment"));

	QMenu *menuSimulation = this->menuBar()->addMenu(tr("&Simulation"));

	action_simulationRun = new QAction(tr("Run\tF5"), this);
	action_simulationRun->setIcon(QIcon("data\\icons\\simulationRun.ico"));
	action_simulationRun->setShortcut(QKeySequence(tr("F5")));
	action_simulationRun->setToolTip("Run simulation");
	connect(action_simulationRun, SIGNAL(triggered()), this, SLOT(slot_simulationRun()));
	menuSimulation->addAction(action_simulationRun);

	menuSimulation->addAction("Pause");
	menuSimulation->addAction("Stop");
	menuSimulation->addSeparator();
	menuSimulation->addAction("Settings");

	QMenu *menuTools = this->menuBar()->addMenu(tr("&Tools"));
	menuTools->addAction("Editor settings");

	QMenu *menuWindow = this->menuBar()->addMenu(tr("&Window"));
	menuWindow->addAction("Plot area");
	menuWindow->addAction("Simulation results");
	menuWindow->addAction("Simulation log");
	menuWindow->addSeparator();
	menuWindow->addAction("Debug log");

	QMenu *menuHelp = this->menuBar()->addMenu(tr("&Help"));

	action_aboutQt = new QAction("About Qt", this);
	connect(action_aboutQt, SIGNAL(triggered()), this, SLOT(slot_aboutQt()));
	menuHelp->addAction(action_aboutQt);

	menuHelp->addAction("About Circuit Simulator");



}

void MainWindow::createToolBar() {

	QToolBar* toolbar = new QToolBar();

	toolbar->setWindowTitle("Toolbar");

	//toolbar->setContextMenuPolicy(Qt::PreventContextMenu);

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