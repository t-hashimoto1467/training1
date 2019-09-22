//**************************************************
//!	@file	MaterialRenderingShaderHeader.hlsli
//!	@brief	�}�e���A���`��V�F�[�_��`
//!	@date	2019 / 09 / 21
//!	@author	t_hashimoto
//**************************************************


/**
*	@cbuffer	CameraInfo
*	@brief		�J�������
**/
cbuffer CameraInfo : register( b0 ) {

	matrix	view_matrix;
	matrix	projection_matrix;

}


/**
*	@cbuffer	ObjectInfo
*	@brief		�I�u�W�F�N�g���
**/
cbuffer ObjectInfo : register( b1 ) {

	matrix	world_matrix;

}


/**
*	@cbuffer	MaterialInfo
*	@brief		�}�e���A�����
**/
cbuffer MaterialInfo : register( b2 ) {

	float4	diffuse_color;

}


/**
*	@cbuffer	LightInfo
*	@brief		���C�g���
**/
cbuffer LightInfo : register( b3 ) {

	float4	light_position;
	float4	light_direction;
	float4	light_color;

}


//**************************************************
//	End of File.
//**************************************************