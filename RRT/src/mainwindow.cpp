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
    solution = false;
	
	setWindowTitle( tr("Pupeteer"));
	setMinimumSize( 600, 600 );
	resize(600, 600);
}
		
//void MainWindow :: reset()
//{
//}

void MainWindow :: createActions()
{
	// Reset and Exit actions for the Applications menu
    resetPosAct = new QAction( tr("Planning"), this);
    resetPosAct -> setShortcut(tr("Ctrl+P"));
    resetPosAct -> setStatusTip(tr("Begin RRT contruction"));
	connect( resetPosAct, SIGNAL(triggered()), this, SLOT(resetPos()) );
	
    resetOrientAct = new QAction(tr("Animate"), this);
    resetOrientAct -> setShortcut(tr("Ctrl+A"));
    resetOrientAct -> setStatusTip(tr("Move the mothel throught a solution path"));
	connect( resetOrientAct, SIGNAL(triggered()), this, SLOT(resetOrient()));
	
    resetJointsAct = new QAction(tr("Reset"), this);
	resetJointsAct -> setShortcut(tr("Ctrl+N"));
    resetJointsAct -> setStatusTip(tr("Reset RTT and Path"));
    connect( resetJointsAct, SIGNAL(triggered()), this, SLOT(resetJoints()));
	
	exitAct = new QAction( tr("E&xit"), this);
	exitAct -> setShortcut( tr("Ctrl+Q"));
	exitAct -> setStatusTip( tr("Exit the application"));
	connect( exitAct, SIGNAL(triggered()), this, SLOT(close()) );
	
	// Options Menu Actions
    toggleCircleAct = new QAction( tr("Show Trayectory"), this);
	toggleCircleAct -> setShortcut( tr("Ctrl+C"));
    toggleCircleAct -> setStatusTip( tr("Show Trayectory curve"));
	toggleCircleAct -> setCheckable(true);
	toggleCircleAct -> setChecked(false);
	connect( toggleCircleAct, SIGNAL(triggered()), this, SLOT(toggleTrackball()));
	
    toggleZbufferAct = new QAction( tr("Show begining tree"), this);
    toggleZbufferAct -> setShortcut( tr("Ctrl+B"));
    toggleZbufferAct -> setStatusTip( tr("Show the rrt tree that begin at init state"));
	toggleZbufferAct -> setCheckable(true);
	toggleZbufferAct -> setChecked(false);
	connect( toggleZbufferAct, SIGNAL(triggered()), this, SLOT(toggleZBuffer()));
	
    toggleBackfaceCullAct = new QAction( tr("Show ending tree"), this);
    toggleBackfaceCullAct -> setShortcut( tr("Ctrl+E"));
    toggleBackfaceCullAct -> setStatusTip( tr("Show the rrt tree that end at init state"));
	toggleBackfaceCullAct -> setCheckable(true);
	toggleBackfaceCullAct -> setChecked(false);
	connect( toggleBackfaceCullAct, SIGNAL(triggered()), this, SLOT(toggleBackfaceCull()));
	
    toggleFrontfaceCullAct = new QAction( tr("Reset Scene"), this);
    toggleFrontfaceCullAct -> setShortcut( tr("Ctrl+R"));
    toggleFrontfaceCullAct -> setStatusTip( tr("Reset scene position and orientation"));
	toggleFrontfaceCullAct -> setCheckable(true);
	toggleFrontfaceCullAct -> setChecked(false);
	connect( toggleFrontfaceCullAct, SIGNAL(triggered()), this, SLOT(toggleFrontfaceCull()));
}

void MainWindow :: createMenus()
{
	// Applications menu
	appMenu = menuBar() -> addMenu( tr("Application") );
	
    //resetMenu = appMenu -> addMenu(tr("&Reset"));
    appMenu -> addAction(resetPosAct);
    appMenu -> addAction(resetOrientAct);
    appMenu -> addAction(resetJointsAct);

	
	appMenu -> addAction( exitAct );
	
	// Options menu
	optionsMenu = menuBar() -> addMenu( tr("&Options"));
	optionsMenu -> addAction( toggleCircleAct );
	optionsMenu -> addAction( toggleZbufferAct );
	optionsMenu -> addAction( toggleBackfaceCullAct );
    //optionsMenu -> addAction( toggleFrontfaceCullAct );

    optionsMenu->setEnabled(false);
}

void MainWindow :: createStatusBar()
{
	statusBar() -> showMessage(tr(" Position/Orientation trackball mode "));
}

void MainWindow :: resetPos()
{
    std::vector< Vector3D> path;
    solution = m_rrt->bidirectional_plan(path);

    m_path = path;
    glWidget->begintree = m_rrt->begin_tree;
    glWidget->endtree= m_rrt->end_tree;

    optionsMenu->setEnabled(true);
    //cout << "reset position" << endl;
}

void MainWindow :: resetOrient()
{
    if(solution){
        for(size_t i = 0; i < m_path.size(); i++){
            m_sceneNode->set_modelstate(m_path[i]);
            glWidget->updateGL();
        }
    }
    //cout << "reset orientation" << endl;
}

void MainWindow :: resetJoints()
{
    m_sceneNode->set_modelstate(m_path[0]);
    glWidget->updateGL();
    toggleCircleAct->setChecked(false);
    glWidget -> toggleDrawPaht(toggleCircleAct->isChecked(),m_path);
    toggleZbufferAct->setChecked(false);
    glWidget->toggleBeginTree(false);
    toggleBackfaceCullAct->setChecked(false);
    glWidget->toggleEndTree(false);
    optionsMenu->setEnabled(false);
    solution = false;

}

void MainWindow :: toggleTrackball()
{
    glWidget -> toggleDrawPaht(toggleCircleAct->isChecked(),m_path);
    //cout << "Show/Hide Trackball" << endl;
}

void MainWindow :: toggleZBuffer()
{
    glWidget->toggleBeginTree(toggleZbufferAct->isChecked());
    //cout << "Enable/Disable z-buffer" << endl;
}

void MainWindow :: toggleBackfaceCull()
{
    glWidget->toggleEndTree(toggleBackfaceCullAct->isChecked());
    //cout << "Enable/Disable Backface Culling" << endl;
}

void MainWindow :: toggleFrontfaceCull()
{

    //cout << "Enable/Disable Frontface Culling" << endl;
}

/* Version 3 */
void MainWindow :: setScene( SceneNode* root )
{
    glWidget->setScene(root);
    m_sceneNode = root;
    m_rrt = new rrt(root);    

//	if( root ){
//		std::vector<JointNode*> joints;
//		root -> getAllJoints(joints);
//		for( std::vector<JointNode*>::iterator i = joints.begin(); i != joints.end(); i++ ){
//			const std::string& name = (*i)->getName();
//			m_jointMap[name] = *i;
//			cout << "el nombre de mi Joint es:" << name << endl;
//		}
//	}
}
