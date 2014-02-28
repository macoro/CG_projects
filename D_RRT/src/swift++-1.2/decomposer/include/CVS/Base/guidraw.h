
//////////////////////////////////////////////////////////////////////////////
//
// guidraw.h
//
// Description:
//      Draw utility functions for the user interface.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _GUIDRAW_H_
#define _GUIDRAW_H_

#include <SWIFT_array.h>
#include <SWIFT_mesh.h>

void Draw_Triangle_Mesh( SWIFT_Tri_Mesh* m, bool color
                         , SWIFT_Array<int>& piece_ids );
void Draw_Vertices( SWIFT_Tri_Mesh* m );
void Draw_Edge_Convexity( SWIFT_Tri_Mesh* m, SWIFT_Array<bool>& ecs );
void Draw_Convex_Pieces( SWIFT_Tri_Mesh* m, bool color, bool vfaces, bool tcol,
                         bool explode, SWIFT_Triple& expl_cen,
                         SWIFT_Array<SWIFT_Triple>& coms,
                         SWIFT_Array<int>& piece_ids,
                         SWIFT_Array< SWIFT_Array<int> >& mfs,
                         SWIFT_Array< SWIFT_Array<SWIFT_Tri_Face> >& vfs,
                         SWIFT_Array<int>& wcps );
void Draw_Hierarchy( SWIFT_Tri_Mesh* m, bool color, bool vfaces, bool tcol,
                     bool explode, SWIFT_Triple& expl_cen,
                     SWIFT_Array<int>& piece_ids,
                     SWIFT_Array<SWIFT_BV*>& which_pieces );

#endif

