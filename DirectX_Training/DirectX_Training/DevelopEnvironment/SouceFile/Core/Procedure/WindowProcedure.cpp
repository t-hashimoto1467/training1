//**************************************************
//!	@file	WindowProcedure.cpp
//!	@brief	�E�B���h�E�v���V�[�W��
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
//!	@brief	�E�C���h�E�v���V�[�W��
//!	@param	[in] HWND : �E�C���h�E�n���h��
//!	@param	[in] UINT : ���b�Z�[�W
//!	@param	[in] WPARAM : W�p�����[�^
//!	@param	[in] LPARAM : L�p�����[�^
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