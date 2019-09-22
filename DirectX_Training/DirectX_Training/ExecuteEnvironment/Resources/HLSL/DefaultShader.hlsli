//**************************************************
//!	@file	DefaultShader.hlsli
//!	@brief	�f�t�H���g�V�F�[�_��`
//!	@date	2019 / 09 / 21
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"MaterialRenderingShaderHeader.hlsli"


/**
*	@struct	VSInput
*	@brief	���_�V�F�[�_����
**/
struct VSInput {

	float3	position	: POSITION;
	float3	normal		: NORMAL;

};


/**
*	@struct	PSInput
*	@brief	�s�N�Z���V�F�[�_����
**/
struct PSInput {

	float4	position		: SV_POSITION;
	float4	world_position	: POSITION;
	float4	normal			: NORMAL;

};


//**************************************************
//	End of File.
//**************************************************