#include "main.h"

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);//anti-aliasing

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PlayGround", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	//Shader ourShader("shader/shader.vs", "shader/shader.fs");
	Shader lightingShader("shader/lightshader.vs", "shader/lightshader.fs");
	Shader simpleDepthShader("shader/shadow_mapping_depth.vs", "shader/shadow_mapping_depth.fs");

	// Configure depth map FBO
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// - Create depth texture
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	lightingShader.use();
	lightingShader.setInt("diffuseTexture", 0);
	lightingShader.setInt("shadowMap", 1);

	//gameobjects.push_back(camera);
	//Particles particles;
	BackGround background;
	TextRenderHelper text_helper(SCR_WIDTH, SCR_HEIGHT);
	GuiRenderHelper gui_helper(SCR_WIDTH, SCR_HEIGHT);
	MiniMap mini_map(SCR_WIDTH, SCR_HEIGHT);
	//MiniMapDot mini_map_dot(SCR_WIDTH, SCR_HEIGHT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	text_helper.Render(0,"Loading...", SCR_WIDTH - 225,25);
	text_helper.Render(0, "Dragon Hunter", SCR_WIDTH / 2 - 110, SCR_HEIGHT / 2 - 25);
	glfwSwapBuffers(window);

	gameobjects.reserve(1000);
	creatGameobject();
	gameobjects[1].setParent(camera);
	gameobjects[2].setParent(camera);
	//glm::vec3 lightPos(1.0f, 6.0f, 1.0f);

	srand((unsigned)time(NULL));
	// render loop
	int startTime = (int)glfwGetTime();
	int remainTime = 60;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		remainTime = 60 - (int)currentFrame + startTime;
		if (remainTime < 0)
			remainTime = 0;
		// input
		processInput(window);

		updateLogic(window);

		// 1. Render depth of scene to texture (from ligth's perspective)
		// - Get light projection/view matrix.
		//glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		glm::mat4 model;
		float near_plane = 0.1f, far_plane = 50.5f;
		
		{
			lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
			lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;
			// - now render scene from light's point of view
			simpleDepthShader.use();
			simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);


			for (int i = 0; i < gameobjects.size(); i++) {
				if (gameobjects[i].isActive) {
					model = glm::mat4();
					glm::mat4 trans = glm::translate(glm::mat4(), gameobjects[i].position);
					glm::mat4 rot = glm::eulerAngleXYZ(gameobjects[i].rotation.x, gameobjects[i].rotation.y, gameobjects[i].rotation.z);
					glm::mat4 sca = glm::scale(model, gameobjects[i].scale);
					model = trans * rot * sca;
					simpleDepthShader.setMat4("model", model);
					gameobjects[i].model.Draw(simpleDepthShader, false);
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		{		// render as normal
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// be sure to activate shader when setting uniforms/drawing objects
			lightingShader.use();
			lightingShader.setVec3("viewPos", camera.position);
			lightingShader.setVec3("lightPos", lightPos);

			lightingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glActiveTexture(GL_TEXTURE0);

			glm::mat4 projection;
			if (isPerspective) {
				projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
			}
			else {
				projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
			}

			glm::mat4 view;
			view = glm::lookAt(camera.position, camera.position + camera.Front, camera.Up);

			lightingShader.setMat4("view", view);
			lightingShader.setMat4("projection", projection);

			for (int i = 0; i < gameobjects.size(); i++) {
				if (gameobjects[i].isActive) {
					model = glm::mat4();
					glm::mat4 trans = glm::translate(glm::mat4(), gameobjects[i].position);
					glm::mat4 rot = glm::eulerAngleXYZ(gameobjects[i].rotation.x, gameobjects[i].rotation.y, gameobjects[i].rotation.z);
					glm::mat4 sca = glm::scale(model, gameobjects[i].scale);
					model = trans * rot * sca;
					lightingShader.setMat4("model", model);
					gameobjects[i].model.Draw(lightingShader, true);
				}
			}
			if (!scope)
			{
				std::vector<glm::vec3> tmp;
				for (int i = 0; i < enemies.size(); i++)
				{
					tmp.push_back(gameobjects[enemies[i]].position);
				}
				glm::vec2 cameraXZ(camera.Front.x, -camera.Front.z);
				glm::vec2 direction = glm::normalize(cameraXZ);
				mini_map.Render(camera.position, tmp, direction);
			}
			//particles.setPosition(gameobjects[1].position);
			//particles.setPosition(glm::vec3(0, 0, -2));

			for (int i = 0; i < ParticlesList.size(); i++) {
				ParticlesList[i].render(camera, projection);
			}
			//particles.render(camera, projection);

			background.render(camera, projection);
			std::stringstream ss;
			if (remainTime > 0) {
				ss << score;
				finalScore = score;
			}
			else {
				ss << finalScore;
			}
			ss >> danmu;
			danmu = "SCORE: " + danmu;
			text_helper.Render(scope, danmu, 25, 25);
			
			std::stringstream ss1;
			ss1 << remainTime;
			string timestr;
			ss1 >> timestr;
			timestr = "Time: " + timestr;
			text_helper.Render(scope, timestr, SCR_WIDTH / 2 - 110, SCR_HEIGHT - 55);

			gui_helper.Render(scope);

		}
		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	for (int i = 0; i < ParticlesList.size(); i++) {
		ParticlesList[i].clean();
	}
	//particles.clean();
	background.clean();
	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	bool hasMoved = false;
	glm::vec3 tryPosition;
	float cameraSpeed = !isSpeed ? speed * deltaTime : speed * 4 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		hasMoved = true;
		tryPosition = camera.position + cameraSpeed * camera.Front;
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		hasMoved = true;
		tryPosition = camera.position - cameraSpeed * camera.Front;

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		hasMoved = true;
		tryPosition = camera.position - glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		hasMoved = true;
		tryPosition = camera.position + glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
	}
	//if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
	//	hasMoved = true;
	//	tryPosition = camera.position + glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
	//}
	

	//if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
	//	gameobjects[0].position = gameobjects[0].position + glm::vec3(0.1, 0, 0);
	//}
	//if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
	//	gameobjects[0].setRotation(gameobjects[0].rotation + glm::vec3(1 * PI / 180, 0, 0));
	//}
	//if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
	//	gameobjects[0].setRotation(gameobjects[0].rotation + glm::vec3(0, 1 * PI / 180, 0));

	//}
	if (hasMoved) {
		//for (int i = 0; i < gameobjects.size(); i++) {
		//if (gameobjects[i].isActive) {
		//if (gameobjects[i].model.collider.containPoint(tryPosition)) {
		//cout << "in collider!" << endl;
		//return;
		//}
		//}
		//}
		//camera.position = tryPosition;
		static float prev_d = 0;

		glm::vec3 try1, try2;
		try1 = try2 = tryPosition;
		float d1 = aabb_tree_ground.Query(try1);
		float d2 = aabb_tree_others.Query(try2);
		//std::cout << "distance to ground: " << d1 << "\t\tdistance to others: " << d2 << "\n";
		if (positionJudge(tryPosition)) {
			if (d2 > 2000 || d2 > prev_d)
			{

				camera.position = tryPosition;
				prev_d = d2;
			}
		}
		
		
	}

	//cout << "camera " << camera.position.x << ", " << camera.position.y<< ", " << camera.position.z <<endl;
}
void updateChildPosRot()
{
	camera.updatePosRotFromParent();
	for (int i = 0; i < ParticlesList.size(); i++) {
		ParticlesList[i].updatePosRotFromParent();
	}
	for (int i = 0; i < gameobjects.size(); i++) {
		gameobjects[i].updatePosRotFromParent();
	}
}
bool positionJudge(glm::vec3 &position)
{
	//if (sqrt(position.x * position.x + position.z * position.z) > circlexy)
	//	return false;
	if (position.y > maxy)
		position.y = maxy;
	else if (position.y < miny) {
		position.y = miny;
	}
	return true;
}
void updateKinematics()
{

}
//update logic 
void updateLogic(GLFWwindow *window) {
	adjust();
	updateScript(window);
	updateKinematics();
	updateChildPosRot();
}

void updateScript(GLFWwindow *window)
{
	for (int i = 0; i < gameobjects.size(); i++) {
		if (gameobjects[i].isActive) {
			for (int j = 0; j < gameobjects[i].scripts.size(); j++) {
				gameobjects[i].scripts[j]->update(window, &gameobjects[i]);
			}
		}
	}
}





// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	camera.Yaw += xoffset;
	camera.Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (camera.Pitch > 89.0f)
		camera.Pitch = 89.0f;
	if (camera.Pitch < -89.0f)
		camera.Pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
	front.y = sin(glm::radians(camera.Pitch));
	front.z = sin(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
	camera.Front = glm::normalize(front);
	camera.updateRotation();
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (camera.Zoom >= 1.0f && camera.Zoom <= 45.0f)
		camera.Zoom -= yoffset;
	if (camera.Zoom <= 1.0f)
		camera.Zoom = 1.0f;
	if (camera.Zoom >= 45.0f)
		camera.Zoom = 45.0f;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) 
		isSpeed = true;
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) 
		isSpeed = false;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		isPerspective = !isPerspective;
	//if (key == GLFW_KEY_C && action == GLFW_PRESS) {
	//	gameobjects[0].position = camera.position;
	//}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		MonoBehaviour *fly = new Flying(glm::vec3(-1, 0, 0), 0.1);
		gameobjects[3].scripts.push_back(fly);
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		adjust();
	}
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		switchWeapon();
	}
	if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
		if (cursorMode) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cursorMode = false;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cursorMode = true;
		}
	}
		//creatGameobject();
}
void mouse_button_callback(GLFWwindow * window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (nowWeapon == 2) {
			scope = !scope;
		}
	}
	camera.Zoom = 45.0f - (int)scope * 33.0f;

}
void adjust() {
	float x, y, z, rx, ry, rz;
	ifstream in;
	in.open("in.txt");
	in >> x >> y >> z >> rx >> ry >> rz;
	rx = rx / 180 * PI;
	ry = ry / 180 * PI;
	rz = rz / 180 * PI;
	//ParticlesList[0].localPosition = glm::vec3(x, y, z);
	//ParticlesList[0].localRotation = glm::vec3(rx, ry, rz);
	in.close();
}
void switchWeapon()
{
	scope = false;
	gameobjects[nowWeapon].isActive = false;
	gameobjects[3 - nowWeapon].isActive = true;
	nowWeapon = 3 - nowWeapon;
}
void creatGameobject() {
	string path1 = "resources/model/dao/dao.fbx";
	string path2 = "resources/model/M4CQB/M4CQB.FBX";
	string path3 = "resources/model/SVD/svd.obj";
	string path4 = "resources/model/762/762.obj";
	string path5 = "resources/model/dragon/dragon.obj";
	string path6 = "resources/model/nanosuit/nanosuit.obj";

	GameObject dao(path1, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	GameObject M4(path2, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.006f, 0.006f, 0.006f), glm::vec3(0, 0, 0));
	GameObject RSASS(path3, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.006f, 0.006f, 0.006f), glm::vec3(0, 0, 0));
	GameObject bullet762(path4, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0001f, 0.0001f, 0.0001f));
	GameObject dragon(path5, glm::vec3(4.0218, 10.5945, -0.371), glm::vec3(0.0020f, 0.0020f, 0.0020f));
	//GameObject man(path6, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0, 0, 0));

	prefabs.push_back(dao);
	prefabs.push_back(M4);
	prefabs.push_back(RSASS);
	prefabs.push_back(bullet762);
	prefabs.push_back(dragon);
	//prefabs.push_back(man);


	gameobjects.push_back(prefabs[0]);//dao

	gameobjects.push_back(prefabs[1]);//m4
	gameobjects[1].localPosition = glm::vec3(0.85, -0.21, 0.15);
	gameobjects[1].localRotation = glm::vec3(273.0 / 180.0 * PI, -1.2 / 180.0 * PI, 273.0 / 180.0 * PI);
	MonoBehaviour *shoot = new Shoot(&prefabs[3], &gameobjects, &camera, &enemies, &score, false, 0.25f, 100,0.1);
	
	gameobjects[1].scripts.push_back(shoot);
	gameobjects[1].isActive = true;


	gameobjects.push_back(prefabs[2]);//sniper
	gameobjects[2].localPosition = glm::vec3(0.42, -0.18, 0.05);
	gameobjects[2].localRotation = glm::vec3(-1 / 180.0 * PI, 1 / 180.0 * PI, -4 / 180.0 * PI);
	MonoBehaviour *shoot1 = new Shoot(&prefabs[3], &gameobjects, &camera, &enemies, &score, true, 1.1f, 200,0.3);
	gameobjects[2].scripts.push_back(shoot1);
	gameobjects[2].isActive = false;


	gameobjects.push_back(prefabs[3]);//bullet

	//gameobjects.push_back(prefabs[4]);//dragon
	//enemies.push_back(4);
	//MonoBehaviour *enemyAction = new EnemyAction();
	//gameobjects[4].scripts.push_back(enemyAction);
	Particles particles1;
	particles1.setSpread(1.55f);
	particles1.setSize(0.3f);
	particles1.setGravity(glm::vec3(0, 0, 0));
	particles1.setLife(1.3f);
	particles1.setBeamspeed(15.0f);
	particles1.setRgb(glm::vec4(255, 69, 0, 255 *3));
	particles1.setOriginalDirection(glm::vec3(0, 0, 1));
	particles1.localPosition = glm::vec3(0, 2, 1.3);
	particles1.setIfDisplay(true);
	for (int i = 4; i < 4 + dragonNum; i++) {
		ParticlesList.push_back(particles1);
		prefabs[4].setPosition(glm::vec3((rand() % 60 - 30), (rand() % 20 - 5), (rand() % 60 - 30)));
		cout << "prefabs is " << prefabs[4].position.x << " " <<prefabs[4].position.y << " " << prefabs[4].position.z << endl;
		gameobjects.push_back(prefabs[4]);//dragon
		enemies.push_back(i);
		MonoBehaviour *enemyAction = new EnemyAction(&ParticlesList, i - 4, (float)(rand() % 2 + 1) / 10.0f);
		gameobjects[i].scripts.push_back(enemyAction);
		ParticlesList[i - 4].setParent(gameobjects[i]);
	}

	//Particles particles;
	//particles.setSpread(0.225f);
	//particles.setSize(0.003f);
	//particles.setGravity(glm::vec3(0, 0, 0));
	//particles.setLife(0.05f);
	//particles.setBeamspeed(3.0f);
	//ParticlesList.push_back(particles);
	//ParticlesList[0].setParent(gameobjects[1]);
	//ParticlesList[0].localPosition = glm::vec3(0, 0.42, 0);




	//build AABB Tree for dao
	std::vector<int> used;
	used = std::vector<int>(gameobjects[0].model.meshes.size(), 1);
	used[3] = used[4] = used[20] = used[27] = used[16] = used[17] = 0;
	aabb_tree_others.Build(gameobjects[0].model.meshes, used);
	used = std::vector<int>(gameobjects[0].model.meshes.size(), 0);
	used[16] = used[17] = 1;
	aabb_tree_ground.Build(gameobjects[0].model.meshes, used);
}
