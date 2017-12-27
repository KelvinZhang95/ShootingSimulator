#ifndef OBJECT_H
#define OBJECT_H
//class MonoBehaviour;
#define PI 3.1415926f
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include "MonoBehaviour.h"
#include<iostream>
#include<vector>
class MonoBehaviour;
//class GameObject;
class Object
{
public:
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
	void rotateInWorld(glm::vec3 rot);//可以指定绕着世界坐标系转某个欧拉角
	void setParent(Object &par);
	std::vector<class MonoBehaviour*> scripts;
private:
	void updatePosRotFromTR(glm::mat4 &TR);

};
#endif