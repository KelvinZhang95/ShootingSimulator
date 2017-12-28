#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Flying.h"
#include "Gravity.h"
#include "Camera.h"
class GameObject;
class Shoot :
	public MonoBehaviour
{
public:
	float shootInterval;
	float flySpeed;
	float lastShootTime;
	GameObject *bulletprefab;
	vector<GameObject> *gameobjects;
	Camera *cam;
	Shoot(GameObject *bulletprefab_, vector<GameObject> *gameobjects_, Camera *cam,float shootInterval_ = 1.0f, float flySpeed_ = 10);
	~Shoot();
	void update(GLFWwindow* window, GameObject *obj);
};

