#pragma once

#include "Circuit.h"
#include "CircuitEditor.h"

#include "LogWindow.h"

#include <QMainWindow>
#include <QWidget>

constexpr int MAINWINDOW_DEF_WIDTH = 1440;
constexpr int MAINWINDOW_DEF_HEIGHT = 900;

class MainWindow : public QMainWindow {

	private:

		LogWindow *logWindow;

		QAction* action_componentProperties;

		QAction* action_createMidpointNode;
		QAction* action_nextNode;
		QAction* action_connectToClosestNode;
				 
		QAction* action_fileNew;
		QAction* action_fileOpen;
		QAction* action_fileSave;
		QAction* action_fileSaveAs;
		QAction* action_fileExit;
				 
		QAction* action_schematicPlaceComponent;
				 
		QAction* action_editUndo;
		QAction* action_editRedo;
				 
		QAction* action_editSelectAll;
				 
		QAction* action_simulationRun;

		QDockWidget *dockComponentProperties;
		QDockWidget *dockSimulationResults;
		QDockWidget *dockLog;

		QAction* action_aboutQt;

    Q_OBJECT
	public:
		
		explicit MainWindow();

		void createDockWidgets();
		void createMenu();
		void createToolBar();
		void createStatusBar();

		CircuitEditor* editor;

	signals:
		
	public slots:

		void slot_fileOpen();
		void slot_fileSaveAs();
		void slot_fileExit();

		void slot_schematicPlaceComponent(QAction *action);

		void slot_createMidpointNode();

		void slot_simulationRun();

		void slot_componentProperties();

		void slot_aboutQt();


};
