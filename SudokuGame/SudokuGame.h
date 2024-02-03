
// SudokuGame.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CSudokuGameApp:
// 有关此类的实现，请参阅 SudokuGame.cpp
//

class CSudokuGameApp : public CWinApp
{
public:
	CSudokuGameApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSudokuGameApp theApp;

#define WM_MSGRECVPRO WM_USER+1

enum MSGUSER
{
	MSGUSER_AUTOCALCCOUNT = 0,
	MSGUSER_AUTOCALCRESULT
};
