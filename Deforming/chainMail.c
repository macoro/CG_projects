//-------------------------------------------------------------------------
//	Simple OpenGL Win32 application to demonstrate 3DChainMail, from 
//	"3D ChainMail: a Fast Algorithm for Deforming Volumetric Objects", 
//	Proceedings 1997 Symposium on Interactive 3D Graphics, April 1997, 
//	pp. 149-154, by S. Frisken (Gibson).
//
//	Disclaimer: this is research code! It is not optimized and is probably
//	not bug-free but is provided as a courtesy to people who are interested
//	in understanding and/or furthering this work. Note that there have been 
//	a number of papers written by others that extend the original 
//	ChainMail algorithm.
//
//	Key commands:
//		'e': toggles between ChainMail and ChainMail + equilibrium
//		'p': toggles between rendering surface triangles and points
//
//	Written by S. Frisken, 1996, modified by R. Perry, 1998, updated 2003.
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//	Copyright 2002 Mitsubishi Electric Research Laboratories All Rights 
//	Reserved.
//    
//	Permission to use, copy and modify this software and its documentation 
//	without fee for educational, research and non-profit purposes, is 
//	hereby granted, provided that the above copyright notice and the 
//	following three paragraphs appear in all copies. To request permission 
//	to incorporate this software into commercial products contact:  
//	
//		Vice President Marketing and Business Development
//		Mitsubishi Electric Research Laboratories (MERL)
//		201 Broadway 
//		Cambridge, MA   02139 
//		or 
//		license@merl.com 
//	          
//	
//	IN NO EVENT SHALL MERL BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, 
//	SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, 
//	ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN 
//	IF MERL HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
//	       
//	MERL SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED 
//	TO, THE IMPLIED WARRANTIES OR MERCHANTABILITY AND FITNESS FOR A 
//	PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" 
//	BASIS AND MERL HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, 
//	UPDATES, ENHANCEMENTS OR MODIFICATIONS.
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Win32 data
//-------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>
#include <stdio.h>
#include <GL/gl.h> 
#include <GL/glu.h> 
#include <GL/glaux.h> 


//----Win32 globals
CHAR	szAppName[]="Win OpenGL";	//----Application name
HWND	ghWnd;						//----Window handle
HDC		ghDC;						//----Window drawing context
HGLRC	ghRC;						//----Window GL rendering context


//----Defines
#define WIN_WIDTH	650 
#define WIN_HEIGHT	650
#define WIN_DEPTH	650
#define TRUE		1
#define FALSE		0


//----Function prototypes for OS specific routines
LONG WINAPI MainWndProc (HWND, UINT, WPARAM, LPARAM); 
BOOL bSetupPixelFormat(HDC); 


//----OpenGL prototypes
GLvoid GLResize(GLsizei, GLsizei); 
GLvoid GLInitialize(GLsizei, GLsizei); 
GLvoid GLDrawScene(GLsizei, GLsizei); 
 

//-------------------------------------------------------------------------
// 3D ChainMail data
//-------------------------------------------------------------------------
#define	V_WORLD_X_SPAN	100
#define	V_WORLD_Y_SPAN	100
#define	V_WORLD_Z_SPAN	100
#define CM3D_MAX_VSIZE	50*50*50
#define	MAX_DEFORM		3.0


typedef struct DeformVector {
  float x, y, z;                
} DeformVector;


typedef struct DeformElement {
  int nodeType;
  float x, y, z;                
  struct DeformElement *left, *right, *top, *bottom, *front, *back;   
} DeformElement;


typedef struct DeformMarker {
  struct DeformElement *elPtr;
  struct DeformElement *v1, *v2, *v3, *v4;    //----Max of 4 surface nbrs on cube
} DeformMarker;


typedef struct DeformObject {
  float maxDeform;
  float minDeform;
  float maxDeformShear;
  float maxDeformInit;
  float maxEqu;
  float minEqu;
  float maxEquShear;
  float maxEquInit;
  int equilize;
  int doingChainMail;
  int objSize;
  int numMarkers;
  int numSurface;
  DeformElement *selectedElementPtr;	//----Pointer to selected element
  DeformMarker marker[10];				//----Pointers to marked elements
  DeformElement **surfacePtr;			//----Array of pointers to surface elements
  DeformElement *object;				//----Array of elements
} DeformObject;


typedef struct {
  float oldX, oldY, oldZ;				//----Previous voxel position
  DeformElement *p;						//----Pointer to changed voxel
} MovedElement;


typedef struct {
  int n;								//----Number of moved elements
  MovedElement m[CM3D_MAX_VSIZE];		//----Moved elements
} MovedList;


//----Global ChainMail objects
static DeformObject cmObject;
static MovedList movedList;


//----Global rendering and direct manipulation state variables
static int lightsAreOn = 0;
static int drawPts = 0;
static int buttonPressFlag = FALSE;
static float viewAngle = 0;
static float rotationStep = 3;
static float pointerX, pointerY;


//----Lists of neighbor pointers for movement consideration
static DeformElement *top	[2*CM3D_MAX_VSIZE];
static DeformElement *bottom[2*CM3D_MAX_VSIZE];
static DeformElement *left	[2*CM3D_MAX_VSIZE];         
static DeformElement *right	[2*CM3D_MAX_VSIZE];
static DeformElement *front	[2*CM3D_MAX_VSIZE];
static DeformElement *back	[2*CM3D_MAX_VSIZE];


//----Procedure prototypes 
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
int MakeCMCubeObject(DeformObject *T, int xSize, int ySize, int zSize,
float xOffset, float yOffset, float zOffset, float xyzScale, int xhLo,
int xhHi, int yhLo, int yhHi);
int ButtonRelease(HWND hWnd);
int ButtonPress(HWND hWnd, int mX, int mY);
int PointerMotion(HWND hWnd, int mX, int mY, int translate);
int EquilizeElementPositions(DeformObject *T);
int TestMove(DeformObject *T, float *dx, float *dy, float *dz, int translate);
int AddToNeighborList(DeformElement *elementPtr, DeformElement *list[], int *listIndex);
void GenGLTriangle(float x1, float y1, float z1, float x2, float y2, float z2, 
float x3, float y3, float z3);
int NewPosition(float *x, float *y, float *z, float x2, float y2, float z2, float maxDx, 
float minDx, float maxDy, float minDy, float maxDz, float minDz);
int MoveVertex(DeformObject *T, DeformElement *obj, float x, float y, float z, int justTest);


//-------------------------------------------------------------------------
// Create window; get and dispatch events.
//-------------------------------------------------------------------------
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
LPSTR lpCmdLine, int nCmdShow) 
{ 
    MSG			msg; 
    WNDCLASS	wndclass; 


    //----Register the frame class  
    wndclass.style         = 0; 
    wndclass.lpfnWndProc   = (WNDPROC)MainWndProc; 
    wndclass.cbClsExtra    = 0; 
    wndclass.cbWndExtra    = 0; 
    wndclass.hInstance     = hInstance; 
    wndclass.hIcon         = LoadIcon (hInstance, szAppName); 
    wndclass.hCursor       = LoadCursor (NULL,IDC_ARROW); 
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
    wndclass.lpszMenuName  = szAppName; 
    wndclass.lpszClassName = szAppName; 


    if (!RegisterClass (&wndclass) ) 
        return FALSE; 
 

    //----Create the frame  
    ghWnd = CreateWindow (szAppName, 
			"OpenGL Sample with 3D ChainMail", 
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			WIN_WIDTH, 
			WIN_HEIGHT, 
			NULL, 
			NULL, 
			hInstance, 
			NULL); 
 

    //----Make sure window was created  
    if (!ghWnd) 
        return FALSE; 
 

    //----Show and update main window  
    ShowWindow (ghWnd, nCmdShow); 
    UpdateWindow (ghWnd); 
 

	//----Set timer for relaxation 
	SetTimer(ghWnd, 1, 10, TimerProc);


	//----Get and dispatch events 
    while (1) { 
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) 
        { 
            if (GetMessage(&msg, NULL, 0, 0) ) 
            { 
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
            } else { 
                return TRUE; 
            } 
        } 
    } 
} 


//-------------------------------------------------------------------------
// Event handler for window
//-------------------------------------------------------------------------
LONG WINAPI MainWndProc ( 
HWND    hWnd, 
UINT    uMsg, 
WPARAM  wParam, 
LPARAM  lParam) 
{
    LONG		lRet = 1; 
    PAINTSTRUCT	ps; 
    RECT		rect; 
	int			xPos, yPos;
 

    switch (uMsg) {

		case WM_CREATE: 
			ghDC = GetDC(hWnd); 
			if (!bSetupPixelFormat(ghDC)) PostQuitMessage (0); 
			ghRC = wglCreateContext(ghDC); 
			wglMakeCurrent(ghDC, ghRC); 
			GetClientRect(hWnd, &rect); 
			GLInitialize(rect.right, rect.bottom); 
			break; 
 
		case WM_PAINT: 
			BeginPaint(hWnd, &ps); 
   			GetClientRect(ghWnd, &rect); 
			GLDrawScene(rect.right, rect.bottom);
			EndPaint(hWnd, &ps); 
			break; 
 
		case WM_SIZE: 
			GetClientRect(hWnd, &rect); 
			GLResize(rect.right, rect.bottom);
			InvalidateRect(hWnd, &rect, FALSE);
			break; 
 
		case WM_CLOSE: 
			if (ghRC) wglDeleteContext(ghRC); 
			if (ghDC) ReleaseDC(hWnd, ghDC); 
			ghRC = 0; 
			ghDC = 0; 
			DestroyWindow (hWnd); 
			break; 
 
		case WM_DESTROY: 
			if (ghRC) wglDeleteContext(ghRC); 
			if (ghDC) ReleaseDC(hWnd, ghDC); 
			PostQuitMessage (0); 
			break; 
     
		case WM_CHAR:
			switch(wParam) {
				case 'p':
				{
					drawPts = drawPts ? 0 : 1;
					GetClientRect(hWnd, &rect);
					InvalidateRect(hWnd, &rect, FALSE);
					break;
				}
				case 'e':
				{
					if (cmObject.equilize) cmObject.equilize = 0;
					else cmObject.equilize = 1;
					break;
				}
			}
			break;
		
		case WM_LBUTTONDOWN:
			xPos = LOWORD(lParam); 
			yPos = HIWORD(lParam); 
			ButtonPress(hWnd, xPos, yPos);
			break;

		case WM_LBUTTONUP:
			ButtonRelease(hWnd);
			break;

		case WM_MOUSEMOVE:
			if (buttonPressFlag == TRUE) {
				xPos = LOWORD(lParam); 
				yPos = HIWORD(lParam); 
				PointerMotion(hWnd, xPos, yPos, wParam & MK_SHIFT);
			}
			break;

		default: 
			lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
			break; 
    } 
 
    return lRet; 
} 


//-------------------------------------------------------------------------
// Setup pixel format for the window
//-------------------------------------------------------------------------
BOOL bSetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd, *ppfd; 
    int pixelformat; 
 

    ppfd = &pfd; 
    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    ppfd->nVersion = 1; 
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_RGBA;
    ppfd->cColorBits = 8; 
    ppfd->cDepthBits = 32;
    ppfd->cAccumBits = 0; 
    ppfd->cStencilBits = 0; 
    pixelformat = ChoosePixelFormat(hdc, ppfd); 
 

    if ( pixelformat == 0 ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 

    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 

    return TRUE; 
} 
 

//-------------------------------------------------------------------------
// Resize the OpenGL window
//-------------------------------------------------------------------------
GLvoid GLResize( GLsizei width, GLsizei height ) 
{ 
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, V_WORLD_X_SPAN, 0, V_WORLD_Y_SPAN, -V_WORLD_Z_SPAN, V_WORLD_Z_SPAN);
	glMatrixMode(GL_MODELVIEW);
}     


//-------------------------------------------------------------------------
// Initialize the OpenGL window
//-------------------------------------------------------------------------
GLvoid GLInitialize(GLsizei width, GLsizei height) 
{
	GLint   rc					= 0;
	GLfloat global_ambient[]	= { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat mat_ambient[]		= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_specular[]		= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[]		= { 10.0f };
	GLfloat light_position0[]	= { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat light_position1[]	= {-1.0f, 1.0f, 1.0f, 0.0f };


	//----Create ChainMail object
	DeformObject *p = &cmObject;
	rc = MakeCMCubeObject(p, 10, 10, 5, 55, 55, 10, 1.0, 0,  0, 0,  0);
	if (rc == 0) return;


	//----Setup GL for rendering the scene
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	if (lightsAreOn) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_EMISSION);
} 


//-------------------------------------------------------------------------
// Draw the OpenGL window
//-------------------------------------------------------------------------
GLvoid GLDrawScene(GLsizei width, GLsizei height) 
{ 
	int i, j;
	DeformObject *T = &cmObject;


	//----Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//----Rotate view by viewAngle
	glPushMatrix();
	glRotatef(viewAngle, 0.0, 50.0, 0.0);


	//----Draw each ChainMail object as point cloud or triangulated surface
	if (drawPts) {
		glColor3ub(255, 255, 0);
		glBegin(GL_POINTS);
		for (j = 0; j < T->numSurface; j++) { 
			glVertex3f(T->surfacePtr[j]->x, T->surfacePtr[j]->y, T->surfacePtr[j]->z); 
		}
		glEnd();
	} else {
		for (j = 0; j < T->numSurface; j++) {
			DeformElement *s = T->surfacePtr[j];
			if (s->right && s->top && s->right->top && !s->back) {

				glColor3ub(200, 200, 0);
				GenGLTriangle(s->x, s->y, s->z, 
				s->right->x, s->right->y, s->right->z,
				s->top->x, s->top->y, s->top->z);

				glColor3ub(160, 160, 0);
				GenGLTriangle(s->right->x, s->right->y, s->right->z, 
				s->right->top->x, s->right->top->y, s->right->top->z,
				s->top->x, s->top->y, s->top->z);
			}
			if (s->right && s->top && s->right->top && !s->front) {

				glColor3ub(0, 200, 0);
				GenGLTriangle(s->x, s->y, s->z, 
				s->top->x, s->top->y, s->top->z,
				s->right->x, s->right->y, s->right->z);

				glColor3ub(0, 160, 0);
				GenGLTriangle(s->right->x, s->right->y, s->right->z,
				s->top->x, s->top->y, s->top->z,
				s->right->top->x, s->right->top->y, s->right->top->z);
			}
			if (s->right && s->front && s->right->front && (!s->top || !s->right->top)) {

				glColor3ub(0, 200, 200);
				GenGLTriangle(s->x, s->y, s->z, 
				s->right->x, s->right->y, s->right->z,
				s->front->x, s->front->y, s->front->z);

				glColor3ub(0, 160, 160);
				GenGLTriangle(s->right->x, s->right->y, s->right->z, 
				s->right->front->x, s->right->front->y, s->right->front->z,
				s->front->x, s->front->y, s->front->z);
			}
			if (s->right && s->front && s->right->front && (!s->bottom || !s->right->bottom)) {

				glColor3ub(0, 0, 200);
				GenGLTriangle(s->x, s->y, s->z,
				s->front->x, s->front->y, s->front->z,
				s->right->x, s->right->y, s->right->z);

				glColor3ub(0, 0, 160);
				GenGLTriangle(s->right->x, s->right->y, s->right->z,
				s->front->x, s->front->y, s->front->z,
				s->right->front->x, s->right->front->y, s->right->front->z);
			}
			if (s->front && s->top && s->front->top && (!s->right || !s->top->right)) {

				glColor3ub(200, 0, 200);
				GenGLTriangle(s->x, s->y, s->z, 
				s->front->x, s->front->y, s->front->z,
				s->top->x, s->top->y, s->top->z);

				glColor3ub(160, 0, 160);
				GenGLTriangle(s->front->x, s->front->y, s->front->z, 
				s->front->top->x, s->front->top->y, s->front->top->z,
				s->top->x, s->top->y, s->top->z);
			}
			if (s->front && s->top && s->front->top && (!s->left || !s->top->left)) {

				glColor3ub(200, 0, 0);
				GenGLTriangle(s->x, s->y, s->z, 
				s->top->x, s->top->y, s->top->z,
				s->front->x, s->front->y, s->front->z);

				glColor3ub(160, 0, 0);
				GenGLTriangle(s->front->x, s->front->y, s->front->z,
				s->top->x, s->top->y, s->top->z,
				s->front->top->x, s->front->top->y, s->front->top->z);
			}
		}


		//----Draw markers
		glColor3ub(255, 0, 0); 
		for (i = 0; i < T->numMarkers; i++) {
			glPushMatrix();
			glTranslatef(T->marker[i].elPtr->x, T->marker[i].elPtr->y, T->marker[i].elPtr->z);
			auxSolidSphere(0.5);
			glPopMatrix();
		}
	}


	//----Restore matrix stack and swap buffers
	glPopMatrix();
    SwapBuffers(ghDC); 
	return;
} 


//-------------------------------------------------------------------------
// Generate a GL triangle from the given three vertices
//-------------------------------------------------------------------------
void GenGLTriangle(float x1, float y1, float z1, float x2, float y2, float z2, 
float x3, float y3, float z3)
{
	float d1[3];
	float d2[3];
	float norm[3];
	float d;


	if (lightsAreOn) {
		d1[0] = x1 - x2;
		d1[1] = y1 - y2;
		d1[2] = z1 - z2;
		d2[0] = x2 - x3;
		d2[1] = y2 - y3;
		d2[2] = z2 - z3;
		norm[0] = d1[1] * d2[2] - d1[2] * d2[1];
		norm[1] = d1[2] * d2[0] - d1[0] * d2[2];
		norm[2] = d1[0] * d2[1] - d1[1] * d2[0];
		d = (float) sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);
		norm[0] /= (d ? d : 1);
		norm[1] /= (d ? d : 1);
		norm[2] /= (d ? d : 1);
	}


	glBegin(GL_TRIANGLES);
		if (lightsAreOn) glNormal3f(norm[0], norm[1], norm[2]);
		glVertex3f(x1, y1, z1); 
		glVertex3f(x2, y2, z2); 
		glVertex3f(x3, y3, z3); 
	glEnd();
}


 
//------------------------------------------------------------------------
//	Shift the elements to equilize distances between them. The constraints 
//	are that a selected element can not move and edges tend not to move.
//------------------------------------------------------------------------
int EquilizeElementPositions(DeformObject *T)
{
	int				i;
	int				equilibrium = 1;
	int				neighbors;
	float			threshold = 0.05f;
	float			dif;
	float			moveX, moveY, moveZ;
	float			x, y, z;
	float			minEqu = T->minEqu;
	float			maxEqu = T->maxEqu;
	float			maxEquShear = T->maxEquShear;
	DeformElement	*object = T->object;


	//----Un-ordered adjustment of interior points except selected element 
	for (i= 0; i < T->objSize; i++) {
		if (&(object[i]) != T->selectedElementPtr) {
			x = object[i].x;
			y = object[i].y;
			z = object[i].z;
			moveX = moveY = moveZ = 0;
			neighbors = 0;


			//----Adjust voxel position if equilibrium constraints are not satisfied 
		    //----Distance to left voxel neighbor 
			if (object[i].left != NULL) {
				neighbors++;

				if ((dif = (x - object[i].left->x)) > maxEqu) moveX -= dif - maxEqu;
				else if (dif < minEqu) moveX -= dif - minEqu;
			
				if ((dif = (y - object[i].left->y)) > maxEquShear) moveY -= dif - maxEquShear;
				else if (dif < - maxEquShear) moveY -= dif + maxEquShear;
			}


			//----Distance to right voxel neighbor 
			if (object[i].right != NULL) {
				neighbors++;

				if ((dif = (object[i].right->x - x)) > maxEqu) moveX += dif - maxEqu;
				else if (dif < minEqu) moveX += dif - minEqu;
		  
				if ((dif = (object[i].right->y - y)) > maxEquShear) moveY += dif - maxEquShear;	  
				else if (dif < - maxEquShear) moveY += dif + maxEquShear;
			}


			//----Distance to top voxel neighbor 
			if (object[i].top != NULL) {
				neighbors++;

				if ((dif = (object[i].top->y - y)) > maxEqu) moveY += dif - maxEqu;
				else if (dif < minEqu) moveY += dif - minEqu;

				if ((dif = (object[i].top->x - x)) > maxEquShear) moveX += dif - maxEquShear;
				else if (dif < - maxEquShear) moveX += dif + maxEquShear;
			}

			//----Distance to bottom voxel neighbor 
			if (object[i].bottom != NULL) {
				neighbors++;

				if ((dif = (y - object[i].bottom->y)) > maxEqu) moveY -= dif - maxEqu;
				else if (dif < minEqu) moveY -= dif - minEqu;

				if ((dif = (x - object[i].bottom->x)) > maxEquShear) moveX -= dif - maxEquShear;
				else if (dif < - maxEquShear) moveX -= dif + maxEquShear;
			}
    
			//----Distance to front voxel neighbor 
			if (object[i].back != NULL) {
				neighbors++;
				
				if ((dif = (z - object[i].back->z)) > maxEqu) moveZ -= dif - maxEqu;
				else if (dif < minEqu) moveZ -= dif - minEqu;
				
				if ((dif = (x - object[i].back->x)) > maxEquShear) moveX -= dif - maxEquShear; 
				else if (dif < - maxEquShear) moveX -= dif + maxEquShear;
      
				if ((dif = (y - object[i].back->y)) > maxEquShear) moveY -= dif - maxEquShear;
				else if (dif < - maxEquShear) moveY -= dif + maxEquShear;
			}
      
			//----Distance to back voxel neighbor 
			if (object[i].front != NULL) {
				neighbors++;

				if ((dif = (object[i].front->z - z)) > maxEqu) moveZ += dif - maxEqu;
				else if (dif < minEqu) moveZ += dif - minEqu;

				if ((dif = (object[i].front->x - x)) > maxEquShear) moveX += dif - maxEquShear;
				else if (dif < - maxEquShear) moveX += dif + maxEquShear;

				if ((dif = (object[i].front->y - y)) > maxEquShear) moveY += dif - maxEquShear;
				else if (dif < - maxEquShear) moveY += dif + maxEquShear;
			}

			
			if ((fabs(moveX) > threshold) || (fabs(moveY) > threshold) || 
			(fabs(moveZ) > threshold)) {
				//----Average over 6 neighbors 
				moveX /= neighbors;
				moveY /= neighbors;
				moveZ /= neighbors;
				object[i].x += moveX;
				object[i].y += moveY;
				object[i].z += moveZ;
				equilibrium = 0;
			}
		}
	}
	return(equilibrium);
}


  
//-------------------------------------------------------------------------
//	Move the selected element to the desired position.  Then move neighbors
//	to their minimum allowable positions relative to the moved element 
//	until no more movement is required.
//------------------------------------------------------------------------
int TestMove(DeformObject *T, float *dx, float *dy, float *dz, int translate)
{
	int				n;
	int				nTop, nBottom, nLeft, nRight, nFront, nBack;
	int				iTop, iBottom, iLeft, iRight, iFront, iBack;
	float			x, y, z;
	float			maxDeform = T->maxDeform;
	float			minDeform = T->minDeform;
	float			maxDeformShear = T->maxDeformShear;
	DeformElement	*selectedElement = T->selectedElementPtr;


	iTop = iBottom = iLeft = iRight = iFront = iBack = 0;
	nTop = nBottom = nLeft = nRight = nFront = nBack = 0;


	if (translate) {
		for (n = 0; n < T->objSize; ++n) {
			T->object[n].x += *dx;
			T->object[n].y += *dy;
			T->object[n].z += *dz;
		}
		return TRUE;
	} else {
		selectedElement->x += *dx;
		selectedElement->y += *dy;
		selectedElement->z += *dz;
	}
  

	//----Add neighbors of the selected voxel to list for movement consideration 
	AddToNeighborList(selectedElement->top, top, &nTop);
	AddToNeighborList(selectedElement->bottom, bottom, &nBottom);
	AddToNeighborList(selectedElement->left, left, &nLeft);
	AddToNeighborList(selectedElement->right, right, &nRight);
	AddToNeighborList(selectedElement->front, front, &nFront);
	AddToNeighborList(selectedElement->back, back, &nBack);


	//----Process lists of neighbor vertices to be considered for movement 
	while ((iRight < nRight) || (iLeft < nLeft) || (iBottom < nBottom) ||
		(iTop < nTop) || (iFront < nFront) || (iBack < nBack)) {


		//----Consider front vertices of the selected neighbor 
		while (iFront < nFront) {
			x = front[iFront]->x;
			y = front[iFront]->y;
			z = front[iFront]->z;

			if (NewPosition(&x, &y, &z, 
			front[iFront]->back->x, front[iFront]->back->y, front[iFront]->back->z, 
			maxDeformShear, -maxDeformShear, maxDeformShear, -maxDeformShear, 
			maxDeform, minDeform) == TRUE) {
	
				//----Save old position and move to new position if it is allowable 
				front[iFront]->x = x;
				front[iFront]->y = y;
				front[iFront]->z = z;


				//----Add neighbors to list for movement consideration 
				AddToNeighborList(front[iFront]->front, front, &nFront);
				AddToNeighborList(front[iFront]->left, left, &nLeft);
				AddToNeighborList(front[iFront]->right, right, &nRight);
				AddToNeighborList(front[iFront]->top, top, &nTop);
				AddToNeighborList(front[iFront]->bottom, bottom, &nBottom); 
			}     
			iFront++;
		}


		//----Consider back vertices of the selected neighbor 
		while (iBack < nBack) {
			x = back[iBack]->x;
			y = back[iBack]->y;
			z = back[iBack]->z;
      
			if (NewPosition(&x, &y, &z, 
			back[iBack]->front->x, back[iBack]->front->y, back[iBack]->front->z, 
			maxDeformShear, -maxDeformShear, maxDeformShear, -maxDeformShear,
			-minDeform, -maxDeform) == TRUE) {
		
				//----Save old position and move to new position if it is allowable 
				back[iBack]->x = x;
				back[iBack]->y = y;
				back[iBack]->z = z;


				//----Add neighbors to list for movement consideration 
				AddToNeighborList(back[iBack]->back, back, &nBack);
				AddToNeighborList(back[iBack]->left, left, &nLeft);
				AddToNeighborList(back[iBack]->right, right, &nRight);
				AddToNeighborList(back[iBack]->top, top, &nTop);
				AddToNeighborList(back[iBack]->bottom, bottom, &nBottom); 
			}     
			iBack++;
		}

		//----Consider right vertices of the selected neighbor 
		while (iRight < nRight) {
			x = right[iRight]->x;
			y = right[iRight]->y;
			z = right[iRight]->z;
      
			if (NewPosition(&x, &y, &z, 
			right[iRight]->left->x, right[iRight]->left->y, right[iRight]->left->z, 
			maxDeform, minDeform, maxDeformShear, -maxDeformShear,
			maxDeformShear, -maxDeformShear) == TRUE) {
		
				//----Save old position and move to new position if it is allowable 
				right[iRight]->x = x;
				right[iRight]->y = y;
				right[iRight]->z = z;


				//----Add neighbors to list for movement consideration 
				AddToNeighborList(right[iRight]->right, right, &nRight);
				AddToNeighborList(right[iRight]->top, top, &nTop);
				AddToNeighborList(right[iRight]->bottom, bottom, &nBottom); 
		  }     
		  iRight++;
		}

		//----Consider left vertices of the selected neighbor 
		while (iLeft < nLeft) {
			x = left[iLeft]->x;
			y = left[iLeft]->y;
			z = left[iLeft]->z;

			if (NewPosition(&x, &y, &z, 
			left[iLeft]->right->x, left[iLeft]->right->y, left[iLeft]->right->z,
			-minDeform, -maxDeform, maxDeformShear, -maxDeformShear, 
			maxDeformShear, -maxDeformShear) == TRUE) {

				//----Save old position and move to new position if it is allowable 
				left[iLeft]->x = x;
				left[iLeft]->y = y;
				left[iLeft]->z = z;


				//----Add neighbors to list for movement consideration 
				AddToNeighborList(left[iLeft]->left, left, &nLeft);
				AddToNeighborList(left[iLeft]->top, top, &nTop);
				AddToNeighborList(left[iLeft]->bottom, bottom, &nBottom);
		  }
		  iLeft++;
		}


		//----Consider top neighboring vertices of the selected neighbor 
		while (iTop < nTop) {
			x = top[iTop]->x;
			y = top[iTop]->y;
			z = top[iTop]->z;

			if (NewPosition(&x, &y, &z, 
			top[iTop]->bottom->x, top[iTop]->bottom->y, top[iTop]->bottom->z,
			maxDeformShear, -maxDeformShear, maxDeform, minDeform,
			maxDeformShear, -maxDeformShear) == TRUE) {

				//----Save old position and move to new position if it is allowable 
				top[iTop]->x = x;
				top[iTop]->y = y;
				top[iTop]->z = z;


 				//----Add top neighbor to list for movement consideration 
				AddToNeighborList(top[iTop]->top, top, &nTop);
				if (1) {
				  if (top[iTop]->top && top[iTop]->top->right && !top[iTop]->right)	
					  AddToNeighborList(top[iTop]->top->right, right, &nRight);
				  if (top[iTop]->top && top[iTop]->top->left && !top[iTop]->left) 
					  AddToNeighborList(top[iTop]->top->left, left, &nLeft);

				} else {
				  if (top[iTop]->bottom->right) AddToNeighborList(top[iTop]->right, right, &nRight);
				  if (top[iTop]->bottom->left ) AddToNeighborList(top[iTop]->left,   left,  &nLeft);
				}
		  }
		  iTop++;
		}


		//----Consider bottom neighboring vertices of the selected neighbor 
		while (iBottom < nBottom) {
			x = bottom[iBottom]->x;
			y = bottom[iBottom]->y;
			z = bottom[iBottom]->z;

			if (NewPosition(&x, &y, &z, 
			bottom[iBottom]->top->x, bottom[iBottom]->top->y, bottom[iBottom]->top->z,
			maxDeformShear, -maxDeformShear, -minDeform, -maxDeform,
			maxDeformShear, -maxDeformShear) == TRUE) {
		
				//----Save old position and move to new position if it is allowable 
				bottom[iBottom]->x = x;
				bottom[iBottom]->y = y;
				bottom[iBottom]->z = z;


				//----Add neighbors to list for movement consideration 
				AddToNeighborList(bottom[iBottom]->bottom, bottom, &nBottom);
				if (1) {
					if (bottom[iBottom]->bottom && bottom[iBottom]->bottom->right && 
					!bottom[iBottom]->right)
					AddToNeighborList(bottom[iBottom]->bottom->right, right, &nRight);
					if (bottom[iBottom]->bottom && bottom[iBottom]->bottom->left && 
					!bottom[iBottom]->left)
					AddToNeighborList(bottom[iBottom]->bottom->left, left, &nLeft);

				} else {
				  if (bottom[iBottom]->top->right) AddToNeighborList(bottom[iBottom]->right, right, &nRight);
				  if (bottom[iBottom]->top->left ) AddToNeighborList(bottom[iBottom]->left,   left,  &nLeft);
				}
			}	
			iBottom++;
		}
	}

	 //----Return success 
	return TRUE;
}


//------------------------------------------------------------------------
//	Add an element to the list of neighbors for movement consideration
//------------------------------------------------------------------------
int AddToNeighborList(DeformElement *elementPtr, DeformElement *list[], 
int *listIndex)
{
  if (elementPtr != NULL){
    list[*listIndex] = elementPtr;
    (*listIndex)++;
    return TRUE;
  }

  return FALSE;
}


//--------------------------------------------------------------------------
//	Compare the positions of a vertex at (x,y) and its neighbor at (x2,y2).  
//	If the relative x and y distances are outside of the given ranges 
//	(MinDx, MaxDx) and (minDy, maxDy) respectively, the position (x,y) is 
//	adjusted minimally to satisfy the constraints.
//--------------------------------------------------------------------------
int NewPosition(float *x, float *y, float *z, float x2, float y2, float z2,
float maxDx, float minDx, float maxDy, float minDy, float maxDz, float minDz)
{
	int moved = FALSE;
	float dif;


	if ((dif = *x - x2) > maxDx) {
		moved = TRUE;
		*x = x2 + maxDx;
	} else if (dif < minDx) {
		moved = TRUE;
		*x = x2 + minDx;
	}


	if ((dif = *y - y2) > maxDy) {
		moved = TRUE;
		*y = y2 + maxDy;
	} else if (dif < minDy) {
		moved = TRUE;
		*y = y2 + minDy;
	}


	if ((dif = *z - z2) > maxDz) {
		moved = TRUE;
		*z = z2 + maxDz;
	} else if (dif < minDz) {
		moved = TRUE;
		*z = z2 + minDz;
	}
	return moved;
}


 
//------------------------------------------------------------------------
//	Create a rectangular ChainMail object
//------------------------------------------------------------------------
int MakeCMCubeObject(DeformObject *T, int xSize, int ySize, int zSize,
float xOffset, float yOffset, float zOffset, float xyzScale, int xhLo,
int xhHi, int yhLo, int yhHi)
{
	int surface;
	int i, j, k, index;
	int rightOffset, leftOffset;
	int frontOffset, backOffset;
	int topOffset, bottomOffset;
	float x, y, z;
	DeformElement *object;
	DeformElement *tempPtr[CM3D_MAX_VSIZE];
	DeformElement *v1, *v2, *v3, *v4;


	if ((xhHi - xhLo) < 2 || (yhHi - yhLo) < 2) {
		xhLo = 0;
		xhHi = 0;
		yhLo = 0;
		yhHi = 0;
	}


	//T->equilize = FALSE;
	T->equilize = TRUE;
	T->doingChainMail = FALSE;
	T->selectedElementPtr = 0;


	//----Allocate space for the object elements 
	T->object = object = (DeformElement *) calloc(xSize*ySize*zSize, sizeof(DeformElement));
	if (!object) return(0);


	//----Create object, placing it at the center of the screen 
	rightOffset = 1;
	leftOffset = -1;
	topOffset = -xSize;
	bottomOffset = xSize;
	frontOffset = xSize * ySize;
	backOffset = - xSize * ySize;


	T->numSurface = 0;
	index = 0;
	for (k = 0; k < zSize; k++) {                     
		for (j = 0; j < ySize; j++) {                     
			for (i = 0; i < xSize; i++) {
				surface = FALSE;
				x = object[index].x = xyzScale * (xOffset + i);
				y = object[index].y = xyzScale * (yOffset + ySize - 1 - j);
				z = object[index].z = xyzScale * (zOffset + k);


				if (i == xhLo && j > yhLo && j < yhHi) {
					object[index].right = NULL;
					surface = TRUE;
				} else if (i == xhLo && (yhHi - yhLo) > 0 && (j == yhLo || j == yhHi)) {
					object[index].right = &(object[index + rightOffset]);
					surface = TRUE;
				} else if (i > xhLo && i < xhHi && j > yhLo && j < yhHi) {
					object[index].right = NULL;
				} else if (i < xSize - 1) {
					object[index].right = &(object[index + rightOffset]);
				} else {
					object[index].right = NULL;	
					surface = TRUE;
				}


				if (i == xhHi && j > yhLo && j < yhHi) {
					object[index].left = NULL;
					surface = TRUE;
				} else if (i == xhHi && (yhHi - yhLo) > 0 && (j == yhLo || j == yhHi)) {
					object[index].left = &(object[index + leftOffset]);
					surface = TRUE;
				} else if (i > xhLo && i < xhHi && j > yhLo && j < yhHi) {
					object[index].left = NULL;
				} else if (i > 0) {
					object[index].left = &(object[index + leftOffset]);
				} else {
					object[index].left = NULL;
					surface = TRUE;
				}


				if (j == yhLo && i > xhLo && i < xhHi) {
					object[index].bottom = NULL;
					surface = TRUE;
				} else if ((j == yhLo) && (xhHi - xhLo) > 0 && (i == xhLo || i == xhHi)) {
					object[index].bottom = &(object[index + bottomOffset]);
					surface = TRUE;
				} else if (i > xhLo && i < xhHi && j > yhLo && j < yhHi) {
					object[index].bottom = NULL;
				} else if (j < ySize - 1) {
					object[index].bottom = &(object[index + bottomOffset]);
				} else {
					object[index].bottom = NULL;
					surface = TRUE;
				}


				if (j == yhHi && i > xhLo && i < xhHi) {
					object[index].top = NULL;
					surface = TRUE;
				} else if ((j == yhHi) && (xhHi - xhLo) > 0 && (i == xhLo || i == xhHi)) {
					object[index].top = &(object[index + topOffset]);
					surface = TRUE;
				} else if (i > xhLo && i < xhHi && j > yhLo && j < yhHi) {
					object[index].top = NULL;
				} else if (j > 0) {
					object[index].top = &(object[index + topOffset]);
				} else {
					object[index].top = NULL;
					surface = TRUE;
				}


				if (i > xhLo && i < xhHi && j > yhLo && j < yhHi) {
					object[index].front = NULL;
					object[index].back = NULL;
				} else {
					if (k < zSize - 1) object[index].front = &(object[index + frontOffset]);
					else {
						object[index].front = NULL;
						surface = TRUE;
					}
					if (k > 0) object[index].back = &(object[index + backOffset]);
					else {
						object[index].back = NULL;
						surface = TRUE;
					}
				}

				if (surface == TRUE) tempPtr[T->numSurface++] = &(object[index]);
				index++;
			}
		}
	}
  

	//----Record object size 
	T->objSize = index;


	//----Allocate space for the array of surface element pointers 
	T->surfacePtr = (DeformElement **) calloc(T->numSurface, sizeof(DeformElement *));
	if (!T->surfacePtr) {
		free(T->object);
		return(0);
	}
  

	//----Set surface elements 
	for (i = 0; i < T->numSurface; i++) T->surfacePtr[i] = tempPtr[i];


	//----Pre-set marker points at cube corners for selection
	T->numMarkers = 0;


	//----Marker 0: in middle of back surface 
	T->marker[T->numMarkers].elPtr = &(object[xSize/2 + (ySize/2)*xSize]);
	T->marker[T->numMarkers].v1 = v1 = T->marker[T->numMarkers].elPtr->right;
	T->marker[T->numMarkers].v2 = v2 = T->marker[T->numMarkers].elPtr->bottom;
	T->marker[T->numMarkers].v3 = v3 = T->marker[T->numMarkers].elPtr->left;
	T->marker[T->numMarkers].v4 = v4 = T->marker[T->numMarkers].elPtr->top;
	if (v1 && v2 && v3 && v4) T->numMarkers++;


	//----Marker 1: in middle of right surface 
	T->marker[T->numMarkers].elPtr = &(object[xSize-1 + (ySize/2)*xSize + (zSize/2)*xSize*ySize]);
	T->marker[T->numMarkers].v1 = v1 = T->marker[T->numMarkers].elPtr->front;
	T->marker[T->numMarkers].v2 = v2 = T->marker[T->numMarkers].elPtr->bottom;
	T->marker[T->numMarkers].v3 = v3 = T->marker[T->numMarkers].elPtr->back;
	T->marker[T->numMarkers].v4 = v4 = T->marker[T->numMarkers].elPtr->top;
	if (v1 && v2 && v3 && v4) T->numMarkers++;


	//----Marker 3: in top, back, left corner 
	T->marker[T->numMarkers].elPtr = &(object[0]);
	T->marker[T->numMarkers].v1 = T->marker[T->numMarkers].elPtr->front;
	T->marker[T->numMarkers].v2 = T->marker[T->numMarkers].elPtr->right;
	T->marker[T->numMarkers].v3 = T->marker[T->numMarkers].elPtr->bottom;
	T->marker[T->numMarkers].v4 = NULL;
	T->numMarkers++;


	//----Marker 4: in top, back, right corner 
	T->marker[T->numMarkers].elPtr = &(object[xSize-1]);
	T->marker[T->numMarkers].v1 = T->marker[T->numMarkers].elPtr->front;
	T->marker[T->numMarkers].v2 = T->marker[T->numMarkers].elPtr->right;
	T->marker[T->numMarkers].v3 = T->marker[T->numMarkers].elPtr->bottom;
	T->marker[T->numMarkers].v4 = NULL;
	T->numMarkers++;


	//----Marker 5: in bottom, back, left corner 
	T->marker[T->numMarkers].elPtr = &(object[xSize * (ySize - 1)]);
	T->marker[T->numMarkers].v1 = T->marker[T->numMarkers].elPtr->front;
	T->marker[T->numMarkers].v2 = T->marker[T->numMarkers].elPtr->right;
	T->marker[T->numMarkers].v3 = T->marker[T->numMarkers].elPtr->bottom;
	T->marker[T->numMarkers].v4 = NULL;
	T->numMarkers++;
  

	//----Marker 6: in bottom, back, right corner 
	T->marker[T->numMarkers].elPtr = &(object[xSize * ySize - 1]);
	T->marker[T->numMarkers].v1 = T->marker[T->numMarkers].elPtr->front;
	T->marker[T->numMarkers].v2 = T->marker[T->numMarkers].elPtr->right;
	T->marker[T->numMarkers].v3 = T->marker[T->numMarkers].elPtr->bottom;
	T->marker[T->numMarkers].v4 = NULL;
	T->numMarkers++;
  

	//----Marker 7: in top, front, left corner 
	T->marker[T->numMarkers].elPtr = &(object[xSize * ySize * (zSize - 1)]);
	T->marker[T->numMarkers].v1 = T->marker[T->numMarkers].elPtr->front;
	T->marker[T->numMarkers].v2 = T->marker[T->numMarkers].elPtr->right;
	T->marker[T->numMarkers].v3 = T->marker[T->numMarkers].elPtr->bottom;
	T->marker[T->numMarkers].v4 = NULL;
	T->numMarkers++;
  

	//----Marker 8: in top, front, right corner 
	T->marker[T->numMarkers].elPtr = &(object[xSize * ySize * (zSize - 1) + (xSize - 1)]);
	T->marker[T->numMarkers].v1 = T->marker[T->numMarkers].elPtr->front;
	T->marker[T->numMarkers].v2 = T->marker[T->numMarkers].elPtr->right;
	T->marker[T->numMarkers].v3 = T->marker[T->numMarkers].elPtr->bottom;
	T->marker[T->numMarkers].v4 = NULL;
	T->numMarkers++;


	//----Set equilibrium and deformation constraints 
	T->minDeform		= (object[0].right->x - object[0].x);
	T->maxDeform		= (float) MAX_DEFORM * (object[0].right->x - object[0].x);
	T->minEqu			= (object[0].right->x - object[0].x);
	T->maxEqu			= (object[0].right->x - object[0].x);
	T->maxDeformShear	= 1 * (object[0].right->x - object[0].x);
	T->maxEquShear		= 1 * (object[0].right->x - object[0].x);
	T->maxDeformInit	= T->maxDeform;
	T->maxEquInit		= T->maxEqu;


	//----Return success 
	return 1;
}


//------------------------------------------------------------------------
//	Callback for button press
//------------------------------------------------------------------------
int ButtonPress(HWND hWnd, int mX, int mY)
{
	int i;
	int width, height;
	float x, y;
	float xTemp;
	float minDist;
	RECT rect;
	DeformObject *T;


	GetClientRect(ghWnd, &rect);
	width = rect.right;
	height = rect.bottom;
	pointerX = (float) mX;
	pointerY = (float) mY;
	pointerY = (height - pointerY);
	buttonPressFlag = TRUE;
	x = pointerX * V_WORLD_X_SPAN / width;
	y = pointerY * V_WORLD_Y_SPAN / height;


	//----If button press is close to a ChainMail marker, select that element 
	T = &cmObject;
	T->selectedElementPtr = NULL;
	minDist = 2.0;

	for (i = 0; i < T->numMarkers; i++) {
		xTemp = (float) (cos(3.14159 * viewAngle / 180) * T->marker[i].elPtr->x + 
		sin(3.14159 * viewAngle / 180) * T->marker[i].elPtr->z);

		if ((fabs(x - xTemp) < minDist) && (fabs(y - T->marker[i].elPtr->y) < minDist)) {
		T->selectedElementPtr = T->marker[i].elPtr;
		return(0);
		}
	}

	return(0);
}


//------------------------------------------------------------------------
//	Callback for button release
//------------------------------------------------------------------------
int ButtonRelease(HWND hWnd)
{
	buttonPressFlag = FALSE;
	cmObject.selectedElementPtr = NULL;
	return(0);
}


//------------------------------------------------------------------------
//	Callback for pointer motion
//------------------------------------------------------------------------
int PointerMotion(HWND hWnd, int mX, int mY, int translate)
{
	int width, height;
	int moveValid;
	float x1, y1, xStep, yStep, zStep;
	RECT rect;
	DeformObject *T = 0;


	if (!buttonPressFlag) return 0;
	GetClientRect(ghWnd, &rect);
	width = rect.right;
	height = rect.bottom;
	x1 = (float) mX;
	y1 = (float) mY;
	y1 = height - y1;


	if (cmObject.selectedElementPtr) {
		//----Grasp object and pull, allowing it to deform. Scale step size to object size 
		T = &cmObject;
		xStep = (float) cos(3.14159 * viewAngle / 180) * (x1 - pointerX) * V_WORLD_X_SPAN / width;
		yStep = (y1 - pointerY) * V_WORLD_Y_SPAN / height;
		zStep = 0;
		T->doingChainMail = TRUE;
		moveValid = TestMove(T, &xStep, &yStep, &zStep, translate);
		T->doingChainMail = FALSE;
		if (moveValid) {
			pointerX = x1;
			pointerY = y1;
			InvalidateRect(ghWnd, &rect, FALSE);
		}
	}
	return(0);
}


//------------------------------------------------------------------------
//	Callback for timing function
//------------------------------------------------------------------------
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	int relaxed;
	RECT rect;
	if (cmObject.equilize && !cmObject.doingChainMail) {
		relaxed = EquilizeElementPositions(&cmObject);
		if (!relaxed) {
			GetClientRect(hWnd, &rect);
			InvalidateRect(hWnd, &rect, FALSE);
		}
	}
}
