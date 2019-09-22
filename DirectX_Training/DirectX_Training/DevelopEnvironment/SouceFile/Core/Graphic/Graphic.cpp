//**************************************************
//!	@file	Graphic.cpp
//!	@brief	グラフィックスクラス
//!	@date	2019 / 09 / 11
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"Graphic.h"


#include <D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include	"Utility/DXUtil/DirectXUtility.h"

namespace th {
namespace core {
namespace graphic {



using namespace Microsoft::WRL;


#pragma region GraphicDevice.


GraphicDevice::GraphicDevice()
	: m_p_device_( nullptr ) {
}


GraphicDevice::~GraphicDevice() {}


s32 GraphicDevice::Initialize( HWND in_h_wnd, bool in_wnd_mode ) {

	HRESULT	hr = S_OK;

#if GRAPHIC_VERSION_DIRECTX_12

#if DEBUG_ONRY
	{
		ComPtr<ID3D12Debug>	debug_controller;
		if( SUCCEEDED( D3D12GetDebugInterface( IID_PPV_ARGS( &debug_controller ) ) ) ) {
			debug_controller->EnableDebugLayer();
		}
	}
#endif

	ComPtr<IDXGIFactory4>	factory;
	if( FAILED( CreateDXGIFactory1( IID_PPV_ARGS( &factory ) ) ) ) {
		return RETURN_FAILURE;
	}

	ComPtr<IDXGIAdapter1>	adapter;
	for( u32 i = 0; factory->EnumAdapters1( i, &adapter ) != DXGI_ERROR_NOT_FOUND; ++i ) {

		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1( &desc );

		if( desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE ) continue;
		if( desc.VendorId == 0x10DE )	break;
		if( desc.VendorId == 0x8086 )	break;

	}

	hr = D3D12CreateDevice(
		adapter.Get(),
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS( &m_p_device_ )
	);
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create GraphicDevice.\n" );
		return RETURN_FAILURE;
	}

#elif GRAPHIC_VERSION_DIRECTX_11

	D3D_FEATURE_LEVEL	feature_level;
	D3D_FEATURE_LEVEL	feature_levels[] = { D3D_FEATURE_LEVEL_11_0 };

	hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		feature_levels,
		ARRAYSIZE( feature_levels ),
		D3D11_SDK_VERSION,
		&m_p_device_,
		&feature_level,
		nullptr
	);

	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create GraphicDevice.\n" );
		return RETURN_FAILURE;
	}
	if( feature_level != D3D_FEATURE_LEVEL_11_0 ) {
		DEBUG_PRINT_STR( "Don't Support Graphic Device.\n" );
		return RETURN_FAILURE;
	}

#endif

	return RETURN_SUCCESS;

}


void GraphicDevice::Finalize() {}


#pragma endregion


#pragma region GraphicContext.

#if GRAPHIC_VERSION_DIRECTX_11

GraphicContext::GraphicContext()
	: m_p_context_( nullptr ) {
}


GraphicContext::~GraphicContext() {}


s32 GraphicContext::Initialize( ComPtr<ID3D11Device> in_p_device ) {

	if( !in_p_device ) {
		DEBUG_PRINT_STR( "Fail to Initialize GraphicContext.\n" );
		return RETURN_FAILURE;
	}

	in_p_device->GetImmediateContext( &m_p_context_ );

	return RETURN_SUCCESS;

}


void GraphicContext::Finalize() {}

#endif

#pragma endregion


#pragma region GraphicCommandQueue.

#if GRAPHIC_VERSION_DIRECTX_12

GraphicCommandQueue::GraphicCommandQueue()
	: m_p_command_queue_( nullptr ) {
}


GraphicCommandQueue::~GraphicCommandQueue() {}


s32 GraphicCommandQueue::Initialize( ComPtr<ID3D12Device> in_p_device ) {

	D3D12_COMMAND_QUEUE_DESC	desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	HRESULT hr = in_p_device->CreateCommandQueue(
		&desc,
		IID_PPV_ARGS( &m_p_command_queue_ )
	);
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create GraphicCommandQueue.\n" );
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;

}


void GraphicCommandQueue::Finalize() {}

#endif

#pragma endregion


#pragma region GraphicCommandAllocator.

#if GRAPHIC_VERSION_DIRECTX_12

GraphicCommandAllocator::GraphicCommandAllocator()
	: m_p_command_allocator_( nullptr ) {
}


GraphicCommandAllocator::~GraphicCommandAllocator() {}


s32 GraphicCommandAllocator::Initialize( ComPtr<ID3D12Device> in_p_device ) {

	HRESULT hr = in_p_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS( &m_p_command_allocator_ )
	);
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create GraphicCommandAllocator.\n" );
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;

}


void GraphicCommandAllocator::Finalize() {}

#endif

#pragma endregion


#pragma region GraphicCommandList.

#if GRAPHIC_VERSION_DIRECTX_12

GraphicCommandList::GraphicCommandList()
	: m_p_command_list_( nullptr ) {
}


GraphicCommandList::~GraphicCommandList() {}


s32 GraphicCommandList::Initialize(
	ComPtr<ID3D12Device>			in_p_device,
	ComPtr<ID3D12CommandAllocator>	in_p_command_allocator
) {

	HRESULT hr = in_p_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		in_p_command_allocator.Get(),
		nullptr,
		IID_PPV_ARGS( &m_p_command_list_ )
	);
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create GraphicCommandList.\n" );
		return RETURN_FAILURE;
	}

	hr = m_p_command_list_->Close();
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Close GraphicCommandList.\n" );
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;

}


void GraphicCommandList::Finalize() {}

#endif

#pragma endregion


#pragma region GraphicSwapChain.


GraphicSwapChain::GraphicSwapChain()
	: m_p_swap_chain_( nullptr ) {
}


GraphicSwapChain::~GraphicSwapChain() {}


#if GRAPHIC_VERSION_DIRECTX_12

s32 GraphicSwapChain::Initialize(
	HWND						in_h_wnd,
	bool						in_wnd_mode,
	ComPtr<ID3D12CommandQueue>	in_p_command_queue
) {

	HRESULT	hr = S_OK;

	RECT	client_rect;
	if( !GetClientRect( in_h_wnd, &client_rect ) ) {
		DEBUG_PRINT_STR( "Fail to Get ClientRect.\n" );
		return RETURN_FAILURE;
	}

	DXGI_SWAP_CHAIN_DESC	desc = {};
	desc.BufferCount = FRAME_COUNT;
	desc.BufferDesc.Width = client_rect.right;
	desc.BufferDesc.Height = client_rect.bottom;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.OutputWindow = in_h_wnd;
	desc.SampleDesc.Count = 1;
	desc.Windowed = in_wnd_mode;

	ComPtr<IDXGIFactory4>	factory;
	hr = CreateDXGIFactory1( IID_PPV_ARGS( &factory ) );
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create DXGIFactory.\n" );
		return RETURN_FAILURE;
	}

	ComPtr<IDXGISwapChain>	swap_chain;
	hr = factory->CreateSwapChain(
		in_p_command_queue.Get(),
		&desc,
		&swap_chain
	);
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create DXGISwapChain.\n" );
		return RETURN_FAILURE;
	}

	hr = swap_chain.As( &m_p_swap_chain_ );
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Change DXGISwapChain.\n" );
		return RETURN_FAILURE;
	}

	m_frame_index_ = m_p_swap_chain_->GetCurrentBackBufferIndex();

	return RETURN_SUCCESS;

}

#elif GRAPHIC_VERSION_DIRECTX_11

s32 GraphicSwapChain::Initialize(
	HWND					in_h_wnd,
	bool					in_wnd_mode,
	ComPtr<ID3D11Device>	in_p_device
) {

	HRESULT	hr = S_OK;

	RECT	client_rect;
	if( !GetClientRect( in_h_wnd, &client_rect ) ) {
		DEBUG_PRINT_STR( "Fail to Get ClientRect.\n" );
		return RETURN_FAILURE;
	}

	DXGI_SWAP_CHAIN_DESC	desc = {};
	desc.BufferCount = FRAME_COUNT;
	desc.BufferDesc.Width = client_rect.right;
	desc.BufferDesc.Height = client_rect.bottom;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = in_h_wnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = in_wnd_mode;

	ComPtr<IDXGIFactory>	factory;
	hr = CreateDXGIFactory( IID_PPV_ARGS( &factory ) );
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create DXGIFactory.\n" );
		return RETURN_FAILURE;
	}

	hr = factory->CreateSwapChain(
		in_p_device.Get(),
		&desc,
		&m_p_swap_chain_
	);
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create DXGISwapChain.\n" );
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;

}

#endif


void GraphicSwapChain::Finalize() {}


#pragma endregion


#pragma region GraphicFrameBuffer.


GraphicFrameBuffer::~GraphicFrameBuffer() {}


#if GRAPHIC_VERSION_DIRECTX_12

GraphicFrameBuffer::GraphicFrameBuffer()
	: m_p_heap_( nullptr )
	, m_p_frame_buffer_{ nullptr, nullptr }{}


s32 GraphicFrameBuffer::Initialize(
	ComPtr <ID3D12Device>	in_p_device,
	ComPtr<IDXGISwapChain3>	in_p_swap_chain
) {

	HRESULT hr = S_OK;

	{

		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = FRAME_COUNT;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		hr = in_p_device->CreateDescriptorHeap(
			&desc,
			IID_PPV_ARGS( &m_p_heap_ )
		);
		if( FAILED( hr ) ) {
			DEBUG_PRINT_STR( "Fail to Create DescriptorHeap.\n" );
			return RETURN_FAILURE;
		}

		m_descript_size_ = in_p_device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );

	}

	{

		D3D12_CPU_DESCRIPTOR_HANDLE descriptor_cpu_handle( m_p_heap_->GetCPUDescriptorHandleForHeapStart() );

		/*D3D12_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;*/

		for( s32 i = 0; i < FRAME_COUNT; ++i ) {

			hr = in_p_swap_chain->GetBuffer( i, IID_PPV_ARGS( &m_p_frame_buffer_[i] ) );
			if( FAILED( hr ) ) {
				DEBUG_PRINT_STR( "Fail to Get Buffer.\n" );
				return RETURN_FAILURE;
			}

			in_p_device->CreateRenderTargetView(
				m_p_frame_buffer_[i].Get(),
				nullptr,
				descriptor_cpu_handle
			);
			if( FAILED( hr ) ) {
				DEBUG_PRINT_STR( "Fail to Create FrameBuffer.\n" );
				return RETURN_FAILURE;
			}

			descriptor_cpu_handle.ptr += m_descript_size_;

		}

	}

	return RETURN_SUCCESS;

}

#elif GRAPHIC_VERSION_DIRECTX_11

GraphicFrameBuffer::GraphicFrameBuffer()
	: m_p_frame_buffer_( nullptr ) {
}


s32 GraphicFrameBuffer::Initialize(
	ComPtr<ID3D11Device>	in_p_device,
	ComPtr<IDXGISwapChain>	in_p_swap_chain
) {

	HRESULT hr = S_OK;

	ComPtr<ID3D11Texture2D>	back_buffer;
	hr = in_p_swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&back_buffer );
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Get Buffer.\n" );
		return RETURN_FAILURE;
	}

	hr = in_p_device->CreateRenderTargetView( back_buffer.Get(), nullptr, &m_p_frame_buffer_ );
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create FrameBuffer.\n" );
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;

}

#endif

void GraphicFrameBuffer::Finalize() {}


#pragma endregion


#pragma region GraphicDepthStencilBuffer.


GraphicDepthStencilBuffer::~GraphicDepthStencilBuffer() {}


#if GRAPHIC_VERSION_DIRECTX_12

GraphicDepthStencilBuffer::GraphicDepthStencilBuffer()
	: m_p_heap_( nullptr )
	, m_p_resource_( nullptr ) {}


s32 GraphicDepthStencilBuffer::Initialize(
	HWND					in_h_wnd,
	ComPtr <ID3D12Device>	in_p_device
) {

	HRESULT	hr = S_OK;

	{

		D3D12_DESCRIPTOR_HEAP_DESC	desc = {};
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		hr = in_p_device->CreateDescriptorHeap(
			&desc,
			IID_PPV_ARGS( &m_p_heap_ )
		);
		if( FAILED( hr ) ) {
			DEBUG_PRINT_STR( "Fail to Create DescriptorHeap.\n" );
			return RETURN_FAILURE;
		}

	}

	{

		D3D12_HEAP_PROPERTIES prop = {};
		prop.Type = D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask = 1;
		prop.VisibleNodeMask = 1;

		RECT	client_rect;
		GetClientRect( in_h_wnd, &client_rect );

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = client_rect.right;
		desc.Height = client_rect.bottom;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 0;
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

		D3D12_CLEAR_VALUE clear_value;
		clear_value.Format = DXGI_FORMAT_D32_FLOAT;
		clear_value.DepthStencil.Depth = 1.0f;
		clear_value.DepthStencil.Stencil = 0;

		hr = in_p_device.Get()->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clear_value,
			IID_PPV_ARGS( &m_p_resource_ ) );
		if( FAILED( hr ) ) {
			DEBUG_PRINT_STR( "Fail to Create DepthStencilResource.\n" );
			return RETURN_FAILURE;
		}

	}

	{

		D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = D3D12_DSV_FLAG_NONE;

		in_p_device.Get()->CreateDepthStencilView(
			m_p_resource_.Get(),
			&desc,
			m_p_heap_.Get()->GetCPUDescriptorHandleForHeapStart()
		);

	}

	return RETURN_SUCCESS;

}

#elif GRAPHIC_VERSION_DIRECTX_11

GraphicDepthStencilBuffer::GraphicDepthStencilBuffer() {}

#endif


void GraphicDepthStencilBuffer::Finalize() {}


#pragma endregion


#pragma region GraphicFence.

#if GRAPHIC_VERSION_DIRECTX_12

GraphicFence::GraphicFence()
	: m_p_fence_( nullptr )
	, m_h_event_( nullptr )
	, m_fence_value_( 0 ) {}


GraphicFence::~GraphicFence() {}


s32 GraphicFence::Initialize( ComPtr<ID3D12Device> in_p_device ) {
	
	m_h_event_ = CreateEventEx( nullptr, FALSE, FALSE, EVENT_ALL_ACCESS );
	if( m_h_event_ == nullptr ) {
		DEBUG_PRINT_STR( "Fail to Create FenceEvent.\n" );
		return RETURN_FAILURE;
	}

	HRESULT hr = in_p_device->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS( &m_p_fence_ )
	);
	if( FAILED( hr ) ) {
		DEBUG_PRINT_STR( "Fail to Create Fence.\n" );
		return RETURN_FAILURE;
	}

	m_fence_value_ = 1;

	return RETURN_SUCCESS;

}


void GraphicFence::Finalize() {

	if( m_h_event_ != nullptr ) {
		CloseHandle( m_h_event_ );
		m_h_event_ = nullptr;
	}

}

#endif

#pragma endregion


#pragma region Graphic.


namespace {


D3D12_VIEWPORT	g_view_port;
D3D12_RECT		g_rect;


ComPtr<ID3D12RootSignature>	g_root_signature;
ComPtr<ID3D12PipelineState>	g_pipeline_state;

ComPtr<ID3DBlob>	g_vertex_shader;
ComPtr<ID3DBlob>	g_pixel_shader;



__declspec( align( 256 ) )
struct CameraInfo {

	DirectX::XMFLOAT4X4	view_matrix;
	DirectX::XMFLOAT4X4	projection_matrix;

};
CameraInfo		g_camera_info;

__declspec( align( 256 ) )
struct ObjectInfo {

	DirectX::XMFLOAT4X4	world_matrix;

};
ObjectInfo		g_object_info;

__declspec( align( 256 ) )
struct MaterialInfo {

	DirectX::XMFLOAT4	diffuse_color;

};
MaterialInfo	g_material_info;

__declspec( align( 256 ) )
struct LightInfo {

	DirectX::XMFLOAT4	position;
	DirectX::XMFLOAT4	direction;
	DirectX::XMFLOAT4	color;

};
LightInfo		g_light_info;

ComPtr<ID3D12DescriptorHeap>	g_cbv_heap;
ComPtr<ID3D12Resource>			g_cbv_buffer[4];
u8								*g_p_cbv_data_begin[4];



struct Vertex {

	DirectX::XMFLOAT3	position;
	DirectX::XMFLOAT3	normal;

};

u32							g_vertex_num;
ComPtr<ID3D12Resource>		g_vertex_buffer;
D3D12_VERTEX_BUFFER_VIEW	g_vertex_buffer_view;

u32							g_index_num;
ComPtr<ID3D12Resource>		g_index_buffer;
D3D12_INDEX_BUFFER_VIEW		g_index_buffer_view;


}


s32 Graphic::Initialize( HWND in_h_wnd, bool in_wnd_mode ) {

	// デバイス生成
	if( m_device_.Initialize( in_h_wnd, in_wnd_mode ) ) {
		return RETURN_FAILURE;
	}

#if GRAPHIC_VERSION_DIRECTX_12

	// コマンドキュー生成
	if( m_command_queue_.Initialize( m_device_.Get() ) ) {
		return RETURN_FAILURE;
	}

	// コマンドアロケーター生成
	if( m_command_allocator_.Initialize( m_device_.Get() ) ) {
		return RETURN_FAILURE;
	}

	// コマンドリスト生成
	if( m_command_list_.Initialize( m_device_.Get(), m_command_allocator_.Get() ) ) {
		return RETURN_FAILURE;
	}

	// SwapChain生成
	if( m_swap_chain_.Initialize( in_h_wnd, in_wnd_mode, m_command_queue_.Get() ) ) {
		return RETURN_FAILURE;
	}

	// Fence生成
	if( m_fence_.Initialize( m_device_.Get() ) ) {
		return RETURN_FAILURE;
	}

#elif GRAPHIC_VERSION_DIRECTX_11

	// コンテキスト生成
	if( m_context_.Initialize( m_device_.Get() ) ) {
		return RETURN_FAILURE;
	}

	// SwapChain生成
	if( m_swap_chain_.Initialize( in_h_wnd, in_wnd_mode, m_device_.Get() ) ) {
		return RETURN_FAILURE;
	}

#endif

	// FrameBuffer生成
	if( m_frame_buffer_.Initialize( m_device_.Get(), m_swap_chain_.Get() ) ) {
		return RETURN_FAILURE;
	}

	// DepthStencilBuffer生成
	if( m_depth_stencil_buffer_.Initialize( in_h_wnd, m_device_.Get() ) ) {
		return RETURN_FAILURE;
	}



#if GRAPHIC_VERSION_DIRECTX_12


	GetClientRect( in_h_wnd, &g_rect );

	g_view_port.TopLeftX = g_view_port.TopLeftY = 0.f;
	g_view_port.Width = static_cast<f32>( g_rect.right );
	g_view_port.Height = static_cast<f32>( g_rect.bottom );
	g_view_port.MinDepth = D3D12_MIN_DEPTH;
	g_view_port.MaxDepth = D3D12_MAX_DEPTH;


	// ToDo : リソース管理は時間の都合で雑になっていますが、いづれもっときちんとした形で作り直します。
	{

		// ConstantBuffer
		{

			// DescriptorHeap
			{

				D3D12_DESCRIPTOR_HEAP_DESC	descriptor_heap_desc = {};
				descriptor_heap_desc.NumDescriptors = 4;
				descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

				if( FAILED( m_device_.Get()->CreateDescriptorHeap(
					&descriptor_heap_desc,
					IID_PPV_ARGS( &g_cbv_heap )
				) ) ) {
					return RETURN_FAILURE;
				}

			}

			// Descriptor
			{

				D3D12_CPU_DESCRIPTOR_HANDLE	heap_cpu_handle = g_cbv_heap->GetCPUDescriptorHandleForHeapStart();

				D3D12_HEAP_PROPERTIES	heap_properties = {};
				heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
				heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
				heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
				heap_properties.CreationNodeMask = 1;
				heap_properties.VisibleNodeMask = 1;

				D3D12_RESOURCE_DESC	resource_desc = {};
				resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
				resource_desc.Alignment = 0;
				resource_desc.Width = sizeof( CameraInfo );
				resource_desc.Height = 1;
				resource_desc.DepthOrArraySize = 1;
				resource_desc.MipLevels = 1;
				resource_desc.Format = DXGI_FORMAT_UNKNOWN;
				resource_desc.SampleDesc.Count = 1;
				resource_desc.SampleDesc.Quality = 0;
				resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
				resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

				if( FAILED( m_device_.Get()->CreateCommittedResource(
					&heap_properties,
					D3D12_HEAP_FLAG_NONE,
					&resource_desc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS( &g_cbv_buffer[0] )
				) ) ) {
					return RETURN_FAILURE;
				}

				D3D12_CONSTANT_BUFFER_VIEW_DESC	constant_buffer_view_desc = {};
				constant_buffer_view_desc.BufferLocation = g_cbv_buffer[0]->GetGPUVirtualAddress();
				constant_buffer_view_desc.SizeInBytes = sizeof( CameraInfo );

				m_device_.Get()->CreateConstantBufferView(
					&constant_buffer_view_desc,
					heap_cpu_handle
				);
				heap_cpu_handle.ptr += m_device_.Get()->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

				if( FAILED( g_cbv_buffer[0]->Map(
					0,
					nullptr,
					reinterpret_cast< void** >( &g_p_cbv_data_begin[0] )
				) ) ) {
					return RETURN_FAILURE;
				}

				DirectX::XMStoreFloat4x4(
					&g_camera_info.view_matrix,
					DirectX::XMMatrixTranspose( DirectX::XMMatrixLookAtLH(
						DirectX::XMVectorSet( 0.f, 3.f, -5.f, 1.f ),
						DirectX::XMVectorSet( 0.f, 0.f, 0.f, 1.f ),
						DirectX::XMVectorSet( 0.f, 1.f, 0.f, 0.f )
					) )
				);
				DirectX::XMStoreFloat4x4(
					&g_camera_info.projection_matrix,
					DirectX::XMMatrixTranspose( DirectX::XMMatrixPerspectiveFovLH(
						DirectX::XMConvertToRadians( 45.f ),
						g_view_port.Width / g_view_port.Height,
						1.f,
						1000.f
					) )
				);

				memcpy( g_p_cbv_data_begin[0], &g_camera_info, sizeof( g_camera_info ) );

				/*--------------------------------------------------*/

				resource_desc.Width = sizeof( ObjectInfo );

				if( FAILED( m_device_.Get()->CreateCommittedResource(
					&heap_properties,
					D3D12_HEAP_FLAG_NONE,
					&resource_desc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS( &g_cbv_buffer[1] )
				) ) ) {
					return RETURN_FAILURE;
				}

				constant_buffer_view_desc.BufferLocation = g_cbv_buffer[1]->GetGPUVirtualAddress();
				constant_buffer_view_desc.SizeInBytes = sizeof( ObjectInfo );

				m_device_.Get()->CreateConstantBufferView(
					&constant_buffer_view_desc,
					heap_cpu_handle
				);
				heap_cpu_handle.ptr += m_device_.Get()->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

				if( FAILED( g_cbv_buffer[1]->Map(
					0,
					nullptr,
					reinterpret_cast< void** >( &g_p_cbv_data_begin[1] )
				) ) ) {
					return RETURN_FAILURE;
				}

				DirectX::XMStoreFloat4x4( &g_object_info.world_matrix, DirectX::XMMatrixTranspose( DirectX::XMMatrixIdentity() ) );

				memcpy( g_p_cbv_data_begin[1], &g_object_info, sizeof( g_object_info ) );

				/*--------------------------------------------------*/

				resource_desc.Width = sizeof( MaterialInfo );

				if( FAILED( m_device_.Get()->CreateCommittedResource(
					&heap_properties,
					D3D12_HEAP_FLAG_NONE,
					&resource_desc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS( &g_cbv_buffer[2] )
				) ) ) {
					return RETURN_FAILURE;
				}

				constant_buffer_view_desc.BufferLocation = g_cbv_buffer[2]->GetGPUVirtualAddress();
				constant_buffer_view_desc.SizeInBytes = sizeof( MaterialInfo );

				m_device_.Get()->CreateConstantBufferView(
					&constant_buffer_view_desc,
					heap_cpu_handle
				);
				heap_cpu_handle.ptr += m_device_.Get()->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

				if( FAILED( g_cbv_buffer[2]->Map(
					0,
					nullptr,
					reinterpret_cast< void** >( &g_p_cbv_data_begin[2] )
				) ) ) {
					return RETURN_FAILURE;
				}

				g_material_info.diffuse_color = DirectX::XMFLOAT4( 1.f, 1.f, 1.f, 1.f );

				memcpy( g_p_cbv_data_begin[2], &g_material_info, sizeof( g_material_info ) );

				/*--------------------------------------------------*/

				resource_desc.Width = sizeof( LightInfo );

				if( FAILED( m_device_.Get()->CreateCommittedResource(
					&heap_properties,
					D3D12_HEAP_FLAG_NONE,
					&resource_desc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS( &g_cbv_buffer[3] )
				) ) ) {
					return RETURN_FAILURE;
				}

				constant_buffer_view_desc.BufferLocation = g_cbv_buffer[3]->GetGPUVirtualAddress();
				constant_buffer_view_desc.SizeInBytes = sizeof( LightInfo );

				m_device_.Get()->CreateConstantBufferView(
					&constant_buffer_view_desc,
					heap_cpu_handle
				);
				heap_cpu_handle.ptr += m_device_.Get()->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

				if( FAILED( g_cbv_buffer[3]->Map(
					0,
					nullptr,
					reinterpret_cast< void** >( &g_p_cbv_data_begin[3] )
				) ) ) {
					return RETURN_FAILURE;
				}

				g_light_info.position = DirectX::XMFLOAT4( 0.f, 0.f, -3.f, 1.f );
				g_light_info.direction = DirectX::XMFLOAT4( 0.f, -1.f, 0.f, 0.f );
				g_light_info.color = DirectX::XMFLOAT4( 1.f, 1.f, 1.f, 1.f );

				memcpy( g_p_cbv_data_begin[3], &g_light_info, sizeof( g_light_info ) );

			}

		}


		// PipelineState
		{

			D3D12_GRAPHICS_PIPELINE_STATE_DESC	pipeline_state_desc = {};

			// RootSignature
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

				ComPtr<ID3DBlob>	signature;
				ComPtr<ID3DBlob>	error;
				if( FAILED( D3D12SerializeRootSignature(
					&root_signature_desc,
					D3D_ROOT_SIGNATURE_VERSION_1,
					&signature,
					&error
				) ) ) {
					return RETURN_FAILURE;
				}

				if( FAILED( m_device_.Get()->CreateRootSignature(
					0,
					signature->GetBufferPointer(),
					signature->GetBufferSize(),
					IID_PPV_ARGS( &g_root_signature )
				) ) ) {
					return RETURN_FAILURE;
				}

				pipeline_state_desc.pRootSignature = g_root_signature.Get();

			}

			// InputLayout
			D3D12_INPUT_ELEMENT_DESC	input_element_desc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};
			pipeline_state_desc.InputLayout = { input_element_desc, _countof( input_element_desc ) };

			// ShaderCompile
			{

				if( util::dx_util::CompileShader(
					"Resources/HLSL/DefaultVertexShader.hlsl",
					"VSMain",
					util::dx_util::ShaderType::VERTEX,
					util::dx_util::ShaderVersion::VERSION_5,
					g_vertex_shader
				) ) {
					return RETURN_FAILURE;
				}

				if( util::dx_util::CompileShader(
					"Resources/HLSL/DefaultPixelShader.hlsl",
					"PSMain",
					util::dx_util::ShaderType::PIXEL,
					util::dx_util::ShaderVersion::VERSION_5,
					g_pixel_shader
				) ) {
					return RETURN_FAILURE;
				}

				{

					D3D12_SHADER_BYTECODE	shader_bytecode;

					shader_bytecode.pShaderBytecode = g_vertex_shader->GetBufferPointer();
					shader_bytecode.BytecodeLength = g_vertex_shader->GetBufferSize();
					pipeline_state_desc.VS = shader_bytecode;

					shader_bytecode.pShaderBytecode = g_pixel_shader->GetBufferPointer();
					shader_bytecode.BytecodeLength = g_pixel_shader->GetBufferSize();
					pipeline_state_desc.PS = shader_bytecode;

				}

			}

			// RasterizerState
			{

				D3D12_RASTERIZER_DESC	rasterizer_desc = {};
				rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;
				rasterizer_desc.CullMode = D3D12_CULL_MODE_FRONT;
				rasterizer_desc.FrontCounterClockwise = FALSE;
				rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
				rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
				rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
				rasterizer_desc.DepthClipEnable = TRUE;
				rasterizer_desc.MultisampleEnable = FALSE;
				rasterizer_desc.AntialiasedLineEnable = FALSE;
				rasterizer_desc.ForcedSampleCount = 0;
				rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
				pipeline_state_desc.RasterizerState = rasterizer_desc;

			}

			// BlendState
			{

				D3D12_BLEND_DESC	blend_desc = {};
				blend_desc.AlphaToCoverageEnable = FALSE;
				blend_desc.IndependentBlendEnable = FALSE;
				for( UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i ) {
					blend_desc.RenderTarget[i].BlendEnable = FALSE;
					blend_desc.RenderTarget[i].LogicOpEnable = FALSE;
					blend_desc.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
					blend_desc.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
					blend_desc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
					blend_desc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
					blend_desc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
					blend_desc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
					blend_desc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
					blend_desc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				}
				pipeline_state_desc.BlendState = blend_desc;

			}

			// DepthStencilState
			{

				D3D12_DEPTH_STENCIL_DESC	depth_stencil_desc = {};
				depth_stencil_desc.StencilEnable = FALSE;
				depth_stencil_desc.DepthEnable = TRUE;
				depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
				depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
				pipeline_state_desc.DepthStencilState = depth_stencil_desc;
				pipeline_state_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

			}

			pipeline_state_desc.SampleMask = UINT_MAX;
			pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			pipeline_state_desc.NumRenderTargets = 1;
			pipeline_state_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			pipeline_state_desc.SampleDesc.Count = 1;

			if( FAILED( m_device_.Get()->CreateGraphicsPipelineState(
				&pipeline_state_desc,
				IID_PPV_ARGS( &g_pipeline_state )
			) ) ) {
				DEBUG_PRINT_STR( "Fail to Create PipelineState.\n" );
				return RETURN_FAILURE;
			}

		}

		// 頂点バッファを作成
		{

			Vertex	vertices[] = {
				{ { -1.f,  1.f, -1.f }, { -0.577350f,  0.577350f, -0.577350f } },
				{ {  1.f,  1.f, -1.f }, {  0.577350f,  0.577350f, -0.577350f } },
				{ {  1.f,  1.f,  1.f }, {  0.577350f,  0.577350f,  0.577350f } },
				{ { -1.f,  1.f,  1.f }, { -0.577350f,  0.577350f,  0.577350f } },
				{ { -1.f, -1.f, -1.f }, { -0.577350f, -0.577350f, -0.577350f } },
				{ {  1.f, -1.f, -1.f }, {  0.577350f, -0.577350f, -0.577350f } },
				{ {  1.f, -1.f,  1.f }, {  0.577350f, -0.577350f,  0.577350f } },
				{ { -1.f, -1.f,  1.f }, { -0.577350f, -0.577350f,  0.577350f } }
			};

			g_vertex_num = _countof( vertices );
			const u32	vertex_buffer_size = sizeof( vertices );

			{

				D3D12_HEAP_PROPERTIES	heap_properties = {};
				heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
				heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
				heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
				heap_properties.CreationNodeMask = 1;
				heap_properties.VisibleNodeMask = 1;

				D3D12_RESOURCE_DESC	resource_desc = {};
				resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
				resource_desc.Alignment = 0;
				resource_desc.Width = vertex_buffer_size;
				resource_desc.Height = 1;
				resource_desc.DepthOrArraySize = 1;
				resource_desc.MipLevels = 1;
				resource_desc.Format = DXGI_FORMAT_UNKNOWN;
				resource_desc.SampleDesc.Count = 1;
				resource_desc.SampleDesc.Quality = 0;
				resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
				resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

				if( FAILED( m_device_.Get()->CreateCommittedResource(
					&heap_properties,
					D3D12_HEAP_FLAG_NONE,
					&resource_desc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS( &g_vertex_buffer )
				) ) ) {
					return RETURN_FAILURE;
				}

			}

			// 頂点バッファに頂点データをコピー
			u8			*p_vertex_data_begin;
			D3D12_RANGE	read_range = { 0, 0 };		// CPUからバッファを読み込まない設定
			if( FAILED( g_vertex_buffer->Map(
				0,
				&read_range,
				reinterpret_cast< void** >( &p_vertex_data_begin )
			) ) ) {
				return RETURN_FAILURE;
			}

			memcpy( p_vertex_data_begin, vertices, sizeof( vertices ) );

			g_vertex_buffer->Unmap( 0, nullptr );

			// 頂点バッファビューを初期化
			g_vertex_buffer_view.BufferLocation = g_vertex_buffer->GetGPUVirtualAddress();
			g_vertex_buffer_view.StrideInBytes = sizeof( Vertex );
			g_vertex_buffer_view.SizeInBytes = vertex_buffer_size;

		}

		// インデックスバッファを作成
		{

			u32	indexs[] = {
				0, 1, 2,
				0, 2, 3,
				4, 5, 1,
				4, 1, 0,
				7, 4, 0,
				7, 0, 3,
				5, 6, 2,
				5, 2, 1,
				7, 6, 5,
				7, 5, 4,
				3, 2, 6,
				3, 6, 7
			};

			g_index_num = _countof( indexs );
			const u32	index_buffer_size = sizeof( indexs );

			{

				D3D12_HEAP_PROPERTIES	heap_properties = {};
				heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
				heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
				heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
				heap_properties.CreationNodeMask = 1;
				heap_properties.VisibleNodeMask = 1;

				D3D12_RESOURCE_DESC	resource_desc = {};
				resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
				resource_desc.Alignment = 0;
				resource_desc.Width = index_buffer_size;
				resource_desc.Height = 1;
				resource_desc.DepthOrArraySize = 1;
				resource_desc.MipLevels = 1;
				resource_desc.Format = DXGI_FORMAT_UNKNOWN;
				resource_desc.SampleDesc.Count = 1;
				resource_desc.SampleDesc.Quality = 0;
				resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
				resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

				if( FAILED( m_device_.Get()->CreateCommittedResource(
					&heap_properties,
					D3D12_HEAP_FLAG_NONE,
					&resource_desc,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS( &g_index_buffer )
				) ) ) {
					return RETURN_FAILURE;
				}

			}

			// 頂点バッファに頂点データをコピー
			u8			*p_index_data_begin;
			D3D12_RANGE	read_range = { 0, 0 };		// CPUからバッファを読み込まない設定
			if( FAILED( g_index_buffer->Map(
				0,
				&read_range,
				reinterpret_cast< void** >( &p_index_data_begin )
			) ) ) {
				return RETURN_FAILURE;
			}

			memcpy( p_index_data_begin, indexs, sizeof( indexs ) );

			g_index_buffer->Unmap( 0, nullptr );

			// 頂点バッファビューを初期化
			g_index_buffer_view.BufferLocation = g_index_buffer->GetGPUVirtualAddress();
			g_index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
			g_index_buffer_view.SizeInBytes = index_buffer_size;

		}

	}


#endif


	return RETURN_SUCCESS;

}


void Graphic::Finalize() {

#if GRAPHIC_VERSION_DIRECTX_12

	// GPU処理待ち
	this->WaitForPreviousFrame();

	// ConstantBufferをUnmap
	g_cbv_buffer[0]->Unmap( 0, nullptr );
	g_cbv_buffer[1]->Unmap( 0, nullptr );
	g_cbv_buffer[2]->Unmap( 0, nullptr );
	g_cbv_buffer[3]->Unmap( 0, nullptr );

#endif

	// DepthStencilBuffer破棄
	m_depth_stencil_buffer_.Finalize();

	// FrameBuffer破棄
	m_frame_buffer_.Finalize();

	// SwapChain破棄
	m_swap_chain_.Finalize();

#if GRAPHIC_VERSION_DIRECTX_12

	// Fence破棄
	m_fence_.Finalize();

	// コマンドリスト破棄
	m_command_list_.Finalize();

	// コマンドアロケーター破棄
	m_command_allocator_.Finalize();

	// コマンドキュー破棄
	m_command_queue_.Finalize();

#elif GRAPHIC_VERSION_DIRECTX_11

	// コンテキスト破棄
	m_context_.Finalize();

#endif

	// デバイス破棄
	m_device_.Finalize();

}


void Graphic::OnRender() {

	this->BeginRender();

	{

		// オブジェクトの挙動処理
		static bool rot_flg = false;
		static bool key_push_flg = false;
		if( GetAsyncKeyState( VK_SPACE ) & 0x8000 ) {
			if( !key_push_flg )	rot_flg ^= true;
			key_push_flg = true;
		}
		else {
			key_push_flg = false;
		}
		
		static f32 rot_y = 0.f;
		DirectX::XMMATRIX	world_mat = DirectX::XMLoadFloat4x4( &g_object_info.world_matrix );
		world_mat = DirectX::XMMatrixRotationY( DirectX::XMConvertToRadians( rot_y ) );
		DirectX::XMStoreFloat4x4( &g_object_info.world_matrix, DirectX::XMMatrixTranspose( world_mat ) );
				
		if( rot_flg ) {
			rot_y += 1.f;
			if( rot_y >= 360.f ) { rot_y -= 360.f; }
		}
		
		memcpy( g_p_cbv_data_begin[1], &g_object_info, sizeof( g_object_info ) );


		// ライトの挙動処理
		if( GetAsyncKeyState( 'A' ) ) {
			g_light_info.position.x -= 1.f;
		}
		else if( GetAsyncKeyState( 'D' ) ) {
			g_light_info.position.x += 1.f;
		}

		if( GetAsyncKeyState( 'W' ) ) {
			g_light_info.position.z += 1.f;
		}
		else if( GetAsyncKeyState( 'S' ) ) {
			g_light_info.position.z -= 1.f;
		}

		if( GetAsyncKeyState( 'Q' ) ) {
			g_light_info.position.y += 1.f;
		}
		else if( GetAsyncKeyState( 'E' ) ) {
			g_light_info.position.y -= 1.f;
		}

		memcpy( g_p_cbv_data_begin[3], &g_light_info, sizeof( g_light_info ) );

	}

	// モデル描画
	m_command_list_.Get()->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	m_command_list_.Get()->IASetIndexBuffer( &g_index_buffer_view );
	m_command_list_.Get()->IASetVertexBuffers( 0, 1, &g_vertex_buffer_view );
	m_command_list_.Get()->DrawIndexedInstanced( 36, 1, 0, 0, 0 );

	this->EndRender();

}


void Graphic::BeginRender() {

	const f32	frame_buffer_clear_color[] = { 0.f, 0.f, 0.f, 1.f };

#if GRAPHIC_VERSION_DIRECTX_12

	m_command_allocator_.Get()->Reset();
	m_command_list_.Get()->Reset( m_command_allocator_.Get().Get(), g_pipeline_state.Get() );

	ID3D12DescriptorHeap	*p_descriptor_heaps[] = { g_cbv_heap.Get() };
	m_command_list_.Get()->SetDescriptorHeaps( 1, p_descriptor_heaps );
	m_command_list_.Get()->SetGraphicsRootSignature( g_root_signature.Get() );
	m_command_list_.Get()->SetGraphicsRootDescriptorTable( 0, g_cbv_heap->GetGPUDescriptorHandleForHeapStart() );

	m_command_list_.Get()->RSSetViewports( 1, &g_view_port );
	m_command_list_.Get()->RSSetScissorRects( 1, &g_rect );

	D3D12_RESOURCE_BARRIER	resource_barrier = {};
	resource_barrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource	= m_frame_buffer_.GetFrameBaffer( m_swap_chain_.GetFrameIndex() ).Get();
	resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	resource_barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_command_list_.Get()->ResourceBarrier( 1, &resource_barrier );

	D3D12_CPU_DESCRIPTOR_HANDLE h_cpu_rtv = {};
	h_cpu_rtv.ptr = m_frame_buffer_.GetHeap()->GetCPUDescriptorHandleForHeapStart().ptr + m_swap_chain_.GetFrameIndex() * m_frame_buffer_.GetDescriptSize();
	D3D12_CPU_DESCRIPTOR_HANDLE h_cpu_dsv = {};
	h_cpu_dsv.ptr = m_depth_stencil_buffer_.GetHeap()->GetCPUDescriptorHandleForHeapStart().ptr;
	m_command_list_.Get()->OMSetRenderTargets( 1, &h_cpu_rtv, TRUE, &h_cpu_dsv );

	m_command_list_.Get()->ClearRenderTargetView( h_cpu_rtv, frame_buffer_clear_color, 0, nullptr );
	m_command_list_.Get()->ClearDepthStencilView( h_cpu_dsv, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr );

#elif GRAPHIC_VERSION_DIRECTX_11

	m_context_.Get()->ClearRenderTargetView(
		m_frame_buffer_.Get().Get(),
		frame_buffer_clear_color
	);

#endif

}


void Graphic::EndRender() {

#if GRAPHIC_VERSION_DIRECTX_12

	D3D12_RESOURCE_BARRIER	resource_barrier = {};
	resource_barrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = m_frame_buffer_.GetFrameBaffer( m_swap_chain_.GetFrameIndex() ).Get();
	resource_barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	resource_barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_PRESENT;
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_command_list_.Get()->ResourceBarrier( 1, &resource_barrier );

	m_command_list_.Get()->Close();

	ID3D12CommandList *p_command_lists[] = { m_command_list_.Get().Get() };
	m_command_queue_.Get()->ExecuteCommandLists( _countof( p_command_lists ), p_command_lists );

	m_swap_chain_.Get()->Present( 1, 0 );

	this->WaitForPreviousFrame();

	m_swap_chain_.ChangeFrameIndex();

#elif GRAPHIC_VERSION_DIRECTX_11

	m_swap_chain_.Get()->Present( 0, 0 );

#endif

}


#if GRAPHIC_VERSION_DIRECTX_12

void Graphic::WaitForPreviousFrame() {

	HRESULT hr = S_OK;

	const u64 fence_value = m_fence_.GetFenceValue();
	hr = m_command_queue_.Get()->Signal( m_fence_.Get().Get(), fence_value );
	if( FAILED( hr ) ) {
		return;
	}
	m_fence_.AddFenceValue();

	if( m_fence_.Get()->GetCompletedValue() < fence_value ) {

		hr = m_fence_.Get()->SetEventOnCompletion( fence_value, m_fence_.GetEventHandle() );
		if( FAILED( hr ) ) {
			return;
		}

		WaitForSingleObject( m_fence_.GetEventHandle(), INFINITE );

	}

}

#endif


#pragma endregion


}
}
}


//**************************************************
//	End of File.
//**************************************************