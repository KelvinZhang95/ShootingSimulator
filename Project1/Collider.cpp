#include "Collider.h"

Collider::Collider(glm::vec3 min_,glm::vec3 max_)
{
	min = min_;
	max = max_;
}


Collider::~Collider()
{
}

void Collider::updateMinMax(const glm::vec3 &point)
{
	if (point.x < min.x)
		min.x = point.x;

	// Lowest point.  
	if (point.y < min.y)
		min.y = point.y;

	// Farthest point.  
	if (point.z < min.z)
		min.z = point.z;

	// Rightmost point.  
	if (point.x > max.x)
		max.x = point.x;

	// Highest point.  
	if (point.y > max.y)
		max.y = point.y;

	// Nearest point.  
	if (point.z > max.z)
		max.z = point.z;
}

void Collider::updateOnScalePos(glm::vec3 scale_, glm::vec3 position_)
{
	box_min = glm::vec3(min.x * scale_.x, min.y * scale_.y, min.z * scale_.z);
	box_max = glm::vec3(max.x * scale_.x, max.y * scale_.y, max.z * scale_.z);
	box_min = box_min + position_;
	box_max = box_max + position_;
}

bool Collider::containPoint(const glm::vec3 point) const
{
	if (point.x < box_min.x) return false;
	if (point.y < box_min.y) return false;
	if (point.z < box_min.z) return false;
	if (point.x > box_max.x) return false;
	if (point.y > box_max.y) return false;
	if (point.z > box_max.z) return false;
	return true;
}
