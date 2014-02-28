#include "mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces, bool hide_box)
  : m_verts(verts),
    m_faces(faces)
{
    this->hide_box= hide_box;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";
  
  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin()) std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}

bool Mesh::IntersectRay(Ray &r, Intersection &isect, Matrix4x4 &inv_transf, double far){
    NonhierBox* bbox = bounding_box(inv_transf.invert());
    bool intersect_box = (*bbox).wIntersectRay(r);

    if( intersect_box && hide_box){
        return closest_triangle(r,isect,inv_transf,far,m_faces);
    }
    else if(intersect_box){
        return (*bbox).IntersectRay(r,isect,inv_transf);
    }
    else
        return false;
}

bool Mesh::closest_triangle(Ray &rp, Intersection &isect, Matrix4x4 &inv_transf
                            , double far, const std::vector< std::vector<int> > &triangles){
    Ray r( (inv_transf*rp.origin()), (inv_transf*rp.direction()));
    double t = far;
    Vector3D normal;
    for (size_t i =0; i < triangles.size(); i++){
        Mesh::Face tri = triangles[i];
        for(size_t v=1;v< tri.size()-1;v++){
            Vector3D ab = m_verts[tri[v]] - m_verts[tri[0]];
            Vector3D ac = m_verts[tri[v+1]] - m_verts[tri[0]];
            Vector3D n = ab.cross(ac);

            double detA = (-r.direction()).dot(n);
            if(detA == 0.0) // ray parallel to triangle
                continue;
            Vector3D ao = r.origin() - m_verts[tri[0]];

            double tmp_t = (ao.dot(n))/detA;
            if(tmp_t < eps)
                continue;

            double alpha = ((-r.direction()).dot(ao.cross(ac)))/detA;
            double beta = ((-r.direction()).dot(ab.cross(ao)))/detA;

            if( (alpha < 0.0) || (beta < 0.0) || ((alpha + beta) > (1.0 + eps)) )
                continue;
            if(tmp_t < t){
                t = tmp_t;
                normal = n;
            }
        }
    }

    if(t < far){
        Point3D h = r.origin() + t*r.direction();
        normal = transNorm(inv_transf,normal);
        normal.normalize();
        isect.distance() = t;
        isect.position() = h;
        isect.normal() = normal;
        return true;
    }

    return false;
}

NonhierBox* Mesh::bounding_box(Matrix4x4 transf){
    double minx, miny, minz, maxx, maxy, maxz;
    Point3D p0(m_verts[0][0],m_verts[0][1],m_verts[0][2]);
    p0 = transf*p0;
    minx = maxx = p0[0];
    miny = maxy = p0[1];
    minz = maxz = p0[2];

    for(size_t vert = 1; vert < m_verts.size(); vert++){
        Point3D p(m_verts[vert][0],m_verts[vert][1],m_verts[vert][2]);
        p = transf*p;
        //x
        if(p[0] < minx)
            minx = p[0];
        else if(p[0] > maxx)
            maxx = p[0];
        //y
        if(p[1] < miny)
            miny = p[1];
        else if(p[1] > maxy)
            maxy = p[1];
        //z
        if(p[2] < minz)
            minz = p[2];
        else if(p[2] > maxz)
            maxz = p[2];
    }

    double size = std::max((maxx - minx),std::max((maxy - miny), (maxz - minz)));
    Point3D pos(minx,miny,minz);

    return new NonhierBox(pos,size);
}
