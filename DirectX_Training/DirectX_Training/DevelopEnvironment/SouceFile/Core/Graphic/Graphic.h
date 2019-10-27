//**************************************************
//!	@file	Graphic.h
//!	@brief	グラフィックスクラス
//!	@date	2019 / 09 / 11
//!	@author	t_hashimoto
//**************************************************

#pragma once

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
// DirectX
#include	"DirectX12.h"

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
*	@brief	グラフィックスデバイス
**/
class GraphicDevice {

public:
	GraphicDevice();
	~GraphicDevice();

	s32 Initialize( HWND, bool );
	void Finalize();

public:
	Microsoft::WRL::ComPtr<ID3D12Device> Get() const {
		return m_p_device_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Device>	m_p_device_;

};



/**
*	@class	GraphicCommandQueue
*	@brief	グラフィックコマンドキュー
**/
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



/**
*	@class	GraphicCommandAllocator
*	@brief	グラフィックコマンドアロケーター
**/
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



/**
*	@class	GraphicCommandList
*	@brief	グラフィックコマンドリスト
**/
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



/**
*	@class	GraphicSwapChain
*	@brief	グラフィックスワップチェイン
**/
class GraphicSwapChain {

public:
	GraphicSwapChain();
	~GraphicSwapChain();

	s32 Initialize( HWND, bool, Microsoft::WRL::ComPtr<ID3D12CommandQueue> );
	void Finalize();

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

};



/**
*	@class	GraphicFrameBuffer
*	@brief	グラフィックフレームバッファ
**/
class GraphicFrameBuffer {

public:
	GraphicFrameBuffer();
	~GraphicFrameBuffer();

	s32 Initialize( Microsoft::WRL::ComPtr<ID3D12Device>, Microsoft::WRL::ComPtr<IDXGISwapChain3> );
	void Finalize();

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

};



/**
*	@class	GraphicDepthStencilBuffer
*	@brief	グラフィックデプスステンシルバッファ
**/
class GraphicDepthStencilBuffer {

public:
	GraphicDepthStencilBuffer();
	~GraphicDepthStencilBuffer();

	s32 Initialize( HWND, Microsoft::WRL::ComPtr <ID3D12Device> );
	void Finalize();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetHeap() const {
		return m_p_heap_;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_p_heap_;
	Microsoft::WRL::ComPtr<ID3D12Resource>			m_p_resource_;

};



/**
*	@class	GraphicFence
*	@brief	グラフィックフェンス
**/
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



/**
*	@class	Graphic
*	@brief	グラフィックスクラス
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

	void WaitForPreviousFrame();

private:
	GraphicDevice				m_device_;
	GraphicSwapChain			m_swap_chain_;
	GraphicFrameBuffer			m_frame_buffer_;
	GraphicDepthStencilBuffer	m_depth_stencil_buffer_;

	GraphicCommandQueue			m_command_queue_;
	GraphicCommandAllocator		m_command_allocator_;
	GraphicCommandList			m_command_list_;
	GraphicFence				m_fence_;

};



}
}
}


//**************************************************
//	End of File.
//**************************************************