// DlgUserMng.cpp : 实现文件
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgUserMng.h"

// CDlgUserMng 对话框

IMPLEMENT_DYNAMIC(CDlgUserMng, CDialog)

CDlgUserMng::CDlgUserMng(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserMng::IDD, pParent)
	, m_userName(_T(""))
	, m_passWord(_T(""))
	, m_cmbGradeTxt(_T(""))
{

}

CDlgUserMng::~CDlgUserMng()
{
}

void CDlgUserMng::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_lstUser);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_userName);
	DDX_Text(pDX, IDC_EDIT_PSW, m_passWord);
	DDX_Control(pDX, IDC_COMBO_GRADE, m_cmbGrade);
	DDX_CBString(pDX, IDC_COMBO_GRADE, m_cmbGradeTxt);
}


BEGIN_MESSAGE_MAP(CDlgUserMng, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgUserMng::OnCbnSelchangeCombo1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USER, &CDlgUserMng::OnLvnItemchangedListUser)
	ON_BN_CLICKED(IDC_BTN_USER_ADD, &CDlgUserMng::OnBnClickedBtnUserAdd)
	ON_BN_CLICKED(IDC_BTN_USER_DEL, &CDlgUserMng::OnBnClickedBtnUserDel)
	ON_BN_CLICKED(IDC_BTN_USER_UPD, &CDlgUserMng::OnBnClickedBtnUserUpd)
END_MESSAGE_MAP()


// CDlgUserMng 消息处理程序
void CDlgUserMng::Encrypt(char *dest, const char *src, int len)
{
    for(int i = 0; i < len; i++)
	{
		char tmpCh = src[i];
		int iTmp = 0;
		iTmp = (tmpCh >> 4) & 0xF;
		dest[2*i] = iTmp + 'A';
		iTmp = tmpCh & 0xF;
		dest[2*i+1] = iTmp + 'A';
	}
}

void CDlgUserMng::Decrypt(char *dest, const char *src, int len)
{
	for(int i = 0; i < len/2; i++)
	{
		dest[i] = (src[2*i] - 'A')*16 + (src[2*i+1] - 'A');
	}
}

BOOL CDlgUserMng::LoadUserData()
{
	m_mapUserData.clear();
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
		if("用户" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		//pos_ini.Next();	
		int iCount = 0;
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
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
			iCount += 1;
			//序号
			m_lstUser.InsertItem(iCount-1, str_id);
			//用户名
			m_lstUser.SetItemText(iCount-1, 1, _T(vStr[0].c_str()));
			//密码
			m_lstUser.SetItemText(iCount-1, 2, _T(ud.pswd.c_str()));
			//等级
			m_lstUser.SetItemText(iCount-1, 3, _T(vStr[2].c_str()));
		}
	}
	return TRUE;
}

BOOL CDlgUserMng::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
	char stmp[256];
	memset(stmp,0,sizeof(stmp));
	Encrypt(stmp,"admin123",8);
	m_cmbGrade.AddString("0");
	m_cmbGrade.AddString("1");
	m_cmbGrade.AddString("2");
	m_cmbGrade.AddString("3");
	//
    DWORD dwStyle = m_lstUser.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstUser.SetExtendedStyle(dwStyle);	
	CRect rect;
	m_lstUser.GetClientRect(&rect); //获得当前客户区信息 
    m_lstUser.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rect.Width()/8);
    m_lstUser.InsertColumn(1, _T("用户名"), LVCFMT_LEFT, rect.Width()/4);
	m_lstUser.InsertColumn(2, _T("密码"), LVCFMT_LEFT, rect.Width()/4);
	m_lstUser.InsertColumn(3, _T("等级"), LVCFMT_LEFT, rect.Width()/4);
    ////
	LoadUserData();
	//if(m_mapUserData.size() == 0)
	//{
	//	BOOL bol = WritePrivateProfileString("用户","1","admin,GBGEGNGJGODBDCDD,0",".\\cfg\\user.ini");
	//	LoadUserData();
	//}
	//
	GetDlgItem(IDC_BTN_USER_DEL)->EnableWindow(FALSE);
	//
	return TRUE;
}
void CDlgUserMng::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgUserMng::OnLvnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION ps;
	int nIndex;
	ps=m_lstUser.GetFirstSelectedItemPosition();
	nIndex=m_lstUser.GetNextSelectedItem(ps);
	m_userName = m_lstUser.GetItemText(nIndex,1);
	m_passWord   = m_lstUser.GetItemText(nIndex,2);
	CString strTmp = m_lstUser.GetItemText(nIndex,3);
	m_cmbGradeTxt = strTmp;
	//m_cmbGrade.SetWindowTextA(strTmp);
	UpdateData(FALSE);
	//
	*pResult = 0;
}

void CDlgUserMng::OnBnClickedBtnUserAdd()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	char stmp[256];
	char sitem[16];
	char spath[256];
	memset(stmp,0,sizeof(stmp));
	memset(sitem,0,sizeof(sitem));
	memset(spath,0,sizeof(spath));
	//合法性判断
	int len = m_userName.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("用户名不合法！");
		return ;
	}
    len = m_passWord.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("密码不合法！");
		return ;
	}
    len = m_cmbGradeTxt.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("等级不合法！");
		return ;
	}
	//写配置文件
	int lstCount = m_lstUser.GetItemCount();
	sprintf(sitem,"%d",lstCount+1);
	strcpy(spath,".\\cfg\\user.ini");
	char dest[256];
	memset(dest,0,sizeof(dest));
	Encrypt(dest,m_passWord.GetBuffer(),m_passWord.GetLength());
	sprintf(stmp,"%s,%s,%s",m_userName.GetBuffer(),dest,m_cmbGradeTxt.GetBuffer());
	
	BOOL bFind = FALSE;
	for(int i = 0; i < lstCount; i++)
	{
		CString strUserNo = m_lstUser.GetItemText(i,1);
		if(strUserNo == m_userName)
		{
			bFind = TRUE;
			break;
		}
	}
	if(FALSE == bFind)
	{
		BOOL bol = WritePrivateProfileString("用户",sitem,stmp,spath);
		//序号
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",lstCount+1);
		m_lstUser.InsertItem(lstCount, _T(stmp));
		//用户名
		m_lstUser.SetItemText(lstCount, 1, m_userName);
		//密码
		m_lstUser.SetItemText(lstCount, 2, m_passWord);
		//等级
		m_lstUser.SetItemText(lstCount, 3, m_cmbGradeTxt);
		//
	}
	else
	{
		MessageBox("该用户已存在！");
		return;
	}
	MessageBox("添加成功！");
}

void CDlgUserMng::OnBnClickedBtnUserDel()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgUserMng::OnBnClickedBtnUserUpd()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	char stmp[256];
	char sitem[16];
	char spath[256];
	memset(stmp,0,sizeof(stmp));
	memset(sitem,0,sizeof(sitem));
	memset(spath,0,sizeof(spath));
	//合法性判断
	int len = m_userName.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("用户名不合法！");
		return ;
	}
    len = m_passWord.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("密码不合法！");
		return ;
	}
    len = m_cmbGradeTxt.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("等级不合法！");
		return ;
	}
	//写配置文件
	int lstCount = m_lstUser.GetItemCount();
	sprintf(sitem,"%d",lstCount+1);
	strcpy(spath,".\\cfg\\user.ini");
	char dest[256];
	memset(dest,0,sizeof(dest));
	Encrypt(dest,m_passWord.GetBuffer(),m_passWord.GetLength());
	sprintf(stmp,"%s,%s,%s",m_userName.GetBuffer(),dest,m_cmbGradeTxt.GetBuffer());
	
	BOOL bFind = FALSE;
	for(int i = 0; i < lstCount; i++)
	{
		CString strUserNo = m_lstUser.GetItemText(i,1);
		if(strUserNo == m_userName)
		{
			//密码
			m_lstUser.SetItemText(i, 2, m_passWord);
			//等级
			m_lstUser.SetItemText(i, 3, m_cmbGradeTxt);
			bFind = TRUE;
			sprintf(sitem,"%d",i+1);
			BOOL bol = WritePrivateProfileString("用户",sitem,stmp,spath);
			break;
		}
	}
	
	if(FALSE == bFind)
	{
		MessageBox("该用户不存在！");
		//
		return;
	}
	MessageBox("更新成功！");
}
