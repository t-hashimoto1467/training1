//**************************************************
//!	@file	DefaultVertexShader.hlsl
//!	@brief	デフォルト頂点シェーダ
//!	@date	2019 / 09 / 21
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"DefaultShader.hlsli"


//==================================================
//!	@brief	頂点シェーダ
//!	@param	[in] VSInput : 頂点シェーダ入力
//!	@return PSInput
//==================================================
PSInput VSMain( VSInput input ) {

	PSInput output = ( PSInput )0;

	output.position = mul( float4( input.position, 1.f ), world_matrix );
	output.world_position = output.position;
	output.position = mul( output.position, view_matrix );
	output.position = mul( output.position, projection_matrix );

	output.normal = mul( float4( input.normal, 0.f ), world_matrix );

	return output;

}


//**************************************************
//	End of File.
//**************************************************