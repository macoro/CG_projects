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
#include "trackball.h"

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
    m_doPick = false;
	
	setWindowTitle( tr("Pupeteer"));
    setMinimumSize( 600, 600 );
    resize(600, 600);

    leftdown = false;
    rightdown = false;
    middledown = false;
    nWinWidth = width();
    nWinHeight= height();
    s_capacity =20;
    s_cursor = -1;
    editMenu->setEnabled(false);

}
		
//void MainWindow :: reset()
//{
//}

void MainWindow :: createActions()
{
	// Reset and Exit actions for the Applications menu
	resetPosAct = new QAction( tr("Position"), this);
	resetPosAct -> setShortcut(tr("Ctrl+I"));
	resetPosAct -> setStatusTip(tr("Reset puppet's position in the world"));
	connect( resetPosAct, SIGNAL(triggered()), this, SLOT(resetPos()) );
	
	resetOrientAct = new QAction(tr("Orientation"), this);
	resetOrientAct -> setShortcut(tr("Ctrl+O"));
	resetOrientAct -> setStatusTip(tr("Reset puppet's orientation in the world"));
	connect( resetOrientAct, SIGNAL(triggered()), this, SLOT(resetOrient()));
	
	resetJointsAct = new QAction(tr("Joints"), this);
	resetJointsAct -> setShortcut(tr("Ctrl+N"));
	resetJointsAct -> setStatusTip(tr("Reset joints to initial value"));
	connect( resetJointsAct, SIGNAL(triggered()), this, SLOT(resetJoints()));
	
	resetAllAct = new QAction(tr("All"), this);
	resetAllAct -> setShortcut(tr("Ctrl+A"));
	resetAllAct -> setStatusTip(tr("Reset all"));
	connect( resetAllAct, SIGNAL(triggered()), this, SLOT(resetAll()));
	
	exitAct = new QAction( tr("E&xit"), this);
	exitAct -> setShortcut( tr("Ctrl+Q"));
	exitAct -> setStatusTip( tr("Exit the application"));
	connect( exitAct, SIGNAL(triggered()), this, SLOT(close()) );
	
	// Actions for the Trackball Mode menu
	
	posOrientAct = new QAction( tr("Position/Orientation"), this);
	posOrientAct -> setShortcut( tr("Ctrl+P"));
	posOrientAct -> setStatusTip( tr("Trackball mode: Position/Orientation"));
	posOrientAct -> setCheckable(true);
	posOrientAct -> setChecked(true);
	
	jointsAct = new QAction( tr("Joints"), this);
	jointsAct -> setShortcut( tr("Ctrl+J"));
	jointsAct -> setStatusTip( tr("Trackball mode: Joints"));
	jointsAct -> setCheckable(true);
	
	QActionGroup *modeGroup = new QActionGroup( this );
	modeGroup -> addAction( posOrientAct );
	modeGroup -> addAction( jointsAct );
	connect( modeGroup, SIGNAL(triggered( QAction*)), this, SLOT( setTrackballMode( QAction*)));
	
	// Edit Menu Actions
	undoAct = new QAction( tr("&Undo"), this );
	undoAct -> setShortcut( tr("Ctrl+U"));
	undoAct -> setStatusTip( tr("Undo the last operation"));
	connect( undoAct, SIGNAL(triggered()), this, SLOT(undo()));
	
	redoAct = new QAction( tr("&Redo"), this);
	redoAct -> setShortcut( tr("Ctrl+R"));
	redoAct -> setStatusTip( tr("Redo the last operation"));
	connect( redoAct, SIGNAL(triggered()), this, SLOT(redo()));

	// Options Menu Actions
	toggleCircleAct = new QAction( tr("Show trackball"), this);
	toggleCircleAct -> setShortcut( tr("Ctrl+C"));
	toggleCircleAct -> setStatusTip( tr("Show trackball circle"));
	toggleCircleAct -> setCheckable(true);
	toggleCircleAct -> setChecked(false);
	connect( toggleCircleAct, SIGNAL(triggered()), this, SLOT(toggleTrackball()));
	
	toggleZbufferAct = new QAction( tr("Z-buffer"), this);
	toggleZbufferAct -> setShortcut( tr("Ctrl+Z"));
	toggleZbufferAct -> setStatusTip( tr("Enable z-buffer"));
	toggleZbufferAct -> setCheckable(true);
	toggleZbufferAct -> setChecked(false);
	connect( toggleZbufferAct, SIGNAL(triggered()), this, SLOT(toggleZBuffer()));
	
	toggleBackfaceCullAct = new QAction( tr("Backface culling"), this);
	toggleBackfaceCullAct -> setShortcut( tr("Ctrl+B"));
	toggleBackfaceCullAct -> setStatusTip( tr("Enable backface culling"));
	toggleBackfaceCullAct -> setCheckable(true);
	toggleBackfaceCullAct -> setChecked(false);
	connect( toggleBackfaceCullAct, SIGNAL(triggered()), this, SLOT(toggleBackfaceCull()));
	
	toggleFrontfaceCullAct = new QAction( tr("Frontface culling"), this);
	toggleFrontfaceCullAct -> setShortcut( tr("Ctrl+F"));
	toggleFrontfaceCullAct -> setStatusTip( tr("Enable frontface culling"));
	toggleFrontfaceCullAct -> setCheckable(true);
	toggleFrontfaceCullAct -> setChecked(false);
	connect( toggleFrontfaceCullAct, SIGNAL(triggered()), this, SLOT(toggleFrontfaceCull()));
}

void MainWindow :: createMenus()
{
	// Applications menu
	appMenu = menuBar() -> addMenu( tr("Application") );
	
	resetMenu = appMenu -> addMenu(tr("&Reset"));
	resetMenu -> addAction(resetPosAct);
	resetMenu -> addAction(resetOrientAct);
	resetMenu -> addAction(resetJointsAct);
	resetMenu -> addAction(resetAllAct);
	
	appMenu -> addAction( exitAct );
	
	// Mode menu
	modeMenu = menuBar() -> addMenu( tr("&Mode") );
    modeMenu -> addAction( posOrientAct );
	modeMenu -> addAction( jointsAct );
	
	// Edit menu
	editMenu = menuBar() -> addMenu( tr("&Edit"));
	editMenu -> addAction( undoAct );
	editMenu -> addAction( redoAct );
	
	// Options menu
	optionsMenu = menuBar() -> addMenu( tr("&Options"));
	optionsMenu -> addAction( toggleCircleAct );
	optionsMenu -> addAction( toggleZbufferAct );
	optionsMenu -> addAction( toggleBackfaceCullAct );
	optionsMenu -> addAction( toggleFrontfaceCullAct );
}

void MainWindow :: createStatusBar()
{
	statusBar() -> showMessage(tr(" Position/Orientation trackball mode "));
}

void MainWindow :: resetPos()
{
    Matrix4x4 t = m_sceneNode->get_transform();
    t[0][3]=0.0;
    t[1][3]=0.0;
    t[2][3]=0.0;
    m_sceneNode->set_transform(t);
    glWidget->updateGL();
	cout << "reset position" << endl;
}

void MainWindow :: resetOrient()
{
    Matrix4x4 t = m_sceneNode->get_transform();
    Matrix4x4 newt;
    for(int i=0;i<4;i++)
        newt[i][3]=t[i][3];
    m_sceneNode->set_transform(newt);
    glWidget->updateGL();
	cout << "reset orientation" << endl;

}

void MainWindow :: resetJoints()
{
    std::vector<JointNode*> joints;
    m_sceneNode -> getAllJoints(joints);
    for( std::vector<JointNode*>::iterator i = joints.begin(); i != joints.end(); i++ ){
        const std::string& name = (*i)->getName();
        Matrix4x4 t = (*i)->get_transform();
        Matrix4x4 newt;
        for(int i=0;i<3;i++)
            newt[i][3]=t[i][3];
        double cxi = 1.0;
        double sxi=0.0;
        newt[0][0]=cxi; newt[1][1]=cxi;
        newt[1][0]=sxi; newt[0][1]=-sxi;

        (*i)->set_transform(newt);
        (*i)->m_joint_x.current = 0.0;
        (*i)->m_joint_y.current = 0.0;
    }
    m_jointstack.clear();
    s_cursor = -1.0;
    angle = 0.0;
    headangle =0.0;
    editMenu->setEnabled(false);

    glWidget->updateGL();
	cout << "reset joints" << endl;
}

void MainWindow :: resetAll()
{
    m_sceneNode->add_transform(m_sceneNode->get_inverse());
    resetJoints();
    glWidget->updateGL();
	cout << "reset all" << endl;
}

void MainWindow :: setTrackballMode( QAction *action )
{
	if( action == posOrientAct ){
		statusBar() -> showMessage(tr("Position/Orientation trackball mode"));
        m_doPick = false;
        glWidget->clearpicked();
	}
	else if( action == jointsAct ){
		statusBar() -> showMessage(tr("Joints trackball mode"));
        m_doPick = true;
	}
}

void MainWindow :: undo()
{
    if(m_jointstack.size() && s_cursor >= 0){
        editMenu->setEnabled(true);
        itemstack state= m_jointstack[s_cursor];
        apply_undo(state,false);
        s_cursor--;

        if((m_jointstack.size()-s_cursor)> 3){
            std::vector<itemstack> new_jointstack(s_cursor + 1);
            copy(m_jointstack.begin(),m_jointstack.begin()+s_cursor+1,new_jointstack.begin());
            m_jointstack = new_jointstack;
            editMenu->removeAction(redoAct);
        }
        else if(m_jointstack.size()){
            editMenu->addAction(redoAct);
        }

    }
    else if(s_cursor< 0)
        editMenu->setEnabled(false);


    glWidget->update();
	cout << "Undo last action" << endl;
}

void MainWindow :: redo()
{
    if( (s_cursor+1)<m_jointstack.size()){
        itemstack state= m_jointstack[s_cursor+1];
        apply_undo(state,true);
        s_cursor++;
    }
    glWidget->update();
	cout << "Redo last action" << endl;
}

void MainWindow :: toggleTrackball()
{
	cout << "Show/Hide Trackball" << endl;
	glWidget -> toggleCircle( toggleCircleAct->isChecked() );
}

void MainWindow :: toggleZBuffer()
{
    glWidget->toggleZBuffer(toggleZbufferAct->isChecked());
    cout << "Enable/Disable Backface ZBuffer" << endl;
}

void MainWindow :: toggleBackfaceCull()
{
    glWidget->toggleBackfaceCull(toggleBackfaceCullAct->isChecked());
	cout << "Enable/Disable Backface Culling" << endl;
}

void MainWindow :: toggleFrontfaceCull()
{
    glWidget->toggleFrontfaceCull(toggleFrontfaceCullAct->isChecked());
	cout << "Enable/Disable Frontface Culling" << endl;
}

/* Version 3 */
void MainWindow :: setScene( SceneNode* root )
{
	glWidget->setScene(root);
	
	// m_menu_pick.items().clear();
	
	if( root ){
        m_sceneNode = root;
		std::vector<JointNode*> joints;
		root -> getAllJoints(joints);
		for( std::vector<JointNode*>::iterator i = joints.begin(); i != joints.end(); i++ ){
			const std::string& name = (*i)->getName();
            cout << "el nombre de mi Joint es:" << name <<endl;
            m_jointMap[name]= (*i);
		}
	}
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
        itrackx = event->x();
        itracky = event->y();
        if(jointsAct->isChecked()){
            headangle = 0.0;
        }
        break;
    case Qt::RightButton:
        rightdown = true;
        initz = event->y();
        if(jointsAct->isChecked()){
            angle = 0.0;
        }
        break;
    default:
        break;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    switch( event->button()){
    case Qt::LeftButton:
        leftdown = false;
        if(m_doPick){
            glWidget->set_dopick(true);
            glWidget->setposition(event->x(),event->y());
            glWidget->updateGL();
            //turnOn_joint();
            glWidget->set_dopick(false);
            glWidget->updateGL();
        }
        break;
    case Qt::MidButton:
        middledown = false;
        if(jointsAct->isChecked()){
            if(m_jointstack.size() == s_capacity){
                for( std::vector<itemstack>::iterator k = m_jointstack.begin();k != m_jointstack.end(); k++ )
                    *k=*(++k);
            }

            itemstack item = {true,m_jointpicked,headangle};
            m_jointstack.push_back(item);
            s_cursor++;
            if(m_jointstack.size()>1)
                s_cursor=m_jointstack.size()-2;
            editMenu->setEnabled(true);
        }
        break;
    case Qt::RightButton:
        rightdown = false;
        if(jointsAct->isChecked()){
            if(m_jointstack.size() == s_capacity){
                for( std::vector<itemstack>::iterator k = m_jointstack.begin();k != m_jointstack.end(); k++ )
                    *k=*(++k);
            }

            itemstack item = {false,m_jointpicked,angle};
            m_jointstack.push_back(item);
            s_cursor++;
            s_cursor++;
            if(m_jointstack.size()>1)
                s_cursor=m_jointstack.size()-2;
            editMenu->setEnabled(true);
        }
        break;
    default:
        break;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    int posx=event->x();
    int posy=event->y();
    if(leftdown && posOrientAct->isChecked()){
        Vector3D vpos(((posx-initx)/100.0),((inity - posy)/100.0),0.0);

        m_sceneNode->translate(vpos);

        initx = posx;
        inity = posy;
    }
    if(rightdown ){
        int posz=posy;
        double deltha=(posz - initz);
        if(posOrientAct->isChecked()){
            Vector3D vpos(0.0,0.0,(deltha/100.0));
            m_sceneNode->translate(vpos);
        }
        else if(jointsAct->isChecked()){
            double alpha =0.0;
                    alpha = (deltha < 0)? -1.0 : 1.0;
            std::vector<SceneNode*> items;
            m_jointpicked.clear();
            glWidget->get_itemspicked(items);
            m_jointpicked.resize(items.size());
            copy(items.begin(),items.end(),m_jointpicked.begin());

            for( std::vector<SceneNode*>::iterator k = items.begin();k != items.end(); k++ ){
                const std::string& name = (*k)->getName();

                char axis = 'z';
                if(name.compare("Nuca") == 0 || name.compare("Articulacion Cabeza-Cuello") == 0)
                    axis = 'x';
                Matrix4x4 t=m_jointMap[name]->get_transform();
                //salvar la pos
                double x=t[0][3];
                double y=t[1][3];
                double z=t[2][3];
                //regresar al origen
                t[0][3]=0.0;
                t[1][3]=0.0;
                t[2][3]=0.0;
                //newt=newt*t;
                m_jointMap[name]->set_transform(t);
                angle += alpha;
                m_jointMap[name]->rotate(axis,alpha);
                t=m_jointMap[name]->get_transform();
                t[0][3]=x;
                t[1][3]=y;
                t[2][3]=z;
                m_jointMap[name]->set_transform(t);

            }
        }
        initz = posz;
    }
    if(middledown ){
        if(posOrientAct->isChecked())
            vPerformTransfo(itrackx,posx,itracky,posy);

        else if(jointsAct->isChecked()){
            double deltha=(posx - itrackx);
            double alpha =0.0;
            alpha = (deltha < 0)? -1.0 : 1.0;
            std::vector<SceneNode*> items;
            m_jointpicked.clear();
            glWidget->get_itemspicked(items);
            m_jointpicked.resize(items.size());
            copy(items.begin(),items.end(),m_jointpicked.begin());
            for( std::vector<SceneNode*>::iterator k = items.begin();k != items.end(); k++ ){
                const std::string& name = (*k)->getName();
                if(name.compare("Articulacion Cabeza-Cuello") == 0){
                    char axis = 'y';
                    Matrix4x4 t=m_jointMap[name]->get_transform();
                    //salvar la pos
                    double x=t[0][3];
                    double y=t[1][3];
                    double z=t[2][3];
                    //regresar al origen
                    t[0][3]=0.0;
                    t[1][3]=0.0;
                    t[2][3]=0.0;
                    //newt=newt*t;
                    m_jointMap[name]->set_transform(t);
                    headangle += alpha;
                    m_jointMap[name]->rotate(axis,alpha);
                    t=m_jointMap[name]->get_transform();
                    t[0][3]=x;
                    t[1][3]=y;
                    t[2][3]=z;
                    m_jointMap[name]->set_transform(t);
                }

            }
        }

        itrackx=posx;
        itracky=posy;
    }
    glWidget->updateGL();
}

void MainWindow::vPerformTransfo(float fOldX, float fNewX, float fOldY, float fNewY)
{
    float  fRotVecX, fRotVecY, fRotVecZ;
    Matrix4x4 mNewMat;

    /*
     * Track ball rotations are being used.
     */
    float fDiameter;
    int iCenterX, iCenterY;
    float fNewModX, fNewModY, fOldModX, fOldModY;

    /* vCalcRotVec expects new and old positions in relation to the center of the
         * trackball circle which is centered in the middle of the window and
         * half the smaller of nWinWidth or nWinHeight.
         */
    fDiameter = ( nWinWidth < nWinHeight) ? nWinWidth * 0.5 : nWinHeight * 0.5;
    iCenterX = nWinWidth / 2;
    iCenterY = nWinHeight / 2;
    fOldModX = fOldX - iCenterX;
    fOldModY = fOldY - iCenterY;
    fNewModX = fNewX - iCenterX;
    fNewModY = fNewY - iCenterY;

    vCalcRotVec(fNewModX, fNewModY,
                fOldModX, fOldModY,
                fDiameter,
                &fRotVecX, &fRotVecY, &fRotVecZ);
    /* Negate Y component since Y axis increases downwards
         * in screen space and upwards in OpenGL.
         */
    vAxisRotMatrix(fRotVecX, -fRotVecY, fRotVecZ, mNewMat);


    Matrix4x4 trot = mNewMat.transpose();
    double x = trot[2][1];
    double rx = asin(x)* 180.0 / M_PI;
    m_sceneNode->rotate('x',rx);
    double y = trot[0][2];
    double ry = asin(y)* 180.0 / M_PI;
    m_sceneNode->rotate('y',ry);
    double z = trot[1][0];
    double rz =asin(z)* 180.0 / M_PI;
    m_sceneNode->rotate('z',rz);

    glWidget->updateGL();
}

void MainWindow::apply_undo(itemstack state,bool redo){
    double deltha=state.angle;
    double alpha = (deltha < 0)? -1.0 : 1.0;
    int iredo = (redo)? 1 : -1;
    for(int xi=alpha; xi!=deltha; xi+=alpha){
        if(state.yhead){
            for( std::vector<SceneNode*>::iterator k = state.jpicked.begin();k != state.jpicked.end(); k++ ){
                const std::string& name = (*k)->getName();
                if(name.compare("Articulacion Cabeza-Cuello") == 0){
                    char axis = 'y';
                    Matrix4x4 t=m_jointMap[name]->get_transform();
                    //salvar la pos
                    double x=t[0][3];
                    double y=t[1][3];
                    double z=t[2][3];
                    //regresar al origen
                    t[0][3]=0.0;
                    t[1][3]=0.0;
                    t[2][3]=0.0;
                    //newt=newt*t;
                    m_jointMap[name]->set_transform(t);
                    m_jointMap[name]->rotate(axis,(iredo*alpha));
                    t=m_jointMap[name]->get_transform();
                    t[0][3]=x;
                    t[1][3]=y;
                    t[2][3]=z;
                    m_jointMap[name]->set_transform(t);
                }
            }
        }
        else{
            for( std::vector<SceneNode*>::iterator k = state.jpicked.begin();k != state.jpicked.end(); k++ ){
                const std::string& name = (*k)->getName();
                char axis = 'z';
                if(name.compare("Nuca") == 0 || name.compare("Articulacion Cabeza-Cuello") == 0)
                    axis = 'x';
                Matrix4x4 t=m_jointMap[name]->get_transform();
                //salvar la pos
                double x=t[0][3];
                double y=t[1][3];
                double z=t[2][3];
                //regresar al origen
                t[0][3]=0.0;
                t[1][3]=0.0;
                t[2][3]=0.0;
                //newt=newt*t;
                m_jointMap[name]->set_transform(t);
                m_jointMap[name]->rotate(axis,(iredo*alpha));
                t=m_jointMap[name]->get_transform();
                t[0][3]=x;
                t[1][3]=y;
                t[2][3]=z;
                m_jointMap[name]->set_transform(t);

            }
        }
    }
}
