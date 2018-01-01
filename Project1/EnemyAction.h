#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
#include <stdlib.h> 
#include "Particles.h"
class GameObject;
class EnemyAction :
	public MonoBehaviour
{
public:
	int offsetTime;
	vector<Particles> *ParticlesList;
	int ParticlesIndex;
	float lastTime;
	float lastChangeTime;
	float interval;
	float randAngle;
	float randAngle1;
	float rate;
	EnemyAction(vector<Particles> *ParticlesList, int ParticlesIndex,float rate = 0.1f);
	~EnemyAction();
	void update(GLFWwindow* window, GameObject *obj);
};

