#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#define PI 3.1415926f
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>


#include "Object.h"
#include "Model.h"
using namespace std;

//class MonoBehaviour;
class Object;
class GameObject: public Object
{
public:
	GameObject(string modelPath_, glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f),
		bool isActive_ = true, bool isCollider_ = false);
	~GameObject();
	void setScale(glm::vec3 &newScale);
public:
	Model model;
	glm::vec3 scale;
	bool isActive;
	bool isCollider;
	bool isShadow;
	string modelPath;

};

#endif