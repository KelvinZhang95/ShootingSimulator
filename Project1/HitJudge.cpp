#include "HitJudge.h"

HitJudge::HitJudge(vector<GameObject> *gameobjects_, vector<int> *enemies_, int *score_):enemies(enemies_),score(score_), gameobjects(gameobjects_)
{
}


HitJudge::~HitJudge()
{
}
void HitJudge::update(GLFWwindow* window, GameObject * obj) {
	assert(enemies);
	for (int i = 0; i < enemies->size(); i++) {
		//Happy new year!2018
		if ((*gameobjects)[(*enemies)[i]].model.collider.containPoint(obj->position)) {
			cout << "hit!1" << endl;
			cout << "hit!22" << endl;
			cout << "hit!333" << endl << endl;
			(*score) += 10;
			obj->isActive = false;
			break;
		}
	}
}