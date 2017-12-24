#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#define PI 3.1415926f
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "Model.h"

using namespace std;
class Object
{
protected:
	Object(glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f), Object *child_ = NULL, Object *parent_ = NULL);
	~Object();
public:
	bool needRender;
	glm::vec3 position;
	glm::vec3 rotation;
	void setPosition(glm::vec3 &newPosition);
	void setRotation(glm::vec3 &newRotation);
public:
	Object *child;
	Object *parent;
	glm::vec3 localRotation;
	glm::vec3 localPosition;
	glm::mat4 getTR();
	void updatePosRotFromParent();
	void setParent(Object &par);
};

class GameObject: public Object
{
public:
	GameObject(string modelPath_, glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f),
		 bool isActive_ = true);
	~GameObject();
	void setScale(glm::vec3 &newScale);
public:
	Model model;
	glm::vec3 scale;
	bool isActive;
	string modelPath;
};

class FlyingGameObject :GameObject 
{
public:
	FlyingGameObject(string modelPath_, float speed_ = 0, glm::vec3 velocity_ = glm::vec3(1, 0, 0));
	float speed;
	glm::vec3 velocity;
};

#endif