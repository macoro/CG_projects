
//////////////////////////////////////////////////////////////////////////////
//
// cvxutils.h
//
// Description:
//      Convex decomposition utilities
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _CVXUTILS_H_
#define _CVXUTILS_H_

#include <SWIFT_common.h>
#include <SWIFT_array.h>
#include <SWIFT_mesh.h>

bool Check_Euler_Characteristic( SWIFT_Tri_Mesh* m, SWIFT_Array<int>& mfs,
                                 SWIFT_Array<SWIFT_Tri_Face>& vfs );

void Convex_Utilities_Initialize( SWIFT_Tri_Mesh* m );

void Prepare_Mesh_For_Decomposition( SWIFT_Tri_Mesh* m );

bool Test_For_Intersection( SWIFT_Tri_Mesh* m, int f, int v,
                            SWIFT_Array<int>& ex );

bool Is_Convex_Hull_Sane( int vn, int* fs, int fn );


void Create_First_Face( SWIFT_Tri_Face* f,
                        SWIFT_Array<SWIFT_Tri_Face>& chull,
                        SWIFT_Array<SWIFT_Tri_Face*>& cfs );

// chull, cfs, cvs, exclude, addedfs are created by the parent.
// addedfs should be a list of length 0 and a big enough max_length.
bool Add_To_Convex_Hull( SWIFT_Tri_Mesh* m,
                         SWIFT_Array<SWIFT_Tri_Face>& chull,
                         SWIFT_Array<SWIFT_Tri_Face*>& cfs,
                         SWIFT_Array<bool>& fallowed,
                         SWIFT_Array<bool>& cvs,
                         SWIFT_Array<int>& addedfs,
                         SWIFT_Tri_Face* f,
                         SWIFT_Tri_Edge* e,
                         SWIFT_Tri_Vertex* v );

void Attach_Twins( SWIFT_Array<SWIFT_Tri_Face>& new_faces );

// Sorting used to sort pieces in ascending order by size
void Quicksort( SWIFT_Array< SWIFT_Array<int> >& mfs,
                SWIFT_Array<int>& mfs_mapping, int p, int r );



// Priority Queue for face list lengths -- descend. order (used by cresting BFS)
void Verify_Heap( SWIFT_Array<int>& lengths );
void Build_Heap( SWIFT_Array<int>& lengths, SWIFT_Array<int>& bmap,
                 SWIFT_Array<int>& fmap );
void Heapify( SWIFT_Array<int>& lengths, SWIFT_Array<int>& bmap,
              SWIFT_Array<int>& fmap, int i );
void Up_Heap( SWIFT_Array<int>& lengths, SWIFT_Array<int>& bmap,
              SWIFT_Array<int>& fmap, int i );
void Heap_New_Value( SWIFT_Array<int>& lengths, SWIFT_Array<int>& bmap,
                     SWIFT_Array<int>& fmap, int i, int new_val );
void Heap_Dec_Value( SWIFT_Array<int>& lengths, SWIFT_Array<int>& bmap,
                     SWIFT_Array<int>& fmap, int i );
void Delete_Heap_Top( SWIFT_Array<int>& lengths, SWIFT_Array<int>& bmap,
                      SWIFT_Array<int>& fmap );
void Delete_From_Heap( SWIFT_Array<int>& lengths, SWIFT_Array<int>& bmap,
                       SWIFT_Array<int>& fmap, int i );

// Is flipping from e1 to e2 allowed?
bool Edge_Flip_Allowed( SWIFT_Tri_Edge* e1, SWIFT_Tri_Edge* e2,
                        SWIFT_Real etol );

// Returns 6*volume of the tetra given by e->Adj_Face and the additional pt
inline SWIFT_Real Tetra_Volume( SWIFT_Tri_Edge* e, const SWIFT_Triple& pt )
{   // pt is a
    const SWIFT_Triple& v1 = e->Origin()->Coords();   // b
    const SWIFT_Triple& v2 = e->Next()->Origin()->Coords();   // c
    const SWIFT_Triple& v3 = e->Prev()->Origin()->Coords();   // d
    SWIFT_Real cd23 = v2.Y() * v3.Z() - v2.Z() * v3.Y();
    SWIFT_Real cd13 = v2.X() * v3.Z() - v2.Z() * v3.X();
    SWIFT_Real cd12 = v2.X() * v3.Y() - v2.Y() * v3.X();
    SWIFT_Real half = v1.X() * cd23 - v1.Y() * cd13 + v1.Z() * cd12 -
                      pt.X() * cd23 + pt.Y() * cd13 - pt.Z() * cd12;
    cd23 = pt.Y() * v1.Z() - pt.Z() * v1.Y();
    cd13 = pt.Y() * v1.Z() - pt.Z() * v1.Y();
    cd12 = pt.Y() * v1.Z() - pt.Z() * v1.Y();
    return fabs( half + v3.X() * cd23 - v3.Y() * cd13 + v3.Z() * cd12 -
                        v2.X() * cd23 + v2.Y() * cd13 - v2.Z() * cd12 );
}


#endif

