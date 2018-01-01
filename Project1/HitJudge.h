#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Particles.h"
class GameObject;
class HitJudge :
	public MonoBehaviour
{
public:
	int *score;
	vector<int> *enemies;
	vector<GameObject> *gameobjects;
	vector<Particles> *partList;
	HitJudge(vector<GameObject> *gameobjects_, vector<int> *enemies_, vector<Particles> *partList_,int *score_);
	~HitJudge();
	void update(GLFWwindow* window, GameObject *obj);
};

