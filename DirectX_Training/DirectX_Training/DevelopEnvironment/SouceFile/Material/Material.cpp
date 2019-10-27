//**************************************************
//!	@file	Material.cpp
//!	@brief	マテリアルクラス
//!	@date	2019 / 10 / 14
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"Material.h"

// DirectXUtil
#include	"Utility/DXUtil/DirectXUtility.h"


namespace th {
namespace material {



Material::Material()
	: m_p_pipeline_state_( nullptr ) {}


Material::~Material() {}


s32 Material::Initialize( Comptr<ID3D12Device> in_p_device ) {
	
	// パイプラインステートの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	desc = {};

	// RootSignatureの設定
	{

		D3D12_DESCRIPTOR_RANGE	descriptor_range = {};
		descriptor_range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		descriptor_range.NumDescriptors = 4;
		descriptor_range.BaseShaderRegister = 0;
		descriptor_range.RegisterSpace = 0;
		descriptor_range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER	root_parameter = {};
		root_parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		root_parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		root_parameter.DescriptorTable.NumDescriptorRanges = 1;
		root_parameter.DescriptorTable.pDescriptorRanges = &descriptor_range;

		D3D12_ROOT_SIGNATURE_DESC	root_signature_desc;
		root_signature_desc.NumParameters = 1;
		root_signature_desc.pParameters = &root_parameter;
		root_signature_desc.NumStaticSamplers = 0;
		root_signature_desc.pStaticSamplers = nullptr;
		root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		Comptr<ID3DBlob>			signature;
		Comptr<ID3DBlob>			error;
		if( FAILED( D3D12SerializeRootSignature(
			&root_signature_desc,
			D3D_ROOT_SIGNATURE_VERSION_1,
			&signature,
			&error
		) ) ) {
			return RETURN_FAILURE;
		}

		if( FAILED( in_p_device->CreateRootSignature(
			0,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS( &m_p_root_signature_ )
		) ) ) {
			return RETURN_FAILURE;
		}

		desc.pRootSignature = m_p_root_signature_.Get();

	}

	// InputLayoutの設定
	D3D12_INPUT_ELEMENT_DESC	input_element_desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	desc.InputLayout = { input_element_desc, _countof( input_element_desc ) };

	// Shaderの設定
	{

		if( util::dx_util::CompileShader(
			"Resources/HLSL/DefaultVertexShader.hlsl",
			"VSMain",
			util::dx_util::ShaderType::VERTEX,
			util::dx_util::ShaderVersion::VERSION_5,
			m_p_vertex_shader
		) ) {
			return RETURN_FAILURE;
		}

		if( util::dx_util::CompileShader(
			"Resources/HLSL/DefaultPixelShader.hlsl",
			"PSMain",
			util::dx_util::ShaderType::PIXEL,
			util::dx_util::ShaderVersion::VERSION_5,
			m_p_pixel_shader
		) ) {
			return RETURN_FAILURE;
		}

		D3D12_SHADER_BYTECODE	shader_bytecode;

		shader_bytecode.pShaderBytecode = m_p_vertex_shader->GetBufferPointer();
		shader_bytecode.BytecodeLength = m_p_vertex_shader->GetBufferSize();
		desc.VS = shader_bytecode;

		shader_bytecode.pShaderBytecode = m_p_pixel_shader->GetBufferPointer();
		shader_bytecode.BytecodeLength = m_p_pixel_shader->GetBufferSize();
		desc.PS = shader_bytecode;

	}

	// ステートの設定
	{

		desc.BlendState = util::dx_util::CreateBlendState( util::dx_util::BlendStateType::BLEND_OPAQUE );
		desc.RasterizerState = util::dx_util::CreateRasterizerState( util::dx_util::RasterizerStateType::SOLID_FRONT );
		auto depth_stencil_state = util::dx_util::CreateDepthStencilState( util::dx_util::DepthStencilStateType::DEPTH_DEFAULT );
		desc.DepthStencilState = depth_stencil_state.desc;
		desc.DSVFormat = depth_stencil_state.format;

	}

	// その他の設定
	desc.SampleMask = UINT_MAX;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;

	if( FAILED( in_p_device->CreateGraphicsPipelineState( &desc, IID_PPV_ARGS( &m_p_pipeline_state_ ) ) ) ) {
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;

}


s32 Material::Load( Comptr<ID3D12Device> in_p_device, std::string in_file_path ) {
	
	// ファイルの読み込み


	// パイプラインステートの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	desc = {};

	if( FAILED( in_p_device->CreateGraphicsPipelineState( &desc, IID_PPV_ARGS( &m_p_pipeline_state_ ) ) ) ) {
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;

}



}
}


//**************************************************
//	End of File.
//**************************************************