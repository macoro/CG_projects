#ifndef MODEL_H
#define MODEL_H
#include "algebra.h"
#include "vector"
#include "mesh.h"

/*
  Based on http://msl.cs.uiuc.edu/rrt/README Model2DRigidCar
  */

class model
{
public:
    model(Vector3D initial, double length);
    double Speed;
    Vector3D InitialState;
    Vector3D GoalState;
    Vector3D CurrentState;
    Vector3D StateTransitionEquation(const Vector3D &x, const Point2D &u);
    void setGoalState(Vector3D goal){
        GoalState = goal;
    }
    Mesh* model_mesh;
    double Metric(const Vector3D &x1, const Vector3D &x2);
    std::vector< Point2D> controls;
    Vector3D Integrate(const Vector3D &x, const Point2D &u, const double &h);
private:
    Vector3D RungeKuttaIntegrate(const Vector3D &x, const Point2D &u,
                                 const double &h);
    double length;

};

#endif // MODEL_H
