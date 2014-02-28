//-------------------------------------------------------------------------
//	"3D ChainMail: a Fast Algorithm for Deforming Volumetric Objects", 
//	Proceedings 1997 Symposium on Interactive 3D Graphics, April 1997, 
//	pp. 149-154, by S. Frisken (Gibson).
//
//	Written by S. Frisken, 1996, modified by R. Perry, 1998, updated 2003.
//-------------------------------------------------------------------------


#include <math.h>
#include <stdio.h>

//-------------------------------------------------------------------------
// 3D ChainMail data
//-------------------------------------------------------------------------

#define CM3D_MAX_VSIZE	5*5*5

struct DeformElement {
  float x, y, z;
  struct DeformElement *left, *right, *top, *bottom, *front, *back;   
};

struct DeformObject {
  float maxDeform;
  float minDeform;
  float maxDeformShear;
  int objSize;
  DeformElement *selectedElementPtr;	//----Pointer to selected element
  DeformElement *object;				//----Array of elements
  DeformElement **surfacePtr;			//----Array of pointers to surface elements
  int numinSurface;
  DeformElement *centre;
};
 
//----Procedure prototypes
int TestMove(DeformObject *T, float *dx, float *dy, float *dz, int translate = 0);
int AddToNeighborList(DeformElement *elementPtr, DeformElement *list[], int *listIndex);
int NewPosition(float *x, float *y, float *z, float x2, float y2, float z2, float maxDx,
float minDx, float maxDy, float minDy, float maxDz, float minDz);  

