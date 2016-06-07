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

#include "mesh.h"
#include "ogl/light/BasicLightingTechnique.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1024

static const float g_fieldDepth = 10.0f;

class Tutorial22 : public ICallbacks
{
public:
	Tutorial22()
		: m_pEffect(NULL)
		, m_pGameCamera(NULL)
		, m_scale(0.0f)
		, m_directionalLight()
		, m_pMesh(NULL)
		, m_persProj()
	{

		m_directionalLight.color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.ambientIntensity = 1.0f;
		m_directionalLight.diffuseIntensity = 0.01f;
		m_directionalLight.direction = Vector3f(1.0f, -1.0, 0.0);

		m_persProj.FOV = 60.0f;
		m_persProj.height = WINDOW_HEIGHT;
		m_persProj.width = WINDOW_WIDTH;
		m_persProj.zNear = 1.0f;
		m_persProj.zFar = 50.0f;
	}

	~Tutorial22()
	{
		SAFE_DELETE(m_pEffect);
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(m_pMesh);
	}

	bool init()
	{
		do
		{
			//camera
			Vector3f pos(3.0f, 7.0f, -10.0f);
			Vector3f target(0.0f, -0.5f, 1.0f);
			Vector3f up(0.0f, 1.0f, 0.0f);
			m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

			//shader effect
			m_pEffect = new BasicLightingTechnique();
			if (!m_pEffect->init())
			{
				printf("Error initializing the light technique\n");
				break;
			}
			m_pEffect->enable();
			m_pEffect->setTextureUnit(0);

			//mesh
			m_pMesh = new Mesh();
			if (!m_pMesh->loadMesh("../content/phoenix_ugv.md2"))
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
		m_scale += 0.01f;

		m_pGameCamera->onRender();

		//清除帧缓存（使用 glClearColor 指定过的颜色）
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Point Light
		PointLight pl[2];
		pl[0].diffuseIntensity = 0.25f;
		pl[0].color = Vector3f(1.0f, 0.5f, 0.0f);
		pl[0].position = Vector3f(3.0f, 1.0f, g_fieldDepth * (cosf(m_scale) + 1.0f) / 2.0f);
		pl[0].attenuation.linear = 0.1f;

		pl[1].diffuseIntensity = 0.25f;
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
		p.scale(0.1f, 0.1f, 0.1f);
		p.rotate(0.0f, m_scale, 0.0f);
		p.worldPos(0.0f, 0.0f, 10.0f);
		p.setCamera(*m_pGameCamera);
		p.setPerspectiveProj(m_persProj);

		m_pEffect->setWVP(p.getWVPTrans());
		m_pEffect->SetWorldMatrix(p.getWorldTrans());
		m_pEffect->setDirectionalLight(m_directionalLight);
		m_pEffect->setEyeWorldPos(m_pGameCamera->getPos());
		m_pEffect->setMatSpecularIntensity(0.0f);
		m_pEffect->setMatSpecularPower(0);

		m_pMesh->render();

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
	BasicLightingTechnique *m_pEffect = NULL;
	Camera *m_pGameCamera = NULL;
	float m_scale;
	DirectionalLight m_directionalLight;
	Mesh *m_pMesh;
	PersProjInfo m_persProj;
};

int main(int argc, char * argv[])
{
	GLUTBackendInit(argc, argv, true, false);
	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 21"))
	{
		assert(0);
		//return 1;
	}

	printf("GL versions: %s\n", glGetString(GL_VERSION));

	Tutorial22 *pApp = new Tutorial22();
	if (!pApp->init())
	{
		assert(0);
		//return 1;
	}

	pApp->run();

	delete pApp;
	return 0;
}

