#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <iostream>
class Collider
{
public:
	Collider(glm::vec3 min_ = glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3 max_ = glm::vec3(0.1f, 0.1f, 0.1f));
	~Collider();
	void updateMinMax(const glm::vec3 &point);
	void updateOnScalePos(glm::vec3 scale_, glm::vec3 position_);
	bool containPoint(const glm::vec3 point) const;
public:
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 box_min;//in world
	glm::vec3 box_max;//in world
};
#endif
