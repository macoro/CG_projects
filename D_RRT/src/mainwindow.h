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
#include "rrt.h"

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
		void resetPos();
		void resetOrient();
		void resetJoints();		
        void toggleTrackball();
		void toggleZBuffer();
		void toggleBackfaceCull();

	private:
		SceneNode* m_sceneNode;
        SWIFT_Scene* m_swiftscene;
        rrt* m_rrt;
        bool solution;

		bool m_doPick;
        //std::map<std::string,JointNode*> m_jointMap;

        std::vector< std::vector<double> > m_path;
        std::vector<node_data*> m_dfndata;
	
		// central widget that holds glWidget
		QWidget *centralWidget;
		
		// the main OpenGL area
		GLWidget *glWidget;
	
		void createActions();
		void createMenus();
		void createStatusBar();
		
		// Menus
		QMenu *appMenu;        
        QMenu *modeMenu;
		QMenu *optionsMenu;
		
		// Actions
		QAction *resetPosAct;
		QAction *resetOrientAct;
		QAction *resetJointsAct;		
		QAction *exitAct;
        QAction *toggleCircleAct;
		QAction *toggleZbufferAct;
		QAction *toggleBackfaceCullAct;
		QAction *toggleFrontfaceCullAct;
        QAction *scalablemodelAct;
        QAction *deepmodelAct;

        bool rightdown, middledown, leftdown;
        int initx,inity,initz;

protected:
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
        void keyPressEvent( QKeyEvent *e );
        void keyReleaseEvent(QKeyEvent *);

};

#endif
