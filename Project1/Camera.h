#ifndef CAMERA_H
#define CAMERA_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
	[CAMERA CLASS] 

	1. Initialize the camera
			Camera camera

	2. Initialize  GLint view location for uniform
			GLint view_location
			view_location = glGetUniformLocation(shaderProgram, "view");

	3. In loop function, call
			camera.input(window);
			camera.update(view_location);

	4. Update the Vertex Shader Source
			uniform mat4 view;
			gl_Position = view * vec4(aPos, 1.0);
*/

class Camera
{
private:
	glm::vec3 initPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	float horizontalAngle = 0.0f; // Azimuth
	float verticalAngle = 0.0f;   // Elevation
	float radius = 20.0f;          // Distance from the target

	float speed = 0.1f;
	float sens = 0.005f;
	int width, height;

	bool shouldRotate = true;

public:
	Camera(int w, int h);

	// update
	void update(GLint view_location);
	void rotate(float hAngle, float vAngle);
	void input(GLFWwindow* window);
};

#endif