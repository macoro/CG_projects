#ifndef MODEL_H
#define MODEL_H
#include "algebra.h"
#include "vector"
#include "mesh.h"
#include "DEEP/include/SWIFT.h"

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>


/*
  Based on http://msl.cs.uiuc.edu/rrt/ model
  */
typedef boost::mt19937 base_generator_type;
typedef boost::uniform_real<> distribution_type;

class model
{
public:
    model();
    double Speed;    
    std::vector<double> LowerState;   // Vector of minimum values for each state variable
    std::vector<double> UpperState;   // Vector of maximum values for each state variable
    std::vector< std::vector<double> > controls;
    int StateDim;
    int ControlDim;
    Mesh* model_mesh; // geometry

    void setGoalState(std::vector<double> goal){ GoalState = goal;}
    void setInitialState(std::vector<double> init) { InitialState = init;}
    void set_metricweights(std::vector<double> ws){ weights = ws; }
    //State from tansformation matrix
    virtual void setGoalState(Matrix4x4 goal)=0;
    virtual void setInitialState(Matrix4x4 init)=0;


    std::vector<double> EulerIntegrate(const std::vector<double> &x, const std::vector<double> &u, const double &h);
    std::vector<double> RungeKuttaIntegrate(const std::vector<double> &x, const std::vector<double> &u,
                                 const double &h);

    virtual double Metric(const std::vector<double> &x1, const std::vector<double> &x2);
    virtual std::vector<double> RandomState();
    virtual std::vector< std::vector<double> > get_controls(){ return controls; }
    virtual std::vector<double> Integrate(const std::vector<double> &x, const std::vector<double> &u,const double &h) = 0;
    virtual std::vector<double> StateTransitionEquation(const std::vector<double> &x, const std::vector<double> &u)=0;
    virtual std::vector<double> LinearInterpolate(const std::vector<double> &x1, const std::vector<double> &x2,
                       const double &a);
    virtual bool free_collition(SWIFT_Scene* swift_scene, const std::vector<double> &x, int& objs_c, int* objs_ids=0, int scale_level=0)=0;
    virtual bool free_collitionPD(SWIFT_Scene* swift_scene, const std::vector<double> &x, double pdeep)=0;

    std::vector<double> InitialState;
    std::vector<double> GoalState;
    std::vector<double> weights;

protected:
    //randon number generator from boost
    static base_generator_type generator;
    static distribution_type uni_dist;
    static boost::variate_generator<base_generator_type&, boost::uniform_real<> > uni;

};

class freedef3D_model:public model
{
public:
    freedef3D_model();
    virtual double Metric(const std::vector<double> &x1, const std::vector<double> &x2);
    virtual std::vector<double> Integrate(const std::vector<double> &x, const std::vector<double> &u,const double &h);
    virtual std::vector<double> StateTransitionEquation(const std::vector<double> &x, const std::vector<double> &u);
    virtual std::vector<double> RandomState();
    virtual std::vector<double> LinearInterpolate(const std::vector<double> &x1, const std::vector<double> &x2,
                       const double &a);
    //State from tansformation matrix
    virtual void setGoalState(Matrix4x4 goal);
    virtual void setInitialState(Matrix4x4 init);

    //model ids for swift
    std::vector<int> ids;
    int scalable_levels;
    double scalable_coeff;

    bool inputin_swiftscene(SWIFT_Scene* swift_scene);
    virtual bool free_collition(SWIFT_Scene* swift_scene, const std::vector<double> &x, int& objs_c, int* objs_ids=0, int scale_level=0);
    virtual bool free_collitionPD(SWIFT_Scene *swift_scene, const std::vector<double> &x, double pdeep);
};

#endif // MODEL_H
