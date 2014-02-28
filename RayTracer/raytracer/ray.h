#ifndef RAY_H
#define RAY_H
#include "algebra.h"
#include "material.h"

class Ray
{
public:
    Ray();
    Ray(Point3D origin, Vector3D direction){
        m_e = origin;
        m_dir = direction;
    }

    Ray(const Ray& other){
        m_e = other.m_e;
        m_dir = other.m_dir;
    }

    Ray& operator =(const Ray& other)
    {
      m_e = other.m_e;
      m_dir = other.m_dir;
      return *this;
    }

    Point3D origin()const{
        return m_e;
    }
    Point3D& origin(){
        return m_e;
    }

    Vector3D direction()const{
        return m_dir;
    }

    Vector3D& direction(){
        return m_dir;
    }
private:
    Point3D m_e;
    Vector3D m_dir;
};

class Intersection
{
public:
    Intersection(){}
    Intersection(const Intersection& other){
        m_position = other.m_position;
        m_normal = other.m_normal;
        m_mtrl = other.m_mtrl;
        m_distance = other.m_distance;
    }
    Intersection& operator =(const Intersection& other)
    {
        m_position = other.m_position;
        m_normal = other.m_normal;
        m_mtrl = other.m_mtrl;
        m_distance = other.m_distance;
        return *this;
    }


    Point3D position()const{
        return m_position;
    }
    Point3D& position(){
        return m_position;
    }

    Vector3D normal()const{
        return m_normal;
    }
    Vector3D& normal(){
        return m_normal;
    }

    Material* mtrl()const{
        return m_mtrl;
    }
    void set_material(Material* material){
        m_mtrl = material;
    }

    double distance()const{
        return m_distance;
    }
    double& distance(){
        return m_distance;
    }

private:
    Point3D m_position;
    Vector3D m_normal;
    Material *m_mtrl;
    double m_distance;
};


#endif // RAY_H
