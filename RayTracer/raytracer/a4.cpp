#include "a4.h"
#include "image.h"

using namespace std;

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights
               )
{
    // Fill in raytracing code here.
    Image img(width, height, 3);
    Matrix4x4 inv_transf;
    //pinhole camera
    Vector3D m_cx = (view.cross(up));
    m_cx.normalize();
    Vector3D m_cy = (-up);

    double m_fovrad = (fov*M_PI)/360.0;
    double m_cw = tan(m_fovrad);
    double a =(double(width)/double(height));
    double m_ch = m_cw;

    //for percent display
    float done = 0;
#pragma omp parallel for default(shared) reduction(+:done) schedule(guided)
    for (int y = 0; y < height; y++) {
        done = done + 1.0f;
        display_percentage( done, (float)height);
        for (int x = 0; x < width; x++) {
            //background color
            // Red: increasing from top to bottom
            double red = (double)y / height;
            // Green: increasing from top and bottom to middle
            double green = ((red <= 0.5) ? 2.0*red : (2.0 - 2.0*red));
            // Blue: decreasing from top to bottom
            double blue = 1.0 - red;
            Colour back(red,green,blue);
            //antialiasing  http://www.codermind.com/articles/Raytracer-in-C++-Part-II-Specularity-post-processing.html
            double sampleRatio= 0.25;
            Colour aux(0.0, 0.0, 0.0);
            for (double fragmentx = double(x) ; fragmentx < x + 1.0; fragmentx += 0.5 )
                for (double fragmenty = double(y) ; fragmenty < y + 1.0; fragmenty += 0.5 )
                {
                    Vector3D m_raydir;
                    //get primary ray direction
                    m_raydir = view + (fragmentx / (double(width))*2.0 -1.0 )*m_cw*a*m_cx + ((fragmenty) / (double(height))*2.0 - 1.0) * m_ch*m_cy;
                    m_raydir.normalize();
                    //m_raydir = Vector3D(0.0,0.0,-1.0);
                    Ray r(eye,m_raydir);
                    //Ray r(Point3D(x,y,800),m_raydir);
                    double coef = 1.0;
                    int level = 0;
                    bool hit = false;
                    Colour tmp(0.0, 0.0, 0.0);
                    do{
                        // find the closest intersection
                        Intersection closest_isect;
                        hit = root->closest_intersect(r,closest_isect,inv_transf);
                        //if(!hit) cout<< "fail hit"<< endl;
                        if(hit){
                            //cout<< "success hit"<< endl;
                            // put color to pixel
                            Colour diffuse(0.0, 0.0, 0.0);
                            Colour specular(0.0, 0.0, 0.0);
                            Ray reflected_ray;//reflected ray
                            reflected_ray.origin()= closest_isect.position();

                            for (std::list<Light*>::const_iterator cl = lights.begin(); cl != lights.end(); ++cl){

                                Vector3D L = (*cl)->position - closest_isect.position();
                                double light_dist = L.length();
                                if((L.dot(closest_isect.normal())) <= 0.0 || light_dist < 0.0)// test if the point if lighted
                                    continue;
                                L.normalize();
                                reflected_ray.direction()=L;

                                // attenuation factor
                                double fatt = 1.0/((*cl)->falloff[0]+ (*cl)->falloff[1]*light_dist
                                                   + (*cl)->falloff[2]*light_dist*light_dist);
                                coef = fatt;
                                if(coef < double(1e-6)) break;

                                // test if point is in shadow
                                bool in_shadow = false;
                                Intersection aux_isect;
                                if(root->closest_intersect(reflected_ray,aux_isect,inv_transf,light_dist))
                                    in_shadow = true;

                                //ilumination
                                if(!in_shadow){
                                    double lambert = reflected_ray.direction().dot(closest_isect.normal())*fatt;
                                    diffuse = diffuse + (lambert*(*cl)->colour)*closest_isect.mtrl()->get_diffuse();
                                    // reflection by blinn-phong model reference:http://www.codermind.com/
                                    Vector3D blinnDir = reflected_ray.direction() - r.direction();
                                    double temp = blinnDir.length2();
                                    if (temp != 0.0 )
                                    {
                                        blinnDir.normalize();
                                        double blinnTerm = max(blinnDir.dot(closest_isect.normal()), 0.0);
                                        blinnTerm = pow(blinnTerm ,closest_isect.mtrl()->get_shininess())*fatt;

                                        specular =specular + blinnTerm*closest_isect.mtrl()->get_specular()*(*cl)->colour;
                                    }
                                }
                            }
                            tmp = tmp + ambient*closest_isect.mtrl()->get_diffuse() + diffuse + specular;
                            if(closest_isect.mtrl()->get_shininess() < eps)
                                break;
                            double reflet = 2.0*(r.direction().dot(closest_isect.normal()));
                            r.origin() = closest_isect.position() + eps*closest_isect.normal();
                            r.direction() = (r.direction() - reflet * closest_isect.normal());
                            r.direction().normalize();
                            level++;
                        }
                    }while((coef > double(1e-6)) && level < 5 && hit);

                    aux =aux + sampleRatio*tmp;
                }
            if(aux.R()!=0.0 || aux.G()!= 0.0 || aux.B()!= 0.0)
                back = aux;

            img(x, y, 0) = back.R();
            img(x, y, 1) =back.G();
            img(x, y, 2) = back.B();
        }
    }

    std::cerr << "Stub: a4_render(" << root << ",\n     "
              << filename << ", " << width << ", " << height << ",\n     "
              << eye << ", " << view << ", " << up << ", " << fov << ",\n     "
              << ambient << ",\n     {";

    for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
        if (I != lights.begin()) std::cerr << ", ";
        std::cerr << **I;
    }
    std::cerr << "});" << std::endl;
    img.savePng(filename);
}

//display percentage
void display_percentage( const float num, const float total ) {
  const float tmp = (100.0f)*(num / total);
  if(((int)(tmp*100.0f))%1000 == 0)
      cout<< tmp <<"% ";
  if(tmp == 100)
      cout<<"Done "<<endl;

}
