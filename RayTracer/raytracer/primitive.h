#ifndef CS488_PRIMITIVE_H
#define CS488_PRIMITIVE_H

#include "algebra.h"
#include "ray.h"

class Primitive {
public:
    virtual bool IntersectRay(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far=double(1e+4))=0;
  virtual ~Primitive();
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual bool IntersectRay(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far=double(1e+4));
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  virtual bool IntersectRay(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far=double(1e+4));
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual bool IntersectRay(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far=double(1e+4));


private:
  Point3D m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }  
  virtual ~NonhierBox();
  virtual bool IntersectRay(Ray &r,Intersection &isect, Matrix4x4 &inv_transf, double far=double(1e+4));
  bool wIntersectRay(Ray &r);

private:
  Point3D m_pos;
  double m_size;
};

#endif
