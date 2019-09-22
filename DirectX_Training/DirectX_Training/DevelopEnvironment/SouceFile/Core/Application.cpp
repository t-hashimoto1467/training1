//**************************************************
//!	@file	Application.cpp
//!	@brief	�A�v���P�[�V�����N���X
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

	// �E�B���h�E�̏�����
	if( m_wnd_.Create( in_h_instance, _T( "Win32A" ), _T( "" ), 640, 480 ) ) {
		return RETURN_FAILURE;
	}

	// �O���t�B�b�N�X�̏�����
	if( graphic::Graphic::GetInstance().Initialize( m_wnd_.GetWndHandle(), m_wnd_.IsWndMode() ) ) {
		return RETURN_FAILURE;
	}

	// �E�B���h�E��\������
	m_wnd_.Show();

	return RETURN_SUCCESS;

}


void Application::Finalize() {

	// �O���t�B�b�N�X�̏I������
	graphic::Graphic::GetInstance().Finalize();

}


}
}


//**************************************************
//	End of File.
//**************************************************