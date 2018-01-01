#pragma once
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include <vector>

#include <iostream>

#include "shader.h"
#include "Camera.h"
#include "Object.h"


struct Particle {
	glm::vec3 pos, p_speed;
	unsigned int r, g, b, a; // Color
							 //unsigned int r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

const int MaxParticles = 100000;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;

//// Initial position : on +Z
//glm::vec3 position = glm::vec3(0, 0, 5);
//// Initial horizontal angle : toward -Z
//float horizontalAngle = 3.14f;
//// Initial vertical angle : none
//float verticalAngle = 0.0f;
//// Initial Field of View
//float initialFoV = 45.0f;

//float speed_2 = 3.0f; // 3 units / second

static const GLfloat g_vertex_buffer_data[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	0.5f,  0.5f, 0.0f,
};

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

GLuint loadDDS(const char * imagepath) {

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}
	cout << "Load Image correctly." << endl;
	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return textureID;


}


int FindUnusedParticle() {

	for (int i = LastUsedParticle; i < MaxParticles; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void SortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

class Particles : public Object 
{
public:
	Particles() :ParticlesShader("shader/Particle.vertexshader", "shader/Particle.fragmentshader")
	{

		g_particule_position_size_data = new GLfloat[MaxParticles * 4];
		g_particule_color_data = new GLubyte[MaxParticles * 4];
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		//g_particule_position_size_data = new GLfloat[MaxParticles * 4];
		//g_particule_color_data = new GLubyte[MaxParticles * 4];

		for (int i = 0; i < MaxParticles; i++) {
			ParticlesContainer[i].life = -1.0f;
			ParticlesContainer[i].cameradistance = -1.0f;
		}

		ParticlesTexture = loadDDS("resources/textures/particle/particle.DDS");

		glGenBuffers(1, &billboard_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		// The VBO containing the positions and sizes of the particles

		glGenBuffers(1, &particles_position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

		// The VBO containing the colors of the particles

		glGenBuffers(1, &particles_color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	}

	void start() {
		lastTime = glfwGetTime();
	}

	void render(Camera& camera, glm::mat4& projection)
	{
		glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.Front, camera.Up);
		ParticlesShader.setMat4("view", view);
		ParticlesShader.setMat4("projection", projection);


		glm::mat4 ViewProjectionMatrix = projection * view;

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		glBindVertexArray(VertexArrayID);

		//cout << "delta_time:" <<  delta << endl;

		// Generate 10 new particule each millisecond,
		// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
		// newparticles will be huge and the next frame even longer.
		int newparticles = (int)(delta*10000.0);
		if (newparticles > (int)(0.016f*10000.0))
			newparticles = (int)(0.016f*10000.0);

		for (int i = 0; i < newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = life; // This particle will live 5 seconds.
			ParticlesContainer[particleIndex].pos = position;

			//glm::vec3 front;
			//front.x = cos(-rotation.y) * cos(rotation.z);
			//front.y = sin(rotation.z);
			//front.z = sin(-rotation.y) * cos(rotation.z);
			//glm::vec3 Front = glm::normalize(front);
			glm::vec4 front(1, 0, 0, 0);
			glm::mat4 rot = glm::eulerAngleXYZ(camera.rotation.x, camera.rotation.y, camera.rotation.z);
			front = rot * front;
			glm::vec3 Front = glm::normalize(glm::vec3(front.x, front.y, front.z));

			//cout << "Front:" << Front.x << " " << Front.y << " " << Front.z << endl;
			glm::vec3 maindir = glm::vec3(beamspeed * Front.x, beamspeed * Front.y, beamspeed * Front.z);
			// Very bad way to generate a random direction; 
			// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
			// combined with some user-controlled parameters (main direction, spread, etc)
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f
			);

			ParticlesContainer[particleIndex].p_speed = maindir + randomdir*spread;

			// Very bad way to generate a random color
			//ParticlesContainer[particleIndex].r = rand() % 256;
			//ParticlesContainer[particleIndex].g = rand() % 256;
			//ParticlesContainer[particleIndex].b = rand() % 256;
			//ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

			//ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

			ParticlesContainer[particleIndex].r = 255;
			ParticlesContainer[particleIndex].g = 153;
			ParticlesContainer[particleIndex].b = 18;
			ParticlesContainer[particleIndex].a = 256 / 3;

			ParticlesContainer[particleIndex].size = size;
		}

		// Simulate all particles
		int ParticlesCount = 0;
		for (int i = 0; i < MaxParticles; i++) {

			Particle& p = ParticlesContainer[i]; // shortcut

			if (p.life > 0.0f) {

				// Decrease life
				p.life -= delta;
				if (p.life > 0.0f) {

					// Simulate simple physics : gravity only, no collisions
					p.p_speed += gravity * (float)delta * 0.5f;
					p.pos += p.p_speed * (float)delta;
					p.cameradistance = glm::length2(p.pos - camera.position);
					//p.cameradistance = glm::length2(p.pos - position);
					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

					// Fill the GPU buffer
					g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
					g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
					g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

					g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

					g_particule_color_data[4 * ParticlesCount + 0] = p.r;
					g_particule_color_data[4 * ParticlesCount + 1] = p.g;
					g_particule_color_data[4 * ParticlesCount + 2] = p.b;
					g_particule_color_data[4 * ParticlesCount + 3] = p.a;

				}
				else {
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}

				ParticlesCount++;

			}
		}

		//cout << "ParticlesCount:" << ParticlesCount << endl;
		SortParticles();

		// Update the buffers that OpenGL uses for rendering.
		// There are much more sophisticated means to stream data from the CPU to the GPU, 
		// but this is outside the scope of this tutorial.
		// http://www.opengl.org/wiki/Buffer_Object_Streaming

		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//// Use our shader
		//glUseProgram(programID);
		ParticlesShader.use();
		//glBindVertexArray(VertexArrayID);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ParticlesTexture);
		//// Set our "myTextureSampler" sampler to use Texture Unit 0
		ParticlesShader.setInt("myTextureSampler", 0);
		//glUniform1i(TextureID, 0);

		//// Same as the billboards tutorial
		//glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		//glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

		ParticlesShader.setVec3("CameraRight_worldspace", view[0][0], view[1][0], view[2][0]);
		ParticlesShader.setVec3("CameraUp_worldspace", view[0][1], view[1][1], view[2][1]);

		//glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
		ParticlesShader.setMat4("VP", ViewProjectionMatrix);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// These functions are specific to glDrawArrays*Instanced*.
		// The first parameter is the attribute buffer we're talking about.
		// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
		// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

									 // Draw the particules !
									 // This draws many times a small triangle_strip (which looks like a quad).
									 // This is equivalent to :
									 // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
									 // but faster.
									 //glBindVertexArray(VertexArrayID);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glBindVertexArray(0);
		//glDepthFunc(GL_LESS);
		//cout << "ParticlesContainer[1].pos:"  <<ParticlesContainer[1].pos.x  << " " << ParticlesContainer[1].pos .y << " " <<  ParticlesContainer[1].pos.z<< endl;
	}

	void clean()
	{
		delete[] g_particule_position_size_data;
		glDeleteBuffers(1, &particles_color_buffer);
		glDeleteBuffers(1, &particles_position_buffer);
		glDeleteBuffers(1, &billboard_vertex_buffer);
		//glDeleteProgram(programID);
		glDeleteTextures(1, &ParticlesTexture);
		glDeleteVertexArrays(1, &VertexArrayID);
	}

	void setPosition(glm::vec3 &newPosition)
	{
		position = newPosition;
	}



	void setGravity(glm::vec3 &newGravity)
	{
		gravity = newGravity;
	}

	void setBeamspeed(unsigned int newspeed)
	{
		beamspeed = newspeed;
	}

	void setSpread(float newspread)
	{
		spread = newspread;
	}

	void setSize(float newsize)
	{
		size = newsize;
	}

	void setLife(float newlife)
	{
		life = newlife;
	}


private:

	Shader ParticlesShader;

	GLuint billboard_vertex_buffer, particles_position_buffer, particles_color_buffer;
	GLuint VertexArrayID;

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;

	double lastTime;

	unsigned int ParticlesTexture;
	glm::vec3 gravity;

	unsigned int beamspeed;
	float spread, size, life;

};

