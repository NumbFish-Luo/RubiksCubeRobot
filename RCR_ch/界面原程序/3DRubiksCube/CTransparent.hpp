#pragma once

// https://blog.csdn.net/u011822516/article/details/41946631

#include <windows.h>

typedef BOOL(WINAPI *TRANFUNC)(HWND, COLORREF, BYTE, DWORD);

#define WS_EX_LAYERED	0x00080000  
#define LWA_COLORKEY		0x00000001  
#define LWA_ALPHA		0x00000002  

class CTransparent
{
public:
	static BOOL SetTransparentWnd(HWND hWnd, COLORREF crKey, UINT cAlpha, BOOL dwFlags);
	static BOOL FreeUser32DLL();
	static BOOL ChangeTransparency(HWND hWnd, COLORREF crKey, UINT bAlpha, DWORD dwFlags);
	static BOOL LoadUser32DLL();

	static TRANFUNC m_pFunc;
	static HINSTANCE m_hInstance;
	static BOOL initialized;

};

HINSTANCE CTransparent::m_hInstance = NULL;
TRANFUNC  CTransparent::m_pFunc = NULL;
BOOL CTransparent::initialized = FALSE;
//////////////////////////////////////////////////////////////////////  
// Construction/Destruction  
//////////////////////////////////////////////////////////////////////  
BOOL CTransparent::LoadUser32DLL()
{
	BOOL bRet = FALSE;
	if (!initialized)
	{
		m_hInstance = LoadLibrary("User32.DLL");
		if (m_hInstance)
		{
			//取得SetLayeredWindowAttributes函数指针   
			m_pFunc = (TRANFUNC)GetProcAddress(m_hInstance, "SetLayeredWindowAttributes");
			if (m_pFunc != NULL)
			{
				initialized = TRUE;
				bRet = TRUE;
			}
		}
	}
	return bRet;
}

BOOL CTransparent::ChangeTransparency(HWND hWnd, COLORREF crKey, UINT bAlpha, DWORD dwFlags)
{
	//透明度Sp的值是0-255   
	//其中0为完全透明应该避免出现  
	BOOL bRet = FALSE;
	if (m_pFunc == NULL)
		LoadUser32DLL();

	if (m_pFunc != NULL)
	{
		m_pFunc(hWnd, crKey, bAlpha, dwFlags);
		bRet = TRUE;
	}
	return bRet;
}

BOOL CTransparent::FreeUser32DLL()
{
	if (m_hInstance != NULL)
	{
		FreeLibrary(m_hInstance);
		if (m_pFunc != NULL) //置空  
			m_pFunc = NULL;
		if (initialized != NULL)       //置空  
			initialized = NULL;
	}
	return TRUE;
}

BOOL CTransparent::SetTransparentWnd(HWND hWnd, COLORREF crKey, UINT cAlpha, BOOL dwFlags)
{
	BOOL bRet = TRUE;

	//要使使窗体拥有透明效果,首先要有WS_EX_LAYERED扩展属性 WS_EX_LAYERED = 0x80000   
	//SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);  
	SetWindowLong(hWnd,
		GWL_EXSTYLE,
		GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	if (dwFlags)
	{
		ChangeTransparency(hWnd, crKey, cAlpha, LWA_ALPHA); //设置cAlpha值0-255  
	}
	else
	{
		ChangeTransparency(hWnd, crKey, cAlpha, LWA_COLORKEY);    //crkey相同色变成透明  
	}

	return bRet;
}
