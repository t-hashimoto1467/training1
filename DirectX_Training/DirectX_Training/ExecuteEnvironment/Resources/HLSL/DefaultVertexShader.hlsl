//**************************************************
//!	@file	DefaultVertexShader.hlsl
//!	@brief	�f�t�H���g���_�V�F�[�_
//!	@date	2019 / 09 / 21
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"DefaultShader.hlsli"


//==================================================
//!	@brief	���_�V�F�[�_
//!	@param	[in] VSInput : ���_�V�F�[�_����
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