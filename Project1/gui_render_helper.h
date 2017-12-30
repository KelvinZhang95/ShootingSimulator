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
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	GuiRenderHelper(int w, int h)
		:shader("shader/gui.vs", "shader/gui.fs"),
		width(w), height(h)
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
		shader.use();
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		shader.setMat4("projection", projection);

		float vertices[] = {
			width / 2 - 400, height / 2 + 1, 0.0f, 
			width / 2 - 400, height / 2 - 0, 0.0f, 
			width / 2 - 40, height / 2 + 1, 0.0f,  

			width / 2 + 40, height / 2 + 1, 0.0f,
			width / 2 + 40, height / 2 - 0, 0.0f,
			width / 2 + 400, height / 2 + 1, 0.0f,

			width / 2 - 40, height / 2 + 1, 0.0f,
			width / 2 - 400, height / 2 - 0, 0.0f,
			width / 2 - 40, height / 2 - 0, 0.0f,

			width / 2 + 400, height / 2 + 1, 0.0f,
			width / 2 + 40, height / 2 - 0, 0.0f,
			width / 2 + 400, height / 2 - 0, 0.0f,

			width / 2 + 1, height / 2 - 400, 0.0f,
			width / 2 + 1, height / 2 - 40, 0.0f,
			width / 2 - 0, height / 2 - 400, 0.0f,

			width / 2 + 1, height / 2 - 400, 0.0f,
			width / 2 + 1, height / 2 - 40, 0.0f,
			width / 2 - 0, height / 2 - 400, 0.0f,

			width / 2 + 1, height / 2 + 400, 0.0f,
			width / 2 - 0, height / 2 + 400, 0.0f,
			width / 2 - 0, height / 2 + 40, 0.0f,

			width / 2 + 1, height / 2 + 400, 0.0f,
			width / 2 - 0, height / 2 + 400, 0.0f,
			width / 2 - 0, height / 2 + 40, 0.0f,

			// the whole screen
			0, height, 0.0f,
			0, 0, 0.0f,
			width, height, 0.0f,

			width, height, 0.0f,
			0, 0, 0.0f,
			width, 0, 0.0f,
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);
	}

	void Render()
	{
		shader.use();
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 24);

		GLfloat twicePi = 2.0f * PI;

		glBegin(GL_LINE_LOOP);
		for (int i = 0; i <= 100; i++) {
			glVertex2f(
				width / 2 + (100 * cos(i *  twicePi / 100)),
				height / 2 + (100 * sin(i * twicePi / 100))
			);
		}
		glEnd();
	}

private:
	Shader shader;
	GLuint VAO, VBO;
	float width, height;
};