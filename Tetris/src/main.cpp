/*
 *  main.cpp
 *  
 *
 *  Created by Claudia Esteves Jaramillo on 2/14/09.
 *  Main application for the Tetrominoes Game.
 *
 */

#include <QApplication>
#include <QtOpenGL>

#include "mainwindow.h"

int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );
	
	//verifies if OpenGL is supported by this system
	if( !QGLFormat::hasOpenGL() ){
		qWarning("This system has no OpenGL support. Exiting.");
		return -1;
	}
	
	MainWindow window;
	window.show();
	return app.exec();
}



