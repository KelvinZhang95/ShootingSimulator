#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

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
};

class Gameobject: public Object
{
public:
	Gameobject(string modelPath_, glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f),
		 bool isActive_ = true);
	~Gameobject();
	void setScale(glm::vec3 &newScale);
public:
	Model model;
	glm::vec3 scale;
	bool isActive;
	string modelPath;
};

#endif