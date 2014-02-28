#include "mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
    m_bbox= bounding_box();
    m_chainmail = new DeformObject;
    m_chainmail->objSize = 0;
    m_chainmail->maxDeform = 1.5;
    m_chainmail->minDeform = 1.0;
    m_chainmail->maxDeformShear = 1.0;

    m_selectedpoint = false;
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

void Mesh::walk_gl(bool picking){

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

    //draw bounding box
    m_bbox->walk_gl(picking);
    gen_controlpoints(5,5,5);
}

BBox* Mesh::bounding_box(){
    double minx, miny, minz, maxx, maxy, maxz;
    Point3D p0(m_verts[0][0],m_verts[0][1],m_verts[0][2]);
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

    Vector3D size( (maxx - minx),(maxy - miny),(maxz - minz) );
    Point3D pos(minx,miny,minz);

    m_Po = pos;
    m_S = Vector3D(size[0],0,0);
    m_T = Vector3D(0,size[1],0);
    m_U = Vector3D(0,0,size[2]);

    global2local();

    return new BBox(pos,size);
}

void Mesh::gen_controlpoints(int cpx, int cpy, int cpz, bool picking)
{
    //allocate memory for control points
    if(m_chainmail->objSize == 0){
        m_chainmail->object = new DeformElement[cpx*cpy*cpz];
    }
    // center of points
    double step_x = m_bbox->m_size[0]/(cpx - 1);
    double step_y = m_bbox->m_size[1]/(cpy - 1);
    double step_z = m_bbox->m_size[2]/(cpz - 1);

    double radius = std::min(step_x, std::min( step_y, step_z) );
    radius = radius/6.0;
    int cx= cpx/2; int cy = cpy/2; int cz = cpz/2;
    int index = 0;
    for(int i= 0; i < cpx; i++)
        for(int j= 0; j< cpy; j++)
            for(int k=0; k< cpz; k++){
                //translate the point
                Matrix4x4 m_trans;
                if(m_chainmail->objSize == 0 || m_chainmail->objSize <= index){
                    m_trans[0][3] = m_bbox->m_pos[0] + i*step_x;
                    m_trans[1][3] = m_bbox->m_pos[1] + j*step_y;
                    m_trans[2][3] = m_bbox->m_pos[2] + k*step_z;

                    //insert the point
                    m_chainmail->object[index].x = m_trans[0][3];
                    m_chainmail->object[index].y = m_trans[1][3];
                    m_chainmail->object[index].z = m_trans[2][3];
                    DeformElement* link = &m_chainmail->object[index];

                    //if point is'nt on left face add left neightbor
                    if(i>0){
                        link->left = &m_chainmail->object[index - cpy*cpz];
                        m_chainmail->object[index - cpy*cpz].right = link;
                    }
                    else{
                        link->left = NULL;
                    }
                    //if point is'nt on bottom face add bottom meightbor
                    if(j>0){
                        link->bottom = &m_chainmail->object[index - cpz];
                        m_chainmail->object[index - cpz].top = link;
                    }
                    else{
                        link->bottom = NULL;
                    }
                    //if point is'nt on back face add back meightbor
                    if(k>0){
                        link->back = &m_chainmail->object[index - 1];
                        m_chainmail->object[index - 1].front = link;
                    }
                    else{
                        link->back = NULL;
                    }
                    if(k == cpz-1){
                        link->front = NULL;
                    }
                    if(j == cpy -1){
                        link->top = NULL;
                    }
                    if(i == cpx - 1){
                        link->right = NULL;
                    }
                    if(i==cx && j == cy && k == cz)
                        m_chainmail->centre = link;

                    m_chainmail->objSize++;
                }
                else{
                    m_trans[0][3] = m_chainmail->object[index].x;
                    m_trans[1][3] = m_chainmail->object[index].y;
                    m_trans[2][3] = m_chainmail->object[index].z;
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

                Sphere cpoint;
                cpoint.walk_gl(radius, picking);

                glPopMatrix();
                glMatrixMode(mm);

                index++;
            }
}


bool Mesh::select_controlpoint(int index){
    if(index < m_chainmail->objSize){
        m_chainmail->selectedElementPtr = &m_chainmail->object[index];
        m_selectedpoint = true;
        return true;
    }
    m_selectedpoint = false;
    return false;
}

void Mesh::move_controlpoint(float dx, float dy, float dz){
    if(m_selectedpoint){
        TestMove(m_chainmail,&dx,&dy,&dz,0);
        center_cm2bb();
        deform_verts(5,5,5);
    }
}

void Mesh::global2local(){

    for(size_t vert = 0; vert < m_verts.size(); vert++){
        Vector3D tmp = m_verts[vert] - m_Po;
        Point3D lv;
        lv[0] = (m_T.cross(m_U)).dot(tmp)/((m_T.cross(m_U)).dot(m_S));
        lv[1] = (m_U.cross(m_S)).dot(tmp)/((m_U.cross(m_S)).dot(m_T));
        lv[2] = (m_S.cross(m_T)).dot(tmp)/((m_S.cross(m_T)).dot(m_U));

        m_localverts.push_back(lv);
    }
}


void Mesh::deform_verts(int cpx,int cpy, int cpz)
{


    for(size_t vert = 0; vert < m_localverts.size(); vert++){

        Vector3D gvert;
        boost::math::binomial_distribution<double> x_dist = boost::math::binomial_distribution<double>((cpx - 1),m_localverts[vert][0]);
        for(int i= 0; i<cpx; i++){

            Vector3D gvert_j;
            boost::math::binomial_distribution<double> y_dist = boost::math::binomial_distribution<double>((cpy - 1),m_localverts[vert][1]);
            for(int j=0; j<cpy; j++){

                Vector3D gvert_k;
                boost::math::binomial_distribution<double> z_dist = boost::math::binomial_distribution<double>((cpz - 1),m_localverts[vert][2]);
                for(int k=0; k<cpz; k++){
                    int index = i*cpy*cpz + j*cpz + k;
                    Vector3D Pijk(m_chainmail->object[index].x, m_chainmail->object[index].y, m_chainmail->object[index].z);
                    gvert_k = gvert_k + boost::math::pdf(z_dist,k)*Pijk;
                }

                gvert_j = gvert_j + boost::math::pdf(y_dist,j)*gvert_k;
            }

            gvert = gvert + boost::math::pdf(x_dist,i)*gvert_j;

        }

        //deforming vertice
        m_verts[vert][0] = gvert[0];
        m_verts[vert][1] = gvert[1];
        m_verts[vert][2] = gvert[2];

    }
}

void Mesh::center_cm2bb(){

    float dx = m_bbox->centre[0] - m_chainmail->centre->x;
    float dy = m_bbox->centre[1] - m_chainmail->centre->y;
    float dz = m_bbox->centre[2] - m_chainmail->centre->z;

    Vector3D trsl(dx,dy,dz);
    if(trsl.length2() > M_EPS)
        TestMove(m_chainmail, &dx, &dy, &dz, 1);
}





