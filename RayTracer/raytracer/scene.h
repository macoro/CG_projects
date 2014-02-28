#ifndef SCENE_H
#define SCENE_H

#include <list>
#include "algebra.h"
#include "primitive.h"
#include "material.h"

class SceneNode {
public:
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  const Matrix4x4& get_transform() const { return m_trans; }
  const Matrix4x4& get_inverse() const { return m_invtrans; }
  
  void set_transform(const Matrix4x4& m)
  {
    m_trans = m;
    m_invtrans = m.invert();
  }

  void set_transform(const Matrix4x4& m, const Matrix4x4& i)
  {
    m_trans = m;
    m_invtrans = i;
  }

  void add_child(SceneNode* child)
  {
    m_children.push_back(child);
  }

  void remove_child(SceneNode* child)
  {
    m_children.remove(child);
  }

  // Callbacks to be implemented.
  // These will be called from Lua.
  void rotate(char axis, double angle);
  void scale(const Vector3D& amount);
  void translate(const Vector3D& amount);

  static Matrix4x4 rotationMatrix( char axis, double angle );
  static Matrix4x4 scaleMatrix( const Vector3D& amount );
  static Matrix4x4 translationMatrix( const Vector3D& amount );

  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;
  // Find the closest intersection
  virtual bool closest_intersect(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far=double(1e+4));
  
protected:
  
  // Useful for picking
  int m_id;
  std::string m_name;

  // Transformations
  Matrix4x4 m_trans;
  Matrix4x4 m_invtrans;

  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;
};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual bool is_joint() const;

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  struct JointRange {
    double min, init, max;
  };  
  // Find the closest intersection
  virtual bool closest_intersect(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far=double(1e+4));

protected:

  JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  const Material* get_material() const;
  Material* get_material();

  void set_material(Material* material)
  {
    m_material = material;
  }

  // Find the closest intersection
  virtual bool closest_intersect(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far=double(1e+4));

protected:
  Material* m_material;
  Primitive* m_primitive;
};

#endif
