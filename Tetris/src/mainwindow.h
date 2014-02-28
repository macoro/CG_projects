/*
 *  mainwindow.h
 *  
 *
 *  Created by Claudia Esteves Jaramillo on 2/14/09.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class QMenu;

class GLWidget;

class MainWindow : public QMainWindow 
{
	Q_OBJECT
	
	public:
		MainWindow();

    public slots:
        void checkSpeed(int level);
        void setlabeltext(int rows);
	private slots:
		void newGame();
		void resetGame();
		void setDrawingMode( QAction *action);
		void setSpeed( QAction *action );
		void buffering( bool );
		
	protected:
		void mousePressEvent( QMouseEvent *event );
        void keyPressEvent( QKeyEvent *e );
        void mouseMoveEvent(QMouseEvent * e);
        void keyReleaseEvent(QKeyEvent *);
        void mouseReleaseEvent(QMouseEvent * event);
	
	private:
		void createActions();
		void createMenus();
	
		// menus
		QMenu *fileMenu;
		QMenu *drawMenu;
		QMenu *speedMenu;
		QMenu *bufferingMenu;
		
		// actions 
		QAction *newAct;
		QAction *resetAct;
		QAction *exitAct;
		
		// example of checkable buttons
		QAction *wireAct;
		QAction *fillAct;
		QAction *multiAct;

        QAction *slowAct;
        QAction *mediumAct;
        QAction *fastAct;

		QAction *bufferAct;
	
		QWidget *centralWidget;
        QLabel *lines;
		GLWidget *glWidget; // OpenGLwidget
		
		int timerid;
        int anglex,angley,anglez;
        bool onwire,shiftpress,leftpress;
        int iposx;
};

#endif
