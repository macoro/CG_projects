#include "scene.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stack>


unsigned int idCounter = 1;

model *SceneNode::m_model = 0;
GeometryNode *SceneNode::m_geomodel=0;
std::map <int,GeometryNode*> SceneNode::fixed_objs = std::map <int,GeometryNode*>();
bool SceneNode::show_modelcp = true;



SceneNode::SceneNode(const std::string& name)
  : m_name(name), m_parent(NULL), m_picked(false), m_id( idCounter++ )
{
}

SceneNode::~SceneNode()
{
}

struct node_data{
    node_data(std::vector<double> s,kdres *p){
        x= s;
        parent = p;
    }
    std::vector<double> x;          //node state
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
//        Matrix4x4 child_transf = child->get_transform();
        m_geomodel = dynamic_cast<GeometryNode*>(child);
        m_model = new freedef3D_model();
        m_model->model_mesh = dynamic_cast<Mesh*>(m_geomodel->get_primitive());
//        m_model->setInitialState(child_transf);
    }
    m_children.push_back(child);
    child->m_parent = this;
}

void SceneNode::draw_path(std::vector<std::vector<double> > &path){
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
         glVertex3d(path[i][0],path[i][1],path[i][2]);
     }
     glEnd();
     glDisable(GL_LINE_SMOOTH);

     glPopMatrix();
     glMatrixMode(mm);
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
                std::vector<double> xl = ((node_data*)node->left->data)->x;
                glVertex3d(xl[0],xl[1],xl[2]);
                node_stack.push(node->left);
            }

            //parent
            std::vector<double> xn = ((node_data*)node->data)->x;
            glVertex3d(xn[0],xn[1],xn[2]);

            //right
            if(node->right){
                std::vector<double> xr = ((node_data*)node->right->data)->x;
                glVertex3d(xr[0],xr[1],xr[2]);
                node_stack.push(node->right);
            }

            glEnd();
        }

        glDisable(GL_LINE_SMOOTH);
        glPopMatrix();
        glMatrixMode(mm);
    }
}

void SceneNode::set_modelstate(std::vector<double> x){
    Vector4D Q(x[3],x[4],x[5],x[6]);
    //rotation
    Matrix4x4 model_transf = Q.getRotMatrix();
    //traslation
    model_transf[0][3]= x[0];
    model_transf[1][3]=x[1];
    model_transf[2][3]=x[2];
    model_transf[3][3]= 1.0;

    m_geomodel->set_transform(model_transf);
}

void SceneNode::set_model_finalstate(Vector3D translation, Vector3D rotation){
    std::vector<double> state(m_model->StateDim);
    //euler angles to Quaternions
    double cita = rotation[0]/2.0;
    double deltha = rotation[1]/2.0;
    double phi = rotation[2]/2.0;

    state[3] = cos(cita)*cos(deltha)*cos(phi) + sin(cita)*sin(deltha)*sin(phi);
    state[4] = sin(cita)*cos(deltha)*cos(phi) + cos(cita)*sin(deltha)*sin(phi);
    state[5] = cos(cita)*sin(deltha)*cos(phi) + sin(cita)*cos(deltha)*sin(phi);
    state[6] = cos(cita)*cos(deltha)*sin(phi) + sin(cita)*sin(deltha)*cos(phi);

    for(int i = 0; i < 3; i++)
        state[i]=(translation[i]);

    m_model->setGoalState(state);
}

bool SceneNode::fill_swiftscene(SWIFT_Scene *swift_scene){
    for( ChildList::iterator i = m_children.begin(); i != m_children.end(); i++){
        if(! (*i)->fill_swiftscene(swift_scene) )
            return false;
    }
    return true;
}

bool SceneNode::deform_model(int id_deformer, const Matrix4x4 &ray_transf, const Matrix4x4 &inv_deformertransf){
    return m_geomodel->deform_model(id_deformer, ray_transf, inv_deformertransf);
}

void SceneNode::reset_deformation(){
    m_geomodel->reset_deformation();
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

    if(this-> m_name.compare("model") == 0)
        m_primitive -> walk_gl( show_modelcp );
    else
        m_primitive -> walk_gl(picking);
	
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

bool GeometryNode::fill_swiftscene(SWIFT_Scene *swift_scene){
    //Do not insert model object here because you can't know its id. Only fixed objects
    if(this-> m_name.compare("model") == 0)
        return true;
    Mesh* object = dynamic_cast<Mesh*>(m_primitive);
    // Add an object described by arrays.  This model must be convex.
    int id;

    int vn = object->m_verts.size();
    //int vn=6;
    SWIFT_Real *vs = new double[(3*vn)];
    //fill vertex array
    for(int i=0; i<vn; i++){
       int index = 3*i;
       vs[index] = object->m_verts[i][0];
       vs[index+1]= object->m_verts[i][1];
       vs[index+2]= object->m_verts[i][2];
       //std::cout<< vs[index+2] << std::endl;
    }
    int fn = object->m_faces.size();
    int *fv = new int[fn]; //faces valences
    int fl = 0;  //faces array length
    for(int i = 0; i<fn; i++){
        fv[i] = (object->m_faces[i].size());
        //fv[i] = 3;
        fl += fv[i];
    }
    int *fs= new int[fl];
    int index = 0;
    for(int i = 0; i<fn; i++){
        for(int j = 0; j<fv[i]; j++)
            fs[index+j] = object->m_faces[i][j];
        index+=fv[i];
    }
    SWIFT_Real R[9];
    R[0] = m_trans[0][0]; R[1] = m_trans[0][1]; R[2] = m_trans[0][2];
    R[3] = m_trans[1][0]; R[4] = m_trans[1][1]; R[5] = m_trans[1][2];
    R[6] = m_trans[2][0]; R[7] = m_trans[2][1]; R[8] = m_trans[2][2];

    SWIFT_Real T[3];
    T[0] = m_trans[0][3]; T[1] =m_trans[1][3]; T[2] = m_trans[2][3];

    if( !swift_scene->Add_Convex_Object(
                vs, fs,
                vn, fn, id,
                true,
                R,
                T,
                DEFAULT_SCALE,
                DEFAULT_BOX_SETTING,
                DEFAULT_BOX_ENLARGE_REL,
                DEFAULT_BOX_ENLARGE_ABS,
                fv) ) {
        std::cerr << "Adding object failed -- Exiting..." << id<< std::endl;
        return false;
    }
    else {
        fixed_objs[id] = this;
        std::cerr << "Added object to scene id="<< id <<std::endl;
    }
    return true;
}

bool GeometryNode::deform_model(int id_deformer, const Matrix4x4 &ray_transf, const Matrix4x4 &inv_deformertransf){

    Mesh* deformer = dynamic_cast<Mesh*>(fixed_objs[id_deformer]->get_primitive());
    Mesh* deformed = dynamic_cast<Mesh*>(m_primitive);

    deformed->deformation(*deformer, ray_transf, inv_deformertransf);

    return deformed->is_deformed();
}

void GeometryNode::reset_deformation(){

    Mesh* deformed = dynamic_cast<Mesh*>(m_primitive);
    deformed->reset_controlpoints();
}
