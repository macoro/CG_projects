#include "model.h"

model::model(Vector3D initial, double length)
{
    InitialState = initial;
    CurrentState = InitialState;
    this->length = length;

    double MaxSteeringAngle = M_PI/12.0;
    for (double alpha = -MaxSteeringAngle; alpha <= MaxSteeringAngle;
         alpha += 2.0*MaxSteeringAngle/6.0) {
        controls.push_back(Point2D(1.0,alpha));
        controls.push_back(Point2D(-0.1,alpha));
    }

    GoalState[0]=400.0; GoalState[1]=150; GoalState[2]=M_PI/2.0;
    //speed
    Speed = 100;

}

Vector3D model::StateTransitionEquation(const Vector3D &x, const Point2D &u){
    Vector3D dx;

    dx[0] = Speed*u[0]*sin(x[2]);
    dx[1] = Speed*u[0]*cos(x[2]);
    dx[2] = Speed*u[0]*tan(u[1])/length;

    return dx;
}

Vector3D model::RungeKuttaIntegrate(const Vector3D &x, const Point2D &u,
          const double &h)
{
    Vector3D k1 = StateTransitionEquation(x,u);
    Vector3D k2 = StateTransitionEquation(x + (0.5*h)*k1,u);
    Vector3D k3 = StateTransitionEquation(x + (0.5*h)*k2,u);
    Vector3D k4 = StateTransitionEquation(x + h*k3,u);

    return x + h/6.0*(k1 + 2.0*k2 + 2.0*k3 + k4);
}

Vector3D model::Integrate(const Vector3D &x, const Point2D &u, const double &h){
    Vector3D nx = RungeKuttaIntegrate(x,u,h);

    if (nx[2] > 2.0*M_PI)
        nx[2] -= 2.0*M_PI;
    if (nx[2] < 0.0)
        nx[2] += 2.0*M_PI;
    return nx;
}

double model::Metric(const Vector3D &x1, const Vector3D &x2){
    //double dtheta = std::min(fd,2.0*M_PI - fd);
    return (x1-x2).length2();

   //std::sqrt(fabs(x1[0] - x2[0])) + std::sqrt(fabs(x1[1] - x2[1])) + std::sqrt(50.0/M_PI*dtheta);
}


