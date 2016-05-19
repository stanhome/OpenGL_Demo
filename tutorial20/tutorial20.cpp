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

	Vertex(const Vector3f &pos, const Vector2f &tex, const Vector3f &normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

static const float g_fieldDepth = 20.0f;
static const float g_fieldWidth = 10.0f;


class Tutorial20 : public ICallbacks
{
private:
#define SAFE_DELETE(_pObj_) if (_pObj_) { delete _pObj_; _pObj_ = NULL; }

public:
	Tutorial20()
		: m_vbo(0)
		, m_pEffect(NULL)
		, m_pTexture(NULL)
		, m_pGameCamera(NULL)
		, m_scale(0.0f)
		, m_directionalLight()
		, m_persProj()
	{

		m_directionalLight.color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.ambientIntensity = 0.0f;
		m_directionalLight.diffuseIntensity = 0.01f;
		m_directionalLight.direction = Vector3f(1.0f, -1.0, 0.0);

		m_persProj.FOV = 60.0f;
		m_persProj.height = WINDOW_HEIGHT;
		m_persProj.width = WINDOW_WIDTH;
		m_persProj.zNear = 1.0f;
		m_persProj.zFar = 50.0f;
	}

	~Tutorial20()
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
			Vector3f pos(5.0f, 1.0f, -3.0f);
			Vector3f target(0.0f, 0.0f, 1.0f);
			Vector3f up(0.0f, 1.0f, 0.0f);
			m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

			//buffer
			createVertexBuffer();

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
		m_scale += 0.0057f;

		m_pGameCamera->onRender();

		//清除帧缓存（使用 glClearColor 指定过的颜色）
		glClear(GL_COLOR_BUFFER_BIT);

		//Point Light
		PointLight pl[2];
		pl[0].diffuseIntensity = 0.15f;
		pl[0].color = Vector3f(1.0f, 0.5f, 0.0f);
		pl[0].position = Vector3f(3.0f, 1.0f, g_fieldDepth * (cosf(m_scale) + 1.0f) / 2.0f);
		pl[0].attenuation.linear = 0.1f;

		pl[1].diffuseIntensity = 0.15f;
		pl[1].color = Vector3f(0.0f, 0.5f, 1.0f);
		pl[1].position = Vector3f(7.0f, 1.0f, g_fieldDepth * (sinf(m_scale) + 1.0f) / 2.0f);
		pl[1].attenuation.linear = 0.1f;
		m_pEffect->setPointLights(2, pl);

		//Spot Light
		SpotLight sl[2];
		sl[0].diffuseIntensity = 0.9f;
		sl[0].color = Vector3f(0.0f, 1.0f, 1.0f);
		sl[0].position = m_pGameCamera->getPos();
		sl[0].direction = m_pGameCamera->getTarget();
		sl[0].attenuation.linear = 0.1f;
		sl[0].cutoff = 10.0f;

		sl[1].diffuseIntensity = 0.5f;
		sl[1].color = Vector3f(1.0f, 1.0f, 1.0f);
		sl[1].position = Vector3f(5.0f, 3.0f, 10.0f);
		sl[1].direction = Vector3f(0.0f, -1.0f, 0.0f);
		sl[1].attenuation.linear = 0.1f;
		sl[1].cutoff = 20.0f;

		m_pEffect->setSpotLights(2, sl);


		Pipeline p;
		p.worldPos(0.0f, 0.0f, 1.0f);
		p.setCamera(*m_pGameCamera);
		p.setPerspectiveProj(m_persProj);
		m_pEffect->setWVP(p.getWVPTrans());
		m_pEffect->SetWorldMatrix(p.getWorldTrans());
		m_pEffect->setDirectionalLight(m_directionalLight);
		m_pEffect->setEyeWorldPos(m_pGameCamera->getPos());
		m_pEffect->setMatSpecularIntensity(0.0f);
		m_pEffect->setMatSpecularPower(0);

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
		m_pTexture->bind(GL_TEXTURE0);

		/*
		void glDrawArrays (GLenum mode, GLint first, GLsizei count);
		最后，调用这个函数来画几何体。这里就是 GPU 真正开始工作的地方。它现在将结合绘图命令的参数，然后创建一个点并将结果渲染到屏幕。
		@parameter first: 是开始绘制的第一个顶点的索引值
		@parameter count: 用于绘制的顶点的数量。
		*/
		//glDrawArrays(GL_POINTS, 0, 1);
		glDrawArrays(GL_TRIANGLES, 0, 6);


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
	void createVertexBuffer()
	{
		const Vector3f normal = Vector3f(0.0, 1.0f, 0.0f);

		Vertex vertices[6] = {
			Vertex(Vector3f(0.0f),							Vector2f(0.0f, 0.0f), normal),
			Vertex(Vector3f(0.0f, 0.0f, g_fieldDepth),		Vector2f(0.0f, 1.0f), normal),
			Vertex(Vector3f(g_fieldWidth, 0.0f, 0.0f),		Vector2f(1.0f, 0.0f), normal),

			Vertex(Vector3f(g_fieldWidth, 0.0f, 0.0f),		Vector2f(1.0f, 0.0f), normal),
			Vertex(Vector3f(0.0f, 0.0f, g_fieldDepth),		Vector2f(0.0f, 1.0f), normal),
			Vertex(Vector3f(g_fieldWidth, 0.0f, g_fieldDepth),Vector2f(1.0f, 1.0f), normal),
		};

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

	//We allocate a GLuint in the global part of the program to store the handle of the vertex buffer object. 
	GLuint m_vbo;
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
	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 21"))
	{
		assert(0);
		//return 1;
	}

	printf("GL versions: %s\n", glGetString(GL_VERSION));

	Tutorial20 *pApp = new Tutorial20();
	if (!pApp->init())
	{
		assert(0);
		//return 1;
	}

	pApp->run();

	delete pApp;
	return 0;
}

