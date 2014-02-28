#ifndef CS488_MESH_H
#define CS488_MESH_H

#include <vector>
#include <iosfwd>
#include "primitive.h"
#include "algebra.h"
#include <sw/include/PQP.h>

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);

  virtual void walk_gl(bool picking) const;
  PQP_Model* getmodel(){
      return m_model;
  }

  bool lq_tolerance(PQP_REAL R1[3][3], PQP_REAL T1[3],PQP_Model *other_model,PQP_REAL R2[3][3], PQP_REAL T2[3], PQP_REAL tolerance);
  double getLength();

  typedef std::vector<int> Face;
  
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;
  PQP_Model* m_model;
  void builmodel();



  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
