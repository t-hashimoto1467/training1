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
// ShaderCompiler
#include <D3Dcompiler.h>
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



}
}
}

//**************************************************
//	End of File.
//**************************************************