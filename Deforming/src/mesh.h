#ifndef CS488_MESH_H
#define CS488_MESH_H

#include <vector>
#include <iosfwd>
#include "primitive.h"
#include "algebra.h"
#include "chainMail.h"
#include <boost/math/distributions/binomial.hpp>
#include <boost/math/special_functions/factorials.hpp>


// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);

  virtual void walk_gl(bool picking);

  void move_controlpoint(float dx, float dy,float dz);

  bool select_controlpoint(int index);

  typedef std::vector<int> Face;

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);






private:
  BBox* bounding_box();
  void gen_controlpoints(int cpx,int cpy, int cpz, bool picking = false);
  void global2local();
  void deform_verts(int cpx, int cpy, int cpz);
  void center_cm2bb();

  BBox* m_bbox;
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;
  std::vector<Point3D> m_controlpoints;
  DeformObject * m_chainmail;
  bool m_selectedpoint;

  //local data
  Point3D m_Po;
  Vector3D m_S,m_T,m_U;
  std::vector<Point3D> m_localverts;

};

#endif
