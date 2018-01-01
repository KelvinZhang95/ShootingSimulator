#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
#include <stdlib.h> 

class GameObject;
class EnemyAction :
	public MonoBehaviour
{
public:
	float lastTime;
	float lastChangeTime;
	float interval;
	float randAngle;
	float randAngle1;
	EnemyAction();
	~EnemyAction();
	void update(GLFWwindow* window, GameObject *obj);
};

