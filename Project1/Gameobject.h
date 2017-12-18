#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

#include "Model.h"

using namespace std;

class Gameobject
{
public:
	Gameobject(string modelPath_, glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f),
		 bool isActive_ = true);
	~Gameobject();
	void setPosition(glm::vec3 &newPosition);
	void setRotation(glm::vec3 &newRotation);
	void setScale(glm::vec3 &newScale);
public:
	Model model;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	bool isActive;
	string modelPath;

};

#endif