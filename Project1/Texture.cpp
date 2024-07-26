#include "Texture.h"

#include <iostream>
#include <cstdlib>

Texture::Texture(const char * path) {
	// GL TEXTURE 2 DIMENSION

	int width, height, numColCh;
	unsigned char* bytes = stbi_load(path, &width, &height, &numColCh, 0);
	if (!bytes) {
		std::cout << "Failed to load texture: " << path << std::endl;
		exit(1);
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum format;
	if (numColCh == 1)
		format = GL_RED;
	else if (numColCh == 3)
		format = GL_RGB;
	else if (numColCh == 4)
		format = GL_RGBA;
	else {
		std::cout << "Unsupported image format: " << path << std::endl;
		exit(1);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);


}

void Texture::render() {
	glBindTexture(GL_TEXTURE_2D, texture);
}