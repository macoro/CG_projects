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

    m_path = std::vector< std::vector<double> >();
    m_dfndata = std::vector<node_data*>();
	
    setWindowTitle( tr("RRT for deformable objects"));
	setMinimumSize( 600, 600 );
	resize(600, 600);    

    rightdown = leftdown = middledown = false;

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

    scalablemodelAct = new QAction(tr("Scalable"),this);
    scalablemodelAct->setShortcut(tr("Ctrl+K"));
    scalablemodelAct->setStatusTip(tr("Change to scalable mode"));
    scalablemodelAct->setCheckable(true);
    scalablemodelAct->setChecked(true);

    deepmodelAct = new QAction(tr("Penetration Deep"),this);
    deepmodelAct->setShortcut(tr("Ctrl+D"));
    deepmodelAct->setStatusTip(tr("Change to penetration deep mode"));
    deepmodelAct->setCheckable(true);
    deepmodelAct->setChecked(false);

    QActionGroup *modeGroup = new QActionGroup( this );
    modeGroup -> addAction( scalablemodelAct );
    modeGroup -> addAction( deepmodelAct );
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

    // Mode menu
    modeMenu = menuBar() -> addMenu( tr("&Mode") );
    modeMenu -> addAction( scalablemodelAct );
    modeMenu -> addAction( deepmodelAct );
}

void MainWindow :: createStatusBar()
{
	statusBar() -> showMessage(tr(" Position/Orientation trackball mode "));
}

void MainWindow :: resetPos()
{

    if(scalablemodelAct->isChecked()){
        m_path.clear();
        m_dfndata.clear();
        solution = m_rrt->scalable_bidirectional_plan(m_path, m_dfndata);
    }
    else if(deepmodelAct->isChecked()){
        m_path.clear();
        m_dfndata.clear();
        solution = m_rrt->pdeep_bidirectional_plan(m_path,0.01);
    }

    glWidget->begintree = m_rrt->begin_tree;
    glWidget->endtree= m_rrt->end_tree;

    if(m_path.size())
        optionsMenu->setEnabled(true);
    //cout << "reset position" << endl;
}

void MainWindow :: resetOrient()
{
    if(solution){

        bool is_deformed = false;
        for(size_t i = 0; i < m_path.size(); i++){

            m_sceneNode->set_modelstate(m_path[i]);

            if(m_dfndata[i]-> scale < 1.0){

                for(size_t j = 0; j < m_dfndata[i]-> objs_collided; j++)
                    is_deformed =  m_sceneNode->deform_model(m_dfndata[i]-> ids_deformers[j],
                                                             m_sceneNode->m_geomodel->get_transform(),
                                                             SceneNode::fixed_objs[m_dfndata[i]-> ids_deformers[j]]->get_inverse());


            }

            else if(m_dfndata[i]-> scale == 1.0 &&  is_deformed){
                m_sceneNode->reset_deformation();
                is_deformed = false;
            }

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



/* Version 3 */
void MainWindow :: setScene( SceneNode* root )
{
    glWidget->setScene(root);
    m_sceneNode = root;
    m_rrt = new rrt(root);

    //set model initial and final state
    Matrix4x4 init = root->m_geomodel->get_transform();
    root->m_model->setInitialState(init);
    root->set_model_finalstate(Vector3D(10,0,170),Vector3D(0.0,0.0,-(M_PI/4.0)));

    //set lower and upper state
    std::vector<double> lower(7,-1.0);
    lower[0]=-450; lower[1]=-10; lower[2]=-450;
    root->m_model->LowerState=lower;
    std::vector<double> upper(7,1.0);
    upper[0]=450; upper[1]= 10; upper[2]= 450;
    root->m_model->UpperState=upper;
    //set weights and scalable levels and coefficient
    root->m_model->set_metricweights(std::vector<double>(1,0.1));
    dynamic_cast<freedef3D_model*>(root-> m_model)->scalable_levels = 1;
    dynamic_cast<freedef3D_model*>(root-> m_model)->scalable_coeff = 0.5;

    //fill swiftsene
    //create swift scene
    m_swiftscene = new SWIFT_Scene(true, false);
    root->fill_swiftscene(m_swiftscene);
    dynamic_cast<freedef3D_model*>(root-> m_model)->inputin_swiftscene(m_swiftscene);
    m_rrt->m_swiftscene = m_swiftscene;
}


void MainWindow::mousePressEvent(QMouseEvent *event){
    switch( event->button()){
    case Qt::LeftButton:
        leftdown = true;
        initx = event->x();
        inity = event->y();
        break;
    case Qt::MidButton:
        middledown = true;
        break;
    case Qt::RightButton:
        rightdown = true;
        initz = event->y();
        break;
    default:
        break;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    switch( event->button()){
    case Qt::LeftButton:
        leftdown = false;
        break;
    case Qt::MidButton:
        middledown = false;
        break;
    case Qt::RightButton:
        rightdown = false;
        break;
    default:
        break;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    int posx=event->x();
    int posy=event->y();
    if(leftdown ){
        Vector3D vpos((posx-initx),(inity - posy),0.0);

        m_sceneNode->translate(vpos);

        initx = posx;
        inity = posy;
    }
    if(rightdown){
        int posz=posy;

        Vector3D vpos(0.0,0.0,(posz - initz));
        m_sceneNode->translate(vpos);

        initz = posz;
    }
    char axis = 'z';

    //if(middledown )

    glWidget->updateGL();
}


void MainWindow::keyPressEvent(QKeyEvent *e){
    switch( e->key() )
    {
    case Qt::Key_Control:
        if(leftdown)
            m_sceneNode->rotate('x',5);
        if(middledown)
            m_sceneNode->rotate('y',5);
        if(rightdown)
            m_sceneNode->rotate('z',5);
         glWidget->updateGL();
        break;

     case Qt::Key_Shift:
        if(leftdown)
            m_sceneNode->rotate('x',-5);
        if(middledown)
            m_sceneNode->rotate('y',-5);
        if(rightdown)
            m_sceneNode->rotate('z',-5);
         glWidget->updateGL();
        break;
        break;

    default:
        break;
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *e){
    switch( e->key() )
    {
    case Qt::Key_Control:

        break;
    default:
        break;
    }

}
