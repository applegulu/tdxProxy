// dllmain.cpp : DllMain ��ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "TdxProxy_i.h"
#include "dllmain.h"
#include "compreg.h"

CTdxProxyModule _AtlModule;

class CTdxProxyApp : public CWinApp
{
public:

// ��д
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CTdxProxyApp, CWinApp)
END_MESSAGE_MAP()

CTdxProxyApp theApp;

BOOL CTdxProxyApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CTdxProxyApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
