#ifndef SCENE_LUA_H
#define SCENE_LUA_H

#include <string>
#include "scene.h"

SceneNode* import_lua(const std::string& filename);

#endif
