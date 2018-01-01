#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Flying.h"
#include "Gravity.h"
#include "HitJudge.h"
#include "Camera.h"
class GameObject;
class Shoot :
	public MonoBehaviour
{
public:
	vector<GameObject *> *enemies;
	float shootInterval;
	float flySpeed;
	float lastShootTime;
	float gravityRate;
	bool isOffset;
	GameObject *bulletprefab;
	vector<GameObject> *gameobjects;
	Camera *cam;
	Shoot(GameObject *bulletprefab_, vector<GameObject> *gameobjects_, Camera *cam, vector<GameObject *> *enemies_,bool isOffset_ = false, float shootInterval_ = 1.0f, float flySpeed_ = 10, float gravityRate_ = 1.0);
	~Shoot();
	void update(GLFWwindow* window, GameObject *obj);
};

