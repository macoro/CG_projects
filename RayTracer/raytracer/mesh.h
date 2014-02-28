#ifndef CS488_MESH_H
#define CS488_MESH_H

#include <vector>
#include <iosfwd>
#include "primitive.h"
#include "algebra.h"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces, bool hide_box = true);
  //only convexes poligons are supported
  virtual bool IntersectRay(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far=double(1e+4));
  bool closest_triangle(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far
                        , const std::vector< std::vector<int> >& triangles);
  typedef std::vector<int> Face;
  
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;  
  bool hide_box;

  NonhierBox* bounding_box(Matrix4x4 transf);

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
