#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
class GameObject;
class HitJudge :
	public MonoBehaviour
{
public:
	int *score;
	vector<int> *enemies;
	vector<GameObject> *gameobjects;

	HitJudge(vector<GameObject> *gameobjects_, vector<int> *enemies_, int *score_);
	~HitJudge();
	void update(GLFWwindow* window, GameObject *obj);
};

