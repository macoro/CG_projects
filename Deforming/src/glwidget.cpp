/*
 *  glwidget.cpp
 *  
 */
#include <QtGui>
#include <QtOpenGL>

#include <cmath>
#include <iostream>

#include "GL/glut.h"
#include "glwidget.h"
#include "mainwindow.h"

// ask for an OpenGL Setup with:
// - red, green and blue component colour
// - a depth buffer to avoid things overlapping wrongly
// - double-buffered rendering to avoid flickering

GLWidget :: GLWidget( QWidget *parent ) : QGLWidget( QGLFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba ), parent ){

}

GLWidget :: ~GLWidget(){ }

void GLWidget :: initializeGL()
{
	// OpenGL setup
	glShadeModel(GL_SMOOTH);
    glClearColor( 0.6, 0.7, 0.6, 0.0 );
}

void GLWidget :: paintGL()
{
    int current_width = width();
    int current_height = height();

    // Set up for perspective drawing
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glViewport( 0, 0, current_width, current_height );
    gluPerspective( 60.0, (GLfloat)current_width/(GLfloat)current_height, 1.0, 1000.0 );

    //lookAt
    gluLookAt(0.0,0.0,30,
              0.0,0.0,0.0,
              0.00,1.0,0.0);

    //Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // change to model view for drawing
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Clear framebuffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Set up Lighting
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glEnable( GL_NORMALIZE );
    glShadeModel( GL_SMOOTH );
    GLfloat lp[] = {1.0f, 1.0f, 0.0f, 0.0f };
    glLightfv( GL_LIGHT0, GL_POSITION, lp );
    GLfloat la[] = {0.01f, 0.01f, 0.01f, 1.0f };
    glLightfv( GL_LIGHT0, GL_AMBIENT, la );
    GLfloat ld[] = {1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_DIFFUSE, ld);
    GLfloat ls[] = {1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv( GL_LIGHT0, GL_SPECULAR, ls );
    glEnable( GL_LIGHT1 );
    GLfloat lp1[] = {-10.0f, -10.0f, 0.0f, 0.0f };
    glLightfv( GL_LIGHT1, GL_POSITION, lp1 );
    glLightfv( GL_LIGHT1, GL_AMBIENT, la );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, ld);
    glLightfv( GL_LIGHT1, GL_SPECULAR, ls );

    // draw stuff
    if(m_scene){
        m_scene->walk_gl();       
    }

    glFlush();
    // swap the contents of the front and back buffers so we see what we
    // just drew. This should only be done if double buffering is enabled.
    swapBuffers();

    glColor3f(0.0, 0.0, 1.0 );
}

void GLWidget :: resizeGL( int width, int height )
{
	// set up for perspective drawing
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, width, height );
    gluPerspective( 60.0, (GLfloat)width/(GLfloat)height, 1.0, 1000.0 );
	
	// change to model view for drawing
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void GLWidget :: setScene( SceneNode* scene )
{
	m_scene = scene;
	if( m_scene ){
		//m_scene -> clear_all_inverse();
		m_nodeMap.clear();
		m_scene -> getAllNodes( m_nodeMap );
	}
}


	

