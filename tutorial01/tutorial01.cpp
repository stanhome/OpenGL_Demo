﻿// tutorial01.cpp : Defines the entry point for the console application.
//

/*
  Tutorial 01 - Create a window
  Tutorial 02 - dot
*/

#include "stdafx.h"

#include <stdio.h>
#include <string>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogl/math_3d.h"
#include "ogl/util.h"

//We allocate a GLuint in the global part of the program to store the handle of the vertex buffer object. 
GLuint vbo;
GLuint gScaleLocation;

const char *FILE_VERTEX_SHADER = "shader.vert";
const char *FILE_FRAGMENT_SHADER = "shader.frag";

static void RenderSceneCB()
{
	//清除帧缓存（使用 glClearColor 指定过的颜色）
	glClear(GL_COLOR_BUFFER_BIT);

	static float scale = 0.0f;
	scale += 0.001f;
	glUniform1f(gScaleLocation, sinf(scale));

	/*
	在 shaders 章节中，你将看见 Shader 中用到的顶点属性（位置，法线等）有一个映射到它们的索引，使我们在 C/C++ 项目中创建的数据能与 Shader 内的属性名之间进行绑定。此外你必须启用每个顶点属性的索引。在这一节中我们还没使用任何 Shader，但是我们加载到缓冲器的顶点位置属性在固定函数管线（在没有绑定着色器的时候就自动启用固定管线）中被看做顶点属性索引为0。你必须启用所有的顶点属性，否则管线将接收不到数据。
	*/
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/*
	 void glDrawArrays (GLenum mode, GLint first, GLsizei count);
	 最后，调用这个函数来画几何体。这里就是 GPU 真正开始工作的地方。它现在将结合绘图命令的参数，然后创建一个点并将结果渲染到屏幕。
	 @parameter first: 是开始绘制的第一个顶点的索引值
	 @parameter count: 用于绘制的顶点的数量。
	*/
	//glDrawArrays(GL_POINTS, 0, 1);
	glDrawArrays(GL_TRIANGLES, 0, 3);


	/*
	当顶点不是要立即被使用的时候，禁用所有的顶点属性是一个很不错的方法。当着色器不使用顶点的时候， 应将将顶点属性禁用。
	*/
	glDisableVertexAttribArray(0);

	/*
	* 告诉 GLUT 在 backbuffer 和 frontbuffer 之间进行交换。
	* 在通过帧回调函数的下一个循环中，我们将场景渲染到当前的 frontbuffer 中，而 backbuffer 将被显示。
	*/
	glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
}

static void createVertexBuffer()
{
	//Vector3f vertices[1];
	//vertices[0] = Vector3f::ZERO;

	Vector3f vertices[3];
	vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
	vertices[2] = Vector3f(.0f, 1.0f, 0.0f);

	/*
	two parameters:
	the first one specifies the number of objects you want to create
	the second is the address of an array of GLuints to store the handles that the driver allocates for you(make sure the array is large enough to handle your request!).
	以后调用这个函数将不会生成相同的句柄，除非你使用 glDeleteBuffers 函数删除它
	你不需要指定用这些 buffers 用于做什么，所以创建的是普通的 buffers，指定这个 buffer 的功能是下面一个函数的任务( Note that at this point you don't specify what you intend to do with the buffers so they can be regarded as "generic". This is the job of the next function.)
	*/
	glGenBuffers(1, &vbo);

	/*
	- 这些命令的执行只影响到已经绑定的句柄，直到当前句柄被代替或者将0绑定到目标上。
	- 目标 GL_ARRAY_BUFFER 表示 Buffer 用于存储顶点数组。
	- 目标 GL_ELEMENT_ARRAY_BUFFER 表示 Buffer 用于存储数组的索引
	*/
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	/*
	将缓冲区绑定到目标上之后, 用数据进行填充。
	@parameters:
	- 目标点名称（和上面缓冲区绑定的目标点一样），
	- 数据的字节数，
	- 顶点数组的地址
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void addShader(GLuint shaderProgram, const char *pShaderText, GLenum shaderType)
{
	/*
	We start the process of setting up our shaders by creating a program object.
	We will link all the shaders together into this object.
	*/
	GLuint shaderObj = glCreateShader(shaderType);
	if (shaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		exit(0);
	}

	const GLchar *p[1];
	p[0] = pShaderText;
	GLint lengths[1];
	lengths[0] = strlen(pShaderText);
	glShaderSource(shaderObj, 1, p, lengths); // The second parameter to the call is the number of slots in the two arrays (just 1 in our case).
	//Compiling the shader is very easy...
	glCompileShader(shaderObj);

	GLint isSuccess;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
		exit(1);
	}

	/*
	Finally, we attach the compiled shader object to the program object.
	Only the attached objects take part of the linking process.
	*/
	glAttachShader(shaderProgram, shaderObj);
}

static void compileShaders()
{
	GLuint shaderProgram = glCreateProgram();
	if (shaderProgram == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	std::string vs, fs;
	if (!readFile(FILE_VERTEX_SHADER, vs))
	{
		exit(1);
	}
	if (!readFile(FILE_FRAGMENT_SHADER, fs))
	{
		exit(1);
	}

	addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint isSuccess = 0;
	GLchar errorLog[1024] = { 0 };

	/*
	Note that after linking the program you can get rid of the intermediate shader objects by calling glDetachShader and glDeleteShader
	for each and every one of them. 
	*/
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isSuccess);
	if (isSuccess == 0)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}

	/*
	glGetProgramiv() & glValidateProgram() 
	The difference is that linking checks for errors based on the combination of shaders 
	while the call above check whether the program can execute given the current pipeline state. 

	Also, you may want to do this check only during development and avoid this overhead in the final product.
	*/
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}

	/*
	Finally, to use the linked shader program you set it into the pipeline state using the call above.
	This program will stay in effect for all draw calls 
	until you replace it with another or explicitly disable its use (and enable the fixed function pipeline) by calling glUseProgram with NULL.
	*/
	glUseProgram(shaderProgram);

	gScaleLocation = glGetUniformLocation(shaderProgram, "gScale");
	assert(gScaleLocation != 0xFFFFFFFF);
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 02");

	//initialize glut callback
	InitializeGlutCallbacks();

	// Must be done after GLUT has been initialized.
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	createVertexBuffer();

	compileShaders();

	glutMainLoop();

	return 0;
}

