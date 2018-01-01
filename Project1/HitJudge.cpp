#include "HitJudge.h"

HitJudge::HitJudge(vector<GameObject> *gameobjects_, vector<int> *enemies_, vector<Particles> *partList_,int *score_):enemies(enemies_),score(score_), gameobjects(gameobjects_), partList(partList_)
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
			//Particles particles;
			//particles.endTime = glfwGetTime() + 0.3;
			//particles.setSpread(2.5f);
			//particles.setSize(0.05f);
			//particles.setGravity(glm::vec3(0, 0, 0));
			//particles.setLife(0.20f);
			//particles.setBeamspeed(5.0f);
			//particles.setRgb(glm::vec4(255, 0, 0, 255 * 3));
			//particles.setOriginalDirection(glm::vec3(0, 0, 1));
			////particles.localPosition = glm::vec3(0, 2, 1.3);
			//particles.setIfDisplay(true);
			//particles.position = obj->position;
			//(*partList).push_back(particles);
			//(*partList).back().setParent((*gameobjects)[(*enemies)[i]]);
			(*score) += 10;
			obj->isActive = false;
			break;
		}
	}
}