#include "scene.h"
#include <iostream>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::rotate(char axis, double angle)
{
    Matrix4x4 r = rotationMatrix( axis, angle );
    m_trans = m_trans*r;
    m_invtrans = r.invert()*m_invtrans;
}

void SceneNode::scale(const Vector3D& amount)
{
    Matrix4x4 s = scaleMatrix(amount);
    m_trans = m_trans * s;
    m_invtrans = s.invert() * m_invtrans;
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

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}

bool SceneNode::closest_intersect(Ray &rp, Intersection &isect, Matrix4x4 &inv_transf, double far){
    Matrix4x4 inv_tmp = m_invtrans*inv_transf;
    //Ray r( (m_invtrans*rp.origin()), (m_invtrans*rp.direction()));
    double tclosest = far;
    Intersection closest_isect;
    for( ChildList::iterator i = m_children.begin(); i != m_children.end(); i++){
        Intersection tmp_isect;
        if((*i)-> closest_intersect(rp,tmp_isect,inv_tmp)){
            if(tmp_isect.distance() < tclosest){
                closest_isect = tmp_isect;
                tclosest = tmp_isect.distance();
            }
        }
    }

    if(!(tclosest < far))
        return false;
    closest_isect.position() = rp.origin() + closest_isect.distance()*rp.direction();
    isect = closest_isect;
    return true;
}

bool JointNode::closest_intersect(Ray &r, Intersection &isect, Matrix4x4 &inv_transf, double far){
    return SceneNode::closest_intersect(r,isect, inv_transf,far);
}

bool GeometryNode::closest_intersect(Ray &rp, Intersection &isect, Matrix4x4 &inv_transf, double far){
    Matrix4x4 inv_tmp = m_invtrans*inv_transf;
    //Ray r( (m_invtrans*rp.origin()), (m_invtrans*rp.direction()));
    double tclosest = far;
    Intersection closest_isect;
    if(m_primitive->IntersectRay(rp,closest_isect,inv_tmp)){
        tclosest = closest_isect.distance();
        closest_isect.set_material(m_material);        
    }
    for( ChildList::iterator i = m_children.begin(); i != m_children.end(); i++){
        Intersection tmp_isect;
        if((*i)-> closest_intersect(rp,tmp_isect,inv_transf)){
            if(tmp_isect.distance() < tclosest){
                closest_isect = tmp_isect;
                tclosest = tmp_isect.distance();
            }
        }
    }
    if(!(tclosest < far))
        return false;
    //closest_isect.position() = rp.origin() + closest_isect.distance()*rp.direction();
    isect = closest_isect;
    return true;
}

inline Matrix4x4 SceneNode::rotationMatrix( char axis, double angle )
{
    Matrix4x4 r;
    double *row;
    double s = sin( angle*M_PI/180.0 );
    double c = cos( angle*M_PI/180.0 );

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

inline Matrix4x4 SceneNode::scaleMatrix(const Vector3D& amount)
{
    Matrix4x4 s;
    s[0][0] = amount[0];
    s[1][1] = amount[1];
    s[2][2] = amount[2];

    return s;
}
inline Matrix4x4 SceneNode::translationMatrix(const Vector3D& amount)
{
    Matrix4x4 t;
    t[0][3] = amount[0];
    t[1][3] = amount[1];
    t[2][3] = amount[2];

    return t;
}
 
