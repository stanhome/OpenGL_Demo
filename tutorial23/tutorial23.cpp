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
#include "shadow_map_fbo.h"
#include "ShadowMapTechnique.h"
#include "ogl/light/BasicLightingTechnique.h"
#include "LightingTechnique.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1024

static const float g_fieldDepth = 10.0f;

class Tutorial23 : public ICallbacks
{
public:
	Tutorial23()
		: m_pLightingEffect(NULL)
		, m_pShadowMapEffect(NULL)
		, m_pGameCamera(NULL)
		, m_scale(0.0f)
		, m_spotLight()
		, m_pMesh(NULL)
		, m_pQuad(NULL)
		, m_pGroundTex(NULL)
		, m_shadowMapFBO()
		, m_persProj()
	{

		m_spotLight.color = Vector3f(1.0f, 1.0f, 1.0f);
		m_spotLight.ambientIntensity = 0.0f;
		m_spotLight.diffuseIntensity = 0.9f;
		m_spotLight.attenuation.linear = 0.01f;
		m_spotLight.position = Vector3f(-20.0, 20.0, 5.0f);
		m_spotLight.direction = Vector3f(1.0f, -1.0, 0.0);
		m_spotLight.cutoff = 20.0f;

		m_persProj.FOV = 60.0f;
		m_persProj.height = WINDOW_HEIGHT;
		m_persProj.width = WINDOW_WIDTH;
		m_persProj.zNear = 1.0f;
		m_persProj.zFar = 50.0f;
	}

	~Tutorial23()
	{
		SAFE_DELETE(m_pLightingEffect)
		SAFE_DELETE(m_pShadowMapEffect);
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(m_pMesh);
		SAFE_DELETE(m_pQuad);
		SAFE_DELETE(m_pGroundTex);
	}

	bool init()
	{
		do
		{
			if (!m_shadowMapFBO.init(WINDOW_WIDTH, WINDOW_HEIGHT))
			{
				break;
			}

			//camera
			Vector3f pos(3.0f, 8.0f, -10.0f);
			Vector3f target(0.0f, -0.2f, 1.0f);
			Vector3f up(0.0, 1.0f, 0.0f);
			m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

			//shader effect
			m_pLightingEffect = new LightingTechnique();
			if (!m_pLightingEffect->init())
			{
				printf("Error initializing the light technique.\n");
				break;
			}
			m_pLightingEffect->enable();
			m_pLightingEffect->setSpotLights(1, &m_spotLight);
			m_pLightingEffect->setTextureUnit(0);
			m_pLightingEffect->setShadowMapTextureUnit(1);

			m_pShadowMapEffect = new ShadowMapTechnique();
			if (!m_pShadowMapEffect->init())
			{
				printf("Error initializing the shdow map technique\n");
				break;
			}
			//m_pShadowMapEffect->enable();

			m_pQuad = new Mesh();
			if (!m_pQuad->loadMesh("../content/quad.obj"))
			{
				break;
			}

			m_pGroundTex = new Texture(GL_TEXTURE_2D, "../content/test.png");
			if (!m_pGroundTex->load())
			{
				break;
			}

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
		m_scale += 0.05f;

		m_pGameCamera->onRender();
		
		shadowMapPass();
		renderPass();

		/*
		* 告诉 GLUT 在 backbuffer 和 frontbuffer 之间进行交换。
		* 在通过帧回调函数的下一个循环中，我们将场景渲染到当前的 frontbuffer 中，而 backbuffer 将被显示。
		*/
		glutSwapBuffers();
	}

	virtual void shadowMapPass()
	{
		m_shadowMapFBO.bindForWriting();

		glClear(GL_DEPTH_BUFFER_BIT);
		m_pShadowMapEffect->enable();

		Pipeline p;
		p.scale(0.1f, 0.1f, 0.1f);
		p.rotate(0.0f, m_scale, 0.0f);
		p.worldPos(0.0f, 0.0f, 5.0f);
		p.setCamera(m_spotLight.position, m_spotLight.direction, Vector3f(0.0f, 1.0f, 0.0f));
		p.setPerspectiveProj(m_persProj);
		m_pShadowMapEffect->setWVP(p.getWVPTrans());
		m_pMesh->render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual void renderPass()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pLightingEffect->enable();
		m_pLightingEffect->setEyeWorldPos(m_pGameCamera->getPos());

		m_shadowMapFBO.bindForReading(GL_TEXTURE1);

		Pipeline p;
		p.setPerspectiveProj(m_persProj);
		p.scale(10.0f, 10.0f, 10.0f);
		p.worldPos(0.0f, 0.0f, 1.0f);
		p.rotate(90.0f, 0.0f, 0.0f);
		p.setCamera(m_pGameCamera->getPos(), m_pGameCamera->getTarget(), m_pGameCamera->getUp());
		m_pLightingEffect->setWVP(p.getWVPTrans());
		m_pLightingEffect->SetWorldMatrix(p.getWorldTrans());
		p.setCamera(m_spotLight.position, m_spotLight.direction, Vector3f(0.0f, 1.0f, 0.0f));
		m_pLightingEffect->setLightWVP(p.getWVPTrans());
		m_pGroundTex->bind(GL_TEXTURE1);
		m_pQuad->render();

		p.scale(0.1f, 0.1f, 0.1f);
		p.rotate(0.0f, m_scale, 0.0f);
		p.worldPos(0.0f, 0.0f, 3.0f);
		p.setCamera(*m_pGameCamera);
		m_pLightingEffect->setWVP(p.getWVPTrans());
		m_pLightingEffect->SetWorldMatrix(p.getWorldTrans());
		p.setCamera(m_spotLight.position, m_spotLight.direction, Vector3f(0.0f, 1.0f, 0.0f));
		m_pLightingEffect->setLightWVP(p.getWVPTrans());
		m_pMesh->render();
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
			m_spotLight.ambientIntensity += 0.05f;
			break;

		case OGLDEV_KEY_S:
		case OGLDEV_KEY_s:
			m_spotLight.ambientIntensity -= 0.05f;
			break;

		case OGLDEV_KEY_Z:
		case OGLDEV_KEY_z:
			m_spotLight.diffuseIntensity += 0.05f;
			break;

		case OGLDEV_KEY_X:
		case OGLDEV_KEY_x:
			m_spotLight.diffuseIntensity -= 0.05f;
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
	LightingTechnique *m_pLightingEffect;
	ShadowMapTechnique *m_pShadowMapEffect = NULL;
	Camera *m_pGameCamera = NULL;
	float m_scale;
	SpotLight m_spotLight;
	Mesh *m_pMesh;
	Mesh *m_pQuad;
	Texture *m_pGroundTex;
	ShadowMapFBO m_shadowMapFBO;
	PersProjInfo m_persProj;
};

int main(int argc, char * argv[])
{
	GLUTBackendInit(argc, argv, true, false);
	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 24"))
	{
		assert(0);
		//return 1;
	}

	printf("GL versions: %s\n", glGetString(GL_VERSION));

	Tutorial23 *pApp = new Tutorial23();
	if (!pApp->init())
	{
		assert(0);
		//return 1;
	}

	pApp->run();

	delete pApp;
	return 0;
}

