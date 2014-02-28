#include "material.h"
#include <GL/gl.h>
#include <GL/glu.h>

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
	glColor3f( m_kd.R(), m_kd.G(), m_kd.B() );
	// Perform OpenGL calls necessary to set up this material.
	GLfloat diffuse[] = { m_kd.R(), m_kd.G(), m_kd.B(), 1.0f };
	GLfloat ambient[] = { m_kd.R(), m_kd.G(), m_kd.B(), 1.0f };
	GLfloat specular[] = { m_ks.R(), m_ks.G(), m_ks.B(), 1.0f };
	
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialf( GL_FRONT, GL_SHININESS, m_shininess );
}

void PhongMaterial::apply_gl( bool pick ) const
{
	glColor3f( m_kd.R(), m_kd.G(), m_kd.B() );
	GLfloat amb_diff[] = { m_kd.R(), m_kd.G(), m_kd.B(), 1.0f };
	
	if(pick){
		GLfloat em[] = {0.2, 0.2, 0.2, 0.0f};
		GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f };
		
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb_diff );
		glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
		glMaterialf( GL_FRONT, GL_SHININESS, 100 );
		glMaterialfv( GL_FRONT, GL_EMISSION, em );
	} else {
		GLfloat em[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat specular[] = { m_ks.R(), m_ks.G(), m_ks.B(), 1.0f };
		
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb_diff );
		glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
		glMaterialf( GL_FRONT, GL_SHININESS, m_shininess );
		glMaterialfv( GL_FRONT, GL_EMISSION, em );
	}
}
