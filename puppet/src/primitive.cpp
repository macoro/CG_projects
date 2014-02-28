#include "primitive.h"

GLuint Sphere::s_sphereId = 0;

Primitive::~Primitive()
{
}

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl(bool picking) const
{
	if( s_sphereId == 0 ){
		Sphere :: generateGLUSphere();
	}
	glCallList( s_sphereId );
}

void Sphere::generateGLUSphere()
{
	GLUquadricObj* obj = gluNewQuadric();
	gluQuadricDrawStyle( obj, GLU_FILL );
	gluQuadricNormals( obj, GLU_SMOOTH );
	s_sphereId = glGenLists(1);
	glNewList( s_sphereId, GL_COMPILE );
        gluSphere( obj, 1, 60, 60 );
	glEndList();
	gluDeleteQuadric(obj);
}
