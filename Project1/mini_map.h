#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "shader.h"

class MiniMap
{
public:
	MiniMap()
		:shader("shader/mini_map.vs", "shader/mini_map.fs"),
		width(0), height(0)
	{}

	~MiniMap()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	MiniMap(int w, int h)
		:shader("shader/mini_map.vs", "shader/mini_map.fs"),
		width(w / 4), height(h / 4)
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(w), 0.0f, static_cast<GLfloat>(h));
		shader.use();
		shader.setMat4("projection", projection);

		float vertices[] = {
			width,	height,		-1.0,	0.0f,1.0f,
			0,		height,		-1.0,	1.0f,1.0f,
			0,		0,			-1.0,	1.0f,0.0f,

			0,		0,			-1.0,	1.0f,0.0f,
			width,	0,			-1.0,	0.0f,0.0f,
			width,	height,		-1.0,	0.0f,1.0f
		};
		for (int i = 0; i < 6; i++)
		{
			vertices[i * 5] += width * 3;
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		diffuseMap = loadTexture("resources/textures/map.png");
		shader.use();
		shader.setInt("material", 0);
		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		float tmp1[16] = {
			1,0,0,0,
			0,0,1,0,
			0,-1,0,0,
			0,0,-100,1
		};
		memcpy(glm::value_ptr(view), tmp1, sizeof(tmp1));

		float tmp2[16] = {
			1.60948,0,0,0,
			0,2.41421,0,0,
			0,0,-1.0002,-1,
			0,0,-0.20002,0
		};
		memcpy(glm::value_ptr(view), tmp2, sizeof(tmp2));
	}

	glm::vec2 transform(glm::vec3& p)
	{
		glm::vec2 r;

		r[0] = p[0] / 12.0086 * 116 / 4 + 1050;
		r[1] = p[2] / 12.8824 * 124.4 / 4 * -1 + 100;
		r[0] = std::min((float)1200.0, std::max((float)900.0, r[0]));
		r[1] = std::min((float)200.0, std::max((float)0.0, r[1]));
		//std::cout << r[0] << " " << r[1] << "\n";

		//glm::vec4 tmp(p[0], 0, p[2], 1);
		//tmp = projection * view * tmp;
		//float x = tmp[0] / tmp[3];
		//float y = tmp[1] / tmp[3];
		////std::cout << x << " " << y << "\n";
		//r[0] = 1050 + (x) * 150;
		//r[1] = 100 - (y) * 100;
		//r[0] = std::min((float)1200.0, std::max((float)900.0, r[0]));
		//r[1] = std::min((float)200.0, std::max((float)0.0, r[1]));

		return r;
	}

	void Render(glm::vec3& p, std::vector<glm::vec3>& vp, glm::vec2& dirction)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		shader.use();
		shader.setVec2("pos1", transform(p));     
		shader.setVec2("n1", dirction);
		shader.setFloat("size2", vp.size());
		for (int i = 0; i < vp.size(); i++)
		{
			char tmp[20];
			sprintf(tmp, "pos2[%d]", i);
			shader.setVec2(tmp, transform(vp[i]));
		}
		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}


	// utility function for loading a 2D texture from file
	// ---------------------------------------------------
	unsigned int loadTexture(char const * path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
private:
	Shader shader;
	GLuint VAO, VBO;
	float width, height;

	unsigned int diffuseMap;
	glm::mat4 projection;
	glm::mat4 view;
};