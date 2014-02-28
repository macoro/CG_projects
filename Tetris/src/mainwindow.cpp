/*
 *  mainwindow.cpp
 *  
 *
 *  Created by Claudia Esteves Jaramillo on 2/14/09.
 *
 */

#include <QtGui>
#include <QtOpenGL>
#include <iostream>
#include <sstream>
#include <string>
#include "glwidget.h"
#include "mainwindow.h"

using namespace std;

// Class MainWindow

// constructor
MainWindow :: MainWindow()
{
	centralWidget = new QWidget;
	setCentralWidget( centralWidget );
	
	glWidget = new GLWidget;
    lines=new QLabel(tr("   Lines: 0"));
    lines->setStyleSheet("QLabel { background-color : lightblue; color : red; }");
	QGridLayout *centralLayout = new QGridLayout;
	centralLayout -> addWidget( glWidget, 0, 0 );
    centralLayout ->addWidget(lines,1,0);
    centralLayout ->setRowMinimumHeight(1,20);
    centralLayout->setRowStretch(0,500);
	centralWidget -> setLayout( centralLayout );
	
	timerid = -1;
	
	createActions();
	createMenus();
	
	setWindowTitle( tr("Tetrominoes") );
    resize( 400, 600 );

    anglex=angley=anglez=0;
    shiftpress=leftpress=false;
    connect(glWidget,SIGNAL(setspeedcheck(int)),this,SLOT(checkSpeed(int)));
    connect(glWidget,SIGNAL(rowsdonechanged(int)),this,SLOT(setlabeltext(int)));
}

void MainWindow :: newGame()
{
    anglex=angley=anglez=0;
    glWidget->newgame();
}

void MainWindow :: resetGame()
{
    anglex=angley=anglez=0;
    glWidget->resetgame();
}

void MainWindow :: createActions()
{
	newAct = new QAction( tr("&New Game"), this );
	newAct -> setShortcut( tr("Ctrl+N"));
	newAct -> setStatusTip( tr(" Start a new game "));
	connect( newAct, SIGNAL(triggered()), this, SLOT( newGame() ));
	
	resetAct = new QAction( tr("&Reset"), this );
	resetAct -> setShortcut( tr("Ctrl+R"));
	resetAct -> setStatusTip( tr(" Reset this game "));
	connect( resetAct, SIGNAL(triggered()), this, SLOT( resetGame() ));
	
	exitAct = new QAction( tr("E&xit"), this);
	exitAct -> setShortcut( tr("Ctrl+Q"));
	exitAct -> setStatusTip( tr("Exit the application") );
	connect( exitAct, SIGNAL(triggered()), this, SLOT( close()) );

	/* drawing mode action group */
	/* checkable buttons */

	wireAct = new QAction( tr("&Wireframe"), this );
	wireAct -> setShortcut( tr("W") );
	wireAct -> setCheckable(true);
	
	fillAct = new QAction( tr("&Filled"), this );
	fillAct -> setShortcut( tr("F") );
	fillAct -> setCheckable(true);
	fillAct -> setChecked(true);
	
	multiAct = new QAction( tr("&Multicolored"), this );
	multiAct -> setShortcut( tr("M") );
	multiAct -> setCheckable(true);
	
	QActionGroup *drawingModeGroup = new QActionGroup(this);
	drawingModeGroup -> addAction(wireAct);
	drawingModeGroup -> addAction(fillAct);
	drawingModeGroup -> addAction(multiAct);
	connect( drawingModeGroup, SIGNAL(triggered( QAction *)), this, SLOT(setDrawingMode( QAction *)));
	
	/* speed action group */
	slowAct = new QAction( tr("Slow"), this );
	slowAct -> setCheckable(true);
	slowAct -> setChecked(true);
	
	mediumAct = new QAction( tr("Medium"), this );
	mediumAct -> setCheckable(true);
	
	fastAct = new QAction( tr("Fast"), this);
	fastAct -> setCheckable(true);
	
	QActionGroup *speedGroup = new QActionGroup(this);
	speedGroup -> addAction( slowAct );
	speedGroup -> addAction( mediumAct );
	speedGroup -> addAction( fastAct );
	connect( speedGroup, SIGNAL(triggered( QAction *)), this, SLOT( setSpeed( QAction *)));
	
	/* double-buffered */
	bufferAct = new QAction( tr("Double-buffer"), this );
	bufferAct -> setCheckable(true);
	bufferAct -> setShortcut( tr("B"));
	connect( bufferAct, SIGNAL(triggered(bool)), this, SLOT( buffering(bool)) );
	
}

void MainWindow :: createMenus()
{
	fileMenu = menuBar() -> addMenu( tr("&File"));
	fileMenu -> addAction( newAct );
	fileMenu -> addAction( resetAct );
	fileMenu -> addAction( exitAct );
	
	drawMenu = menuBar() -> addMenu( tr("&Drawing Mode"));
	drawMenu -> addAction( wireAct );
	drawMenu -> addAction( fillAct );
	drawMenu -> addAction( multiAct );
	
	speedMenu = menuBar() -> addMenu( tr("&Speed"));
	speedMenu -> addAction( slowAct );
	speedMenu -> addAction( mediumAct );
	speedMenu -> addAction( fastAct );
	
	bufferingMenu = menuBar() -> addMenu( tr("Buffering") );
	bufferingMenu -> addAction( bufferAct );
}

void MainWindow :: setDrawingMode( QAction *action )
{
	if (action == wireAct ){
        if(wireAct->isChecked())
            glWidget->setwife(true);
        else
            glWidget->setwife(false);
	}
	else if ( action == fillAct ){
        if(fillAct->isChecked())
            glWidget->setwife(false);
        else
            glWidget->setwife(true);
	}
	else if ( action == multiAct ){
        if(multiAct->isChecked()){
            glWidget->setmulticolor(true);
        }
        else
            glWidget->setmulticolor(false);
	}
}

void MainWindow :: setSpeed( QAction *action )
{
	if( action == slowAct ){
        glWidget->setspeed(1);
	}
	else if( action == mediumAct){
        glWidget->setspeed(2);
	}
	else if( action == fastAct ){
        glWidget->setspeed(3);
	}
}
void MainWindow::checkSpeed(int level){
    switch( level){
    case 1:
        slowAct->setChecked(true);
        break;
    case 2:
        mediumAct->setChecked(true);
        break;
    case 3:
        fastAct->setChecked(true);
        break;
    }
}

void MainWindow :: buffering( bool checked )
{
	if(!checked)
		cout << "Double-buffering disabled" << endl;
	else
		cout << "Double-buffering enabled" << endl;
}

void MainWindow :: mousePressEvent( QMouseEvent *event )
{
	switch( event->button()){
		case Qt::LeftButton:
            anglex= (anglex+5)%360;
            glWidget->setgamerotatef(anglex,angley,anglez);
            //angley=anglez=0;
            leftpress=true;
            if(shiftpress)
                iposx=event->x();
			break;
		case Qt::MidButton:
            angley= (angley+5)%360;
            glWidget->setgamerotatef(anglex,angley,anglez);
            //anglex=anglez=0;
			break;
		case Qt::RightButton:
            anglez= (anglez+5)%360;
            glWidget->setgamerotatef(anglex,angley,anglez);
            //anglex=angley=0;
			break;
		default:
			break;
	}
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    switch( e->key() )
    {
    case Qt::Key_Up:
        glWidget->rotatepiececcw();
        break;
    case Qt::Key_Down:
        glWidget->rotatepiececw();
        break;
    case Qt:: Key_Left:
        glWidget->moveleft();
        break;
    case Qt::Key_Right:
        glWidget->moveright();
        break;
    case Qt::Key_Space:
        glWidget->droppiece();
        break;
    case Qt::Key_Shift:
            shiftpress=true;
            break;
    default:
        break;
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *e){
    switch( e->key() )
    {
    case Qt::Key_Shift:
        shiftpress=false;
        break;
    default:
        break;
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    switch( event->button()){
    case Qt::LeftButton:
        leftpress=false;
        break;
    default:
        break;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e){
    if(shiftpress && leftpress){
        int posx=e->x();
        float dx= posx - iposx;
        glWidget->scalegame(dx/200.0);
        iposx=posx;
    }
}

void MainWindow::setlabeltext(int rows){
    if(rows >= 0){
        stringstream sstr;
        sstr << rows;
        string str1 = sstr.str();
        lines->setText(tr("Lines: ") + tr(str1.c_str()));
    }
    else
        lines->setText(tr("GAME OVER"));
}
