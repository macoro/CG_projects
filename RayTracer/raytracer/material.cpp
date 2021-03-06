#include "material.h"

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
  // Perform OpenGL calls necessary to set up this material.
}

Colour PhongMaterial::get_diffuse(){
        return m_kd;
}

Colour PhongMaterial::get_specular() {
        return m_ks;
}

double PhongMaterial::get_shininess() {
        return m_shininess;
}
