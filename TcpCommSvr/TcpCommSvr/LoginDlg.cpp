// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "LoginDlg.h"

int g_userGrade;
// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_loginUser(_T(""))
	, m_loginPassword(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_loginUser);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_loginPassword);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// 
void CLoginDlg::Decrypt(char *dest, const char *src, int len)
{
	for(int i = 0; i < len/2; i++)
	{
		dest[i] = (src[2*i] - 'A')*16 + (src[2*i+1] - 'A');
	}
}

BOOL CLoginDlg::LoadUserData()
{
	m_mapUserData.clear();
	if((_access(".\\cfg\\user.ini", 0 )) == -1)
	{
		return FALSE;
	}
	CString filePath = _T(".\\cfg\\user.ini");
	TIniFile iniFile(filePath);
	TIniFile::SectionIterator pos_ini;

	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	int section_count = v_section.size();
	if(section_count <= 0)
	{
		return FALSE;
	}
	for(int sec_idx = 0; sec_idx < section_count; sec_idx++)
	{	
		std::string& str_mapping_type = v_section[sec_idx];
		if("�û�" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		//pos_ini.Next();	
		int iCount = 0;
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "������" || str_first == "��·��" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_data = pos_ini.Data().second.c_str();
		    std::vector<std::string> vStr;
			SpliteStr(str_data, ",", vStr);
			if (vStr.size() != 3)
			{
				continue;
			}
			//
			stuUserData ud;
			ud.username = vStr[0];
			char stmp[256];
			memset(stmp,0,sizeof(stmp));
			Decrypt(stmp,vStr[1].c_str(),strlen(vStr[1].c_str()));
			ud.pswd = stmp;
			ud.grade = atoi(vStr[2].c_str());
			m_mapUserData[ud.username].username = ud.username;
			m_mapUserData[ud.username].pswd = ud.pswd;
			m_mapUserData[ud.username].grade = ud.grade;
			//
		}
	}
	return TRUE;
}
BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
	LoadUserData();
	if(m_mapUserData.size() == 0)
	{
		BOOL bol = WritePrivateProfileString("�û�","1","admin,GBGEGNGJGODBDCDD,0",".\\cfg\\user.ini");
		LoadUserData();
	}
    //
	return TRUE;  //
}
void CLoginDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CLoginDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	string strName = m_loginUser.GetBuffer();
	string strPswd = m_loginPassword.GetBuffer();
	CString strTmp;
	map<string,stuUserData>::iterator iter;
	iter = m_mapUserData.find(strName);
	if(m_mapUserData.end() != iter)
	{
		if(strPswd == iter->second.pswd)
		{
			g_userGrade = iter->second.grade;
		}
		else
		{
			strTmp = "�û����������";
			GetDlgItem(IDC_STATIC_LOGIN_INFO)->SetWindowTextA(strTmp);
			return;
		}
	}
	else
	{
		strTmp = "�û��������ڣ�";
		GetDlgItem(IDC_STATIC_LOGIN_INFO)->SetWindowTextA(strTmp);
		return;
	}
	OnOK();
}

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_LOGIN_INFO)
	{
		pDC->SetTextColor(RGB(255,0,0));//
		CFont titlefont;
		titlefont.CreatePointFont(100,"����");//
		pDC->SelectObject(&titlefont);//
	}//
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

int CLoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}

void CLoginDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	//
	CRect picRect;
    CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_LOGIN_LOGO); // �õ� Picture Control ���
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // �޸���������Ϊλͼ

	pWnd->GetClientRect(&picRect);
	HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\sz_login_logo.bmp"), 
					IMAGE_BITMAP, 
					picRect.Width(), 
					picRect.Height(),
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	pWnd->SetBitmap(hPic);
	DeleteObject(hPic);
	//
}
