#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>
#include <iostream>
#include <cmath>

#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int numParticles);
        vector<int> rc_i(int i);
        vector<int> vicinity1(int i);
        vector<int> vicinity2(int i);
        vector<int> vicinity3(int i);
        //Vector3f acceleration(Vector3f position, Vector3f velocity, Vector3f vc_position, Vector3f vc_velocity, float r);
        Vector3f acceleration(Vector3f position, Vector3f velocity, Vector3f top_position, Vector3f top_velocity, float r1);
        Vector3f accelerationSum( int i, vector<Vector3f> state);
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

private:

};


#endif
