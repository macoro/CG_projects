#include "mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
    builmodel();
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

void Mesh::walk_gl(bool picking) const{
    glBegin(GL_TRIANGLES);
        for (std::vector<Mesh::Face>::const_iterator I = m_faces.begin(); I != m_faces.end(); ++I) {
            for(size_t v=1;v< (*I).size()-1;v++){
                Vector3D ab = m_verts[(*I)[v]] - m_verts[(*I)[0]];
                Vector3D ac = m_verts[(*I)[v+1]] - m_verts[(*I)[0]];
                Vector3D n = ab.cross(ac);
                n.normalize();
                // Vertex one
                glNormal3d(n[0],n[1],n[2]);
                glVertex3d(m_verts[(*I)[0]][0],m_verts[(*I)[0]][1],m_verts[(*I)[0]][2]);
                // Vertex two
                glNormal3d(n[0],n[1],n[2]);
                glVertex3d(m_verts[(*I)[v]][0],m_verts[(*I)[v]][1],m_verts[(*I)[v]][2]);
                // Vertex three
                glNormal3d(n[0],n[1],n[2]);
                glVertex3d(m_verts[(*I)[v+1]][0],m_verts[(*I)[v+1]][1],m_verts[(*I)[v+1]][2]);
            }
        }
    glEnd();
}


void Mesh::builmodel(){
    PQP_Model *model = new PQP_Model;
    model->BeginModel();

    int count = 0;

    for (std::vector<Mesh::Face>::const_iterator I = m_faces.begin(); I != m_faces.end(); ++I) {
        for(size_t v=1;v< (*I).size()-1;v++){
            PQP_REAL p1[3], p2[3], p3[3];
            // Vertex one
            p1[0] = (PQP_REAL)m_verts[(*I)[0]][0];
            p1[1] = (PQP_REAL)m_verts[(*I)[0]][1];
            p1[2] = (PQP_REAL)m_verts[(*I)[0]][2];

            // Vertex two
            p2[0] = (PQP_REAL)m_verts[(*I)[v]][0];
            p2[1] = (PQP_REAL)m_verts[(*I)[v]][1];
            p2[2] = (PQP_REAL)m_verts[(*I)[v]][2];

            // Vertex three
            p3[0] = (PQP_REAL)m_verts[(*I)[v+1]][0];
            p3[1] = (PQP_REAL)m_verts[(*I)[v+1]][1];
            p3[2] = (PQP_REAL)m_verts[(*I)[v+1]][2];

            model->AddTri(p1,p2,p3,count);
            count ++;
        }
    }
    model->EndModel();

    m_model = model;
}

bool Mesh::lq_tolerance(PQP_REAL R1[3][3], PQP_REAL T1[3], PQP_Model *other_model, PQP_REAL R2[3][3], PQP_REAL T2[3], PQP_REAL tolerance){
    PQP_ToleranceResult tres;
    PQP_Tolerance(&tres, R1, T1, m_model, R2, T2, other_model, tolerance);

    return tres.CloserThanTolerance();
}


double Mesh::getLength(){
    double minx, miny, minz, maxx, maxy, maxz;
    Point3D p0(m_verts[0][0],m_verts[0][1],m_verts[0][2]);
    p0 = p0;
    minx = maxx = p0[0];
    miny = maxy = p0[1];
    minz = maxz = p0[2];

    for(size_t vert = 1; vert < m_verts.size(); vert++){
        Point3D p(m_verts[vert][0],m_verts[vert][1],m_verts[vert][2]);
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

    double lenght = std::max((maxx - minx),std::max((maxy - miny), (maxz - minz)));
    return lenght;
}
