#include "ogl/pipeline.h"

const Matrix4f &Pipeline::getWPTrans()
{
	getWorldTrans();
	getProjTrans();

	m_worldProjTransformation = m_projTransformation * m_worldTransformation;
	//m_worldProjTransformation.logToConsole("m_worldProjTransformation");

	return m_worldProjTransformation;
}

const Matrix4f &Pipeline::getVPTrans()
{
	getViewTrans();
	getProjTrans();

	m_VPtransformation = m_projTransformation * m_Vtransformation;

	return m_VPtransformation;
}

const Matrix4f &Pipeline::getWVPTrans()
{
	getWorldTrans();
	getVPTrans();

	m_WVPtransformation = m_VPtransformation * m_worldTransformation;

	return m_WVPtransformation;
}

const Matrix4f &Pipeline::getWorldTrans()
{
	Matrix4f scaleTrans, rotateTrans, translationTrans;
	
	scaleTrans.initScaleTransform(m_scale.x, m_scale.y, m_scale.z);
	rotateTrans.initRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
	translationTrans.initTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);

	//translationTrans.logToConsole("translationTrans");
	//rotateTrans.logToConsole("rotateTrans");
	//scaleTrans.logToConsole("scaleTrans");

	m_worldTransformation = translationTrans * rotateTrans * scaleTrans;
	
	//m_worldTransformation.logToConsole("m_worldTransformation");
	return m_worldTransformation;
}


const Matrix4f &Pipeline::getViewTrans()
{
	Matrix4f cameraTranslationTrans, cameraRotateTrans;

	cameraTranslationTrans.initTranslationTransform(-m_camera.pos.x, -m_camera.pos.y, -m_camera.pos.z);
	cameraRotateTrans.initCameraTransform(m_camera.target, m_camera.up);

	m_Vtransformation = cameraRotateTrans * cameraTranslationTrans;

	return m_Vtransformation;
}

const Matrix4f &Pipeline::getProjTrans()
{
	m_projTransformation.initPersProjTransform(m_persProjInfo);

	return m_projTransformation;
}