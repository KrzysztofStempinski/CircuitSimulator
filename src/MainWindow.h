#pragma once

#include "Circuit.h"
#include "CircuitEditor.h"

#include <QMainWindow>
#include <QWidget>
#include <qtoolbar.h>

constexpr int MAINWINDOW_DEF_WIDTH = 1440;
constexpr int MAINWINDOW_DEF_HEIGHT = 900;

class MainWindow : public QMainWindow {

	QAction* action_componentProperties;

	QAction* action_fileNew;
	QAction* action_fileOpen;
	QAction* action_fileSave;
	QAction* action_fileSaveAs;
	QAction* action_fileExit;

	QAction* action_schematicPlaceComponent;

	QAction* action_editUndo;
	QAction* action_editRedo;
	QAction* action_editCopy;
	QAction* action_editPaste;

	QAction* action_editSelectAll;

	QAction* action_simulationRun;

	QDockWidget* dockComponentProperties;
	QDockWidget* dockSimulationResults;
	QDockWidget* dockLog;

	QAction* action_aboutQt;

	QMenu* menuComponents;
	QToolBar* toolbarComponents;

	void populateComponents();
	void createInterface();
	void createDockWidgets();
	void createMenu();
	void createToolBar();
	void createStatusBar();

	Q_OBJECT
public:

	explicit MainWindow();

	CircuitEditor* editor;

	rapidjson::Document file;

signals:

public slots:

	void slot_fileOpen();
	void slot_fileSaveAs();
	void slot_fileExit();

	void slot_editCopy();
	void slot_editPaste();

	void slot_schematicPlaceComponent(QAction* action);

	void slot_simulationRun();

	void slot_componentProperties();

	void slot_aboutQt();
};
