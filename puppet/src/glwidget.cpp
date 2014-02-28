/*
 *  glwidget.cpp
 *  
 */
#include <QtGui>
#include <QtOpenGL>

#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
//#include <GL/glut.h>

#include "glwidget.h"
#include "mainwindow.h"

// ask for an OpenGL Setup with:
// - red, green and blue component colour
// - a depth buffer to avoid things overlapping wrongly
// - double-buffered rendering to avoid flickering

GLWidget :: GLWidget( QWidget *parent ) : QGLWidget( QGLFormat( QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba ), parent )
{
    m_drawCircle=0;
    m_doPick = 0;
    id_picked = std::vector<int>();    
}

GLWidget :: ~GLWidget(){ }

void GLWidget :: initializeGL()
{
	// OpenGL setup
	glShadeModel(GL_SMOOTH);
	glClearColor( 0.4, 0.4, 0.4, 0.0 );
}

#define BUFSIZE 512
void GLWidget :: paintGL()
{
	int current_width = width();
	int current_height = height();	

    glViewport( 0, 0, current_width, current_height );

    //if picking is enable
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];
    if(m_doPick){
        glGetIntegerv (GL_VIEWPORT, viewport);
        glSelectBuffer (BUFSIZE, selectBuf);
        (void) glRenderMode (GL_SELECT);
        glInitNames();
        //glPushName(0);
    }

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    /*  create 5x5 pixel picking region near cursor location */
    if(m_doPick)
        gluPickMatrix ((GLdouble) posx, (GLdouble) (viewport[3] - posy),
                       10.0f, 10.0f, viewport);

//    else{
//        glMatrixMode( GL_PROJECTION );
//        glLoadIdentity();
//    }

    // Set up for perspective drawing
    gluPerspective( 60.0, (GLfloat)current_width/(GLfloat)current_height, 1.0, 1000.0 );
	
    //lookAt
    gluLookAt(0.0,0.0,10.0,
              0.0,0.0,0.0,
              0.00,1.0,0.0);
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
	
	// draw stuff
	if(m_scene){
        //glPushName(m_scene->getId());
        m_scene->walk_gl(m_doPick);
	}
	
	if( m_drawCircle ){
		draw_trackball_circle();
    }



    glMatrixMode (GL_PROJECTION);
    glPopMatrix();
    glFlush();
    glMatrixMode(GL_MODELVIEW);
    if(m_doPick){
        hits = glRenderMode (GL_RENDER);
        processHits (hits, selectBuf);
    }

//    else{
//        glMatrixMode (GL_PROJECTION);
//        glPopMatrix();
//        glFlush();
//    }

    // swap the contents of the front and back buffers so we see what we
    // just drew. This should only be done if double buffering is enabled.
    swapBuffers();
    glColor3f(0.0, 0.0, 0.0 );

}

void GLWidget :: resizeGL( int width, int height )
{
	// set up for perspective drawing
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, width, height );
    gluPerspective( 40.0, (GLfloat)width/(GLfloat)height, 0.1, 1000.0 );
	
	// change to model view for drawing
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void GLWidget :: draw_trackball_circle()
{
    int current_width = width();
    int current_height = height();

    // set up for orthogonal drawing to draw a circle on screen.
    // You'll want to make the rest of the function conditional on
    // whether or not we want to draw the circle this time  around
    glMatrixMode( GL_PROJECTION);
    glLoadIdentity();
    glViewport( 0, 0, current_width, current_height);
    glOrtho( 0.0, (float)current_width, 0.0, (float)current_height, -1000, 1000);

    // change to model view for drawing
    glMatrixMode(GL_MODELVIEW);

    // reset modelview matrix
    glLoadIdentity();

    // draw a circle for use with the trackball
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glColor4f (1.0f, 1.0f, 1.0f, 0.5f); // blue with 50% opacity

    double radius = current_width < current_height ?
                (float)current_width * 0.25 : (float)current_height * 0.25;
    glTranslated((float)current_width * 0.5, (float)current_height * 0.5, 0);
    glScaled(radius,radius,radius);
    Sphere trackball;
    trackball.walk_gl(false);
    glDisable (GL_BLEND);
}

void GLWidget :: toggleCircle( bool b )
{
	m_drawCircle = b;
	updateGL();
}

void GLWidget :: toggleZBuffer( bool b )
{
	if(b){
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	} else {
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
	}
	updateGL();
}

void GLWidget :: toggleBackfaceCull( bool b )
{
	GLint cur;
	glGetIntegerv( GL_CULL_FACE_MODE, &cur );
	GLboolean enabled = glIsEnabled( GL_CULL_FACE );
	
	if(b){
		if( enabled == false ){
			glEnable(GL_CULL_FACE);
			glCullFace( GL_BACK );
		} else {
			if( cur == GL_FRONT ){
				glCullFace( GL_FRONT_AND_BACK );
			}
		}
	} else if (enabled){
		if( cur == GL_FRONT_AND_BACK){
			glCullFace( GL_FRONT );
		} else {
			glDisable( GL_CULL_FACE );
		}
	}
	updateGL();
}

void GLWidget :: toggleFrontfaceCull( bool b )
{
	GLint cur;
	glGetIntegerv( GL_CULL_FACE_MODE, &cur );
	GLboolean enabled = glIsEnabled( GL_CULL_FACE );
	
	if(b){
		if( enabled == false ){
			glEnable( GL_CULL_FACE );
			glCullFace( GL_FRONT );
		} else {
			if( cur == GL_BACK ){
				glCullFace( GL_FRONT_AND_BACK );
			}
		}
	} else if( enabled ){
		if( cur == GL_FRONT_AND_BACK ){
			glCullFace( GL_BACK );
		} else {
			glDisable( GL_CULL_FACE );
		}
	}
	updateGL();
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

void GLWidget::processHits(GLint hits, GLuint buffer[]){
      unsigned int i, j;
      GLuint names, *ptr;

      printf ("hits = %d\n", hits);
      ptr = (GLuint *) buffer;
      for (i = 0; i < hits; i++) { /*  for each hit  */
         names = *ptr;
         printf (" number of names for hit = %d\n", names); ptr++;
         printf("  z1 is %g;", (float) *ptr/0x7fffffff); ptr++;
         printf(" z2 is %g\n", (float) *ptr/0x7fffffff); ptr++;
         if(hits > 0){
             size_t node_id = *ptr;
             if(node_id > 1 && node_id < 34 ){
                 printf (" node picked id is %d", *ptr);
                 //const std::string& jname = m_nodeMap[node_id]->getName();
                 if(m_nodeMap[node_id]->get_parent()->is_joint()){
                     bool bepicked = !m_nodeMap[node_id]->is_picked();
                     size_t pid = m_nodeMap[node_id]->get_parent()->getId();
                     if(bepicked)
                         id_picked.push_back(pid);
                     else{
                         for( std::vector<int>::iterator k = id_picked.begin();k != id_picked.end(); k++ ){
                             if((*k)== pid){
                                 id_picked.erase(k);
                                 break;
                             }
                         }
                     }
                     m_nodeMap[node_id]->set_picked(bepicked);
                 }
             }
         }
         printf ("\n");
      }
}
	
void GLWidget::clearpicked(){
    for( std::vector<int>::iterator k = id_picked.begin();k != id_picked.end(); k++ )
        m_nodeMap[(*k)]->set_picked(false);
    id_picked.clear();
    updateGL();
}

void GLWidget::get_itemspicked(std::vector<SceneNode*> &items){
    for( std::vector<int>::iterator k = id_picked.begin();k != id_picked.end(); k++ )
        items.push_back(m_nodeMap[(*k)]);
}
