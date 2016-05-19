#include "ogl/technique.h"
#include "ogl/util.h"

Technique::Technique()
	: m_shaderProg(0)
	, m_shaderObjList()
{
}

Technique::~Technique()
{
	// Delete the intermediate shader objects that have been added to the program.
	// The list will only contain something if shaders were compiled but the object itself
	// was destroyed prior to linking.
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
	{
		glDeleteShader(*it);
	}

	if (m_shaderProg != 0)
	{
		glDeleteProgram(m_shaderProg);
		m_shaderProg = 0;
	}
}

bool Technique::init()
{
	m_shaderProg = glCreateProgram();
	if (m_shaderProg == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}

	return true;
}

void Technique::enable()
{
	/*
	Finally, to use the linked shader program you set it into the pipeline state using the call above.
	This program will stay in effect for all draw calls
	until you replace it with another or explicitly disable its use (and enable the fixed function pipeline) by calling glUseProgram with NULL.
	*/
	glUseProgram(m_shaderProg);
}

bool Technique::addShader(GLenum shaderType, const char * pFileName)
{
	std::string s;
	if (!readFile(pFileName, s))
	{
		return false;
	}

	GLuint shaderObj = glCreateShader(shaderType);
	if (shaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		return false;
	}

	// save the shader object - will be deleted in the destructor
	m_shaderObjList.push_back(shaderObj);

	const GLchar *p[1];
	p[0] = s.c_str();
	GLint lengths[1] = { (GLint)s.size() };
	// The second parameter to the call is the number of slots in the two arrays (just 1 in our case).
	glShaderSource(shaderObj, 1, p, lengths);
	//Compiling the shader is very easy...
	glCompileShader(shaderObj);

	GLint isSuccess;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
		return false;
	}

	/*
	Finally, we attach the compiled shader object to the program object.
	Only the attached objects take part of the linking process.
	*/
	glAttachShader(m_shaderProg, shaderObj);

	return true;
}

bool Technique::finalize()
{
	GLint isSuccess = 0;
	GLchar errorLog[1024] = { 0 };

	/*
	Note that after linking the program you can get rid of the intermediate shader objects by calling glDetachShader and glDeleteShader
	for each and every one of them.
	*/
	glLinkProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &isSuccess);
	if (isSuccess == 0)
	{
		glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		return false;
	}

	/*
	glGetProgramiv() & glValidateProgram()
	The difference is that linking checks for errors based on the combination of shaders
	while the call above check whether the program can execute given the current pipeline state.

	Also, you may want to do this check only during development and avoid this overhead in the final product.
	*/
	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetProgramInfoLog(m_shaderProg, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		return false;
	}

	// NOTE: Delete the intermediate shader objects that have been added to the program.
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it)
	{
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	return GLCheckError();
}

GLint Technique::getUniformLocation(const char *pUniformName)
{
	GLuint location = glGetUniformLocation(m_shaderProg, pUniformName);

	if (location == INVALID_UNIFORM_LOCATION)
	{
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return location;
}


