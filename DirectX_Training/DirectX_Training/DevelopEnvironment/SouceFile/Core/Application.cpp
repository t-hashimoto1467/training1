//**************************************************
//!	@file	Application.cpp
//!	@brief	アプリケーションクラス
//!	@date	2019 / 09 / 08
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"Application.h"

// Graphic
#include	"Graphic/Graphic.h"

namespace th {
namespace core {


s32 Application::Initialize( HINSTANCE in_h_instance ) {

	// ウィンドウの初期化
	if( m_wnd_.Create( in_h_instance, _T( "Win32A" ), _T( "" ), 640, 480 ) ) {
		return RETURN_FAILURE;
	}

	// グラフィックスの初期化
	if( graphic::Graphic::GetInstance().Initialize( m_wnd_.GetWndHandle(), m_wnd_.IsWndMode() ) ) {
		return RETURN_FAILURE;
	}

	// ウィンドウを表示する
	m_wnd_.Show();

	return RETURN_SUCCESS;

}


void Application::Finalize() {

	// グラフィックスの終了処理
	graphic::Graphic::GetInstance().Finalize();

}


}
}


//**************************************************
//	End of File.
//**************************************************