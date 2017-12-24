#include "Gameobject.h"

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

glm::mat4 Object::getTR()
{
	glm::mat4 result;
	if (parent == NULL) {
		glm::mat4 rotMat = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
		glm::mat4 tranMat = glm::translate(glm::mat4(), position);
		result = tranMat * rotMat;
	}
	else {
		glm::mat4 rotMat = glm::eulerAngleXYZ(localRotation.x, localRotation.y, localRotation.z);
		glm::mat4 tranMat = glm::translate(glm::mat4(), localPosition);
		result = parent->getTR() * tranMat * rotMat;
	}
	return result;
}

void Object::updatePosRotFromParent()
{
	glm::mat4 TR = getTR();
	//-------------- 根据TR 计算 position------------
	position.x = TR[3][0];
	position.y = TR[3][1];
	position.z = TR[3][2];
	//-------------- 根据TR 计算 rotation------------
	float x1, x2, y1, y2, z1, z2;
	if (TR[2][0] != 1 && TR[2][0] != -1) {
		y1 = -asin(TR[2][0]);
		y2 = PI - y1;
		x1 = atan2(TR[2][1] / cos(y1), TR[2][2] / cos(y1));
		x2 = atan2(TR[2][1] / cos(y2), TR[2][2] / cos(y2));
		z1 = atan2(TR[1][0] / cos(y1), TR[0][0] / cos(y1));
		z2 = atan2(TR[1][0] / cos(y2), TR[0][0] / cos(y2));
	}
	else {
		z1 = 0;
		z2 = z1;
		if (TR[2][0] == -1) {
			y1 = PI / 2;
			y2 = y1;
			x1 = atan2(TR[0][1], TR[0][2]);
			x2 = x1;
		}
		else {
			y1 = -PI / 2;
			y2 = y1;
			x1 = atan2(-TR[0][1], -TR[0][2]);
			x2 = -x1;
		}
	}
	rotation.y = -y1;
	rotation.x = -x1;
	rotation.z = -z1;
}

void Object::setParent(Object &par)
{
	par.child = this;
	parent = &par;
}

FlyingGameObject::FlyingGameObject(string modelPath_, float speed_ = 0, glm::vec3 velocity_ = glm::vec3(1, 0, 0)) :GameObject(modelPath_)
{

}
