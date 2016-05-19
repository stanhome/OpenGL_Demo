/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once
#include <list>
#include <GL/glew.h>

class Technique
{
public:
	Technique();
	~Technique();

	virtual bool init();
	void enable();

protected:
	bool addShader(GLenum shaderType, const char * pFileName);
	bool finalize();
	GLint getUniformLocation(const char *pUniformName);
	
private:
	typedef std::list<GLuint> ShaderObjList;

	GLuint m_shaderProg;
	ShaderObjList m_shaderObjList;
};