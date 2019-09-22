//**************************************************
//!	@file	WindowProcedure.cpp
//!	@brief	ウィンドウプロシージャ
//!	@date	2019 / 09 / 08
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------
#include	"WindowProcedure.h"

namespace th {
namespace core {
namespace procedure {


//==================================================
//!	@brief	ウインドウプロシージャ
//!	@param	[in] HWND : ウインドウハンドル
//!	@param	[in] UINT : メッセージ
//!	@param	[in] WPARAM : Wパラメータ
//!	@param	[in] LPARAM : Lパラメータ
//!	@return	LRESULT
//==================================================
LRESULT CALLBACK WndProc(
	HWND	in_h_wnd,
	UINT	in_msg,
	WPARAM	in_wparam,
	LPARAM	in_lparam
) {

	switch( in_msg ) {

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_KEYDOWN:
			switch( in_wparam ) {

				case VK_ESCAPE:
					DestroyWindow( in_h_wnd );
					break;

			}
			break;

		default:
			return DefWindowProc( in_h_wnd, in_msg, in_wparam, in_lparam );

	}

	return 0;

}


}
}
}

//**************************************************
//	End of File.
//**************************************************