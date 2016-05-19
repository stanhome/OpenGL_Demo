/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once

#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>

class Texture
{
public:
	Texture(GLenum textureTarget, const std::string &fileName);

	bool load();
	void bind(GLenum textureUnit);

private:
	std::string m_fileName;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	Magick::Image m_image;
	Magick::Blob m_blob;
};

