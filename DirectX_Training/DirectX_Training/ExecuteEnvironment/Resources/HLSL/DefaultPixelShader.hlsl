//**************************************************
//!	@file	DefaultPixelShader.hlsl
//!	@brief	デフォルトピクセルシェーダ
//!	@date	2019 / 09 / 21
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"DefaultShader.hlsli"


//==================================================
//!	@brief	ピクセルシェーダ
//!	@param	[in] PSInput : ピクセルシェーダ入力
//!	@return float4
//==================================================
float4 PSMain( PSInput input ) : SV_Target{

	float3 camera_position = {
		-dot( view_matrix[0], view_matrix[3] ),
		-dot( view_matrix[1], view_matrix[3] ),
		-dot( view_matrix[2], view_matrix[3] )
	};
	float4 camera_vector = normalize( float4( camera_position - input.world_position.xyz, 0.f ) );
	float4 light_vector = normalize( float4( light_position.xyz - input.world_position.xyz, 0.f ) );
	float4 normal_vector = normalize( float4( input.normal.xyz, 0.f ) );

	float diffuse_param = ( max( 0.f, dot( light_vector, normal_vector ) ) ) * 0.5f + 0.5f;
	float3 specular_param = pow( max( 0.f, dot( normal_vector, normalize( light_vector + camera_vector ) ) ), 100 );

	return float4( diffuse_color.xyz * ( diffuse_param * diffuse_param ) + specular_param, 1.f );

}

//**************************************************
//	End of File.
//**************************************************