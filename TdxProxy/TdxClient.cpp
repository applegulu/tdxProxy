// TdxClient.cpp : CTdxClient ��ʵ��

#include "stdafx.h"
#include "TdxClient.h"


// CTdxClient

/// <summary>
/// ��ʼ��ͨ����ʵ��
/// </summary>
typedef void(__stdcall* Tdx_InitDelegate)();
/// <summary>
/// �˳�ͨ����ʵ��
/// </summary>
typedef void(__stdcall* Tdx_ExitDelegate)();

/// <summary>
/// �����˻���¼
/// </summary>
/// <param name="IP">���׷�����IP, ��ͨ��ȯ��ͨ���ŵ�¼�������ͨѶ���ò�ѯ�ĸ����׷������ٶ����</param>
/// <param name="Port">���׷������˿�</param>
/// <param name="Version">��������ͨ���ŵİ汾�ţ�������̫�ͣ�ȯ�̷���������Ϊʹ�õ�ͨ�������̫��Ҫ����</param>
/// <param name="AccountNo">�ʽ��˺�</param>
/// <param name="JyPassword">��������</param>
/// <param name="TxPassword">ͨѶ����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�����</param>
/// <returns>�ͻ���ID��ʧ��ʱ����-1</returns>
typedef int(__stdcall* Tdx_LoginDelegate)(BSTR IP, short Port, BSTR Version, BSTR AccountNo, BSTR JyPassword, BSTR TxPassword, BSTR* ErrInfo);

/// <summary>
/// �����˻�ע��
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
typedef void(__stdcall* Tdx_LogoutDelegate)(int ClientID);

/// <summary>
/// ��ѯ���ֽ�������
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">��ʾ��ѯ��Ϣ�����࣬0�ʽ�  1�ɷ�   2����ί��  3���ճɽ�     4�ɳ���   5�ɶ�����  6�������   7��ȯ���  8����֤ȯ</param>
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��صĲ�ѯ����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_QueryDataDelegate)(int ClientID, int Category, char* Result, char* ErrInfo);

//�����ѯ�ɶ�����ʱ���صĽ���ַ������� "�ɶ�����\t�ɶ�����\t�ʺ����\t������Ϣ\n0000064567\t\t0\t\nA000064567\t\t1\t\n2000064567\t\t2\t\nB000064567\t\t3\t"
//��ô�����֮��ͨ���ָ��ַ����� ���Իָ�Ϊ���м��еı����ʽ������


/// <summary>
/// ���������湦��,��ѯ������ʷ����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">��ʾ��ѯ��Ϣ�����࣬0��ʷί��  1��ʷ�ɽ�   2���</param>
/// <param name="StartDate">��ʾ��ʼ���ڣ���ʽΪyyyyMMdd,����2014��3��1��Ϊ  20140301
/// <param name="EndDate">��ʾ�������ڣ���ʽΪyyyyMMdd,����2014��3��1��Ϊ  20140301
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��صĲ�ѯ����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_QueryHistoryDataDelegate)(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);

/// <summary>
/// ���������湦��,������ѯ���ֽ�������,�����鴫��ÿ��ί�еĲ����������i��Ԫ�ر�ʾ��i����ѯ����Ӧ����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">��Ϣ�����������, ��i��Ԫ�ر�ʾ��i����ѯ����Ϣ���࣬0�ʽ�  1�ɷ�   2����ί��  3���ճɽ�     4�ɳ���   5�ɶ�����  6�������   7��ȯ���  8����֤ȯ</param>
/// <param name="Count">��ѯ�ĸ�����������ĳ���</param>
/// <param name="Result">�������ݵ�����, ��i��Ԫ�ر�ʾ��i��ί�еķ�����Ϣ. ��APIִ�з��غ�Result[i]�ڱ����˷��صĲ�ѯ����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">������Ϣ�����飬��i��Ԫ�ر�ʾ��i��ί�еĴ�����Ϣ. ��APIִ�з��غ��������ErrInfo[i]�����˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_QueryBatDataDelegate)(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);



/// <summary>
/// ��ί�н���֤ȯ
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">��ʾί�е����࣬0���� 1����  2��������  3��ȯ����   4��ȯ��ȯ   5��ȯ����  6��ȯ��ȯ</param>
/// <param name="PriceType">��ʾ���۷�ʽ 0  �Ϻ��޼�ί�� �����޼�ί�� 1���ڶԷ����ż۸�  2���ڱ������ż۸�  3���ڼ�ʱ�ɽ�ʣ�೷��  4�Ϻ��嵵����ʣ�� �����嵵����ʣ�� 5����ȫ��ɽ����� 6�Ϻ��嵵����ת�޼�
/// <param name="Gddm">�ɶ�����, �����Ϻ���Ʊ���Ϻ��Ĺɶ����룻�������ڵĹ�Ʊ�������ڵĹɶ�����</param>
/// <param name="Zqdm">֤ȯ����</param>
/// <param name="Price">ί�м۸�</param>
/// <param name="Quantity">ί������</param>
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��ص�ί�б�ŵ�����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_SendOrderDelegate)(int ClientID, int Category, int PriceType, char* Gddm, char* Zqdm, float Price, int Quantity, char* Result, char* ErrInfo);



/// <summary>
/// ���������湦��,������ί�н���֤ȯ�������鴫��ÿ��ί�еĲ����������i��Ԫ�ر�ʾ��i��ί�е���Ӧ����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Category">ί����������飬��i��Ԫ�ر�ʾ��i��ί�е����࣬0���� 1����  2��������  3��ȯ����   4��ȯ��ȯ   5��ȯ����  6��ȯ��ȯ</param>
/// <param name="PriceType">��ʾ���۷�ʽ������,  ��i��Ԫ�ر�ʾ��i��ί�еı��۷�ʽ, 0  �Ϻ��޼�ί�� �����޼�ί�� 1���ڶԷ����ż۸�  2���ڱ������ż۸�  3���ڼ�ʱ�ɽ�ʣ�೷��  4�Ϻ��嵵����ʣ�� �����嵵����ʣ�� 5����ȫ��ɽ����� 6�Ϻ��嵵����ת�޼�
/// <param name="Gddm">�ɶ��������飬��i��Ԫ�ر�ʾ��i��ί�еĹɶ����룬�����Ϻ���Ʊ���Ϻ��Ĺɶ����룻�������ڵĹ�Ʊ�������ڵĹɶ�����</param>
/// <param name="Zqdm">֤ȯ�������飬��i��Ԫ�ر�ʾ��i��ί�е�֤ȯ����</param>
/// <param name="Price">ί�м۸����飬��i��Ԫ�ر�ʾ��i��ί�е�ί�м۸�</param>
/// <param name="Quantity">ί���������飬��i��Ԫ�ر�ʾ��i��ί�е�ί������</param>
/// <param name="Count">ί�еĸ�����������ĳ���</param>
/// <param name="Result">�������ݵ����飬��i��Ԫ�ر�ʾ��i��ί�еķ�����Ϣ����APIִ�з��غ�Result�ڱ����˷��ص�ί�б�ŵ�����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">������Ϣ�����飬��i��Ԫ�ر�ʾ��i��ί�еĴ�����Ϣ����APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_SendBatOrdersDelegate)(int ClientID, int Category[], int PriceType[], char* Gddm[], char* Zqdm[], float Price[], int Quantity[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// ��ί��
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="hth">��ʾҪ����Ŀ��ί�еı��</param>
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��صĲ�ѯ����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_CancelOrderDelegate)(int ClientID, char* hth, char* Result, char* ErrInfo);//����֤ȯ���ð汾




/// <summary>
/// ���������湦��,������ί��, �����鴫��ÿ��ί�еĲ����������i��Ԫ�ر�ʾ��i����ί�е���Ӧ����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="hth">��ʾҪ����Ŀ��ί�еı��</param>
/// <param name="Count">��ί�еĸ�����������ĳ���</param>
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��صĲ�ѯ����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_CancelBatOrdersDelegate)(int ClientID, char* hth[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// ��ȡ֤ȯ��ʵʱ�嵵����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Zqdm">֤ȯ����</param>
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��صĲ�ѯ����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_GetSSHQDelegate)(int ClientID, char* Zqdm, char* Result, char* ErrInfo);


/// <summary>
/// ���������湦��,������ȡ֤ȯ��ʵʱ�嵵����
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Zqdm">֤ȯ����</param>
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��صĲ�ѯ����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_GetBatSSHQDelegate)(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// ������ȯֱ�ӻ���
/// </summary>
/// <param name="ClientID">�ͻ���ID</param>
/// <param name="Amount">������</param>
/// <param name="Result">��APIִ�з��غ�Result�ڱ����˷��صĲ�ѯ����, ��ʽΪ������ݣ�������֮��ͨ��\n�ַ��ָ������֮��ͨ��\t�ָ���һ��Ҫ����1024*1024�ֽڵĿռ䡣����ʱΪ���ַ�����</param>
/// <param name="ErrInfo">��APIִ�з��غ�������������˴�����Ϣ˵����һ��Ҫ����256�ֽڵĿռ䡣û����ʱΪ���ַ�������ע����ʾ��Ҳ�ô���ʾ����������Ϣ��</param>
typedef void(__stdcall* Tdx_RefundMoneyDelegate)(int ClientID, char* Amount, char* Result, char* ErrInfo);


/// <summary>
/// ��ȡapi���õ�ȯ���˻�����
/// </summary>
/// <param name="Result">��APIִ�з��غ󣬱����˷��ص��˻����ͽ����Ϊһ�ַ�����һ��Ҫ����256�ֽڵĿռ䡣</param>
typedef void(__stdcall* Tdx_GetAccountEditionDelegate)(char* Result);


STDMETHODIMP CTdxClient::Tdx_Login(BSTR IP, SHORT Port, BSTR version, BSTR AccountNo, BSTR JyPassword, BSTR TxPassword, BSTR* ErrInfo, LONG* ClientID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  �ڴ����ʵ�ִ���
	__try{
		Tdx_LoginDelegate Tdx_Login = (Tdx_LoginDelegate)GetProcAddress(hDLL, "TC_Login");
		LONG clientId = (LONG)Tdx_Login(IP, Port, version, AccountNo, JyPassword, TxPassword, ErrInfo);
		ClientID = &clientId;
	}
	__except (1)
	{
	 _EXCEPTION_POINTERS a=	GetExceptionInformation();
	}
	return S_OK;
}


STDMETHODIMP CTdxClient::Tdx_Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO:  �ڴ����ʵ�ִ���
	LPCWSTR libdll = L"Tc.dll";
	hDLL = LoadLibrary(libdll);//���ض�̬���ӿ�MyDll.dll�ļ��� 
	Tdx_InitDelegate	 Tdx_Init = (Tdx_InitDelegate)GetProcAddress(hDLL, "TC_Init_Environ");
	Tdx_Init();
	return S_OK;
}


STDMETHODIMP CTdxClient::Tdx_Exit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  �ڴ����ʵ�ִ���
	Tdx_ExitDelegate Tdx_Exit = (Tdx_ExitDelegate)GetProcAddress(hDLL, "TC_Uninit");
	Tdx_Exit();
	FreeLibrary(hDLL);
	return S_OK;
}
