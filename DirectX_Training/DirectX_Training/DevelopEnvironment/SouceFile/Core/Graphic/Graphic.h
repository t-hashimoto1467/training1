//**************************************************
//!	@file	Graphic.h
//!	@brief	�O���t�B�b�N�X�N���X
//!	@date	2019 / 09 / 11
//!	@author	t_hashimoto
//**************************************************

#pragma once

//--------------------------------------------------
//	Define Macro.
//--------------------------------------------------
#define GRAPHIC_DIRECTX_12
#ifndef GRAPHIC_DIRECTX_12
#define GRAPHIC_DIRECTX_11
#endif
#define GRAPHIC_VERSION_DIRECTX_12 defined( GRAPHIC_DIRECTX_12 )
#define GRAPHIC_VERSION_DIRECTX_11 defined( GRAPHIC_DIRECTX_11 )


//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
// DirectX
#if GRAPHIC_VERSION_DIRECTX_12
#include	"DirectX12.h"
#elif GRAPHIC_VERSION_DIRECTX_11
#include	"DirectX11.h"
#endif

// DXGI
#include	<dxgi1_4.h>
#pragma comment( lib, "dxgi.lib" )

// ComPtr
#include	<wrl.h>


namespace th {
namespace core {
namespace graphic {



namespace {

ConstantValue( u32, FRAME_COUNT, 2 );

}


/**
*	@class	GraphicDevice
*	@brief	�O���t�B�b�N�X�f�o�C�X
**/
class GraphicDevice {

public:
	GraphicDevice();
	~GraphicDevice();

	s32 Initialize( HWND, bool );
	void Finalize();

#if GRAPHIC_VERSION_DIRECTX_12

public:
	Microsoft::WRL::ComPtr<ID3D12Device> Get() const {
		return m_p_device_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Device>	m_p_device_;

#elif GRAPHIC_VERSION_DIRECTX_11

public:
	Microsoft::WRL::ComPtr<ID3D11Device> Get() const {
		return m_p_device_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Device>	m_p_device_;

#endif

};



/**
*	@class	GraphicContext
*	@brief	�O���t�B�b�N�X�R���e�L�X�g
**/
#if GRAPHIC_VERSION_DIRECTX_11
class GraphicContext {

public:
	GraphicContext();
	~GraphicContext();

	s32 Initialize( Microsoft::WRL::ComPtr<ID3D11Device> );
	void Finalize();

public:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Get() const {
		return m_p_context_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	m_p_context_;

};
#endif



/**
*	@class	GraphicCommandQueue
*	@brief	�O���t�B�b�N�R�}���h�L���[
**/
#if GRAPHIC_VERSION_DIRECTX_12
class GraphicCommandQueue {

public:
	GraphicCommandQueue();
	~GraphicCommandQueue();

	s32 Initialize( Microsoft::WRL::ComPtr<ID3D12Device> );
	void Finalize();

public:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> Get() const {
		return m_p_command_queue_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>	m_p_command_queue_;

};
#endif



/**
*	@class	GraphicCommandAllocator
*	@brief	�O���t�B�b�N�R�}���h�A���P�[�^�[
**/
#if GRAPHIC_VERSION_DIRECTX_12
class GraphicCommandAllocator {

public:
	GraphicCommandAllocator();
	~GraphicCommandAllocator();

	s32 Initialize( Microsoft::WRL::ComPtr<ID3D12Device> );
	void Finalize();

public:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> Get() const {
		return m_p_command_allocator_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>	m_p_command_allocator_;

};
#endif



/**
*	@class	GraphicCommandList
*	@brief	�O���t�B�b�N�R�}���h���X�g
**/
#if GRAPHIC_VERSION_DIRECTX_12
class GraphicCommandList {

public:
	GraphicCommandList();
	~GraphicCommandList();

	s32 Initialize( Microsoft::WRL::ComPtr<ID3D12Device>, Microsoft::WRL::ComPtr<ID3D12CommandAllocator> );
	void Finalize();

public:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Get() const {
		return m_p_command_list_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_p_command_list_;

};
#endif



/**
*	@class	GraphicSwapChain
*	@brief	�O���t�B�b�N�X���b�v�`�F�C��
**/
class GraphicSwapChain {

public:
	GraphicSwapChain();
	~GraphicSwapChain();

#if GRAPHIC_VERSION_DIRECTX_12

	s32 Initialize( HWND, bool, Microsoft::WRL::ComPtr<ID3D12CommandQueue> );

#elif GRAPHIC_VERSION_DIRECTX_11

	s32 Initialize( HWND, bool, Microsoft::WRL::ComPtr<ID3D11Device> );
	
#endif

	void Finalize();

#if GRAPHIC_VERSION_DIRECTX_12

public:
	void ChangeFrameIndex() {
		m_frame_index_ = m_p_swap_chain_->GetCurrentBackBufferIndex();
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain3> Get() const {
		return m_p_swap_chain_;
	}
	u32 GetFrameIndex() const {
		return m_frame_index_;
	}

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain3>	m_p_swap_chain_;

	u32				m_frame_index_;

#elif GRAPHIC_VERSION_DIRECTX_11

public:
	Microsoft::WRL::ComPtr<IDXGISwapChain> Get() const {
		return m_p_swap_chain_;
	}

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>	m_p_swap_chain_;

#endif

};



/**
*	@class	GraphicFrameBuffer
*	@brief	�O���t�B�b�N�t���[���o�b�t�@
**/
class GraphicFrameBuffer {

public:
	GraphicFrameBuffer();
	~GraphicFrameBuffer();

#if GRAPHIC_VERSION_DIRECTX_12

	s32 Initialize( Microsoft::WRL::ComPtr<ID3D12Device>, Microsoft::WRL::ComPtr<IDXGISwapChain3> );

#elif GRAPHIC_VERSION_DIRECTX_11

	s32 Initialize( Microsoft::WRL::ComPtr<ID3D11Device>, Microsoft::WRL::ComPtr<IDXGISwapChain> );

#endif

	void Finalize();

#if GRAPHIC_VERSION_DIRECTX_12

public:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetHeap() const {
		return m_p_heap_;
	}
	Microsoft::WRL::ComPtr<ID3D12Resource> GetFrameBaffer( u32 in_index ) const {
		return m_p_frame_buffer_[in_index];
	}
	u32 GetDescriptSize() const {
		return m_descript_size_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_p_heap_;
	Microsoft::WRL::ComPtr<ID3D12Resource>			m_p_frame_buffer_[FRAME_COUNT];
	u32						m_descript_size_;

#elif GRAPHIC_VERSION_DIRECTX_11

public:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Get() const {
		return m_p_frame_buffer_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_p_frame_buffer_;

#endif

};



/**
*	@class	GraphicDepthStencilBuffer
*	@brief	�O���t�B�b�N�f�v�X�X�e���V���o�b�t�@
**/
class GraphicDepthStencilBuffer {

public:
	GraphicDepthStencilBuffer();
	~GraphicDepthStencilBuffer();

#if GRAPHIC_VERSION_DIRECTX_12

	s32 Initialize( HWND, Microsoft::WRL::ComPtr <ID3D12Device> );

#elif GRAPHIC_VERSION_DIRECTX_11

#endif

	void Finalize();

#if GRAPHIC_VERSION_DIRECTX_12

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetHeap() const {
		return m_p_heap_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_p_heap_;
	Microsoft::WRL::ComPtr<ID3D12Resource>			m_p_resource_;

#elif GRAPHIC_VERSION_DIRECTX_11

private:


#endif

};



/**
*	@class	GraphicFence
*	@brief	�O���t�B�b�N�t�F���X
**/
#if GRAPHIC_VERSION_DIRECTX_12
class GraphicFence {

public:
	GraphicFence();
	~GraphicFence();

	s32 Initialize( Microsoft::WRL::ComPtr<ID3D12Device> );
	void Finalize();

public:
	Microsoft::WRL::ComPtr<ID3D12Fence> Get() const {
		return m_p_fence_;
	}
	HANDLE GetEventHandle() const {
		return m_h_event_;
	}
	u64 GetFenceValue() const {
		return m_fence_value_;
	}
	void AddFenceValue() {
		++m_fence_value_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Fence>	m_p_fence_;
	HANDLE				m_h_event_;
	u64					m_fence_value_;

};
#endif



/**
*	@class	Graphic
*	@brief	�O���t�B�b�N�X�N���X
**/
class Graphic {

#pragma region Singleton.

public:
	static Graphic& GetInstance() {
		static Graphic instance;
		return instance;
	}

private:
	Graphic() {}
	~Graphic() {}

	Graphic( const Graphic& ) {}
	const Graphic& operator=( const Graphic& ) { return *this; }

#pragma endregion

public:
	s32 Initialize( HWND, bool );
	void Finalize();

	void OnRender();

private:
	void BeginRender();
	void EndRender();

#if GRAPHIC_VERSION_DIRECTX_12

	void WaitForPreviousFrame();

#endif

private:
	GraphicDevice				m_device_;
	GraphicSwapChain			m_swap_chain_;
	GraphicFrameBuffer			m_frame_buffer_;
	GraphicDepthStencilBuffer	m_depth_stencil_buffer_;

#if GRAPHIC_VERSION_DIRECTX_12

	GraphicCommandQueue			m_command_queue_;
	GraphicCommandAllocator		m_command_allocator_;
	GraphicCommandList			m_command_list_;
	GraphicFence				m_fence_;

#elif GRAPHIC_VERSION_DIRECTX_11

	GraphicContext			m_context_;

#endif

};



}
}
}


//**************************************************
//	End of File.
//**************************************************