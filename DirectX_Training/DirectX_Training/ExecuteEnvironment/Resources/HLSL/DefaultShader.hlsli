//**************************************************
//!	@file	DefaultShader.hlsli
//!	@brief	デフォルトシェーダ定義
//!	@date	2019 / 09 / 21
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"MaterialRenderingShaderHeader.hlsli"


/**
*	@struct	VSInput
*	@brief	頂点シェーダ入力
**/
struct VSInput {

	float3	position	: POSITION;
	float3	normal		: NORMAL;

};


/**
*	@struct	PSInput
*	@brief	ピクセルシェーダ入力
**/
struct PSInput {

	float4	position		: SV_POSITION;
	float4	world_position	: POSITION;
	float4	normal			: NORMAL;

};


//**************************************************
//	End of File.
//**************************************************