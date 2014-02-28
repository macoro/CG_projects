/*
 *  glwidget.cpp
 *  
 *
 *  Created by Claudia Esteves Jaramillo on 2/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <QtGui>
#include <QtOpenGL>
#include <GLUT/glut.h>
#include <GL/glu.h>
#include <iostream>
#include <math.h>
#include "algebra.hpp"
#include "glwidget.h"

const int lmedium=3;
const int lfast=10;

using namespace std;
//Color pieces and pieces faces
static const Colour pcolors[]={
    Colour(16/255.0,78/255.0,139/255.0),
    Colour(255/255.0,64/255.0,64/255.0),
    Colour(34/255.0,139/255.0,34/255.0),
    Colour(93/255.0,71/255.0,139/255.0),
    Colour(255/255.0,185/255.0,15/255.0),
    Colour(92/255.0,64/255.0,51/255.0),
    Colour(128/255.0,0.0,0.0),
    Colour(0.4,0.4,0.4)

};
static const Colour pfacescolors[]={
    Colour(255/255.0,218/255.0,185/255.0),
    Colour(255/255.0,140/255.0,0.0),
    Colour(127/255.0,255/255.0,0.0),
    Colour(0.0,255/255.0,255/255.0),
    Colour(138/255.0,43/255.0,226/255.0)
};



/* default QGLFormat enables Depth and double buffers, use it explicitely here to show how it is used */
GLWidget :: GLWidget( QWidget *parent ) : QGLWidget( QGLFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba ), parent ),
    iwidth(10),iheight(24),tetrisgame(iwidth,iheight-4)
{
    onwife=false;
    multicolor=false;
    timerid=startTimer(500);
    rowsdone=0;
    slow=true;
    fast=false;
    minscale=0.25;
    maxscale=1.75;
    scale=1;
}

void GLWidget::newgame(){

    tetrisgame.reset();
    onwife=false;
    multicolor=false;
    rg_anglex=rg_angley=rg_anglez=0;
    rowsdone=0;
    killTimer(timerid);
    timerid=startTimer(500);
    slow=true;
    fast=false;
    scale=1.0;
    emit(setspeedcheck(1));
    emit(rowsdonechanged(0));
    updateGL();
}

void GLWidget::resetgame(){
    tetrisgame.reset();
    rg_anglex=rg_angley=rg_anglez=0;
    rowsdone=0;
    killTimer(timerid);
    timerid=startTimer(500);
    slow=true;
    fast=false;
    scale=1.0;
    emit(setspeedcheck(1));
    emit(rowsdonechanged(0));
    updateGL();
}

GLWidget :: ~GLWidget(){ }

void GLWidget :: initializeGL()
{
	// OpenGL setup
	glEnable( GL_DEPTH_TEST );
	glClearColor( 0.7f, 0.7f, 1.0f, 0.0f );    
}

void GLWidget :: paintGL()
{
	// clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glTranslated(0.0, 0.0, -40.0);
	
	glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

	// not implemented: set up lighting (if necessary)
	// not implemented: scale and rotate the scene
    rotatefgame();
    setscale();
    turnwireframe();
    drawhole(10,24);
    drawgame();
    glPopMatrix();
	// we pushed a matrix onto the PROJECTION stack earlier, we need to pop it
	glMatrixMode( GL_PROJECTION);
	glPopMatrix();
	
	// swap the contents of the front and back buffers so we see what we
	// just drew. This should only be done if double buffering is enabled.
	swapBuffers();
}

void GLWidget::drawCube(){
    double currentcolor[3];
    glGetDoublev(GL_CURRENT_COLOR,currentcolor);
    glBegin(GL_QUADS);
        //front
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 1.0, 0.0);
        glVertex3d(1.0, 1.0, 0.0);
        glVertex3d(1.0,0.0, 0.0);
        //floor
        if(multicolor)
            glColor3d(pfacescolors[0].R(),pfacescolors[0].G(),pfacescolors[0].B());
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, -1.0);
        glVertex3d(1.0, 0.0, -1.0);
        glVertex3d(1.0,0.0, 0.0);        
        //right
        if(multicolor)
            glColor3d(pfacescolors[1].R(),pfacescolors[1].G(),pfacescolors[1].B());
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 1.0, 0.0);
        glVertex3d(0.0, 1.0, -1.0);
        glVertex3d(0.0,0.0, -1.0);
        //top
        if(multicolor)
            glColor3d(pfacescolors[2].R(),pfacescolors[2].G(),pfacescolors[2].B());
        glVertex3d(0.0, 1.0, 0.0);
        glVertex3d(1.0, 1.0, 0.0);
        glVertex3d(1.0,1.0, -1.0);
        glVertex3d(0.0, 1.0, -1.0);        
        //left
        if(multicolor)
            glColor3d(pfacescolors[3].R(),pfacescolors[3].G(),pfacescolors[3].B());
        glVertex3d(1.0, 0.0, 0.0);
        glVertex3d(1.0, 1.0, 0.0);
        glVertex3d(1.0, 1.0, -1.0);
        glVertex3d(1.0,0.0, -1.0);
        //back
        if(multicolor)
            glColor3d(pfacescolors[4].R(),pfacescolors[4].G(),pfacescolors[4].B());
        glVertex3d(0.0, 0.0, -1.0);
        glVertex3d(0.0, 1.0, -1.0);
        glVertex3d(1.0, 1.0, -1.0);
        glVertex3d(1.0,0.0, -1.0);
    glEnd();
    glColor3d(currentcolor[0],currentcolor[1],currentcolor[2]);
}
void GLWidget::turnwireframe(){
    if(onwife){
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //onwife=false;
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        //onwife=true;
    }
}
void GLWidget::drawhole(int hwidth, int hheight){
    double currentcolor[3];
    glGetDoublev(GL_CURRENT_COLOR,currentcolor);
    glColor3d(pcolors[7].R(),pcolors[7].G(),pcolors[7].B());
    glPushMatrix();
        //leftcolumn
        glTranslatef(-(hwidth/2+1),0,0);
        for(int i=0;i<hheight/2-4;i++){
            drawCube();
            glTranslatef(0,1,0);
        }
        glTranslatef(0,-(hheight/2-4),0);
        for(int i=0;i<hheight/2+2;i++){
            drawCube();
            glTranslatef(0,-1,0);
        }
        glTranslatef(hwidth/2+1,hheight/2+2,0.0);

        //rightcolumn
        glTranslatef(hwidth/2,0,0);
        for(int i=0;i<hheight/2-4;i++){
            drawCube();
            glTranslatef(0,1,0);
        }
        glTranslatef(0,-(hheight/2-4),0);
        for(int i=0;i<hheight/2+2;i++){
            drawCube();
            glTranslatef(0,-1,0);
        }

        //floor
        glTranslatef(-hwidth,1,0);
        for(int i=0;i<hwidth;i++){
            drawCube();
            glTranslatef(1,0,0);
        }
    glPopMatrix();
    glColor3d(currentcolor[0],currentcolor[1],currentcolor[2]);
}

void GLWidget::drawgame(){
    //save current color
    double currentcolor[3];
    glGetDoublev(GL_CURRENT_COLOR,currentcolor);
    for(int i=0;i<iheight;i++){
        for(int j=0;j<iwidth;j++){
            int c=tetrisgame.get(i,j);
            if(c >-1){
                int y= i - (iheight/2);
                int x= j - (iwidth/2);
                glColor3d(pcolors[c].R(),pcolors[c].G(),pcolors[c].B());
                glPushMatrix();
                    glTranslatef(x,y,0);
                    drawCube();
                glPopMatrix();
            }
        }
    }
    //restore color
    glColor3d(currentcolor[0],currentcolor[1],currentcolor[2]);
}

void GLWidget::setscale(){
    if(scale < minscale)
        glScalef(minscale,minscale,minscale);
    else if(scale>maxscale)
        glScalef(maxscale,maxscale,maxscale);
    else
        glScalef(scale,scale,scale);
}

void GLWidget :: resizeGL( int width, int height )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective( 40.0, (GLfloat)width / (GLfloat)height, 0.1, 1000.0);	
	glMatrixMode( GL_MODELVIEW);
}

void GLWidget :: timerEvent( QTimerEvent * )
{
    int remrows= tetrisgame.tick();
    rowsdone+= remrows;

    if(remrows<0){
        killTimer(timerid);
        emit(rowsdonechanged(-1));
    }
    else if(remrows>0)
        emit(rowsdonechanged(rowsdone));
    else if(slow && rowsdone > lmedium && rowsdone < lfast ){
        killTimer(timerid);
        timerid=startTimer(250);
        slow=false;
        emit(setspeedcheck(2));
    }
    else if(!fast && rowsdone >= lfast){
        killTimer(timerid);
        timerid=startTimer(100);
        fast=true;
        emit(setspeedcheck(3));
    }   
    updateGL();
}

void GLWidget::rotatefgame(){
    glRotatef(rg_anglex,1,0,0);
    glRotatef(rg_angley,0,1,0);
    glRotatef(rg_anglez,0,0,1);
}

void GLWidget::setspeed(int level){
    switch(level)
    {
    case 1:
        killTimer(timerid);
        timerid=startTimer(500);
        break;
    case 2:
        killTimer(timerid);
        timerid=startTimer(250);
        break;
    case 3:
        killTimer(timerid);
        timerid=startTimer(100);
        break;
    default:
        break;
    }
}
