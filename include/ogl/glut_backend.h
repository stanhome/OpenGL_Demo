/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once

#include "types.h"
#include "keys.h"
#include "callback.h"


void GLUTBackendInit(int argc, char **argv, bool withDepth, bool withStencil);
bool GLUTBackendCreateWindow(unsigned int width, unsigned int height, bool isFullScreen, const char *pTitle);
void GLUTBackendRun(ICallbacks *pCallbacks);

//void GLUTBackendSwapBuffers();
//void GLUTBackendLeaveMainLoop();

OGLDEV_KEY GLUTKeyToOGLDEVKey(uint Key);