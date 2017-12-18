#include "Gameobject.h"

Gameobject::Gameobject(string modelPath_, glm::vec3 position_, glm::vec3 scale_, glm::vec3 rotation_, bool isActive_): isActive(isActive_),model(modelPath_)
{
	position = position_;
	rotation = rotation_;
	scale = scale_;
	modelPath = modelPath_;//just store
	model.collider.updateOnScalePos(scale, position);
 }


Gameobject::~Gameobject()
{
}

void Gameobject::setPosition(glm::vec3 & newPosition)
{
	position = newPosition;
}

void Gameobject::setRotation(glm::vec3 & newRotation)
{
	rotation = newRotation;
}

void Gameobject::setScale(glm::vec3 & newScale)
{
	scale = newScale;
}
