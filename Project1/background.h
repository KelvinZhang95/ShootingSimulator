#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "shader.h"
#include "Camera.h"

float skyboxVertices[] = {
	// positions          
	-1000.0f,  1000.0f, -1000.0f,
	-1000.0f, -1000.0f, -1000.0f,
	1000.0f, -1000.0f, -1000.0f,
	1000.0f, -1000.0f, -1000.0f,
	1000.0f,  1000.0f, -1000.0f,
	-1000.0f,  1000.0f, -1000.0f,

	-1000.0f, -1000.0f,  1000.0f,
	-1000.0f, -1000.0f, -1000.0f,
	-1000.0f,  1000.0f, -1000.0f,
	-1000.0f,  1000.0f, -1000.0f,
	-1000.0f,  1000.0f,  1000.0f,
	-1000.0f, -1000.0f,  1000.0f,

	1000.0f, -1000.0f, -1000.0f,
	1000.0f, -1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f, -1000.0f,
	1000.0f, -1000.0f, -1000.0f,

	-1000.0f, -1000.0f,  1000.0f,
	-1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f, -1000.0f,  1000.0f,
	-1000.0f, -1000.0f,  1000.0f,

	-1000.0f,  1000.0f, -1000.0f,
	1000.0f,  1000.0f, -1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	-1000.0f,  1000.0f,  1000.0f,
	-1000.0f,  1000.0f, -1000.0f,

	-1000.0f, -1000.0f, -1000.0f,
	-1000.0f, -1000.0f,  1000.0f,
	1000.0f, -1000.0f, -1000.0f,
	1000.0f, -1000.0f, -1000.0f,
	-1000.0f, -1000.0f,  1000.0f,
	1000.0f, -1000.0f,  1000.0f
};

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

class BackGround
{
public:
	BackGround():skyboxShader("shader/skybox.vs", "shader/skybox.fs")
	{
		//Shader skyboxShader("shader/skybox.vs", "shader/skybox.fs");
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		std::vector<std::string> faces
		{
			"resources/textures/skybox/right.jpg",
			"resources/textures/skybox/left.jpg",
			"resources/textures/skybox/top.jpg",
			"resources/textures/skybox/bottom.jpg",
			"resources/textures/skybox/back.jpg",
			"resources/textures/skybox/front.jpg"
			//"resources/textures/skybox/normandy_rt.tga",
			//"resources/textures/skybox/normandy_lf.tga",
			//"resources/textures/skybox/normandy_tp.tga",
			//"resources/textures/skybox/normandy_bk.tga",
			//"resources/textures/skybox/normandy_bk.tga",
			//"resources/textures/skybox/normandy_bk.tga"
		};
		cubemapTexture = loadCubemap(faces);

		skyboxShader.use();
		skyboxShader.setInt("skybox", 0);
	}

	void render(Camera& camera, glm::mat4& projection)
	{
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
	}

	void clean()
	{
		glDeleteVertexArrays(1, &skyboxVAO);
		glDeleteBuffers(1, &skyboxVAO);
	}

private:
	Shader skyboxShader;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
};