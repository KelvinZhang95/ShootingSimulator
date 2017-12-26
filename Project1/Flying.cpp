#include "Flying.h"


Flying::Flying(glm::vec3 &velocity_, float speed_):velocity(glm::normalize(velocity_)),speed(speed_)
{

}


Flying::~Flying()
{
}
void Flying::update(GLFWwindow* window, GameObject *obj) {
	if (obj->parent == NULL) {
		cout << "velocity is " << velocity.x << " " << velocity.y << " " << velocity.z << " " << endl;
		obj->position = obj->position + velocity * speed;
	}
}
