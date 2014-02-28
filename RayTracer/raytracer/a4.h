#ifndef CS488_A4_H
#define CS488_A4_H

#include <string>
#include "algebra.h"
#include "scene.h"
#include "light.h"

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
               );
void display_percentage( const float num, const float total );

#endif
