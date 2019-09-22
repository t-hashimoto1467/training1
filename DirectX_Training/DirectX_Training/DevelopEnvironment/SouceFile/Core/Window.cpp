//**************************************************
//!	@file	Window.cpp
//!	@brief	ウィンドウクラス
//!	@date	2019 / 09 / 08
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"Window.h"

// Procedure
#include	"Procedure/WindowProcedure.h"

namespace th {
namespace core {


Window::Window() {}


Window::~Window() {}


s32 Window::Create(
	HINSTANCE	in_h_instance,
	LPCTSTR		in_app_name,
	LPCTSTR		in_wnd_title
) {

	if( this->RegistWndClass( in_h_instance, in_app_name ) ) {
		return RETURN_FAILURE;
	}

	RECT wnd_rect;
	GetWindowRect( GetDesktopWindow(), &wnd_rect );

	m_h_wnd_ = CreateWindow(
		in_app_name,
		in_wnd_title,
		WS_VISIBLE | WS_POPUP,
		wnd_rect.left,
		wnd_rect.top,
		wnd_rect.right,
		wnd_rect.bottom,
		nullptr,
		( HMENU )nullptr,
		in_h_instance,
		( LPVOID )nullptr
	);

	if( !m_h_wnd_ ) {
		DEBUG_PRINT_STR( "Fail : Create Window.\n" );
		return RETURN_FAILURE;
	}

	m_wnd_mode_flg_ = false;

	return RETURN_SUCCESS;

}


s32 Window::Create(
	HINSTANCE	in_h_instance,
	LPCTSTR		in_app_name,
	LPCTSTR		in_wnd_title,
	s32			in_width,
	s32			in_height
) {

	if( this->RegistWndClass( in_h_instance, in_app_name ) ) {
		return RETURN_FAILURE;
	}

	m_h_wnd_ = CreateWindow(
		in_app_name,
		in_wnd_title,
		WS_CAPTION | WS_SYSMENU,
		0,
		0,
		in_width,
		in_height,
		HWND_DESKTOP,
		( HMENU )nullptr,
		in_h_instance,
		( LPVOID )nullptr
	);

	if( !m_h_wnd_ ) {
		DEBUG_PRINT_STR( "Fail : Create Window.\n" );
		return RETURN_FAILURE;
	}

	RECT	wnd_rect, client_rect;
	GetWindowRect( m_h_wnd_, &wnd_rect );
	GetClientRect( m_h_wnd_, &client_rect );

	SetWindowPos(
		m_h_wnd_,
		nullptr,
		( GetSystemMetrics( SM_CXSCREEN ) >> 1 ) - ( ( ( wnd_rect.right - wnd_rect.left ) - ( client_rect.right - client_rect.left ) + in_width ) >> 1 ),
		( GetSystemMetrics( SM_CYSCREEN ) >> 1 ) - ( ( ( wnd_rect.bottom - wnd_rect.top ) - ( client_rect.bottom - client_rect.top ) + in_height ) >> 1 ),
		in_width,
		in_height,
		SWP_NOZORDER
	);

	m_wnd_mode_flg_ = true;

	return RETURN_SUCCESS;

}


void Window::Show() {

	ShowWindow( m_h_wnd_, SW_SHOW );
	UpdateWindow( m_h_wnd_ );

}


void Window::Hide() {

	ShowWindow( m_h_wnd_, SW_HIDE );
	UpdateWindow( m_h_wnd_ );

}


int Window::RegistWndClass(
	HINSTANCE	in_h_instance,
	LPCTSTR		in_app_name
) {

	WNDCLASSEX	wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = 0;
	wcex.lpfnWndProc = ( WNDPROC )procedure::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = in_h_instance;
	wcex.hIcon = LoadIcon( ( HINSTANCE )nullptr, IDI_APPLICATION );
	wcex.hCursor = LoadCursor( ( HINSTANCE )nullptr, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = in_app_name;
	wcex.hIconSm = LoadIcon( ( HINSTANCE )nullptr, IDI_WINLOGO );

	if( !RegisterClassEx( &wcex ) ) {
		DEBUG_PRINT_STR( "Fail : Regist WindowClass.\n" );
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;

}


}
}


//**************************************************
//	End of File.
//**************************************************