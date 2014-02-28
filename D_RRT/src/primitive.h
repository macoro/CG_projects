#ifndef CS488_PRIMITIVE_H
#define CS488_PRIMITIVE_H

#include "algebra.h"
#include <GL/gl.h>
#include <GL/glu.h>

class Primitive {
	public:
		virtual ~Primitive();
        virtual void walk_gl(bool picking) = 0;
};

class Sphere : public Primitive {
	public:
		Sphere();
		virtual ~Sphere();
        virtual void walk_gl(bool picking) ;
        virtual void walk_gl(double radius,bool picking);
  
	private:
		static GLuint s_sphereId;
	
		static void generateSphere();
		static void generateGLUSphere();
        static void generateGLUSphere(double radius);
};

class BBox : public Primitive {
public:
  BBox(const Point3D& pos, Vector3D& size)
    : m_pos(pos), m_size(size)
  {
      centre = m_pos + (0.5)* m_size;
  }
  virtual ~BBox();
  virtual void walk_gl(bool picking);

  Point3D m_pos;
  Vector3D m_size;
  Point3D centre;
};

#endif
