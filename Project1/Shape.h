#ifndef SHAPE_H
#define SHAPE_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class Shape {

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, EBO;

public:
	Shape(std::string path);
	void init();
	void render();
	void terminate();
};

#endif