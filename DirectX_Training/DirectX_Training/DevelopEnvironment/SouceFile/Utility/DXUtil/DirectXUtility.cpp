//**************************************************
//!	@file	DirectXUtility.cpp
//!	@brief	DirectXï÷óòä÷êî
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



}
}
}

//**************************************************
//	End of File.
//**************************************************