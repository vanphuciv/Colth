#ifndef VACHAM_H
#define VACHAM_H

#include "vecmath.h"
#include <vector>
#include "particleSystem.h"
class VaCham
{
public:
	virtual void sphereVC(ParticleSystem* particleSystem)=0;
};

//IMPLEMENT YOUR TIMESTEPPERS

class Sphere:public VaCham
{
  void sphereVC(ParticleSystem* particleSystem);
};
#endif