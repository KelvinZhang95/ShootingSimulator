#include "Flying.h"


Flying::Flying(glm::vec3 velocity_, float speed_):velocity(velocity_),speed(speed_)
{

}


Flying::~Flying()
{
}
void Flying::update(GameObject *obj) {
	if (obj->parent == NULL) {
		obj->position = obj->position + velocity * speed;
	}
}
