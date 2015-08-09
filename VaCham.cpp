#include "VaCham.h"


void Sphere::sphereVC(ParticleSystem* particleSystem){
  vector<Vector3f> state = particleSystem->getState();
  vector<Vector3f> result;
	for(unsigned int i = 0; i < state.size(); i++){
              
            if((i%2)==0){
                Vector3f d = state[i] - Vector3f(2.5f, -10.0f, 2.5f);
                float l = d.abs();
                if(l<=1.6){
                    result.push_back(1.1f*d+Vector3f(2.5f, -10.0f, 2.5f));
                   result.push_back(Vector3f(0,0,0));
                    i++;
                }else{
                                result.push_back(state[i]);
            }}else{
            result.push_back(state[i]);
            }
                
	}
        particleSystem->setState(result);
}