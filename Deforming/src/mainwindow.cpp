/*
 *  mainwindow.cpp
 *  
 *
 */

#include <QtGui>
#include <iostream>
#include <string>

#include "glwidget.h"
#include "mainwindow.h"

using namespace std;

// Class MainWindow

MainWindow :: MainWindow()
{
	centralWidget = new QWidget;
	centralWidget -> setMinimumSize(300,300);
	setCentralWidget( centralWidget );
	
	glWidget = new GLWidget;
	
	QGridLayout *centralLayout = new QGridLayout;
	centralLayout -> addWidget( glWidget, 0, 0);
	centralWidget -> setLayout( centralLayout );
	
	createActions();
	createMenus();
	createStatusBar();
	
	m_sceneNode = NULL;
	m_doPick = true;

    setWindowTitle( tr("Deformable objects"));
	setMinimumSize( 600, 600 );
	resize(600, 600);    
}
		
//void MainWindow :: reset()
//{
//}

void MainWindow :: createActions()
{
    move_contropointAct = new QAction( tr("Move control point"), this);
    move_contropointAct -> setShortcut(tr("Ctrl+M"));
    move_contropointAct -> setStatusTip(tr("Move control point"));
    connect( move_contropointAct, SIGNAL(triggered()), this, SLOT(move_controlpoint()) );

}

void MainWindow :: createMenus()
{
    // Applications menu
    appMenu = menuBar() -> addMenu( tr("Application") );
    appMenu -> addAction(move_contropointAct);

}

void MainWindow :: createStatusBar()
{
    //statusBar() -> showMessage(tr(" "));
}

/* Version 3 */
void MainWindow :: setScene( SceneNode* root )
{
    glWidget->setScene(root);
    m_sceneNode = root;
}

void MainWindow::move_controlpoint(){
    int index = 4;
    double dx = -0.6;
    double dy = 0.6;
    double dz = -0.6;
    int steps = 10;

    double inc_x = dx/steps;
    double inc_y = dy/steps;
    double inc_z = dz/steps;

//    for(int i = 0; i< steps; i++){
//        m_sceneNode->move_controlpoint(index,inc_x, inc_y, inc_z);
//        glWidget->updateGL();
//    }

    for(int i = 0; i< steps; i++){
        m_sceneNode->move_controlpoint(4,inc_x, inc_y, inc_z);
        glWidget->updateGL();
    }

}
