#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
class GameObject;
class HitJudge :
	public MonoBehaviour
{
public:
	vector<GameObject *> *enemies;
	HitJudge(vector<GameObject *> *enemies_);
	~HitJudge();
	void update(GLFWwindow* window, GameObject *obj);
};

