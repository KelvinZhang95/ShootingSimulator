#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "shader.h"

class GuiRenderHelper
{
public:
	GuiRenderHelper()
		:shader("shader/gui.vs", "shader/gui.fs"),
		width(0), height(0)
	{}

	~GuiRenderHelper()
	{
		glDeleteVertexArrays(1, &VAO1);
		glDeleteVertexArrays(1, &VAO2);
		glDeleteBuffers(1, &VBO1);
		glDeleteBuffers(1, &VBO2);
	}

	GuiRenderHelper(int w, int h)
		:shader("shader/gui.vs", "shader/gui.fs"),
		width(w), height(h)
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
		shader.use();
		shader.setMat4("projection", projection);

		int ox = width / 2;
		int oy = height / 2;
		int r = std::min(ox, oy);
		int a = std::max(ox, oy);
		int pixel = 1;
		int rr = 0;

		shader.use();
		shader.setFloat("ox", ox);
		shader.setFloat("oy", oy);


		float vertices1[] = {
			// left 
			ox - r,			oy + pixel,		0.0f,		0,
			ox - r,			oy - 0,			0.0f,		0,
			ox - rr,		oy + pixel,		0.0f,		0,
			ox - rr,		oy + pixel,		0.0f,		0,
			ox - r,			oy - 0,			0.0f,		0,
			ox - rr,		oy - 0,			0.0f,		0,
			// right 
			ox + rr,		oy + pixel,		0.0f,		0,
			ox + rr,		oy - 0,			0.0f,		0,
			ox + r,			oy + pixel,		0.0f,		0,
			ox + r,			oy + pixel,		0.0f,		0,
			ox + rr,		oy - 0,			0.0f,		0,
			ox + r,			oy - 0,			0.0f,		0,
			// top
			ox + pixel,		oy + rr,		0.0f,		0,
			ox + pixel,		oy + r,			0.0f,		0,
			ox - 0,			oy + rr,		0.0f,		0,
			ox + pixel,		oy + r,			0.0f,		0,
			ox - 0,			oy + r,			0.0f,		0,
			ox - 0,			oy + rr,		0.0f,		0,
			// button
			ox + pixel,		oy - rr,		0.0f,		0,
			ox - 0,			oy - rr,		0.0f,		0,
			ox + pixel,		oy - r,			0.0f,		0,
			ox + pixel,		oy - r,			0.0f,		0,
			ox - 0,			oy - rr,		0.0f,		0,
			ox - 0,			oy - r,			0.0f,		0,
			//
			ox + a,			oy + a,			0.0,		1,
			ox - a,			oy + a,			0.0,		1,
			ox - a,			oy - a,			0.0,		1,
			ox + a,			oy + a,			0.0,		1,
			ox - a,			oy - a,			0.0,		1,
			ox + a,			oy - a,			0.0,		1,
		};
		
		r /= 7;
		rr = 25;
		float vertices2[] = {
			// left 
			ox - r,			oy + pixel,		0.0f,		0,
			ox - r,			oy - 0,			0.0f,		0,
			ox - rr,		oy + pixel,		0.0f,		0,
			ox - rr,		oy + pixel,		0.0f,		0,
			ox - r,			oy - 0,			0.0f,		0,
			ox - rr,		oy - 0,			0.0f,		0,
			// right 
			ox + rr,		oy + pixel,		0.0f,		0,
			ox + rr,		oy - 0,			0.0f,		0,
			ox + r,			oy + pixel,		0.0f,		0,
			ox + r,			oy + pixel,		0.0f,		0,
			ox + rr,		oy - 0,			0.0f,		0,
			ox + r,			oy - 0,			0.0f,		0,
			// top
			ox + pixel,		oy + rr,		0.0f,		0,
			ox + pixel,		oy + r,			0.0f,		0,
			ox - 0,			oy + rr,		0.0f,		0,
			ox + pixel,		oy + r,			0.0f,		0,
			ox - 0,			oy + r,			0.0f,		0,
			ox - 0,			oy + rr,		0.0f,		0,
			// button
			ox + pixel,		oy - rr,		0.0f,		0,
			ox - 0,			oy - rr,		0.0f,		0,
			ox + pixel,		oy - r,			0.0f,		0,
			ox + pixel,		oy - r,			0.0f,		0,
			ox - 0,			oy - rr,		0.0f,		0,
			ox - 0,			oy - r,			0.0f,		0,
			//
			ox + a,			oy + a,			0.0,		1,
			ox - a,			oy + a,			0.0,		1,
			ox - a,			oy - a,			0.0,		1,
			ox + a,			oy + a,			0.0,		1,
			ox - a,			oy - a,			0.0,		1,
			ox + a,			oy - a,			0.0,		1,
		};

		glGenVertexArrays(1, &VAO2);
		glGenBuffers(1, &VBO2);
		glBindVertexArray(VAO2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glGenVertexArrays(1, &VAO1);
		glGenBuffers(1, &VBO1);
		glBindVertexArray(VAO1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Render(float mask)
	{
		shader.use();
		shader.setFloat("mask", mask);

		//glBindVertexArray(VAO1);
		if (mask)
		{
			glBindVertexArray(VAO1);
			glDrawArrays(GL_TRIANGLES, 0, 30);
		}
		else
		{
			glBindVertexArray(VAO2);
			glDrawArrays(GL_TRIANGLES, 0, 24);
		}
	}

private:
	Shader shader;
	GLuint VAO1, VBO1, VAO2, VBO2;
	float width, height;
};