//**************************************************
//!	@file	DirectXUtility.h
//!	@brief	DirectXï÷óòä÷êî
//!	@date	2019 / 09 / 17
//!	@author	t_hashimoto
//**************************************************

#pragma once

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
// DirectX
#include	<d3d12.h>

// ShaderCompiler
#include	<D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// ComPtr
#include	<wrl.h>


namespace th {
namespace util {
namespace dx_util {



#pragma region ShaderCompiler.

enum ShaderType {

	VERTEX = 0,
	PIXEL,
	HULL,
	DOMEIN,
	GEOMETRY,
	COMPUTE

};


enum ShaderVersion {

	VERSION_5 = 0,
#if GRAPHIC_VERSION_DIRECTX_12
	VERSION_6
#endif

};


s32	CompileShader(
	LPCSTR,
	LPCSTR,
	ShaderType,
	ShaderVersion,
	Microsoft::WRL::ComPtr<ID3DBlob>&
);

#pragma endregion



#pragma region RasterizerState.

enum RasterizerStateType {

	SOLID_NONE = 0,
	SOLID_FRONT,
	SOLID_BACK,
	WIREFRAME_NONE,
	WIREFRAME_FRONT,
	WIREFRAME_BACK,
	MAX_RASTERIZER_STATE

};


D3D12_RASTERIZER_DESC CreateRasterizerState( RasterizerStateType );

#pragma endregion



#pragma region DepthStencilState.

enum DepthStencilStateType {

	DEPTH_DEFAULT = 0,
	MAX_DEPTHSTENCIL_STATE

};


struct DepthStencilStateDesc {

	D3D12_DEPTH_STENCIL_DESC	desc;
	DXGI_FORMAT					format;

};


DepthStencilStateDesc CreateDepthStencilState( DepthStencilStateType );

#pragma endregion



#pragma region BlendState.

enum BlendStateType {

	BLEND_OPAQUE = 0,
	BLEND_ALPHA,
	BLEND_ADDITIVE,
	BLEND_NONPREMULTIPLIED,
	MAX_BLEND_STATE

};


D3D12_BLEND_DESC CreateBlendState( BlendStateType );

#pragma endregion



}
}
}

//**************************************************
//	End of File.
//**************************************************