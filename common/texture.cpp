#include <Windows.h>
#include <sstream>
#include <debugapi.h>

#include "ogl/texture.h"

Texture::Texture(GLenum textureTarget, const std::string &fileName)
	: m_fileName(fileName)
	, m_textureTarget(textureTarget)
	, m_textureObj()
	, m_image()
	, m_blob()
{
	
}

bool Texture::load()
{
	try
	{
		m_image.read(m_fileName);
		m_image.write(&m_blob, "RGBA");
	}
	catch (Magick::Error &error)
	{
		std::ostringstream oss;
		oss << "Error loading texture '" << m_fileName << "': " << error.what() << std::endl;
		OutputDebugStringA(oss.str().c_str());
		printf("%s", oss.str().c_str());

		return false;
	}

	//生成指定数目的的纹理对象并将其句柄储存在 GLuint 数组指针中（第二个参数就是）
	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	/*
	 @param 1:
	 @param 2: LOD(Level-Of-Detail）, 从 0（代表最高的分辨率）开始
	 @param 3: OpenGL 储存纹理数据的内部格式
	 @param 4,5: 纹理的宽度和高度（按像素）
	 @parma 6: 边框的宽度
	 @param 7,8,9: 纹理数据来源，分别为 格式，类型，内存地址
	*/
	glTexImage2D(m_textureTarget, 0, GL_RGBA, m_image.columns(), m_image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(m_textureTarget, 0);

	return true;
}

void Texture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}