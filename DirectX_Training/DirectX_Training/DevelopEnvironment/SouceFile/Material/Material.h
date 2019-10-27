//**************************************************
//!	@file	Material.h
//!	@brief	マテリアルクラス
//!	@date	2019 / 10 / 14
//!	@author	t_hashimoto
//**************************************************

#pragma once

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"Core/Graphic/Graphic.h"


namespace th {
namespace material {



/**
*	@class	Material
*	@brief	マテリアルクラス
**/
class Material {

private:
	template<class T>
	using Comptr = Microsoft::WRL::ComPtr<T>;

public:
	Material();
	~Material();

	s32 Initialize( Comptr<ID3D12Device> );
	s32 Load( Comptr<ID3D12Device>, std::string );

	ID3D12RootSignature* GetRootSignature() const {
		return m_p_root_signature_.Get();
	}
	ID3D12PipelineState* GetPipelineState() const {
		return m_p_pipeline_state_.Get();
	}

private:
	Comptr<ID3DBlob>			m_p_vertex_shader;
	Comptr<ID3DBlob>			m_p_pixel_shader;
	Comptr<ID3D12RootSignature>	m_p_root_signature_;
	Comptr<ID3D12PipelineState>	m_p_pipeline_state_;

};



}
}


//**************************************************
//	End of File.
//**************************************************