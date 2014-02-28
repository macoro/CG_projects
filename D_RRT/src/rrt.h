#ifndef RRT_H
#define RRT_H
#include "kdtree.h"
#include "scene.h"

/*
  references:
  Randomized kinodynamics Planning, Steven M. LaValle
  http://msl.cs.uiuc.edu/rrt/index.html
*/

struct node_data{
    node_data(std::vector<double> s,kdres *p, double scale = 1.0, int objs_c = 0,int* objs_ids = 0){
        x= s;
        parent = p;
        this->scale = scale;
        objs_collided = objs_c;
        ids_deformers = objs_ids;

    }
    std::vector<double> x;          //node state
    kdres *parent;      //point to parent node
    double scale;
    int objs_collided;
    int* ids_deformers;
};

class rrt
{

public:
    rrt(SceneNode* scene);
    bool scalable_bidirectional_plan(std::vector<std::vector<double> > &path, std::vector<node_data*> &df_data);
    bool pdeep_bidirectional_plan(std::vector<std::vector<double> > &path, double pdeep);
    kdtree *begin_tree, *end_tree;
    ~rrt();
    SWIFT_Scene* m_swiftscene;

private:
    SceneNode* m_scene;
    model* m_model;    
    bool Extend(const std::vector<double > &x, kdtree* rrtree, std::vector<double > &x_near);
    bool Extend_PD(const std::vector<double > &x, kdtree* rrtree, std::vector<double > &x_near, double pdeep);
    kdres* NearestNeighbor(const std::vector<double> &x, kdtree* rrtree);
    std::vector<std::vector<double > > path2root(kdres *node, std::vector<node_data*>& df_data);
};

#endif // RRT_H
