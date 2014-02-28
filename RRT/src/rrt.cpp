#include "rrt.h"

const double eps = double(1e-01);
const int max_sampling = double(1e+6);

rrt::rrt(SceneNode *scene)
{
    m_scene = scene;
    m_model = SceneNode::m_model;
    begin_tree = kd_create( 3 );
    end_tree = kd_create( 3 );
}

rrt::~rrt(){
    kd_free(begin_tree);
    kd_free(end_tree);
}

bool rrt::bidirectional_plan(std::vector<Vector3D>& path){
    node_data* d1=new node_data(m_model->InitialState, Point2D(0,0), 0);
    kd_insert3(begin_tree,m_model->InitialState[0], 0.0, m_model->InitialState[1],d1);
    node_data* d2=new node_data(m_model->GoalState, Point2D(0,0),0);
    kd_insert3(end_tree,m_model->GoalState[0], 0.0, m_model->GoalState[1],d2);

    int i = 0; //samples nodes
    Vector3D x_rand,x_near, x_near2;
    Point2D  u_best;

    kdtree *Ta = begin_tree;
    kdtree *Tb = end_tree;
    kdtree *swaptree;
    bool forward = true;

    while( (i<max_sampling)){
        x_rand = random_state();
        if(Extend(x_rand,Ta,x_near,forward)){
            if(Extend(x_near,Tb,x_near2,(!forward))){
                if((x_near - x_near2).length2() < 1.0){
                    // construct path
                    kdres *bnode,*enode;
                    if (forward){
                        bnode= NearestNeighbor(x_near,begin_tree);
                        enode = NearestNeighbor(x_near2,end_tree);
                    }
                    else{
                        bnode= NearestNeighbor(x_near2,begin_tree);
                        enode = NearestNeighbor(x_near,end_tree);
                    }
                    path.clear();
                    std::vector<Vector3D> path1 = path2root(bnode);
                    std::vector<Vector3D> path2 = path2root(enode);
                    for(size_t p1 = 0; p1 < path1.size(); p1++)
                        path.push_back(path1[path1.size()-1-p1]);
                    for(size_t p2 = 0; p2 < path2.size(); p2++)
                        path.push_back(path2[p2]);

                    return true;
                }
            }
            //Swap Ta and Tb
            swaptree = Ta;
            Ta = Tb;
            Tb = swaptree;
            forward = (!forward);
        }

        i++;
    }
    return false;
}

Point2D rrt::SelectBestControl(const Vector3D &x1, const Vector3D &x2, Vector3D &xnear_best,
                               bool &success, bool forward){
    //controls
    Point2D u,u_best;
    //state
    Vector3D x_near;
    //metric
    double d,d_min;
    success = false;

    d_min = m_model->Metric(x1,x2);
    //test all controls and select the one of less metric
    for(size_t c=0; c < m_model->controls.size(); c++){
        u = m_model->controls[c];
        if(forward)
            x_near = m_model->Integrate(x1,u,eps);
        else
            x_near = m_model->Integrate(x1,u,-eps);

        d  = m_model->Metric(x_near,x2);
        if ( (d < d_min) && std::fabs(x_near[0]<=500) && std::fabs(x_near[1])<=500 && ((x1-x_near).length2() > eps) && (m_scene->free_collion(x_near)) ) {
            d_min = d; u_best = u; xnear_best = x_near; success = true;
        }
    }
    return u_best;
}

kdres *rrt::NearestNeighbor(const Vector3D &x, kdtree *rrtree){
    kdres *presults;
    presults = kd_nearest3(rrtree,x[0],0.0,x[1]);
    //Vector3D *data = (Vector3D*)kd_res_item_data(presults);
    return presults;
}

bool rrt::Extend(const Vector3D &x, kdtree *rrtree, Vector3D &x_near, bool forward){
    bool success = false;
    kdres *nn = NearestNeighbor(x,rrtree);
    Vector3D xnn = *((node_data*)kd_res_item_data(nn))->x;
    Point2D u_best = SelectBestControl(xnn,x,x_near,success,forward);
    //check if x_near get the next state
    if(success){
        node_data*d = new node_data(x_near,u_best,nn);
        kd_insert3(rrtree, x_near[0], 0.0, x_near[1],d);
    }
    return success;
}

Vector3D rrt::random_state(){
    return Vector3D(rd(),rd(),rdtheta());
}

std::vector<Vector3D> rrt::path2root(kdres *node){
   std::vector<Vector3D> result;
   Vector3D x = *((node_data*)kd_res_item_data(node))->x;
   result.push_back(x);
   kdres *parent = ((node_data*)kd_res_item_data(node))->parent;
   while( parent != 0 ){
       x = *((node_data*)kd_res_item_data(parent))->x;
       result.push_back(x);
       parent = ((node_data*)kd_res_item_data(parent))->parent;
   }
   return result;
}
