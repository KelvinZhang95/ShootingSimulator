#include "GameObject.h"

GameObject::GameObject(string modelPath_, glm::vec3 position_, glm::vec3 scale_, glm::vec3 rotation_, bool isActive_): isActive(isActive_),model(modelPath_)
{
	position = position_;
	rotation = rotation_;
	scale = scale_;
	modelPath = modelPath_;//just store
	model.collider.updateOnScalePos(scale, position);

 }


GameObject::~GameObject()
{
}



void GameObject::setScale(glm::vec3 & newScale)
{
	scale = newScale;
}



