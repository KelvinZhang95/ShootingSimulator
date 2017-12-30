#ifndef MONOBEHAVIOUR_H
#define MONOBEHAVIOUR_H
//#include "Object.h"
//#include "GameObject.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
class GameObject;
//class Object;
class MonoBehaviour
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();
public:
	virtual void update(GLFWwindow* window, GameObject *obj) {
		//if (obj->parent == NULL) {

		//}
	};
};
#endif // !MONOBEHAVIOUR
