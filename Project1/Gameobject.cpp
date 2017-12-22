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



void Gameobject::setScale(glm::vec3 & newScale)
{
	scale = newScale;
}

Object::Object(glm::vec3 position_, glm::vec3 rotation_, Object *child_, Object *parent_):localPosition(glm::vec3(0.0f, 0.0f, 0.0f)), localRotation(glm::vec3(0.0f, 0.0f, 0.0f))
{
	position = position_;
	rotation = rotation_;
	needRender = true;
}

Object::~Object()
{
}
void Object::setPosition(glm::vec3 & newPosition)
{
	position = newPosition;
}

void Object::setRotation(glm::vec3 & newRotation)
{
	rotation = newRotation;
}