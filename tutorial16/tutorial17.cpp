/*
  Tutorial 11 - Translation Transformation
*/

#include "stdafx.h"

#include <stdio.h>
#include <string>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogl/math_3d.h"
#include "ogl/util.h"
#include "ogl/glut_backend.h"
#include "ogl/pipeline.h"
#include "ogl/texture.h"
#include "ogl/technique.h"

#include "LightingTechnique.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1024

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = Vector3f(0.0f, 0.0f, 0.0f);
	}
};


class Tutorial17 : public ICallbacks
{
private:
#define SAFE_DELETE(_pObj_) if (_pObj_) { delete _pObj_; _pObj_ = NULL; }

public:
	Tutorial17()
		: m_vbo(0)
		, m_ibo()
		, m_pEffect(NULL)
		, m_pTexture(NULL)
		, m_pGameCamera(NULL)
		, m_scale(0.0f)
		, m_directionalLight()
		, m_persProj()
	{

		m_directionalLight.color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.ambientIntensity = 0.01f;
		m_directionalLight.diffuseIntensity = 0.75;
		m_directionalLight.direction = Vector3f(1.0f, 0.0, 0.0);

		m_persProj.FOV = 60.0f;
		m_persProj.height = WINDOW_HEIGHT;
		m_persProj.width = WINDOW_WIDTH;
		m_persProj.zNear = 1.0f;
		m_persProj.zFar = 100.0f;
	}

	~Tutorial17()
	{
		SAFE_DELETE(m_pEffect);
		SAFE_DELETE(m_pTexture);
		SAFE_DELETE(m_pGameCamera);
	}

	bool init()
	{
		do
		{
			//camera
			Vector3f pos(0.0f, 0.0f, -3.0f);
			Vector3f target(0.0f, 0.0f, 1.0f);
			Vector3f up(0.0f, 1.0f, 0.0f);
			m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

			unsigned int indices[] = {
				0, 3, 1,
				1, 3, 2,
				2, 3, 0,
				1, 2, 0
			};

			//buffer
			createVertexBuffer(indices, ARRAY_LEN(indices));
			createIndexBuffer(indices, sizeof(indices));

			//shader effect
			m_pEffect = new LightingTechnique();
			if (!m_pEffect->init())
			{
				break;
			}

			m_pEffect->enable();
			m_pEffect->setTextureUnit(0);

			//texture
			m_pTexture = new Texture(GL_TEXTURE_2D, "../content/test.png");
			if (!m_pTexture->load())
			{
				break;
			}

			return true;
		} while (0);

		return false;
	}

	void run()
	{
		GLUTBackendRun(this);
	}

	virtual void renderSceneCB() override
	{
		m_pGameCamera->onRender();

		//清除帧缓存（使用 glClearColor 指定过的颜色）
		glClear(GL_COLOR_BUFFER_BIT);

		m_scale += 0.1f;

		Pipeline p;
		p.rotate(0.0f, m_scale, 0.0f);
		p.worldPos(0.0f, 0.0f, 1.0f);
		p.setCamera(*m_pGameCamera);
		p.setPerspectiveProj(m_persProj);
		m_pEffect->setWVP(p.getWVPTrans());
		m_pEffect->SetWorldMatrix(p.getWorldTrans());
		m_pEffect->setDirectionalLight(m_directionalLight);
		m_pEffect->setEyeWorldPos(m_pGameCamera->getPos());
		m_pEffect->setMatSpecularIntensity(1.0f);
		m_pEffect->setMatSpecularPower(32);

		/*
		在 shaders 章节中，你将看见 Shader 中用到的顶点属性（位置，法线等）有一个映射到它们的索引，使我们在 C/C++ 项目中创建的数据能与 Shader 内的属性名之间进行绑定。此外你必须启用每个顶点属性的索引。在这一节中我们还没使用任何 Shader，但是我们加载到缓冲器的顶点位置属性在固定函数管线（在没有绑定着色器的时候就自动启用固定管线）中被看做顶点属性索引为0。你必须启用所有的顶点属性，否则管线将接收不到数据。

		* 启用顶点的 位置属性
		*/
		glEnableVertexAttribArray(0);
		// 启用顶点的 纹理属性
		glEnableVertexAttribArray(1);
		// 启用顶点的 法线属性(location = 2)
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		// (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); //指定 顶点坐标在顶点缓冲区的位置, 对应shader中的((location = 0))
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)sizeof(Vector3f)); //指定 纹理坐标在顶点缓冲区的位置，
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(sizeof(Vector3f) + sizeof(Vector2f)));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		m_pTexture->bind(GL_TEXTURE0);

		/*
		void glDrawArrays (GLenum mode, GLint first, GLsizei count);
		最后，调用这个函数来画几何体。这里就是 GPU 真正开始工作的地方。它现在将结合绘图命令的参数，然后创建一个点并将结果渲染到屏幕。
		@parameter first: 是开始绘制的第一个顶点的索引值
		@parameter count: 用于绘制的顶点的数量。
		*/
		//glDrawArrays(GL_POINTS, 0, 1);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		/*
		当顶点不是要立即被使用的时候，禁用所有的顶点属性是一个很不错的方法。当着色器不使用顶点的时候， 应将将顶点属性禁用。
		*/
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		/*
		* 告诉 GLUT 在 backbuffer 和 frontbuffer 之间进行交换。
		* 在通过帧回调函数的下一个循环中，我们将场景渲染到当前的 frontbuffer 中，而 backbuffer 将被显示。
		*/
		glutSwapBuffers();
	}

	virtual void keyboardCB(OGLDEV_KEY ogldevKey, OGLDEV_KEY_STATE ogldevKeyState = OGLDEV_KEY_STATE_PRESS) override
	{
		switch (ogldevKey)
		{
		//case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_Q:
		case OGLDEV_KEY_q:
			glutLeaveMainLoop();
			break;

		case OGLDEV_KEY_A:
		case OGLDEV_KEY_a:
			m_directionalLight.ambientIntensity += 0.05f;
			break;

		case OGLDEV_KEY_S:
		case OGLDEV_KEY_s:
			m_directionalLight.ambientIntensity -= 0.05f;
			break;

		case OGLDEV_KEY_Z:
		case OGLDEV_KEY_z:
			m_directionalLight.diffuseIntensity += 0.05f;
			break;

		case OGLDEV_KEY_X:
		case OGLDEV_KEY_x:
			m_directionalLight.diffuseIntensity -= 0.05f;
			break;

		default:
			break;
		}
	}

	virtual void passiveMouseCB(int x, int y) override
	{
		m_pGameCamera->onMouse(x, y);
	}

private:
	void CalcNormals(const unsigned int *pIndices, unsigned int indexCount, Vertex *pVertices, unsigned int vertexCount)
	{
		for (unsigned int i = 0; i < indexCount; i += 3)
		{
			unsigned int index0 = pIndices[i];
			unsigned int index1 = pIndices[i + 1];
			unsigned int index2 = pIndices[i + 2];
			Vector3f v1 = pVertices[index1].m_pos - pVertices[index0].m_pos;
			Vector3f v2 = pVertices[index2].m_pos - pVertices[index0].m_pos;
			Vector3f normal = v1.cross(v2);
			normal.normalize();

			//求 共享该顶点的 法线的平均值
			pVertices[index0].m_normal += normal;
			pVertices[index1].m_normal += normal;
			pVertices[index2].m_normal += normal;
		}

		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			pVertices[i].m_normal.normalize();
		}
	}

	void createVertexBuffer(const unsigned int *pIndices, unsigned int indexCount)
	{
		Vertex vertices[4] = {
			Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
			Vertex(Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.5f, 0.0f)),
			Vertex(Vector3f(1.0f, -1.0f, 0.5773f), Vector2f(1.0f, 0.0f)),
			Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.5f, 1.0f)),
		};

		unsigned int vertexCount = sizeof(vertices) / sizeof(*vertices);

		CalcNormals(pIndices, indexCount, vertices, vertexCount);

		/*
		two parameters:
		the first one specifies the number of objects you want to create
		the second is the address of an array of GLuints to store the handles that the driver allocates for you(make sure the array is large enough to handle your request!).
		以后调用这个函数将不会生成相同的句柄，除非你使用 glDeleteBuffers 函数删除它
		你不需要指定用这些 buffers 用于做什么，所以创建的是普通的 buffers，指定这个 buffer 的功能是下面一个函数的任务( Note that at this point you don't specify what you intend to do with the buffers so they can be regarded as "generic". This is the job of the next function.)
		*/
		glGenBuffers(1, &m_vbo);

		/*
		- 这些命令的执行只影响到已经绑定的句柄，直到当前句柄被代替或者将0绑定到目标上。
		- 目标 GL_ARRAY_BUFFER 表示 Buffer 用于存储顶点数组。
		- 目标 GL_ELEMENT_ARRAY_BUFFER 表示 Buffer 用于存储数组的索引
		*/
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		/*
		将缓冲区绑定到目标上之后, 用数据进行填充。
		@parameters:
		- 目标点名称（和上面缓冲区绑定的目标点一样），
		- 数据的字节数，
		- 顶点数组的地址
		*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}

	void createIndexBuffer(const unsigned int *pIndices, unsigned int sizeInBytes)
	{
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, pIndices, GL_STATIC_DRAW);
	}

	//We allocate a GLuint in the global part of the program to store the handle of the vertex buffer object. 
	GLuint m_vbo;
	GLuint m_ibo;
	LightingTechnique *m_pEffect = NULL;
	Texture *m_pTexture = NULL;
	Camera *m_pGameCamera = NULL;
	float m_scale;
	DirectionalLight m_directionalLight;
	PersProjInfo m_persProj;
};

int main(int argc, char * argv[])
{
	GLUTBackendInit(argc, argv, false, false);
	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 18"))
	{
		assert(0);
		//return 1;
	}

	printf("GL versions: %s\n", glGetString(GL_VERSION));

	Tutorial17 *pApp = new Tutorial17();
	if (!pApp->init())
	{
		assert(0);
		//return 1;
	}

	pApp->run();

	delete pApp;
	return 0;
}

