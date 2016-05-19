/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once

#include <GL/glew.h>

class ShadowMapFBO
{
public:
	ShadowMapFBO();
	~ShadowMapFBO();

	bool init(unsigned int windowWidth, unsigned int windowsHeight);
	void bindForWriting();
	void bindForReading(GLenum textureUnit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};