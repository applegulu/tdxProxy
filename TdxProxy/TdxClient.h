// TdxClient.h : CTdxClient ������

#pragma once
#include "resource.h"       // ������



#include "TdxProxy_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CTdxClient

class ATL_NO_VTABLE CTdxClient :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTdxClient, &CLSID_TdxClient>,
	public IDispatchImpl<ITdxClient, &IID_ITdxClient, &LIBID_TdxProxyLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTdxClient()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TDXCLIENT)


BEGIN_COM_MAP(CTdxClient)
	COM_INTERFACE_ENTRY(ITdxClient)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



};

OBJECT_ENTRY_AUTO(__uuidof(TdxClient), CTdxClient)
