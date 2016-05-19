/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once
#include "keys.h"

class ICallbacks
{
public:
	virtual void renderSceneCB() {};

	virtual void keyboardCB(OGLDEV_KEY ogldevKey, OGLDEV_KEY_STATE ogldevKeyState = OGLDEV_KEY_STATE_PRESS) {};

	virtual void passiveMouseCB(int x, int y) {};

	virtual void mouseCB(OGLDEV_MOUSE button, OGLDEV_KEY_STATE state, int x, int y) {};
};