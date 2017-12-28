#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
class GameObject;
class Gravity :
	public MonoBehaviour
{
public:
	float rate;
	const float gravityRate = 9.8f;
	float lastTime;
	glm::vec3 fallVelocity;
	Gravity(float rate_ = 1);
	~Gravity();
	void update(GLFWwindow* window, GameObject *obj);
};

