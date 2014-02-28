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

void Sphere::walk_gl(bool picking)
{
	if( s_sphereId == 0 ){
		Sphere :: generateGLUSphere();
	}
	glCallList( s_sphereId );
}

void Sphere::walk_gl(double radius,bool picking)
{
    if( s_sphereId == 0 ){
        Sphere :: generateGLUSphere(radius);
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

void Sphere::generateGLUSphere(double radius)
{
    GLUquadricObj* obj = gluNewQuadric();
    gluQuadricDrawStyle( obj, GLU_FILL );
    gluQuadricNormals( obj, GLU_SMOOTH );
    s_sphereId = glGenLists(1);
    glNewList( s_sphereId, GL_COMPILE );
        gluSphere( obj, radius, 30, 30 );
    glEndList();
    gluDeleteQuadric(obj);
}

BBox::~BBox(){}

void BBox::walk_gl(bool picking)
{
    glEnable(GL_LINE_SMOOTH);
    glColor3f(0.0f, 0.0f, 1.0f );

    //bottom base
    glBegin(GL_LINE_LOOP);
        glVertex3d(m_pos[0],m_pos[1],m_pos[2]);
        glVertex3d(m_pos[0]+m_size[0],m_pos[1],m_pos[2]);
        glVertex3d(m_pos[0]+m_size[0],m_pos[1],m_pos[2] + m_size[2]);
        glVertex3d(m_pos[0],m_pos[1],m_pos[2] + m_size[2]);
    glEnd();

    //top base
    glBegin(GL_LINE_LOOP);
        glVertex3d(m_pos[0],m_pos[1] + m_size[1],m_pos[2]);
        glVertex3d(m_pos[0]+m_size[0],m_pos[1] + m_size[1],m_pos[2]);
        glVertex3d(m_pos[0]+m_size[0],m_pos[1] + m_size[1],m_pos[2] + m_size[2]);
        glVertex3d(m_pos[0],m_pos[1] + m_size[1],m_pos[2] + m_size[2]);
    glEnd();

    //sides edges
    glBegin(GL_LINE_LOOP);
        glVertex3d(m_pos[0],m_pos[1],m_pos[2]);
        glVertex3d(m_pos[0],m_pos[1] + m_size[1],m_pos[2]);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3d(m_pos[0]+m_size[0],m_pos[1],m_pos[2]);
        glVertex3d(m_pos[0]+m_size[0],m_pos[1] + m_size[1],m_pos[2]);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3d(m_pos[0]+m_size[0],m_pos[1],m_pos[2] + m_size[2]);
        glVertex3d(m_pos[0]+m_size[0],m_pos[1] + m_size[1],m_pos[2] + m_size[2]);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3d(m_pos[0],m_pos[1],m_pos[2] + m_size[2]);
        glVertex3d(m_pos[0],m_pos[1] + m_size[1],m_pos[2] + m_size[2]);
    glEnd();

    glDisable(GL_LINE_SMOOTH);
}
