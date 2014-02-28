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
#include "glWidget.h"

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
		void resetAll();
		void setTrackballMode( QAction *action );
		void toggleTrackball();
		void toggleZBuffer();
		void toggleBackfaceCull();
		void toggleFrontfaceCull();
		void undo();
		void redo();
				
	private:
		SceneNode* m_sceneNode;
		bool m_doPick;
        std::map<std::string,JointNode*> m_jointMap;
        std::vector<SceneNode*> m_jointpicked;
        struct itemstack{bool yhead;std::vector<SceneNode*> jpicked; double angle;};
        std::vector<itemstack> m_jointstack;
        double angle;
        double headangle;
        size_t s_capacity;
        int s_cursor ;


		// central widget that holds glWidget
		QWidget *centralWidget;
		
		// the main OpenGL area
		GLWidget *glWidget;
	
		void createActions();
		void createMenus();
		void createStatusBar();
		
		// Menus
		QMenu *appMenu;
		QMenu *resetMenu;
		QMenu *modeMenu;
		QMenu *editMenu;
		QMenu *optionsMenu;
		
		// Actions
		QAction *resetPosAct;
		QAction *resetOrientAct;
		QAction *resetJointsAct;
		QAction *resetAllAct;
		QAction *exitAct;
		QAction *posOrientAct;
		QAction *jointsAct;
		QAction *undoAct;
		QAction *redoAct;
		QAction *toggleCircleAct;
		QAction *toggleZbufferAct;
		QAction *toggleBackfaceCullAct;
		QAction *toggleFrontfaceCullAct;       
        // util for positions
        bool rightdown, middledown, leftdown;
        int initx,inity,initz,itrackx,itracky;
        // for trackball transform
        void vPerformTransfo(float, float, float, float);
        int nWinWidth, nWinHeight;
        void apply_undo(itemstack state, bool redo);

protected:
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
};

#endif
