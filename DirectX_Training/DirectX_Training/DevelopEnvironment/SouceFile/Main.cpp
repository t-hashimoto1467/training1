//**************************************************
//!	@file	Main.cpp
//!	@brief	�G���g���[�|�C���g
//!	@date	2019 / 09 / 08
//!	@author	t_hashimoto
//**************************************************

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------

// Application
#include	"Core/Application.h"

// Graphic
#include	"Core/Graphic/Graphic.h"

//==================================================
//!	@brief	�G���g���[�|�C���g
//!	@param	[in] HINSTANCE	: �A�v���P�[�V�����C���X�^���X�n���h��
//!	@param	[in] HINSTANCE	: ���݂̃o�[�W�����ł͎g�p����Ȃ�
//!	@param	[in] LPSTR		: �R�}���h���C������
//!	@param	[in] s32		: �����\�����@
//!	@return s32
//!	@retval	EXIT_SUCCESS
//!	@retval	EXIT_FAILURE
//==================================================
s32 APIENTRY WinMain(
	HINSTANCE	in_h_instance,
	HINSTANCE	in_h_instance_prev,
	LPSTR		in_args,
	s32			in_cmd_show
) {

	if( th::core::Application::GetInstance().Initialize( in_h_instance ) ) {
		return EXIT_FAILURE;
	}

	MSG	msg;
	do {

		th::core::graphic::Graphic::GetInstance().OnRender();

		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) ) {

			TranslateMessage( &msg );
			DispatchMessage( &msg );

		}

	} while( msg.message != WM_QUIT );


	th::core::Application::GetInstance().Finalize();

	return EXIT_SUCCESS;

}

//**************************************************
//	End of File.
//**************************************************