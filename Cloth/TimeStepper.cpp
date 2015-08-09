#include "TimeStepper.hpp"

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    	vector<Vector3f> state = particleSystem->getState();
	vector<Vector3f> f = particleSystem->evalF(state);
	vector<Vector3f> result;
	for(unsigned int i = 0; i < f.size(); i++){
		result.push_back(state[i] + stepSize * f[i]);
	}
	particleSystem->setState(result);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    	vector<Vector3f> state = particleSystem->getState();
	vector<Vector3f> f = particleSystem->evalF(state);
	vector<Vector3f> temp;
	for(unsigned int i = 0; i < f.size(); i++){
		temp.push_back(state[i] + stepSize * f[i]);
	}
	vector<Vector3f> f1 = particleSystem->evalF(temp);
	vector<Vector3f> result;
	for(unsigned int i = 0; i < state.size(); i++){
		result.push_back(state[i] + (stepSize/2) * (f[i] + f1[i]));
	}
	particleSystem->setState(result);
}
