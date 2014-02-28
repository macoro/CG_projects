/*
 *  main.cpp
 *  
 *  Main Application
 *
 */

#include <iostream>
#include <QApplication>
#include <QtOpenGL>
#include "mainwindow.h"
#include "scene_lua.h"

int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );
	
	// verifies if OpenGL is supported by this system
	if( !QGLFormat::hasOpenGL() ){
		qWarning("This system has no OpenGL support. Exiting. ");
		return(-1);
	}
	
    // Read default file scene.lua if no other name is given as input.
    std::string filename = "scene.lua";
	if( argc >= 2 ){
		filename = argv[1];
	}

	// This is how you might import a scene
	SceneNode* root = import_lua( filename );
	if( !root ){
		std::cerr << "Could not open file " << filename << std::endl;
		return 1;
	}

	// Construct the main window
	MainWindow window;
	
	// Version 3. Set the scene */
    window.setScene( root );

	// And run the application
	window.show();
    return app.exec();
}

