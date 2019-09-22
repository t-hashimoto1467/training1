//**************************************************
//!	@file	Window.h
//!	@brief	ウィンドウクラス
//!	@date	2019 / 09 / 08
//!	@author	t_hashimoto
//**************************************************

#pragma once

namespace th {
namespace core {


/**
*	@class	Window
*	@brief	ウィンドウクラス
**/
class Window {

public:
	Window();
	~Window();

	s32 Create( HINSTANCE, LPCTSTR, LPCTSTR );
	s32 Create( HINSTANCE, LPCTSTR, LPCTSTR, s32, s32 );

	void Show();
	void Hide();

	HWND GetWndHandle() const {
		return m_h_wnd_;
	}
	bool IsWndMode() const {
		return m_wnd_mode_flg_;
	}

private:
	s32 RegistWndClass( HINSTANCE, LPCTSTR );

private:
	HWND	m_h_wnd_;

	bool	m_wnd_mode_flg_;

};


}
}


//**************************************************
//	End of File.
//**************************************************