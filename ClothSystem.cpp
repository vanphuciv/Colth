#include "ClothSystem.h"

//TODO: Initialize here
ClothSystem::ClothSystem(int numParticles):ParticleSystem(numParticles)
{
    m_numParticles = numParticles * numParticles;
    for(int i = 0; i < numParticles; i++)
    {
        for(int j = 0; j < numParticles; j++)
        {
            m_vVecState.push_back(Vector3f(1*j/2.0,0,1*i/2.0));
            m_vVecState.push_back(Vector3f(0,0,0));
        //   std::cout<<m_vVecState[2][0]<<std::endl;
            
        }
    }
    m_vVecState[1] = Vector3f(0,0,1);
    
}

//Xác định trường hợp cần xử lý
vector<int> ClothSystem::rc_i(int i)
{
    vector<int> rc;
    int size = sqrt(m_numParticles);
    int row = (i/2)/size;
    int column = (i/2)%size;
    rc.push_back(row);
    rc.push_back(column);
    return rc;
}
//Xác định hạt lân cận theo hàng và cột ở gần
vector<int> ClothSystem::vicinity1(int i)
{
    vector<int> vc1;
    vector<int> rc = ClothSystem::rc_i(i);
    
    int size = sqrt(m_numParticles);
    if(0<=(rc[0]-1)&&(rc[0]-1)<size)vc1.push_back(i-2*size);
    if(0<=(rc[0]+1)&&(rc[0]+1)<size)vc1.push_back(i+2*size);
    if(0<=(rc[1]-1)&&(rc[1]-1)<size)vc1.push_back(i-2);
    if(0<=(rc[1]+1)&&(rc[1]+1)<size)vc1.push_back(i+2);
    //for(auto &id:vc1)std::cout<<id<<std::endl;
    //std::cout<<i<<std::endl;
    return vc1;
}

//Xác định lân cận theo hàng và cột ở xa
vector<int> ClothSystem::vicinity2(int i)
{
    vector<int> vc2;
    vector<int> rc = ClothSystem::rc_i(i);
    int size = sqrt(m_numParticles);
    if(0<=(rc[0]-2)&&(rc[0]-2)<size)vc2.push_back(i-4*size);
    if(0<=(rc[0]+2)&&(rc[0]+2)<size)vc2.push_back(i+4*size);
    if(0<=(rc[1]-2)&&(rc[1]-2)<size)vc2.push_back(i-4);
    if(0<=(rc[1]+2)&&(rc[1]+2)<size)vc2.push_back(i+4);
    //for(auto &id:vc2)std::cout<<id<<std::endl;
    return vc2;
}

//Xác định lân cận theo đường chéo
vector<int> ClothSystem::vicinity3(int i)
{
    vector<int> vc3;
    vector<int> rc = ClothSystem::rc_i(i);
    int size = sqrt(m_numParticles);
    if(0<=(rc[0]-1)&&(rc[0]-1)<size&&0<=(rc[1]-1)&&(rc[1]-1)<size)vc3.push_back(i-2-2*size);
    if(0<=(rc[0]-1)&&(rc[0]-1)<size&&0<=(rc[1]+1)&&(rc[1]+1)<size)vc3.push_back(i+2-2*size); 
    if(0<=(rc[0]+1)&&(rc[0]+1)<size&&0<=(rc[1]-1)&&(rc[1]-1)<size)vc3.push_back(i-2+2*size);
    if(0<=(rc[0]+1)&&(rc[0]+1)<size&&0<=(rc[1]+1)&&(rc[1]+1)<size)vc3.push_back(i+2+2*size);
    //for(auto &id:vc3)std::cout<<id<<std::endl;
    return vc3;
    
}

//Tính gia tốc một liên kết
Vector3f ClothSystem::acceleration(Vector3f position, Vector3f velocity, Vector3f top_position, Vector3f top_velocity, float r1)
{
    Vector3f Ftop;
    float mass = 1.0f;
    float r = r1;
    float k = 5;
    
    Vector3f Fgravity = mass*Vector3f(0,-0.02,0);
    Vector3f Fdrag1;
    Vector3f d = position - top_position;
    float l = d.abs();
   
    if(l==0.0f&&(velocity.abs())!=0){
         Ftop = k*r*velocity/(velocity.abs());
    }else{
         Ftop = k*(r-l)*d/l;
          Fdrag1 = -k*((velocity-top_velocity)*d.normalized())*d/l;
    }
    return (Fgravity+Fdrag1/1.5+Ftop)/mass;
}


//Tính tổng gia tốc
Vector3f ClothSystem::accelerationSum( int i, vector<Vector3f> state)
{
    Vector3f aSum;
    vector<int> vc1 = ClothSystem::vicinity1(i) ;
    vector<int> vc2 = ClothSystem::vicinity2(i) ;
    vector<int> vc3 = ClothSystem::vicinity3(i) ;
        ///for(auto &id:vc3)std::cout<<id<<std::endl;

    float r1 = 0.5;
    float r2 = 1;
    float r3 = sqrt(0.5);
    
    //std::cout<<i<<std::endl;
    for(auto &id : vc1)
    {
      
        
       aSum +=  ClothSystem::acceleration(state[i] , state[i+1], state[id],  state[id+1], r1);
    //std::cout<<state[id][0]<<std::endl;
    }
    for(auto &id : vc2)
    {
       // std::cout<<id<<std::endl;
       //  std::cout<<state[i]<<std::endl;
     aSum +=  ClothSystem::acceleration(state[i] , state[i+1], state[id],  state[id+1], r2);
    }
    for(auto &id : vc3)
    {
       // std::cout<<i<<std::endl;
       aSum +=  ClothSystem::acceleration(state[i] , state[i+1], state[id],  state[id+1], r3);
    }
    return aSum + 0.01*Vector3f(rand()%2-1, rand()%2-1,rand()%2-1);
    
}




// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
    vector<Vector3f> f;
    for(int i = 0; i < m_numParticles; i++){
        int pos = 2*i;
        int vel = 2*i + 1;
        
    /*
        int size = sqrt(m_numParticles);
        if(i == 0){
             f.push_back(Vector3f(0,0,0));f.push_back(Vector3f(0,0,0));
        }else if(i == size-1){
             f.push_back(Vector3f(0,0,0));f.push_back(Vector3f(0,0,0));
        }else{
            f.push_back(state[vel]);
            f.push_back( ClothSystem::accelerationSum(pos, state));
        }
        */
     f.push_back(state[vel]);
            f.push_back( ClothSystem::accelerationSum(pos, state));
    }
            
            
        //-----------------Mô phỏng va chạm với quả cầu (chỉ là giải pháp tạm thời )
        for(int i = 0; i < m_numParticles; i++){
            int pos = 2*i;
            int vel = 2*i + 1;
            Vector3f d = state[pos] - Vector3f(2.5f, -10.0f, 2.5f);
            float l = d.abs();
            if(l <= 1.6){
                f[pos] = Vector3f(0,0,0);
                f[vel] = Vector3f(0,0,0);
            }
        }
    
    
    return f;
}



//VẼ Vải  
void ClothSystem::draw()
    {  
        int size =sqrt(m_numParticles);
        glBegin(GL_TRIANGLES);
        GLfloat floorColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorColor);
        for (int x = 0; x < size -1; ++x)
        {
            for (int y =0; y < size-1; ++y)
            {
                
                glColor3f(1.0f,1.0f,1.0f);
                Vector3f pos1=m_vVecState[2*size*y+(x+1)*2] ;
                Vector3f pos2=m_vVecState[2*size*y+x*2] ;
                Vector3f pos3=m_vVecState[2*size*(y+1)+x*2] ;
                Vector3f pos4=m_vVecState[2*size*(y+1)+(x+1)*2] ;
                Vector3f pos5=m_vVecState[2*size*y+(x+1)*2] ;
                Vector3f pos6=m_vVecState[2*size*(y+1)+x*2] ;
                glVertex3f(pos1[0], pos1[1], pos1[2]);
                glVertex3f(pos2[0], pos2[1], pos2[2]);
                glVertex3f(pos3[0], pos3[1], pos3[2]);
                glVertex3f(pos4[0], pos4[1], pos4[2]);
                glVertex3f(pos5[0], pos5[1], pos5[2]);
                glVertex3f(pos6[0], pos6[1], pos6[2]);
                
            }
        }
        glEnd();
    }

