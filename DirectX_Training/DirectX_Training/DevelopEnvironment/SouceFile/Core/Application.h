//**************************************************
//!	@file	Application.h
//!	@brief	�A�v���P�[�V�����N���X
//!	@date	2019 / 09 / 08
//!	@author	t_hashimoto
//**************************************************

#pragma once

//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------

// Window
#include	"Window.h"


namespace th {
namespace core {


/**
*	@class	Application
*	@brief	�A�v���P�[�V�����N���X
**/
class Application {

#pragma region Singleton.

public:
	static Application& GetInstance() {
		static Application instance;
		return instance;
	}

private:
	Application() {}
	~Application() {}

	Application( const Application& ) {}
	const Application& operator=( const Application& ) { return *this; }

#pragma endregion

public:
	s32 Initialize( HINSTANCE );
	void Finalize();

private:
	Window		m_wnd_;

};


}
}


//**************************************************
//	End of File.
//**************************************************