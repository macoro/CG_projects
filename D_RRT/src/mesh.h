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
  void dform_walk_gl(bool show_cp);

  void deformation(Mesh& deformer, const Matrix4x4& ray_transf, const Matrix4x4& inv_deformertransf);

  typedef std::vector<int> Face;
  std::vector<Point3D> m_verts;
  std::vector<Point3D> m_deformedverts;
  std::vector<Face> m_faces;
  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
  void show_controlpoints(bool v){ m_showcp = v; }
  void reset_controlpoints();
  bool is_deformed(){ return m_isdeformed; }

private:
  void move_controlpoint(float dx, float dy,float dz);
  bool select_controlpoint(int index);
  void center_cm2bb();

  BBox* bounding_box();
  void gen_controlpoints(int cpx,int cpy, int cpz, bool picking = false);
  void global2local();
  void deform_verts(int cpx, int cpy, int cpz);

  BBox* m_bbox;
  std::vector<Point3D> m_controlpoints;
  DeformObject * m_chainmail;
  bool m_selectedpoint;
  bool m_showcp;
  bool m_isdeformed;

  //local data
  Point3D m_Po;
  Vector3D m_S,m_T,m_U;
  std::vector<Point3D> m_localverts;
};

#endif
