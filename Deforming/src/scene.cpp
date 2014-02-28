#include "scene.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stack>


unsigned int idCounter = 1;

SceneNode::SceneNode(const std::string& name)
  : m_name(name), m_parent(NULL), m_picked(false), m_id( idCounter++ )
{
}

SceneNode::~SceneNode()
{
}


inline Matrix4x4 SceneNode::rotationMatrix( char axis, double angle, bool radians )
{
	Matrix4x4 r;
	double *row;
    double s,c;
    if(!radians){
        s = sin( angle*M_PI/180.0 );
        c = cos( angle*M_PI/180.0 );
    }
    else{
        s = sin(angle);
        c = cos(angle);
    }
	
	switch(axis){
		case 'x':
			row = r[1];
			row[1] = c;
			row[2] = -s;
			row = r[2];
			row[1] = s;
			row[2] = c;
			break;
		case 'y':
			row = r[0];
			row[0] = c;
			row[2] = s;
			row = r[2];
			row[0] = -s;
			row[2] = c;
			break;
      default:
			row = r[0];
			row[0] = c;
			row[1] = -s;
			row = r[1];
			row[0] = s;
			row[1] = c;
			break;
	}
	return r;
}

void SceneNode::rotate( char axis, double angle )
{
	Matrix4x4 r = rotationMatrix( axis, angle );
	m_trans = m_trans*r;
	m_invtrans = r.invert()*m_invtrans;
}

inline Matrix4x4 SceneNode::scaleMatrix(const Vector3D& amount)
{
	Matrix4x4 s;
	s[0][0] = amount[0];
	s[1][1] = amount[1];
	s[2][2] = amount[2];

	return s;
}

void SceneNode::scale(const Vector3D& amount)
{
	Matrix4x4 s = scaleMatrix(amount);
	m_trans = m_trans * s;
	m_invtrans = s.invert() * m_invtrans;
}

inline Matrix4x4 SceneNode::translationMatrix(const Vector3D& amount)
{
	Matrix4x4 t;
	t[0][3] = amount[0];
	t[1][3] = amount[1];
	t[2][3] = amount[2];

	return t;
}

void SceneNode::translate(const Vector3D& amount)
{
  Matrix4x4 t = translationMatrix(amount);
  m_trans = m_trans * t;
  m_invtrans = t.invert() * m_invtrans;
}

bool SceneNode::is_joint() const
{
	return false;
}


void SceneNode::walk_gl(bool picking) const
{
   GLint mm;
   glGetIntegerv( GL_MATRIX_MODE, &mm);
   
   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();
  
   GLdouble t[16];
   int z=0;
   for( int i=0; i < 4; i++ ){
	for( int j=0; j < 4; j++ ){
		t[z] = m_trans[j][i];
		z++;
		}
	}
	glMultMatrixd(t);
	
	for( ChildList :: const_iterator i = m_children.begin(); i != m_children.end(); i++ ){
		(*i) -> walk_gl(picking);
	}

	glPopMatrix();
	glMatrixMode(mm);
}

void SceneNode :: getAllJoints( std::vector<JointNode*>& v)
{
	if( is_joint() ){
		JointNode* n = dynamic_cast<JointNode*>(this);
		if(n){ v.push_back(n); }
	}
	for( ChildList::iterator i = m_children.begin(); i!=m_children.end(); i++){
		(*i)->getAllJoints(v);
	}
}

void SceneNode :: getAllNodes( std::map<unsigned int, SceneNode*>& map)
{
	map[m_id] = this;
	for( ChildList::iterator i = m_children.begin(); i != m_children.end(); i++){
		(*i)->getAllNodes(map);
	}
}

void SceneNode :: clear_all_inverse()
{
	for( ChildList::iterator i = m_children.begin(); i != m_children.end(); i++ ){
		(*i)->clear_all_inverse();
	}
	m_invtrans = Matrix4x4();
}

void SceneNode::add_child(SceneNode* child)
{
    m_children.push_back(child);
    child->m_parent = this;
}

void SceneNode::move_controlpoint(int index, double dx, double dy, double dz){
    for(ChildList::const_iterator i = m_children.begin(); i != m_children.end(); i++){
        (*i)-> move_controlpoint(index,dx,dy,dz);
  }
}

/* 
 * Joint class implementation 
 */

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const{ return true; }

void JointNode::walk_gl(bool picking) const
{
  SceneNode::walk_gl(picking);
}


void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
  
  Matrix4x4 r = rotationMatrix('x',init);
  m_trans = m_trans*r;
  m_invtrans = r.invert() * m_invtrans;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
  
  Matrix4x4 r = rotationMatrix('y', init);
  m_trans = m_trans*r;
  m_invtrans = r.invert()*m_invtrans;
}


GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(bool picking) const
{
	if( picking && m_parent != NULL && m_parent->is_joint()){
		glPushName( (GLuint) m_id );
	}
	
	GLint mm;
	glGetIntegerv( GL_MATRIX_MODE, &mm);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	
	
	GLdouble t[16];
	int z=0;
	for( int i=0; i < 4; i++ ){
		for( int j=0; j < 4; j++ ){
			t[z] = m_trans[j][i];
			z++;
		}
	}
	glMultMatrixd(t);
	
	if(m_material){
		m_material -> apply_gl(m_picked);
	}
	m_primitive -> walk_gl( picking );    

	if(picking && m_parent != NULL && m_parent->is_joint()){
		glPopName();
	}
	
	glPopMatrix();
	
	for(ChildList::const_iterator i = m_children.begin(); i != m_children.end(); i++){
		(*i)->walk_gl(picking);
  }
	glMatrixMode(mm);
}

void GeometryNode::move_controlpoint(int index, double dx, double dy,double dz){
    Mesh * model = dynamic_cast<Mesh*>(m_primitive);

    if(model->select_controlpoint(index)){
        model->move_controlpoint(dx,dy,dz);
    }
}


void JointNode::set_picked( bool b )
{
	SceneNode::set_picked(b);
	for( ChildList::const_iterator i = m_children.begin(); i != m_children.end(); i++ ){
		(*i) -> set_picked(b);
	}
}



