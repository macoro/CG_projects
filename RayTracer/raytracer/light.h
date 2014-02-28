#ifndef CS488_LIGHT_H
#define CS488_LIGHT_H

#include "algebra.h"
#include <iosfwd>

// Represents a simple point light.
struct Light {
  Light();
  
  Colour colour;
  Point3D position;
  double falloff[3];
};

std::ostream& operator<<(std::ostream& out, const Light& l);

#endif
