#include <chainMail.h>

//----Lists of neighbor pointers for movement consideration
static DeformElement *top	[2*CM3D_MAX_VSIZE];
static DeformElement *bottom[2*CM3D_MAX_VSIZE];
static DeformElement *leftlist [2*CM3D_MAX_VSIZE];
static DeformElement *rightlist	[2*CM3D_MAX_VSIZE];
static DeformElement *front	[2*CM3D_MAX_VSIZE];
static DeformElement *back	[2*CM3D_MAX_VSIZE];

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
        return 1;
    } else {
        selectedElement->x += *dx;
        selectedElement->y += *dy;
        selectedElement->z += *dz;
    }


    //----Add neighbors of the selected voxel to list for movement consideration
    AddToNeighborList(selectedElement->top, top, &nTop);
    AddToNeighborList(selectedElement->bottom, bottom, &nBottom);
    AddToNeighborList(selectedElement->left, leftlist, &nLeft);
    AddToNeighborList(selectedElement->right, rightlist, &nRight);
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
            maxDeform, minDeform) == 1) {

                //----Save old position and move to new position if it is allowable
                front[iFront]->x = x;
                front[iFront]->y = y;
                front[iFront]->z = z;


                //----Add neighbors to list for movement consideration
                AddToNeighborList(front[iFront]->front, front, &nFront);
                AddToNeighborList(front[iFront]->left, leftlist, &nLeft);
                AddToNeighborList(front[iFront]->right, rightlist, &nRight);
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
            -minDeform, -maxDeform) == 1) {

                //----Save old position and move to new position if it is allowable
                back[iBack]->x = x;
                back[iBack]->y = y;
                back[iBack]->z = z;


                //----Add neighbors to list for movement consideration
                AddToNeighborList(back[iBack]->back, back, &nBack);
                AddToNeighborList(back[iBack]->left, leftlist, &nLeft);
                AddToNeighborList(back[iBack]->right, rightlist, &nRight);
                AddToNeighborList(back[iBack]->top, top, &nTop);
                AddToNeighborList(back[iBack]->bottom, bottom, &nBottom);
            }
            iBack++;
        }

        //----Consider right vertices of the selected neighbor
        while (iRight < nRight) {
            x = rightlist[iRight]->x;
            y = rightlist[iRight]->y;
            z = rightlist[iRight]->z;

            if (NewPosition(&x, &y, &z,
            rightlist[iRight]->left->x, rightlist[iRight]->left->y, rightlist[iRight]->left->z,
            maxDeform, minDeform, maxDeformShear, -maxDeformShear,
            maxDeformShear, -maxDeformShear) == 1) {

                //----Save old position and move to new position if it is allowable
                rightlist[iRight]->x = x;
                rightlist[iRight]->y = y;
                rightlist[iRight]->z = z;


                //----Add neighbors to list for movement consideration
                AddToNeighborList(rightlist[iRight]->right, rightlist, &nRight);
                AddToNeighborList(rightlist[iRight]->top, top, &nTop);
                AddToNeighborList(rightlist[iRight]->bottom, bottom, &nBottom);
          }
          iRight++;
        }

        //----Consider left vertices of the selected neighbor
        while (iLeft < nLeft) {
            x = leftlist[iLeft]->x;
            y = leftlist[iLeft]->y;
            z = leftlist[iLeft]->z;

            if (NewPosition(&x, &y, &z,
            leftlist[iLeft]->right->x, leftlist[iLeft]->right->y, leftlist[iLeft]->right->z,
            -minDeform, -maxDeform, maxDeformShear, -maxDeformShear,
            maxDeformShear, -maxDeformShear) == 1) {

                //----Save old position and move to new position if it is allowable
                leftlist[iLeft]->x = x;
                leftlist[iLeft]->y = y;
                leftlist[iLeft]->z = z;


                //----Add neighbors to list for movement consideration
                AddToNeighborList(leftlist[iLeft]->left, leftlist, &nLeft);
                AddToNeighborList(leftlist[iLeft]->top, top, &nTop);
                AddToNeighborList(leftlist[iLeft]->bottom, bottom, &nBottom);
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
            maxDeformShear, -maxDeformShear) == 1) {

                //----Save old position and move to new position if it is allowable
                top[iTop]->x = x;
                top[iTop]->y = y;
                top[iTop]->z = z;


                //----Add top neighbor to list for movement consideration
                AddToNeighborList(top[iTop]->top, top, &nTop);
                if (1) {
                  if (top[iTop]->top && top[iTop]->top->right && !top[iTop]->right)
                      AddToNeighborList(top[iTop]->top->right, rightlist, &nRight);
                  if (top[iTop]->top && top[iTop]->top->left && !top[iTop]->left)
                      AddToNeighborList(top[iTop]->top->left, leftlist, &nLeft);

                } else {
                  if (top[iTop]->bottom->right) AddToNeighborList(top[iTop]->right, rightlist, &nRight);
                  if (top[iTop]->bottom->left ) AddToNeighborList(top[iTop]->left,   leftlist,  &nLeft);
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
            maxDeformShear, -maxDeformShear) == 1) {

                //----Save old position and move to new position if it is allowable
                bottom[iBottom]->x = x;
                bottom[iBottom]->y = y;
                bottom[iBottom]->z = z;


                //----Add neighbors to list for movement consideration
                AddToNeighborList(bottom[iBottom]->bottom, bottom, &nBottom);
                if (1) {
                    if (bottom[iBottom]->bottom && bottom[iBottom]->bottom->right &&
                    !bottom[iBottom]->right)
                    AddToNeighborList(bottom[iBottom]->bottom->right, rightlist, &nRight);
                    if (bottom[iBottom]->bottom && bottom[iBottom]->bottom->left &&
                    !bottom[iBottom]->left)
                    AddToNeighborList(bottom[iBottom]->bottom->left, leftlist, &nLeft);

                } else {
                  if (bottom[iBottom]->top->right) AddToNeighborList(bottom[iBottom]->right, rightlist, &nRight);
                  if (bottom[iBottom]->top->left ) AddToNeighborList(bottom[iBottom]->left,   leftlist,  &nLeft);
                }
            }
            iBottom++;
        }
    }

     //----Return success
    return 1;
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
    return 1;
  }

  return 0;
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
    int moved = 0;
    float dif;


    if ((dif = *x - x2) > maxDx) {
        moved = 1;
        *x = x2 + maxDx;
    } else if (dif < minDx) {
        moved = 1;
        *x = x2 + minDx;
    }


    if ((dif = *y - y2) > maxDy) {
        moved = 1;
        *y = y2 + maxDy;
    } else if (dif < minDy) {
        moved = 1;
        *y = y2 + minDy;
    }


    if ((dif = *z - z2) > maxDz) {
        moved = 1;
        *z = z2 + maxDz;
    } else if (dif < minDz) {
        moved = 1;
        *z = z2 + minDz;
    }
    return moved;
}

