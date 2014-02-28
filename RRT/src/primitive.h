#ifndef CS488_PRIMITIVE_H
#define CS488_PRIMITIVE_H

#include "algebra.h"
#include <GL/gl.h>
#include <GL/glu.h>

class Primitive {
	public:
		virtual ~Primitive();
		virtual void walk_gl(bool picking) const = 0;
};

class Sphere : public Primitive {
	public:
		Sphere();
		virtual ~Sphere();
		virtual void walk_gl(bool picking) const;
  
	private:
		static GLuint s_sphereId;
	
		static void generateSphere();
		static void generateGLUSphere();
};

#endif
