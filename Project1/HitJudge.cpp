#include "HitJudge.h"

HitJudge::HitJudge(vector<GameObject *> *enemies_):enemies(enemies_)
{
}


HitJudge::~HitJudge()
{
}
void HitJudge::update(GLFWwindow* window, GameObject * obj) {
	assert(enemies);
	for (int i = 0; i < enemies->size(); i++) {
		//Happy new year!2018
		if ((*enemies)[i]->model.collider.containPoint(obj->position)) {
			cout << "hit!";
			break;
		}
	}
}