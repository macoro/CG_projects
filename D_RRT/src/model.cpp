#include "model.h"

model::model()
{

}

base_generator_type model::generator(42);
distribution_type model::uni_dist(0,1);
boost::variate_generator<base_generator_type&, boost::uniform_real<> > model::uni(generator,uni_dist);

std::vector<double> model::RungeKuttaIntegrate(const std::vector<double> &x, const std::vector<double> &u,
          const double &h)
{
    std::vector<double> result(StateDim);
    std::vector<double> k1 = StateTransitionEquation(x,u);
    std::vector<double> x2;
    for(int i = 0; i< StateDim; i++){
        x2.push_back(x[i] + (0.5*h)*k1[i]);
    }
    std::vector<double> k2 = StateTransitionEquation(x2,u);
    std::vector<double> x3;
    for(int i = 0; i< StateDim; i++){
        x3.push_back(x[i] + (0.5*h)*k2[i]);
    }
    std::vector<double> k3 = StateTransitionEquation(x3,u);
    std::vector<double> x4;
    for(int i = 0; i< StateDim; i++){
        x4.push_back(x[i] + h*k3[i]);
    }
    std::vector<double> k4 = StateTransitionEquation(x4,u);

    double tmp = (h/6.0);
    for(int i = 0; i< StateDim; i++){
        result[i] = x[i] + tmp*(6.0*(k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i]));
    }
    return result;
}

std::vector<double> model::EulerIntegrate(const std::vector<double> &x, const std::vector<double> &u, const double &h){
    std::vector<double> result(StateDim);
    std::vector<double> k1 = StateTransitionEquation(x,u);
    for(int i = 0; i< StateDim; i++){
       result[i] = x[i] + h*k1[i];
    }
    return result;
}

std::vector<double> model::RandomState() {
  double r;
  std::vector<double> rx(StateDim);

  rx = LowerState;
  for (int i = 0; i < StateDim; i++) {
      r = (double)rand()/RAND_MAX;
      //r = uni();
      rx[i] += r * (UpperState[i] - LowerState[i]);
    }

  return rx;
}

double model::Metric(const std::vector<double> &x1, const std::vector<double> &x2){
    double rho=0;
    for (int i = 0; i < StateDim; i++) {
        rho += (x1[i]-x2[i])*(x1[i]-x2[i]);
    }
    return sqrt(rho);
}

std::vector<double> model::LinearInterpolate(const std::vector<double> &x1, const std::vector<double> &x2, const double &a){
    std::vector<double> result(StateDim);
    for(int i = 0; i< StateDim; i++){
       result[i] = (1.0 - a)*x1[i] + a*x2[i];
    }
    return result;
}

/*
 * freedef3D_model class implementation
 */

freedef3D_model::freedef3D_model()
{
    StateDim = 7;
    ControlDim = 1;
    std::vector<double> c1(1);
    controls.push_back(c1);
}

std::vector<double> freedef3D_model::RandomState(){
    double r;
    std::vector<double> rx(StateDim);

    rx = LowerState;
    //sample position coords first
    for (int i = 0; i < 3; i++) {
        //r = (double)rand()/RAND_MAX;
        r = uni();
        rx[i] += r * (UpperState[i] - LowerState[i]);
      }
    //sample rotation as quaternion
    //r = (double)rand()/RAND_MAX;
    r = uni();
    double deltha_1 = sqrt(1-r);
    double deltha_2 = sqrt(r);
    //double phi_1 = 2.0*M_PI*((double)rand()/RAND_MAX);
    double phi_1 = 2.0*M_PI*uni();
    //double phi_2 = 2.0*M_PI*((double)rand()/RAND_MAX);
    double phi_2 = 2.0*M_PI*uni();
    rx[3] = deltha_2*cos(phi_2);//w
    rx[4] = deltha_1*sin(phi_1);//x
    rx[5] = deltha_1*cos(phi_1);//y
    rx[6] = deltha_2*sin(phi_2);//z

    return rx;
}

double freedef3D_model::Metric(const std::vector<double> &x1, const std::vector<double> &x2){
    //euclidean metric for position
    double rho_1=0;
    for (int i = 0; i < 3; i++) {
        rho_1 += (x1[i]-x2[i])*(x1[i]-x2[i]);
    }
    rho_1=weights[0]*sqrt(rho_1);
    //metric for quaternions rotation
    double lambda=0;
    for(int i = 3; i < 7; i++){
        lambda += x1[i]*x2[i];
    }
    double rho_2 = (1 - weights[0])*(1 - fabs(lambda));

    return (rho_1 + rho_2);
}

std::vector<double> freedef3D_model::Integrate(const std::vector<double> &x, const std::vector<double> &u, const double &h){

}

std::vector<double> freedef3D_model::StateTransitionEquation(const std::vector<double> &x, const std::vector<double> &u){

}

std::vector<double> freedef3D_model::LinearInterpolate(const std::vector<double> &x1, const std::vector<double> &x2, const double &a){
    std::vector<double> result(StateDim);
    //position interpolation fist
    for(int i = 0; i< 3; i++){
       result[i] = (1.0 - a)*x1[i] + a*x2[i];
    }
    //rotation interpolation
    double lambda=0;
    for(int i = 3; i < 7; i++){
        lambda += x1[i]*x2[i];
    }
    std::vector<double> tmp_x2 = x2;
    if(lambda < 0.0){
        for(int i = 3; i < 7; i++)
            tmp_x2[i]=-x2[i];
        lambda = -lambda;
    }
    //calculate interpolation factors
    double r,s;
    if(fabs(1-lambda)< double(1e-3)){
        // the quaternions are nearly parallel, so use linear interpolation
        r = 1 - a;
        s = a;
    }
    else{
        //calculate spherical linear interpolation factors
        double alpha = acos(lambda);
        double gamma = (1.0/sin(alpha));
        r = sin((1.0 -a)*alpha)*gamma;
        s = sin(a*alpha)*gamma;
    }
    //set the interpolated quaternion
    result[3] = r*x1[3] + s*tmp_x2[3];  //w = r ∗w1 +s ∗w2;
    result[4] = r*x1[4] + s*tmp_x2[4];  //x = r ∗ x1 +s ∗ x2;
    result[5] = r*x1[5] + s*tmp_x2[5];  //y = r ∗ y1 +s ∗ y2;
    result[6] = r*x1[6] + s*tmp_x2[6];  //z = r ∗ z1 +s ∗ z2;

    // normalize the result
    double length=0;
    for(int i = 3; i < StateDim; i++){
        length += result[i]*result[i];
    }
    if(length > double(1e-6)){
        length = sqrt(length);
        for(int i = 3; i < StateDim; i++){
            result[i]= (result[i]/length);
        }
    }
    return result;
}

bool freedef3D_model::inputin_swiftscene(SWIFT_Scene *swift_scene){
    ids = std::vector<int>();
    // Add an object described by arrays.  This model must be convex.


    int vn = model_mesh->m_verts.size();
    SWIFT_Real vs[3*vn];
    //fill vertex array
    for(int i=0; i<vn; i++){
       int index = 3*i;
       vs[index] = model_mesh->m_verts[i][0];
       vs[index+1]= model_mesh->m_verts[i][1];
       vs[index+2]= model_mesh->m_verts[i][2];
    }

    int fn = model_mesh->m_faces.size();
    int *fv = new int[fn]; //faces valences
    int fl = 0;  //faces array length
    for(int i = 0; i<fn; i++){
        fv[i] = (model_mesh->m_faces[i].size());
        //fv[i] = 3;
        fl += fv[i];
    }
    int *fs= new int[fl];
    int index = 0;
    for(int i = 0; i<fn; i++){
        for(int j = 0; j<fv[i]; j++)
            fs[index+j] = model_mesh->m_faces[i][j];
        index+=fv[i];
    }
    for(int i=0; i <= scalable_levels;i++){
        int id;
        double scale = pow(scalable_coeff,i);
        if( !swift_scene->Add_Convex_Object( vs, fs, vn, fn, id, false, DEFAULT_ORIENTATION,
                                             DEFAULT_TRANSLATION, scale, DEFAULT_BOX_SETTING,
                                             DEFAULT_BOX_ENLARGE_REL, DEFAULT_BOX_ENLARGE_ABS, fv) ) {
            std::cerr << "Adding object failed -- Exiting..."<<id<< std::endl;
            return false;
        }
        else {
            ids.push_back(id);
            std::cerr << "Added object to scene model id="<< id <<std::endl;
        }
    }
    //Active swift pairs only with the first model
    swift_scene->Deactivate();
    swift_scene->Activate(ids[0]);
    for(size_t i=1; i < ids.size(); i++){
        swift_scene->Deactivate(ids[0],ids[i]);
    }
    return true;
}

bool freedef3D_model::free_collition(SWIFT_Scene *swift_scene, const std::vector<double> &x, int &objs_c, int *objs_ids, int scale_level){
    if(scale_level >= ids.size()) //bad model count
        return true;
    //Activate swift pairs only with selected model
    if(scale_level){
        swift_scene->Deactivate();
        swift_scene->Activate(ids[scale_level]);
        for(size_t i=0; i < ids.size(); i++){
            if(i != scale_level)
                swift_scene->Deactivate(ids[scale_level],ids[i]);
        }
    }

    //State to R and T matrix
    Vector4D Q(x[3],x[4],x[5],x[6]);
    SWIFT_Real R[9];
    // Convert the quaternion to a matrix
    R[0] = 1.0 - 2.0 * Q[2] * Q[2] - 2.0 * Q[3] * Q[3];
    R[1] = 2.0 * Q[1] * Q[2] - 2.0 * Q[0] * Q[3];
    R[2] = 2.0 * Q[1] * Q[3] + 2.0 * Q[0] * Q[2];

    R[3] = 2.0 * Q[1] * Q[2] + 2.0 * Q[0] * Q[3];
    R[4] = 1.0 - 2.0 * Q[1] * Q[1] - 2.0 * Q[3] * Q[3];
    R[5] = 2.0 * Q[2] * Q[3] - 2.0 * Q[0] * Q[1];

    R[6] = 2.0 * Q[1] * Q[3] - 2.0 * Q[0] * Q[2];
    R[7] = 2.0 * Q[2] * Q[3] + 2.0 * Q[0] * Q[1];
    R[8] = 1.0 - 2.0 * Q[1] * Q[1] - 2.0 * Q[2] * Q[2];

    SWIFT_Real T[3];
    T[0] = x[0]; T[1] =x[1]; T[2] = x[2];

    //set object transform
    swift_scene->Set_Object_Transformation(ids[scale_level],R,T);
    int np,*oids;
    bool result = swift_scene->Query_Intersection(false,np,&oids);
    if(np && !scale_level){
        objs_c = np;
        for(int i = 0; i< np; i+=2){
            if( oids[i] == ids[scale_level])
                objs_ids[i] = oids[i+1];
            else
                objs_ids[i]= oids[i];
        }
        std::cout<< result <<" intersection test "<< np <<" ids " << oids[0]<<" & " << oids[1]<< " obj_ids "<<objs_ids[0]<< std::endl;
    }



    Q = Vector4D(x[3],-x[4],-x[5],-x[6]); //Quaternion conjugate
    // Convert the quaternion to a matrix
    R[0] = 1.0 - 2.0 * Q[2] * Q[2] - 2.0 * Q[3] * Q[3];
    R[1] = 2.0 * Q[1] * Q[2] - 2.0 * Q[0] * Q[3];
    R[2] = 2.0 * Q[1] * Q[3] + 2.0 * Q[0] * Q[2];

    R[3] = 2.0 * Q[1] * Q[2] + 2.0 * Q[0] * Q[3];
    R[4] = 1.0 - 2.0 * Q[1] * Q[1] - 2.0 * Q[3] * Q[3];
    R[5] = 2.0 * Q[2] * Q[3] - 2.0 * Q[0] * Q[1];

    R[6] = 2.0 * Q[1] * Q[3] - 2.0 * Q[0] * Q[2];
    R[7] = 2.0 * Q[2] * Q[3] + 2.0 * Q[0] * Q[1];
    R[8] = 1.0 - 2.0 * Q[1] * Q[1] - 2.0 * Q[2] * Q[2];

    T[0] = -x[0]; T[1] = -x[1]; T[2] = -x[2];

    //reset object transform
    swift_scene->Set_Object_Transformation(ids[scale_level],R,T);

    if(scale_level){
        swift_scene->Deactivate();
        swift_scene->Activate(ids[0]);
        for(size_t i=1; i < ids.size(); i++){
            swift_scene->Deactivate(ids[0],ids[i]);
        }
    }

    return (!result);
}

bool freedef3D_model::free_collitionPD(SWIFT_Scene *swift_scene, const std::vector<double> &x, double pdeep){
    //State to R and T matrix
    Vector4D Q(x[3],x[4],x[5],x[6]);
    SWIFT_Real R[9];
    // Convert the quaternion to a matrix
    R[0] = 1.0 - 2.0 * Q[2] * Q[2] - 2.0 * Q[3] * Q[3];
    R[1] = 2.0 * Q[1] * Q[2] + 2.0 * Q[0] * Q[3];
    R[2] = 2.0 * Q[1] * Q[3] - 2.0 * Q[0] * Q[2];

    R[3] = 2.0 * Q[1] * Q[2] - 2.0 * Q[0] * Q[3];
    R[4] = 1.0 - 2.0 * Q[1] * Q[1] - 2.0 * Q[3] * Q[3];
    R[5] = 2.0 * Q[2] * Q[3] + 2.0 * Q[0] * Q[1];

    R[6] = 2.0 * Q[1] * Q[3] + 2.0 * Q[0] * Q[2];
    R[7] = 2.0 * Q[2] * Q[3] - 2.0 * Q[0] * Q[1];
    R[8] = 1.0 - 2.0 * Q[1] * Q[1] - 2.0 * Q[2] * Q[2];

    SWIFT_Real T[3];
    T[0] = x[0]; T[1] =x[1]; T[2] = x[2];

    //set object transform
    swift_scene->Set_Object_Transformation(ids[0],R,T);
    //DEEP
    int   num_pairs=0;
    int* oids=NULL;
    int* num_contacts=NULL;
    SWIFT_Real* distances=NULL;
    SWIFT_Real** nearest_pts=NO_NEAREST_PTS;
    SWIFT_Real* normals=NULL;
    int** feature_types=NO_FEAT_TYPES;
    int** feature_ids=NO_FEAT_IDS;

    bool result = swift_scene->Query_Intersection(true,num_pairs,&oids);
    if(result){
        result = swift_scene->Query_Contact_Determination( true, 0.0, num_pairs,
                                                           &oids, &num_contacts, &distances, nearest_pts,
                                                           &normals, feature_types, feature_ids );

        if(result)
            result = (fabs(distances[0]) > pdeep); // free colision if PD is less of pdeep

        std::cout << "PD Dist = " << distances[0] << " PD Dir = ("
                    << normals[0] << ", " << normals[1] << ", " << normals[2] << ")" << std::endl;
    }

    Q = Vector4D(x[3],-x[4],-x[5],-x[6]); //Quaternion conjugate
    // Convert the quaternion to a matrix
    R[0] = 1.0 - 2.0 * Q[2] * Q[2] - 2.0 * Q[3] * Q[3];
    R[1] = 2.0 * Q[1] * Q[2] + 2.0 * Q[0] * Q[3];
    R[2] = 2.0 * Q[1] * Q[3] - 2.0 * Q[0] * Q[2];

    R[3] = 2.0 * Q[1] * Q[2] - 2.0 * Q[0] * Q[3];
    R[4] = 1.0 - 2.0 * Q[1] * Q[1] - 2.0 * Q[3] * Q[3];
    R[5] = 2.0 * Q[2] * Q[3] + 2.0 * Q[0] * Q[1];

    R[6] = 2.0 * Q[1] * Q[3] + 2.0 * Q[0] * Q[2];
    R[7] = 2.0 * Q[2] * Q[3] - 2.0 * Q[0] * Q[1];
    R[8] = 1.0 - 2.0 * Q[1] * Q[1] - 2.0 * Q[2] * Q[2];

    T[0] = -x[0]; T[1] = -x[1]; T[2] = -x[2];

    //reset object transform
    swift_scene->Set_Object_Transformation(ids[0],R,T);

    return (!result);
}


void freedef3D_model::setInitialState(Matrix4x4 init){
    InitialState.resize(StateDim);
    InitialState[0] = init[0][3];
    InitialState[1] = init[1][3];
    InitialState[2] = init[2][3];
    //set rotation
    Vector4D Q = init.getQuaternion();
    InitialState[3] = Q[0];
    InitialState[4] = Q[1];
    InitialState[5] = Q[2];
    InitialState[6] = Q[3];
}

void freedef3D_model::setGoalState(Matrix4x4 goal){
    GoalState.resize(StateDim);
    GoalState[0] = goal[0][3];
    GoalState[1] = goal[1][3];
    GoalState[2] = goal[2][3];
    //set rotation
    Vector4D Q = goal.getQuaternion();
    GoalState[3] = Q[0];
    GoalState[4] = Q[1];
    GoalState[5] = Q[2];
    GoalState[6] = Q[3];
}
