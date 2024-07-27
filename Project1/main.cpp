// OpenGL Libraries
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STL Libraries
#include<iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <numbers> 
#include <vector>

// Helper
#include "Camera.h"
#include "Shape.h"
#include "Texture.h"


// Vertex Shader source code
const char* vertexShaderSource = 
	"#version 330 core\n"

	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec2 aUV;\n"
	"layout (location = 2) in vec3 aNormal;\n"

	"out vec2 TexCoord;\n"
	"out vec3 FragPos;\n"
	"out vec3 Normal;\n"

	"uniform mat4 view;\n"
	"uniform mat4 rotation;\n"
	"uniform mat4 scale;\n"

	"uniform mat4 inner_rotation;\n"

	"uniform vec4 offset;\n"
	"uniform vec4 offset_rotation_location;\n"

	"void main()\n"
	"{\n"
	"   gl_Position = view * (rotation * (scale * inner_rotation * vec4(aPos, 1.0) + offset_rotation_location) + offset);\n"
	"   FragPos = vec3(view * vec4(aPos, 1.0));\n"
	"   TexCoord = aUV;\n"
	"   Normal = mat3(transpose(inverse(view))) * aNormal;\n"
	"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = 
	"#version 330 core\n"
	"in vec2 TexCoord;\n"
	"in vec3 FragPos;\n"
	"in vec3 Normal;\n"

	"out vec4 FragColor;\n"

	"uniform sampler2D textureSampler;\n"  // Texture sampler

	"void main()\n"
	"{\n"
	"   vec4 texColor = texture(textureSampler, TexCoord);\n"
	"   FragColor = texColor;\n"
	"}\n\0";

// Window & Shader Program
GLFWwindow* window;
GLuint shaderProgram;

// Uniform
GLint offset_location;
GLint view_location;
GLint offset_rotation_location;
GLint rotation_location;
GLint inner_rotation_location;
GLint texture_location;
GLint scale_location;

struct Vertex { GLfloat x, y, z; };

void init();

// std::vector<Vertex> loadOBJ(const std::string& path);

int main()
{
	init();

	// Shape
	Shape Head("obj/cube_head.obj");
	Shape LongCube("obj/cube_long.obj");
	Shape Body("obj/cube_body.obj");
	
	Shape Cube("obj/Cube.obj");

	// Camera
	Camera camera(800, 800);

	// Texture
	const char* texture_head_path = "texture/head.png";
	const char* texture_left_leg_path = "texture/left_leg.png";
	const char* texture_right_leg_path = "texture/right_leg.png";
	const char* texture_left_arm_path = "texture/left_arm.png";
	const char* texture_right_arm_path = "texture/right_arm.png";
	const char* texture_body_path = "texture/body.png";
	const char* texture_boots_path = "texture/boots.png";
	const char* texture_cobble_path = "texture/cobble.png";
	const char* texture_torch_path = "texture/torch.png";

	Texture head(texture_head_path);
	Texture left_leg(texture_left_leg_path);
	Texture right_leg(texture_right_leg_path);
	Texture left_arm(texture_left_arm_path);
	Texture right_arm(texture_right_arm_path);
	Texture body(texture_body_path);
	Texture boots(texture_boots_path);

	Texture cobble(texture_cobble_path);
	Texture torch(texture_torch_path);

	// Rotation
	glm::mat4 rotationMatrix;
	glm::mat4 innerRotationMatrix;
	// Scale
	glm::mat4 scaleMatrix;

	// Update angle
	float leg_angle = 0;
	bool dir_leg = true;

	bool shouldUpdate = true;

	while (!glfwWindowShouldClose(window))
	{

		// Clear
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			shouldUpdate = false;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			shouldUpdate = true;
		}

		// Angle Calculation
		if (shouldUpdate) {
			if (dir_leg) {
				leg_angle = leg_angle + 1;
				if (leg_angle == 89) dir_leg = false;
			}
			else {
				leg_angle = leg_angle - 1;
				if (leg_angle == 0) dir_leg = true;
			}
		}

		// Camera
		camera.input(window);
		camera.update(view_location);

		innerRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// Render - Left Leg
		left_leg.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(leg_angle-45), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, -0.5f, -1.5f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, -1.5f, 0.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		LongCube.render();
		
		// Render - Right Leg
		right_leg.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45- leg_angle), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, 0.5f, -1.5f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, -1.5f, 0.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		LongCube.render();

		// Render - Left shoe
		boots.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(leg_angle - 45), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, -0.5f, -1.5f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, -2.5f, 0.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 1.5f, 1.2f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		LongCube.render();

		// Render - Right shoe
		boots.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45 - leg_angle), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, 0.5f, -1.5f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, -2.5f, 0.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 1.5f, 1.2f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		LongCube.render();

		// Render - Body
		body.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, 0.0f, 0.0f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, 0.0f, 0.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 1.0f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		Body.render();

		// Render - Head
		head.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, 0.0f, 2.5f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, 0.0f, 0.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		Head.render();

		// Render - Left Arm
		left_arm.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(leg_angle - 45), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, 1.5f, 1.5f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, -1.5f, 0.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		LongCube.render();

		// Render - Right Arm
		right_arm.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45 - leg_angle), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, -1.5f, 0.5f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, -0.5f, 0.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		LongCube.render();

		// Render - Cube Item
		cobble.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45 - leg_angle), glm::vec3(1.0f, 0.0f, 0.0f));
		innerRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, -1.5f, 0.5f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, -1.5f, 1.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		Cube.render();

		// Render - Torch Item
		torch.render();
		rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(leg_angle-45), glm::vec3(1.0f, 0.0f, 0.0f));
		innerRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniformMatrix4fv(inner_rotation_location, 1, GL_FALSE, glm::value_ptr(innerRotationMatrix));
		glUniform4f(offset_location, 1.5f, 1.5f, 0.0f, 0.0f);
		glUniform4f(offset_rotation_location, 0.0f, -2.75f, 1.0f, 0.0f);
		scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 1.25f));
		glUniformMatrix4fv(scale_location, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
		LongCube.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Destroy
	Head.terminate();
	LongCube.terminate();
	Body.terminate();

	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}


void init() {
	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}
	
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, 800, 800);
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	GLuint vertexShader;
	GLuint fragmentShader;

	// Initialize Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Initialize Program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete Shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Variable
	offset_location = glGetUniformLocation(shaderProgram, "offset");
	view_location = glGetUniformLocation(shaderProgram, "view");
	rotation_location = glGetUniformLocation(shaderProgram, "rotation");
	offset_rotation_location = glGetUniformLocation(shaderProgram, "offset_rotation_location");
	scale_location = glGetUniformLocation(shaderProgram, "scale");
	inner_rotation_location = glGetUniformLocation(shaderProgram, "inner_rotation");
	texture_location = glGetUniformLocation(shaderProgram, "textureSampler");
	glUniform1i(texture_location, 0);
}