#include "scene.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stack>
#include <queue>

unsigned int idCounter = 1;

model *SceneNode::m_model = 0;

SceneNode::SceneNode(const std::string& name)
  : m_name(name), m_parent(NULL), m_picked(false), m_id( idCounter++ )
{
}

SceneNode::~SceneNode()
{
}

struct node_data{
    node_data(Vector3D s, Point2D c, kdres *p){
        x= new Vector3D(s[0],s[1],s[2]);
        control = new Point2D(c[0],c[1]);
        parent = p;
    }
    Vector3D *x;          //node state
    Point2D *control;   //control from parent to node
    kdres *parent;      //point to parent node
};

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
    if(child->getName().compare("model")== 0){
        Matrix4x4 child_transf = child->get_transform();
        Vector3D initial(child_transf[0][3],child_transf[2][3],0.0);
        m_geomodel = dynamic_cast<GeometryNode*>(child);
        m_model = new model(initial,m_geomodel->get_length());
        m_model->model_mesh = dynamic_cast<Mesh*>(m_geomodel->get_primitive());
    }

    m_children.push_back(child);
    child->m_parent = this;
}

bool SceneNode::free_collion(Vector3D &x){
    for( ChildList::iterator i = m_children.begin(); i != m_children.end(); i++){
        if(! (*i)->free_collion(x))
            return false;
    }
    return true;
}

void SceneNode::draw_path(std::vector<Vector3D> &path){
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
     glEnable(GL_LINE_SMOOTH);
     glBegin(GL_LINE_STRIP);
     glColor3f(0.5f, 1.0f, 0.5f );
     for(size_t i=0; i < path.size(); i++){
         glVertex3d(path[i][0],0.0,path[i][1]);
     }
     glEnd();
     glDisable(GL_LINE_SMOOTH);

     glPopMatrix();
     glMatrixMode(mm);
}

std::queue<kdnode*> allleave_of_tree(kdtree *tree){
    std::queue<kdnode*> all_leave;
    all_leave.push(tree->root);
    kdnode* cursor = all_leave.front();
    while(cursor){
        bool l = false;
        bool r = false;
        if(cursor->left){
            all_leave.push(cursor->left);
            l=true;
        }
        if(cursor->right){
            all_leave.push(cursor->right);
            r=true;
        }
        if( l || r ){
            all_leave.pop();
            cursor = all_leave.front();
        }
        else
            break;
    }

    return all_leave;
}

//std::vector<Vector3D> path2root(kdnode *node){
//   std::vector<Vector3D> result;
//   Vector3D x = *((node_data*)node->data)->x;
//   result.push_back(x);
//   kdres *parent = ((node_data*)node->data)->parent;
//   while( parent != 0 ){
//       x = *((node_data*)kd_res_item_data(parent))->x;
//       result.push_back(x);
//       parent = ((node_data*)kd_res_item_data(parent))->parent;
//   }
//   return result;
//}

std::vector<Vector3D> path2root(kdres *node){
   std::vector<Vector3D> result;
   Vector3D x = *((node_data*)kd_res_item_data(node))->x;
   result.push_back(x);
   kdres *parent = ((node_data*)kd_res_item_data(node))->parent;
   while( parent != 0 ){
       x = *((node_data*)kd_res_item_data(parent))->x;
       result.push_back(x);
       parent = ((node_data*)kd_res_item_data(parent))->parent;
   }
   return result;
}

void SceneNode::draw_tree(kdtree *tree, bool end){
    if(tree && tree->root){
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

        std::stack<kdnode*> node_stack;
        node_stack.push(tree->root);

        if(end)
            glColor3f(0.2f, 1.0f, 1.0f );
        else
            glColor3f(1.0f, 1.0f, 0.2f );

        glEnable(GL_LINE_SMOOTH);

        while(!node_stack.empty()){
            glBegin(GL_LINE_STRIP);

            kdnode* node = node_stack.top();
            node_stack.pop();
            //left
            if(node->left){
                Vector3D xl = *((node_data*)node->left->data)->x;
                //glVertex3d(node->left->pos[0],node->left->pos[1],node->left->pos[2]);
                glVertex3d(xl[0],0.0,xl[1]);
                node_stack.push(node->left);
            }

            //parent
            Vector3D xn = *((node_data*)node->data)->x;
            //glVertex3d(node->pos[0],node->pos[1],node->pos[2]);
            glVertex3d(xn[0],0.0,xn[1]);

            //right
            if(node->right){
                Vector3D xr = *((node_data*)node->right->data)->x;
                //glVertex3d(node->right->pos[0],node->right->pos[1],node->right->pos[2]);
                glVertex3d(xr[0],0.0,xr[1]);
                node_stack.push(node->right);
            }

            glEnd();
        }

        glDisable(GL_LINE_SMOOTH);

//        std::queue<kdnode*> all_leave = allleave_of_tree(tree);
//        glEnable(GL_LINE_SMOOTH);
//        while(!all_leave.empty()){
//            kdnode* cursor = all_leave.front();
//            all_leave.pop();
//            kdres *presults = kd_nearest3(tree,cursor->pos[0],cursor->pos[1],cursor->pos[2]);
//            std::vector<Vector3D> path = path2root(presults);

//            glBegin(GL_LINE_STRIP);
//            for(size_t i=0; i < path.size(); i++){
//                glVertex3d(path[i][0],0.0,path[i][1]);
//            }
//            glEnd();
//        }
//        glDisable(GL_LINE_SMOOTH);

        glPopMatrix();
        glMatrixMode(mm);
    }
}

void SceneNode::set_modelstate(Vector3D state, bool radians){
    Matrix4x4 model_transf= rotationMatrix('y',state[2],radians);
    //traslation
    model_transf[0][3]=state[0];
    model_transf[2][3]=state[1];
    //assign to model
    m_geomodel->set_transform(model_transf);
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

void JointNode::rotate(char axis,double angle){ }

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
	// glLoadIdentity();
	
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
 
void JointNode::set_picked( bool b )
{
	SceneNode::set_picked(b);
	for( ChildList::const_iterator i = m_children.begin(); i != m_children.end(); i++ ){
		(*i) -> set_picked(b);
	}
}

bool GeometryNode::free_collion(Vector3D &x){
    if(this-> m_name.compare("model") == 0)
        return true;
    PQP_REAL R1[3][3], R2[3][3], T1[3], T2[3];

    R2[0][0] = R2[1][1] = R2[2][2] = 1.0;
    R2[0][1] = R2[1][0] = R2[2][0] = 0.0;
    R2[0][2] = R2[1][2] = R2[2][1] = 0.0;

    T2[0] = m_trans[0][3];  T2[1] = m_trans[1][3]; T2[2] = m_trans[2][3];

    R1[0][0] = cos(x[2]);  R1[0][1] = 0.0;  R1[0][2] = sin(x[2]);
    R1[1][0] = 0.0;  R1[1][1] = 1.0;  R1[1][2] =0.0;
    R1[2][0] = -sin(x[2]);  R1[2][1] = 0.0;  R1[2][2] = cos(x[2]);
    T1[0] = x[0];  T1[1] = 0.0; T1[2] = x[1];

    bool lq_tol= dynamic_cast<Mesh*>(m_primitive)->lq_tolerance(R2,T2,SceneNode::m_model->model_mesh->getmodel(),R1, T1,(PQP_REAL)1.0);
    return (!lq_tol);
}



