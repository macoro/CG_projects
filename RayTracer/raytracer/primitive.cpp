#include "primitive.h"

using namespace std;

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

NonhierSphere::~NonhierSphere()
{
}

NonhierBox::~NonhierBox()
{
}

bool Sphere::IntersectRay(Ray &rp, Intersection &isect, Matrix4x4 &inv_transf, double far){
    Point3D m_pos(0.0,0.0,0.0);
    //double m_radius = 1.0;
    Ray r( (inv_transf*rp.origin()), (inv_transf*rp.direction()));

//    //geometric method
//    double r2 = m_radius*m_radius;
//    Vector3D oc = m_pos - r.origin();
//    double oc2 = oc.dot(oc);
//    if(oc2 - r2 <= 0.0)// ray inside the sphere
//        return false;
//    double tg = oc.dot(r.direction());
//    if(tg < 0.0) // sphere behind the ray
//        return false;
//    double gc2 = oc2 - (tg*tg);
//    double hg2 = r2 - gc2;
//    if(hg2 < 0.0)
//        return false;
//    //find hit point
//    double t = tg - sqrt(hg2);

//    if(t < eps || t > far)
//        return false;
    //p = VectorSubtract(ray.pos,obj.pos)
      //  d = ray.dir
        //r = obj.size

    //algebraic method
    Vector3D oc = m_pos - r.origin();
    double a = r.direction().dot(r.direction());
    double b = 2.0*(oc.dot(r.direction()));
    double c = oc.dot(oc) - 1.0;
    double det = b*b - 4.0*a*c;
    if (det < 0.0)
        return false;
    det = sqrt(det);
    double t1 = (b - det)/(2.0*a);
    double t2 = (b + det)/(2.0*a);
    if (t2 < eps)
        return false;
    if (t1 < eps || t1 > far)
        return false;
    double t = min(t1,t2);

    Point3D h = r.origin() + t*r.direction();
    Vector3D n = (h - m_pos);
    if(n.length() == 0.0) return false;
    transNorm(inv_transf,n);
    n.normalize();
    isect.normal()=n;
    isect.distance()=t;
    //isect.position()=h;
    return true;
}

bool Cube::IntersectRay(Ray &rp, Intersection &isect, Matrix4x4 &inv_transf, double far){
    Point3D m_pos(0.0,0.0,0.0);
    double m_size = 1.0;
    Ray r( (inv_transf*rp.origin()), (inv_transf*rp.direction()));

    double tx1,tx2,ty1,ty2,tz1,tz2;
    Point3D a = m_pos;
    Point3D b(a[0]+ m_size,a[1] + m_size,a[2]+ m_size);
    tx1 = (a[0] - r.origin()[0])/r.direction()[0];
    tx2 = (b[0] - r.origin()[0])/r.direction()[0];
    ty1 = (a[1] - r.origin()[1])/r.direction()[1];
    ty2 = (b[1] - r.origin()[1])/r.direction()[1];
    tz1 = (a[2] - r.origin()[2])/r.direction()[2];
    tz2 = (b[2] - r.origin()[2])/r.direction()[2];

    double tmin = max(min(tx1,tx2),max(min(ty1,ty2),min(tz1,tz2)));
    double tmax = min(max(tx1,tx2),min(max(ty1,ty2),max(tz1,tz2)));

    if(tmax < eps)
        return false;
    if((tmax - tmin) < eps)
        return false;

    double t = tmin;

    if(t < eps || t > far)
        return false;

    Point3D h = r.origin() + t*r.direction();
    Point3D c(a[0] + (m_size/2.0),a[1] + (m_size/2.0),a[2] + (m_size/2.0) );
    Vector3D p = h - c;
    p = transNorm(inv_transf,p);
    if(p.length() == 0.0) return false;
    int maxindex = 0;
    for(int i=1;i<3;i++){
        if(fabs(p[i]) > fabs(p[maxindex]) ){
            p[maxindex]=0;
            maxindex = i;
        }
        else
            p[i]=0;
    }
    if(p[maxindex] < 0)
        p[maxindex]=-1.0;
    else
        p[maxindex]=1.0;

    isect.distance()=t;
    //isect.position()=h;
    isect.normal()=p;

    return true;
}

bool NonhierSphere::IntersectRay(Ray &r, Intersection &isect, Matrix4x4 &inv_transf, double far){
    //geometric method
    double r2 = m_radius*m_radius;
    Vector3D oc = m_pos - r.origin();
    double oc2 = oc.dot(oc);
    if(oc2 - r2 <= 0.0)// ray inside the sphere
        return false;
    double tg = oc.dot(r.direction());
    if(tg < 0.0) // sphere behind the ray
        return false;
    double gc2 = oc2 - (tg*tg);
    double hg2 = r2 - gc2;
    if(hg2 < 0.0)
        return false;
    //find hit point
    double t = tg - sqrt(hg2);

    if(t < eps || t > far)
        return false;

    Point3D h = r.origin() + t*r.direction();
    Vector3D n = (h - m_pos);
    if(n.length() == 0.0) return false;
    n.normalize();
    isect.distance()=t;
    isect.position()=h;
    isect.normal()=n;

    return true;
}

bool NonhierBox::IntersectRay(Ray &r, Intersection &isect, Matrix4x4 &inv_transf, double far){
    double tx1,tx2,ty1,ty2,tz1,tz2;
    Point3D a = m_pos;
    Point3D b(a[0]+ m_size,a[1] + m_size,a[2]+ m_size);
    tx1 = (a[0] - r.origin()[0])/r.direction()[0];
    tx2 = (b[0] - r.origin()[0])/r.direction()[0];
    ty1 = (a[1] - r.origin()[1])/r.direction()[1];
    ty2 = (b[1] - r.origin()[1])/r.direction()[1];
    tz1 = (a[2] - r.origin()[2])/r.direction()[2];
    tz2 = (b[2] - r.origin()[2])/r.direction()[2];

    double tmin = max(min(tx1,tx2),max(min(ty1,ty2),min(tz1,tz2)));
    double tmax = min(max(tx1,tx2),min(max(ty1,ty2),max(tz1,tz2)));

    if(tmax < eps)
        return false;
    if((tmax - tmin) < eps)
        return false;

    double t = tmin;

    if(t < eps || t > far)
        return false;

    Point3D h = r.origin() + t*r.direction();
    Point3D c(a[0] + (m_size/2.0),a[1] + (m_size/2.0),a[2] + (m_size/2.0) );
    Vector3D p = h - c;
    if(p.length() == 0.0) return false;
    int maxindex = 0;
    for(int i=1;i<3;i++){
        if(fabs(p[i]) > fabs(p[maxindex]) ){
            p[maxindex]=0;
            maxindex = i;
        }
        else
            p[i]=0;
    }
    if(p[maxindex] < 0)
        p[maxindex]=-1.0;
    else
        p[maxindex]=1.0;

    isect.distance()=t;
    isect.position()=h;
    isect.normal()=p;

    return true;
}

bool NonhierBox::wIntersectRay(Ray &r){
    double tx1,tx2,ty1,ty2,tz1,tz2;
    Point3D a = m_pos;
    Point3D b(a[0]+ m_size,a[1] + m_size,a[2]+ m_size);
    tx1 = (a[0] - r.origin()[0])/r.direction()[0];
    tx2 = (b[0] - r.origin()[0])/r.direction()[0];
    ty1 = (a[1] - r.origin()[1])/r.direction()[1];
    ty2 = (b[1] - r.origin()[1])/r.direction()[1];
    tz1 = (a[2] - r.origin()[2])/r.direction()[2];
    tz2 = (b[2] - r.origin()[2])/r.direction()[2];

    double tmin = max(min(tx1,tx2),max(min(ty1,ty2),min(tz1,tz2)));
    double tmax = min(max(tx1,tx2),min(max(ty1,ty2),max(tz1,tz2)));

    if(tmax < eps)
        return false;
    return true;
}
