//**************************************************
//!	@file	MaterialRenderingShaderHeader.hlsli
//!	@brief	マテリアル描画シェーダ定義
//!	@date	2019 / 09 / 21
//!	@author	t_hashimoto
//**************************************************


/**
*	@cbuffer	CameraInfo
*	@brief		カメラ情報
**/
cbuffer CameraInfo : register( b0 ) {

	matrix	view_matrix;
	matrix	projection_matrix;

}


/**
*	@cbuffer	ObjectInfo
*	@brief		オブジェクト情報
**/
cbuffer ObjectInfo : register( b1 ) {

	matrix	world_matrix;

}


/**
*	@cbuffer	MaterialInfo
*	@brief		マテリアル情報
**/
cbuffer MaterialInfo : register( b2 ) {

	float4	diffuse_color;

}


/**
*	@cbuffer	LightInfo
*	@brief		ライト情報
**/
cbuffer LightInfo : register( b3 ) {

	float4	light_position;
	float4	light_direction;
	float4	light_color;

}


//**************************************************
//	End of File.
//**************************************************