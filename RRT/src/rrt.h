#ifndef RRT_H
#define RRT_H
#include "kdtree.h"
#include "scene.h"

/*
  references:
  Randomized kinodynamics Planning, Steven M. LaValle
  http://msl.cs.uiuc.edu/rrt/index.html
*/

class rrt
{

public:
    rrt(SceneNode* scene);
    bool bidirectional_plan(std::vector<Vector3D> &path);
    kdtree *begin_tree, *end_tree;
    ~rrt();

private:
    SceneNode* m_scene;
    model* m_model;
    bool Extend(const Vector3D &x, kdtree* rrtree,  Vector3D &x_near, bool forward = true);
    kdres* NearestNeighbor(const Vector3D &x, kdtree* rrtree);
    Point2D SelectBestControl(const Vector3D &x1, const Vector3D &x2,
                              Vector3D &xnear_best, bool &success,
                              bool forward = true);
    /* get a random double between -500 and 500 */
    double rd( void ) {
        return ((double)rand()/RAND_MAX * 1000.0) - 500.00;
    }
    /* get a random double between 0 and 2.0*PI */
    double rdtheta( void ) {
        return (double)rand()/RAND_MAX* 2.0*M_PI;
    }

    std::vector<Vector3D> path2root(kdres *node);

    Vector3D random_state();
    struct node_data{
        node_data(Vector3D s, Point2D c, kdres *p){
            x= new Vector3D(s[0],s[1],s[2]);
            control = new Point2D(c[0],c[1]);
            parent = p;
        }
        Vector3D *x;          //node state
        Point2D *control;   //control from parent to node
        kdres *parent;      //point to parent node
    };
};

#endif // RRT_H
