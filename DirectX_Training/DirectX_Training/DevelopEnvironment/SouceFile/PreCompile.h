//**************************************************
//!	@file	PreCompile.h
//!	@brief	プリコンパイル済みヘッダ
//!	@date	2019 / 09 / 08
//!	@author	t_hashimoto
//**************************************************

#pragma once

//--------------------------------------------------
//	Define Macro.
//--------------------------------------------------

// ConstantValue
#define ConstantValue( type, name, value )	constexpr type name = value

// ReturnValue
#define RETURN_SUCCESS	0
#define RETURN_FAILURE	1

// DebugMode
#define DEBUG_ONRY		defined( _DEBUG )
#define DEBUG_AND_CHECK	defined( _DEBUG ) || defined( _CHECK )

// ヘッダーからあまり使われない関数を省くために定義
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


//--------------------------------------------------
//	Include Header File.
//--------------------------------------------------

// OS
#include	<Windows.h>

// I/O
#include	<stdio.h>
#include	<iostream>

// Character
#include	<tchar.h>
#include	<locale.h>
#include	<string>

// STL
#include	<vector>
#include	<list>
#include	<map>
#include	<unordered_map>
#include	<deque>

// Mutex
#include	<mutex>

// Math
#include	<math.h>
#include	<DirectXMath.h>

// Assert
#include	<assert.h>

// Type
#include	"Type.h"

// Debugger
#include	"Debugger/Debugger.h"


//**************************************************
//	End of File.
//**************************************************