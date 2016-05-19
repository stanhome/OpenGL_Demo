
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogl/glut_backend.h"
#include "ogl/util.h"

static ICallbacks *s_pCallbacks = NULL;

static bool s_withDepth = false;
static bool s_withStencil = false;


OGLDEV_KEY GLUTKeyToOGLDEVKey(uint Key)
{
	switch (Key) {
	case GLUT_KEY_F1:
		return OGLDEV_KEY_F1;
	case GLUT_KEY_F2:
		return OGLDEV_KEY_F2;
	case GLUT_KEY_F3:
		return OGLDEV_KEY_F3;
	case GLUT_KEY_F4:
		return OGLDEV_KEY_F4;
	case GLUT_KEY_F5:
		return OGLDEV_KEY_F5;
	case GLUT_KEY_F6:
		return OGLDEV_KEY_F6;
	case GLUT_KEY_F7:
		return OGLDEV_KEY_F7;
	case GLUT_KEY_F8:
		return OGLDEV_KEY_F8;
	case GLUT_KEY_F9:
		return OGLDEV_KEY_F9;
	case GLUT_KEY_F10:
		return OGLDEV_KEY_F10;
	case GLUT_KEY_F11:
		return OGLDEV_KEY_F11;
	case GLUT_KEY_F12:
		return OGLDEV_KEY_F12;
	case GLUT_KEY_LEFT:
		return OGLDEV_KEY_LEFT;
	case GLUT_KEY_UP:
		return OGLDEV_KEY_UP;
	case GLUT_KEY_RIGHT:
		return OGLDEV_KEY_RIGHT;
	case GLUT_KEY_DOWN:
		return OGLDEV_KEY_DOWN;
	case GLUT_KEY_PAGE_UP:
		return OGLDEV_KEY_PAGE_UP;
	case GLUT_KEY_PAGE_DOWN:
		return OGLDEV_KEY_PAGE_DOWN;
	case GLUT_KEY_HOME:
		return OGLDEV_KEY_HOME;
	case GLUT_KEY_END:
		return OGLDEV_KEY_END;
	case GLUT_KEY_INSERT:
		return OGLDEV_KEY_INSERT;
	case GLUT_KEY_DELETE:
		return OGLDEV_KEY_DELETE;
	default:
		OGL_ERROR("Unimplemented GLUT key");
		//exit(1);
	}

	return OGLDEV_KEY_UNDEFINED;
}


static void renderSceneCB()
{
	s_pCallbacks->renderSceneCB();
}

static void idleCB()
{
	s_pCallbacks->renderSceneCB();
}

static void specialKeyboardCB(int key, int x, int y)
{
	OGLDEV_KEY ogldevKey = GLUTKeyToOGLDEVKey(key);
	s_pCallbacks->keyboardCB(ogldevKey);
}

static void keyboardCB(unsigned char key, int x, int y)
{
	if (
		((key >= '+') && (key <= '9')) ||
		((key >= 'A') && (key <= 'Z')) ||
		((key >= 'a') && (key <= 'z'))
		)
	{
		OGLDEV_KEY ogldevKey = (OGLDEV_KEY)key;
		s_pCallbacks->keyboardCB(ogldevKey);
	}
	else
	{
		printf("%d\n", key);
		OGL_ERROR("Unimplemented GLUT Key");
	}
}

static void passiveMouseCB(int x, int y)
{
	s_pCallbacks->passiveMouseCB(x, y);
}



static void mouseCB(int button, int state, int x, int y)
{

}

static void initGlutCallbacks()
{
	glutDisplayFunc(renderSceneCB);
	glutIdleFunc(idleCB);
	glutSpecialFunc(specialKeyboardCB);
	glutPassiveMotionFunc(passiveMouseCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
}

void GLUTBackendInit(int argc, char **argv, bool withDepth, bool withStencil)
{
	s_withDepth = withDepth;
	s_withStencil = withStencil;

	glutInit(&argc, argv);

	uint displayMode = GLUT_DOUBLE | GLUT_RGBA;
	if (s_withDepth)
	{
		displayMode |= GLUT_DEPTH;
	}
	if (s_withStencil)
	{
		displayMode |= GLUT_STENCIL;
	}

	glutInitDisplayMode(displayMode);

	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}

bool GLUTBackendCreateWindow(unsigned int width, unsigned int height, bool isFullScreen, const char *pTitle)
{
	if (isFullScreen)
	{
		char modeStr[64] = { 0 };
		int bpp = 32;
		SNPRINTF(modeStr, sizeof(modeStr), "%dx%d:%d@60", width, height, bpp);
		glutGameModeString(modeStr);
		glutEnterGameMode();
		glutFullScreen();
	}
	else
	{
		glutInitWindowSize(width, height);
		glutInitWindowPosition(100, 0);
		glutCreateWindow(pTitle);
	}

	// Must be done after GLUT has been initialized.
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false;
	}

	return true;
}

void GLUTBackendRun(ICallbacks *pCallbacks)
{
	if (!pCallbacks)
	{
		fprintf(stderr, "%s: callbacks not specified!\n", __FUNCTION__);
		return;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//提出操作：三角形顶点按顺时针指定
	glFrontFace(GL_CW);
	//提出操作：剔除三角形背面，一个对象的内部不需要渲染
	glCullFace(GL_BACK);
	//启用剔除效果
	glEnable(GL_CULL_FACE);

	if (s_withDepth)
	{
		glEnable(GL_DEPTH_TEST);
	}

	s_pCallbacks = pCallbacks;

	initGlutCallbacks();
	glutMainLoop();
}

//void GLUTBackendSwapBuffers()
//{
//	glutSwapBuffers();
//}
//
//void GLUTBackendLeaveMainLoop()
//{
//	glutLeaveMainLoop();
//}