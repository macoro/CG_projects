 #include "rrt.h"

const double eps = double(1e-02);
const int max_sampling = double(1e+6);

rrt::rrt(SceneNode *scene)
{
    m_scene = scene;
    m_model = SceneNode::m_model;
    begin_tree = kd_create( 3 );
    end_tree = kd_create( 3 );
    //m_swiftscene = new SWIFT_Scene(true, false);
}

rrt::~rrt(){
    kd_free(begin_tree);
    kd_free(end_tree);
}

bool rrt::scalable_bidirectional_plan(std::vector<std::vector<double> > &path, std::vector<node_data*>& df_data)
{
    node_data* d1=new node_data(m_model->InitialState,0);
    kd_insert3(begin_tree,m_model->InitialState[0], m_model->InitialState[1], m_model->InitialState[2],d1);
    node_data* d2=new node_data(m_model->GoalState,0);
    kd_insert3(end_tree,m_model->GoalState[0], m_model->GoalState[1], m_model->GoalState[2],d2);

    int i = 0; //samples nodes
    std::vector<double> x_rand,x_near, x_near2;

    kdtree *Ta = begin_tree;
    kdtree *Tb = end_tree;
    kdtree *swaptree;
    bool forward = true;

    while( (i<max_sampling)){
        x_rand = m_model->RandomState();
        if(Extend(x_rand,Ta,x_near)){
            if(Extend(x_near,Tb,x_near2)){
                double metric = m_model->Metric(x_near,x_near2);
                if(metric < 1.5){
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
                    //path.clear();
                    std::vector<node_data*> df_data1 = std::vector<node_data*>();
                    std::vector<node_data*> df_data2 = std::vector<node_data*>();
                    std::vector<std::vector<double> > path1 = path2root(bnode, df_data1);
                    std::vector<std::vector<double> > path2 = path2root(enode, df_data2);

//                    for(size_t p1 = (path1.size()-1); p1 >= 0 ; p1--){
//                        path.push_back(path1[p1]);
//                        df_data.push_back(df_data1[p1]);
//                    }

                    for(size_t p1 = 0; p1 < path1.size() ; p1++){
                        int index = path1.size()-1-p1;
                        path.push_back(path1[index]);
                        df_data.push_back(df_data1[index]);
                    }

                    for(size_t p2 = 0; p2 < path2.size(); p2++){
                        path.push_back(path2[p2]);
                        df_data.push_back(df_data2[p2]);
                    }
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

bool rrt::pdeep_bidirectional_plan(std::vector<std::vector<double> > &path, double pdeep){
    node_data* d1=new node_data(m_model->InitialState,0);
    kd_insert3(begin_tree,m_model->InitialState[0], m_model->InitialState[1], m_model->InitialState[2],d1);
    node_data* d2=new node_data(m_model->GoalState,0);
    kd_insert3(end_tree,m_model->GoalState[0], m_model->GoalState[1], m_model->GoalState[2],d2);

    int i = 0; //samples nodes
    std::vector<double> x_rand,x_near, x_near2;

    kdtree *Ta = begin_tree;
    kdtree *Tb = end_tree;
    kdtree *swaptree;
    bool forward = true;

    while( (i<max_sampling)){
        x_rand = m_model->RandomState();
        if(Extend_PD(x_rand,Ta,x_near,pdeep)){
            if(Extend_PD(x_near,Tb,x_near2,pdeep)){
                double metric = m_model->Metric(x_near,x_near2);
                if(metric < 1.5){
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
                    std::vector<node_data*> df_data1 = std::vector<node_data*>();
                    std::vector<node_data*> df_data2 = std::vector<node_data*>();
                    std::vector<std::vector<double> > path1 = path2root(bnode, df_data1);
                    std::vector<std::vector<double> > path2 = path2root(enode, df_data2);
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

kdres *rrt::NearestNeighbor(const std::vector<double> &x, kdtree *rrtree){
    kdres *presults;
    presults = kd_nearest3(rrtree,x[0],x[1],x[2]);
    return presults;
}

bool rrt::Extend(const std::vector<double> &x, kdtree *rrtree, std::vector<double> &x_near)
{
    bool success = false;
    kdres *nn = NearestNeighbor(x,rrtree);
    std::vector<double> xnn = ((node_data*)kd_res_item_data(nn))->x;
    x_near = m_model->LinearInterpolate(xnn,x,eps);
    double metric = m_model->Metric(xnn,x_near);
    double scalefactor = 1.0;
    int objs_c=0;
    int* objs_ids = new int[m_scene->fixed_objs.size()];

    if(metric < 1.5 && metric > eps){
        bool freecollide = m_model->free_collition(m_swiftscene,x_near,objs_c,objs_ids);
        bool hscale = true;
        int i=1;
        int levels = dynamic_cast<freedef3D_model*>(m_model)->scalable_levels;
        double sc_coeff = dynamic_cast<freedef3D_model*>(m_model)->scalable_coeff;

        //test
        if(objs_c)
            std::cout<<"objs_ids "<< objs_ids[0]<< std::endl;

        while(!freecollide && i <= levels){
            int mock_objs_c = 0;
            int* mock_objs_ids = 0;
            freecollide = m_model->free_collition(m_swiftscene,x_near,mock_objs_c,mock_objs_ids,i);
            //put weight to scale
            if(freecollide){
                scalefactor = pow(sc_coeff,i);
                hscale = ((1.0/scalefactor)*metric < 1.5);
                break;
            }
            i++;
        }
        success = (freecollide && hscale);
    }
    if(success){
        node_data*d = new node_data(x_near,nn,scalefactor,objs_c,objs_ids);
        kd_insert3(rrtree, x_near[0], x_near[1], x_near[2],d);
    }
    return success;
}

bool rrt::Extend_PD(const std::vector<double> &x, kdtree *rrtree, std::vector<double> &x_near, double pdeep)
{
    bool success = false;
    kdres *nn = NearestNeighbor(x,rrtree);
    std::vector<double> xnn = ((node_data*)kd_res_item_data(nn))->x;
    x_near = m_model->LinearInterpolate(xnn,x,eps);
    double metric = m_model->Metric(xnn,x_near);
    if(metric < 1.5 && metric > eps){
        success = m_model->free_collitionPD(m_swiftscene,x_near, pdeep);
    }
    if(success){
        node_data*d = new node_data(x_near,nn);
        kd_insert3(rrtree, x_near[0], x_near[1], x_near[2],d);
    }
    return success;
}

std::vector<std::vector<double > > rrt::path2root(kdres *node, std::vector<node_data*>& df_data){
   std::vector<std::vector<double > > result;
   std::vector<double> x = ((node_data*)kd_res_item_data(node))->x;
   result.push_back(x);
   df_data.push_back((node_data*)kd_res_item_data(node));
   kdres *parent = ((node_data*)kd_res_item_data(node))->parent;
   while( parent != 0 ){
       x = ((node_data*)kd_res_item_data(parent))->x;
       result.push_back(x);
       df_data.push_back((node_data*)kd_res_item_data(parent));
       parent = ((node_data*)kd_res_item_data(parent))->parent;
   }
   return result;
}
