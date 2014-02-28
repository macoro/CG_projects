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

#include <QGLWidget>
#include <vector>
#include <set>
#include <map>

#include "algebra.h"
#include "scene.h"


// main OpenGL widget
class GLWidget : public QGLWidget
{
	Q_OBJECT
	
	public:
		GLWidget( QWidget *parent = 0 );
		~GLWidget();
		
		void setScene( SceneNode* scene );
		
		void toggleCircle( bool b );
		void toggleZBuffer( bool b );
		void toggleBackfaceCull( bool b );
		void toggleFrontfaceCull( bool b );
        bool get_dopick(){ return m_doPick;}
        void set_dopick(bool dopick){ m_doPick = dopick;}
        void get_itemspicked(std::vector<SceneNode*>& items);
        void processHits (GLint hits, GLuint buffer[]);
        void clearpicked();
        void setposition(int x, int y){ posx = x; posy=y; }
        std::vector<int> id_picked;


	protected:
		// called with GL is first initialized
		void initializeGL();
		// called when the window needs to be redrawn
		void paintGL();
		// called when the window is resized
		void resizeGL( int width, int height );
		
	private:
		bool m_drawCircle;
		SceneNode* m_scene;
        bool m_doPick;        
        int posx,posy;

		std::map< unsigned int, SceneNode* > m_nodeMap;
	
		void draw_trackball_circle();
};

#endif
