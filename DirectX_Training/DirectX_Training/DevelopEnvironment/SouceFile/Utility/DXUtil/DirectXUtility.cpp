//**************************************************
//!	@file	DirectXUtility.cpp
//!	@brief	DirectX便利関数
//!	@date	2019 / 09 / 17
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"DirectXUtility.h"


namespace th {
namespace util {
namespace dx_util {



#pragma region ShaderCompiler.

using namespace Microsoft::WRL;


s32 DecideShaderTarget(
	ShaderType		in_type,
	ShaderVersion	in_version,
	std::string		&out_r_target
) {

	std::string	shader_type;
	std::string	shader_version;

	switch( in_type ) {

		case ShaderType::VERTEX:
			shader_type = "vs_";
			break;

		case ShaderType::PIXEL:
			shader_type = "ps_";
			break;

		case ShaderType::HULL:
			shader_type = "hs_";
			break;

		case ShaderType::DOMEIN:
			shader_type = "ds_";
			break;
			
		case ShaderType::GEOMETRY:
			shader_type = "gs_";
			break;

		case ShaderType::COMPUTE:
			shader_type = "cs_";
			break;

		default:
			return RETURN_FAILURE;

	}

	switch( in_version ) {
		
		case ShaderVersion::VERSION_5:
			shader_version = "5_0";
			break;

#if GRAPHIC_VERSION_DIRECTX_12
		case ShaderVersion::VERSION_6:
			shader_version = "6_0";
			break;
#endif

		default:
			return RETURN_FAILURE;

	}

	out_r_target = shader_type + shader_version;

	return RETURN_SUCCESS;

}


s32 CompileShader(
	LPCSTR				in_file_path,
	LPCSTR				in_entry_point,
	ShaderType			in_type,
	ShaderVersion		in_version,
	ComPtr<ID3DBlob>	&out_r_blob
) {

	size_t	name_len;
	WCHAR	file_name[512];

	setlocale( LC_ALL, "japanese" );
	if( mbstowcs_s( &name_len, file_name, 512, in_file_path, _TRUNCATE ) ) {
		DEBUG_PRINT_STR( "Fail to Change CHAR to WCHAR.\n" );
		return RETURN_FAILURE;
	}

	u32	compile_flg = D3DCOMPILE_SKIP_OPTIMIZATION;
#if DEBUG_ONRY
	compile_flg |= D3DCOMPILE_DEBUG;
#endif

	std::string	shader_target;
	if( DecideShaderTarget( in_type, in_version, shader_target ) ) {
		return RETURN_FAILURE;
	}

	ComPtr<ID3DBlob>	p_err_blob;
	HRESULT hr = D3DCompileFromFile(
		file_name,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		in_entry_point,
		shader_target.c_str(),
		compile_flg,
		0,
		&out_r_blob,
		&p_err_blob
	);
	if( FAILED( hr ) ) {
		if( p_err_blob != nullptr ) {
			OutputDebugString( ( LPTSTR )p_err_blob->GetBufferPointer() );
		}
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;

}

#pragma endregion



#pragma region RasterizerState.

D3D12_RASTERIZER_DESC CreateRasterizerState( RasterizerStateType in_state_type ) {

	D3D12_RASTERIZER_DESC	desc = {};

	// 共通ステートの設定
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	desc.DepthClipEnable = TRUE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;
	desc.ForcedSampleCount = 0;
	desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	// 個別ステートの設定
	switch( in_state_type ) {

		case RasterizerStateType::SOLID_NONE:

			desc.FillMode = D3D12_FILL_MODE_SOLID;
			desc.CullMode = D3D12_CULL_MODE_NONE;

			break;

		case RasterizerStateType::SOLID_FRONT:

			desc.FillMode = D3D12_FILL_MODE_SOLID;
			desc.CullMode = D3D12_CULL_MODE_FRONT;

			break;

		case RasterizerStateType::SOLID_BACK:

			desc.FillMode = D3D12_FILL_MODE_SOLID;
			desc.CullMode = D3D12_CULL_MODE_BACK;

			break;

		case RasterizerStateType::WIREFRAME_NONE:

			desc.FillMode = D3D12_FILL_MODE_WIREFRAME;
			desc.CullMode = D3D12_CULL_MODE_NONE;

			break;

		case RasterizerStateType::WIREFRAME_FRONT:

			desc.FillMode = D3D12_FILL_MODE_WIREFRAME;
			desc.CullMode = D3D12_CULL_MODE_FRONT;

			break;

		case RasterizerStateType::WIREFRAME_BACK:

			desc.FillMode = D3D12_FILL_MODE_WIREFRAME;
			desc.CullMode = D3D12_CULL_MODE_BACK;

			break;

	}

	return desc;

}

#pragma endregion



#pragma region DepthStencilState.

DepthStencilStateDesc CreateDepthStencilState( DepthStencilStateType in_state_type ) {

	DepthStencilStateDesc desc = {};

	// 共通ステートの設定
	desc.desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// 個別ステートの設定
	switch( in_state_type ) {

		case DepthStencilStateType::DEPTH_DEFAULT:

			desc.desc.StencilEnable = FALSE;
			desc.desc.DepthEnable = TRUE;
			desc.desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			desc.format = DXGI_FORMAT_D32_FLOAT;

			break;

	}

	return desc;

}

#pragma endregion



#pragma region BlendState.

D3D12_BLEND_DESC CreateBlendState( BlendStateType in_state_type ) {

	D3D12_BLEND_DESC desc = {};

	// 共通ステートの設定
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	// 個別ステートの設定
	switch( in_state_type ) {

		case BlendStateType::BLEND_OPAQUE:

			for( UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i ) {
				desc.RenderTarget[i].BlendEnable = FALSE;
				desc.RenderTarget[i].LogicOpEnable = FALSE;
				desc.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
				desc.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
				desc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
				desc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
				desc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
				desc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				desc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
				desc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			}

			break;

		case BlendStateType::BLEND_ALPHA:

			for( UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i ) {
				desc.RenderTarget[i].BlendEnable = TRUE;
				desc.RenderTarget[i].LogicOpEnable = FALSE;
				desc.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				desc.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
				desc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
				desc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
				desc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
				desc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				desc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
				desc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			}

			break;

			break;

		case BlendStateType::BLEND_ADDITIVE:

			for( UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i ) {
				desc.RenderTarget[i].BlendEnable = TRUE;
				desc.RenderTarget[i].LogicOpEnable = FALSE;
				desc.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				desc.RenderTarget[i].DestBlend = D3D12_BLEND_ONE;
				desc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
				desc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
				desc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ONE;
				desc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				desc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
				desc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			}

			break;

			break;

		case BlendStateType::BLEND_NONPREMULTIPLIED:

			for( UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i ) {
				desc.RenderTarget[i].BlendEnable = TRUE;
				desc.RenderTarget[i].LogicOpEnable = FALSE;
				desc.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				desc.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
				desc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
				desc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				desc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
				desc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			}

			break;

			break;

	}

	return desc;

}

#pragma endregion



}
}
}

//**************************************************
//	End of File.
//**************************************************