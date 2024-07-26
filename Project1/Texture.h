#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <string>

class Texture
{
	GLuint texture;

public:
	Texture(const char * path);
	void render();
};

#endif
