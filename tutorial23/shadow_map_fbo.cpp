#include <stdio.h>

#include "shadow_map_fbo.h"

ShadowMapFBO::ShadowMapFBO()
	: m_fbo(0)
	, m_shadowMap(0)
{

}

ShadowMapFBO::~ShadowMapFBO()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_shadowMap != 0)
	{
		glDeleteTextures(1, &m_shadowMap);
	}
}

bool ShadowMapFBO::init(unsigned int windowWidth, unsigned int windowHeight)
{
	// create the FBO
	glGenFramebuffers(1, &m_fbo);

	// create the depth buffer
	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	//最后一个参数 pixels 纹理内存为 NULL, 不提供任何数据进行初始化 buffer 因为我们想要 buffer 包含每一帧的深度值并且每一帧的深度值都可能会变化
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	//如果纹理坐标越界，需要将其截断到[0，1]之间
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	//把 shadow map 纹理附着到 FBO 的深度附着点
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);

	// disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FB error, status: %0x%x\n", status);
		return false;
	}

	return true;
}

void ShadowMapFBO::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void ShadowMapFBO::bindForReading(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
}