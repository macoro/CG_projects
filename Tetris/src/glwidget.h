/*
 *  glwidget.h
 *  
 *
 *  Created by Claudia Esteves Jaramillo on 2/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QTimer>
#include <QGLWidget>
#include "game.hpp"

class GLWidget : public QGLWidget
{
	Q_OBJECT
	
	public:
		GLWidget( QWidget *parent = 0 );
		~GLWidget();
        void setwife(bool wife){ onwife= wife,multicolor=false; updateGL();}
        void setmulticolor(bool multcolor){ multicolor= multcolor,onwife=false; updateGL();}
        void setgamerotatef(float anglex,float angley,float anglez)
           { rg_anglex=anglex; rg_angley=angley;rg_anglez=anglez; updateGL();}
        void moveright(){tetrisgame.moveRight(); updateGL();}
        void moveleft(){tetrisgame.moveLeft(); updateGL();}
        void rotatepiececcw(){tetrisgame.rotateCCW(); updateGL();}
        void rotatepiececw(){tetrisgame.rotateCW(); updateGL();}
        void newgame();
        void resetgame();
        void setspeed(int level);
        void droppiece(){tetrisgame.drop(); updateGL();}
        void scalegame(float gscale){scale+=gscale; updateGL();}
    signals:
        void setspeedcheck(int l);
        void rowsdonechanged(int rows);

	protected:
		// called with GL is first initialized
		void initializeGL();
		void paintGL();
		void resizeGL( int width, int height );
		void timerEvent( QTimerEvent *event );        
private:
        void drawCube();
        void turnwireframe();
        void drawhole(int hwidth, int hheight);
        void rotatefgame();
        void drawgame();
        void setscale();

        bool onwife,multicolor,slow,fast;
        float rg_anglex,rg_angley,rg_anglez;
        int iwidth,iheight,rowsdone;
        Game tetrisgame;
        int timerid;
        float maxscale,minscale,scale;

};

#endif
