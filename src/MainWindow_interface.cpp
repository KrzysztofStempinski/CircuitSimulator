#include "MainWindow.h"

#include "SimulationResultsWindow.h"
#include "Version.h"

#include <filesystem>

#include <qdockwidget.h>
#include <qcombobox.h>
#include <QMenu>
#include <QMenuBar>
#include <qstatusbar.h>

#include "../rapidjson/rapidjson.h"
#include "../rapidjson/istreamwrapper.h"

const QString ICON_PATH = "data\\icons\\";
const std::string COMPONENT_PATH = "data\\components\\"; // TODO move to Settings or sth like that

void MainWindow::createInterface() {

	setWindowIcon(QIcon(ICON_PATH + "appicon.ico"));
	setWindowTitle("untitled.esf - Schematic Editor | CircuitSimulator v." + VersionInfo::getVersionString());
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

	action_componentProperties = new QAction("Component properties", this);
	action_componentProperties->setIcon(QIcon(ICON_PATH + "properties.ico"));
	action_componentProperties->setShortcut(QKeySequence("P"));
	connect(action_componentProperties, SIGNAL(triggered()), this, SLOT(slot_componentProperties()));
	addAction(action_componentProperties);

	action_createMidpointNode = new QAction("Create midpoint node", this);
	action_createMidpointNode->setShortcut(QKeySequence("M"));
	connect(action_createMidpointNode, SIGNAL(triggered()), this, SLOT(slot_createMidpointNode()));
	addAction(action_createMidpointNode);

	// file actions
	action_fileNew = new QAction("New", this);
	action_fileNew->setIcon(QIcon(ICON_PATH + "fileNew.ico"));
	action_fileNew->setShortcut(QKeySequence("Ctrl+N"));
	connect(action_fileNew, SIGNAL(triggered()), this, SLOT(slot_fileNew()));
	addAction(action_fileNew);

	action_fileOpen = new QAction("Open", this);
	action_fileOpen->setIcon(QIcon(ICON_PATH + "fileOpen.ico"));
	action_fileOpen->setShortcut(QKeySequence("Ctrl+O"));
	connect(action_fileOpen, SIGNAL(triggered()), this, SLOT(slot_fileOpen()));
	addAction(action_fileOpen);

	action_fileSave = new QAction("Save", this);
	action_fileSave->setIcon(QIcon(ICON_PATH + "fileSave.ico"));
	action_fileSave->setShortcut(QKeySequence("Ctrl+S"));
	connect(action_fileSave, SIGNAL(triggered()), this, SLOT(slot_fileSave()));
	addAction(action_fileSave);

	action_fileSaveAs = new QAction("Save As...", this);
	action_fileSaveAs->setIcon(QIcon(ICON_PATH + "fileSaveAs.ico"));
	action_fileSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
	connect(action_fileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileSaveAs()));
	addAction(action_fileSaveAs);

	action_fileExit = new QAction("Exit", this);
	action_fileExit->setShortcut(QKeySequence("Alt+F4"));
	connect(action_fileExit, SIGNAL(triggered()), this, SLOT(slot_fileExit()));
	addAction(action_fileExit);
	//

	// edit actions
	action_editUndo = new QAction("Undo", this);
	action_editUndo->setIcon(QIcon(ICON_PATH + "editUndo.ico"));
	action_editUndo->setShortcut(QKeySequence(Qt::Key_Control + Qt::Key_Z));
	connect(action_editUndo, SIGNAL(triggered()), this, SLOT(slot_editUndo()));
	addAction(action_editUndo);

	action_editRedo = new QAction("Redo", this);
	action_editRedo->setIcon(QIcon(ICON_PATH + "editRedo.ico"));
	action_editRedo->setShortcut(QKeySequence(Qt::Key_Control + Qt::Key_Y));
	connect(action_editRedo, SIGNAL(triggered()), this, SLOT(slot_editRedo()));
	addAction(action_editRedo);

	action_editSelectAll = new QAction("Select All", this);
	action_editSelectAll->setIcon(QIcon(ICON_PATH + "editSelectAll.ico"));
	action_editSelectAll->setShortcut(QKeySequence(Qt::Key_Control + Qt::Key_A));
	connect(action_editSelectAll, SIGNAL(triggered()), this, SLOT(slot_editSelectAll()));
	addAction(action_editSelectAll);
	//

	// simulation actions
	action_simulationRun = new QAction("Run", this);
	action_simulationRun->setIcon(QIcon(ICON_PATH + "simulationRun.ico"));
	action_simulationRun->setShortcut(QKeySequence("F5"));
	action_simulationRun->setToolTip("Run simulation");
	connect(action_simulationRun, SIGNAL(triggered()), this, SLOT(slot_simulationRun()));
	addAction(action_simulationRun);
	//

	// help actions
	action_aboutQt = new QAction("About Qt", this);
	connect(action_aboutQt, SIGNAL(triggered()), this, SLOT(slot_aboutQt()));
	//

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

// TODO ! error handling in JSON
void MainWindow::populateComponents() {

	toolbarComponents = new QToolBar();
	toolbarComponents->setIconSize(QSize(40, 40));
	toolbarComponents->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	addToolBar(Qt::LeftToolBarArea, toolbarComponents);

	bool foundFile = false;

	for (auto &p : std::filesystem::directory_iterator(COMPONENT_PATH)) {
		if (p.path().extension() == L".json") {

			// load and parse the JSON component definition file
			std::ifstream ifs(p.path());
			rapidjson::IStreamWrapper isw(ifs);

			rapidjson::Document file;
			file.ParseStream(isw);

			// For instance "Voltage Source" - that's what's displayed in menu
			QAction* action = new QAction(file["displayNameBase"].GetString());

			// TODO what in the name of the unholy fuck is with these strings
			// TODO maybe we should just do "resistor", as in, chop 5 last chars here?
			// for instance, resistor.json
			action->setData(QString(p.path().filename().string().c_str())); 

			// load geometry and draw it to produce a nice thumnbai/icon
			GeometryList geometry(file["display"]);

			QPixmap* pixmap = new QPixmap(36, 36);
			pixmap->fill(Qt::transparent);

			QPainter* painter = new QPainter(pixmap);
			painter->setPen(Qt::cyan);	
				   
			for (const auto& it : geometry.objects)
				it->draw(*painter, QPoint(18, 18));

			action->setIcon(QIcon(*pixmap));

			// finalize
			menuComponents->addAction(action);
			toolbarComponents->addAction(action);

			foundFile = true;

		}
	}

	if (foundFile) {
		connect(menuComponents, SIGNAL(triggered(QAction*)), this, SLOT(slot_schematicPlaceComponent(QAction*)));
	} else {
		QAction* temp = new QAction("No available components");
		temp->setEnabled(false);
		menuComponents->addAction(temp);
	}

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