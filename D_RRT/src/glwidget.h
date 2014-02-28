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
		
        void toggleDrawPaht( bool b, std::vector<std::vector<double> >& path);
        void toggleBeginTree( bool b );
        void toggleEndTree( bool b );
        kdtree *begintree,*endtree;


	protected:
		// called with GL is first initialized
		void initializeGL();
		// called when the window needs to be redrawn
		void paintGL();
		// called when the window is resized
		void resizeGL( int width, int height );
		
	private:
        SceneNode* m_scene;
        bool m_drawpath;
        bool m_begintree;
        bool m_endtree;
        std::vector<std::vector<double> > m_path;

		std::map< unsigned int, SceneNode* > m_nodeMap;

};

#endif
