/*
 *  mainwindow.h
 *  User interface
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>

#include "scene.h"
#include "glwidget.h"


class QMenu;
class QAction;
class QLabel;
class QGroupBox;

class GLWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
	public:
		MainWindow();
	
		/* Version 3 */
		void setScene( SceneNode* root );
	
    private slots:
        void move_controlpoint();

				
	private:
		SceneNode* m_sceneNode;

		bool m_doPick;
		std::map<std::string,JointNode*> m_jointMap;
	
		// central widget that holds glWidget
		QWidget *centralWidget;
		
		// the main OpenGL area
		GLWidget *glWidget;
	
		void createActions();
		void createMenus();
		void createStatusBar();
		
        // Menus
        QMenu *appMenu;
		
        // Actions
        QAction *move_contropointAct;
};

#endif
