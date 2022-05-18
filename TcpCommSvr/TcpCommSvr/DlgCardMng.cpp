// DlgCardMng.cpp : 实现文件
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgCardMng.h"

// CDlgCardMng 对话框

IMPLEMENT_DYNAMIC(CDlgCardMng, CDialog)

CDlgCardMng::CDlgCardMng(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCardMng::IDD, pParent)
	, m_cardNo(_T(""))
	, m_workNo(_T(""))
	, m_workName(_T(""))
	, m_workType(_T(""))
	, m_deptName(_T(""))
	, m_workPos(_T(""))
{

}

CDlgCardMng::~CDlgCardMng()
{
}

void CDlgCardMng::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_cardNo);
	DDX_Text(pDX, IDC_EDIT2, m_workNo);
	DDX_Text(pDX, IDC_EDIT3, m_workName);
	DDX_Text(pDX, IDC_EDIT4, m_workType);
	DDX_Text(pDX, IDC_EDIT5, m_deptName);
	DDX_Text(pDX, IDC_EDIT6, m_workPos);
	DDX_Control(pDX, IDC_LIST_CARD, m_lstCard);
}


BEGIN_MESSAGE_MAP(CDlgCardMng, CDialog)
	ON_BN_CLICKED(IDC_BTN_CARD_ADD, &CDlgCardMng::OnBnClickedBtnCardAdd)
	ON_BN_CLICKED(IDC_BTN_CARD_DEL, &CDlgCardMng::OnBnClickedBtnCardDel)
	ON_BN_CLICKED(IDC_BTN_CARD_UPD, &CDlgCardMng::OnBnClickedBtnCardUpd)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CDlgCardMng::OnHdnItemchangedListCard)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CARD, &CDlgCardMng::OnLvnItemchangedListCard)
END_MESSAGE_MAP()

//加载卡片与工人的映射关系
BOOL CDlgCardMng::LoadCardSignMapping()
{
	m_lstCard.DeleteAllItems();//清空
	CString filePath = _T(".\\cfg\\devcfg.ini");
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
		if("卡号和工人映射关系" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		pos_ini.Next();
		int iCount = 0;
		char stmp[256];
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_dcqk = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_dcqk, ",", vStr);
			if (vStr.size() != 6)
			{
				continue;
			}
			stuCardSignMapping cardSignMapping;
			memset(&cardSignMapping,0,sizeof(stuCardSignMapping));
			int len = strlen(vStr[0].c_str());
			if(len > 19) len = 19;
			memcpy(cardSignMapping.cardNo,vStr[0].c_str(),len);
			len = strlen(vStr[1].c_str());
			if(len > 19) len = 19;
			memcpy(cardSignMapping.signNo,vStr[1].c_str(),len);
			len = strlen(vStr[2].c_str());
			if(len > 255) len = 255;
			memcpy(cardSignMapping.signName,vStr[2].c_str(),len);
			len = strlen(vStr[3].c_str());
			if(len > 255) len = 255;
			memcpy(cardSignMapping.workType,vStr[3].c_str(),len);
			len = strlen(vStr[4].c_str());
			if(len > 255) len = 255;
			memcpy(cardSignMapping.deptName,vStr[4].c_str(),len);
			len = strlen(vStr[5].c_str());
			if(len > 255) len = 255;
			memcpy(cardSignMapping.workPost,vStr[5].c_str(),len);
			//
			string tmpstr = cardSignMapping.cardNo;
			memcpy(&m_csMap[tmpstr],&cardSignMapping,sizeof(stuCardSignMapping));
			//
			iCount++;
			//序号
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%d",iCount);
			m_lstCard.InsertItem(iCount-1, _T(stmp));
			//卡号
			m_lstCard.SetItemText(iCount-1, 1, _T(vStr[0].c_str()));
			//工号
			m_lstCard.SetItemText(iCount-1, 2, _T(vStr[1].c_str()));
			//姓名
			m_lstCard.SetItemText(iCount-1, 3, _T(vStr[2].c_str()));
			//工种
			m_lstCard.SetItemText(iCount-1, 4, _T(vStr[3].c_str()));
			//部门
			m_lstCard.SetItemText(iCount-1, 5, _T(vStr[4].c_str()));
			//岗位
			m_lstCard.SetItemText(iCount-1, 6, _T(vStr[5].c_str()));
		}
	}
	return TRUE;
}
BOOL CDlgCardMng::SetListCard()
{
	return TRUE;
}
// CDlgCardMng 消息处理程序
BOOL CDlgCardMng::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
    DWORD dwStyle = m_lstCard.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstCard.SetExtendedStyle(dwStyle);	
	CRect rect;
	m_lstCard.GetClientRect(&rect); //获得当前客户区信息 
    m_lstCard.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rect.Width()/14);
    m_lstCard.InsertColumn(1, _T("卡号"), LVCFMT_LEFT, rect.Width()*3/14);
	m_lstCard.InsertColumn(2, _T("工号"), LVCFMT_LEFT, rect.Width()/7);
	m_lstCard.InsertColumn(3, _T("姓名"), LVCFMT_LEFT, rect.Width()/7);
	m_lstCard.InsertColumn(4, _T("工种"), LVCFMT_LEFT, rect.Width()/7);
	m_lstCard.InsertColumn(5, _T("部门"), LVCFMT_LEFT, rect.Width()/7);
    m_lstCard.InsertColumn(6, _T("岗位"), LVCFMT_LEFT, rect.Width()/7); 
    ////
	LoadCardSignMapping();
	//
	SetListCard();
	//
	//GetDlgItem(IDC_BTN_CARD_DEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_CARD_DEL)->EnableWindow(FALSE);
	return TRUE;
}
void CDlgCardMng::OnBnClickedBtnCardAdd()
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
	int len = m_cardNo.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("卡号不合法！");
		return ;
	}
    len = m_workNo.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("工号不合法！");
		return ;
	}
    len = m_workName.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("姓名不合法！");
		return ;
	}
	len = m_workType.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("工种不合法！");
		return ;
	}
    len = m_deptName.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("部门不合法！");
		return ;
	}
    len = m_workPos.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("工种不合法！");
		return ;
	}
	//写配置文件
	int lstCount = m_lstCard.GetItemCount();
	sprintf(sitem,"%d",lstCount+1);
	strcpy(spath,".\\cfg\\devcfg.ini");
	sprintf(stmp,"%s,%s,%s,%s,%s,%s",m_cardNo.GetBuffer(),m_workNo.GetBuffer(),m_workName.GetBuffer(),m_workType.GetBuffer(),m_deptName.GetBuffer(),m_workPos.GetBuffer());
	
	BOOL bFind = FALSE;
	for(int i = 0; i < lstCount; i++)
	{
		CString strCardNo = m_lstCard.GetItemText(i,1);
		if(strCardNo == m_cardNo)
		{
			bFind = TRUE;
			break;
		}
	}
	if(FALSE == bFind)
	{
		BOOL bol = WritePrivateProfileString("卡号和工人映射关系",sitem,stmp,spath);
		//序号
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",lstCount+1);
		m_lstCard.InsertItem(lstCount, _T(stmp));
		//卡号
		m_lstCard.SetItemText(lstCount, 1, m_cardNo);
		//工号
		m_lstCard.SetItemText(lstCount, 2, m_workNo);
		//姓名
		m_lstCard.SetItemText(lstCount, 3, m_workName);
		//工种
		m_lstCard.SetItemText(lstCount, 4, m_workType);
		//部门
		m_lstCard.SetItemText(lstCount, 5, m_deptName);
		//岗位
		m_lstCard.SetItemText(lstCount, 6, m_workPos);
		//
	}
	else
	{
		MessageBox("该卡号已存在！");
		return;
	}
	MessageBox("添加成功！");
}

void CDlgCardMng::OnBnClickedBtnCardDel()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgCardMng::OnBnClickedBtnCardUpd()
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
	int len = m_cardNo.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("卡号不合法！");
		return ;
	}
    len = m_workNo.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("工号不合法！");
		return ;
	}
    len = m_workName.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("姓名不合法！");
		return ;
	}
	len = m_workType.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("工种不合法！");
		return ;
	}
    len = m_deptName.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("部门不合法！");
		return ;
	}
    len = m_workPos.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("工种不合法！");
		return ;
	}
	//写配置文件
	int lstCount = m_lstCard.GetItemCount();
	strcpy(spath,".\\cfg\\devcfg.ini");
	sprintf(stmp,"%s,%s,%s,%s,%s,%s",m_cardNo.GetBuffer(),m_workNo.GetBuffer(),m_workName.GetBuffer(),m_workType.GetBuffer(),m_deptName.GetBuffer(),m_workPos.GetBuffer());
	
	BOOL bFind = FALSE;
	for(int i = 0; i < lstCount; i++)
	{
		CString strCardNo = m_lstCard.GetItemText(i,1);
		if(strCardNo == m_cardNo)
		{
			m_lstCard.SetItemText(i, 2,m_workNo);
			m_lstCard.SetItemText(i, 3,m_workName);
			m_lstCard.SetItemText(i, 4,m_workType);
			m_lstCard.SetItemText(i, 5,m_deptName);
			m_lstCard.SetItemText(i, 6,m_workPos);
			bFind = TRUE;
			sprintf(sitem,"%d",i+1);
			BOOL bol = WritePrivateProfileString("卡号和工人映射关系",sitem,stmp,spath);
			break;
		}
	}
	if(FALSE == bFind)
	{
		MessageBox("该卡号不存在！");
		//
		return;
	}
	MessageBox("更新成功！");
}

void CDlgCardMng::OnHdnItemchangedListCard(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}

void CDlgCardMng::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

}

void CDlgCardMng::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码

}

int CDlgCardMng::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CDlgCardMng::OnLvnItemchangedListCard(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION ps;
	int nIndex;
	ps=m_lstCard.GetFirstSelectedItemPosition();
	nIndex=m_lstCard.GetNextSelectedItem(ps);
	m_cardNo = m_lstCard.GetItemText(nIndex,1);
	m_workNo   = m_lstCard.GetItemText(nIndex,2);
	m_workName = m_lstCard.GetItemText(nIndex,3);
	m_workType = m_lstCard.GetItemText(nIndex,4);
	m_deptName = m_lstCard.GetItemText(nIndex,5);
	m_workPos = m_lstCard.GetItemText(nIndex,6);
	UpdateData(FALSE);
	*pResult = 0;
}
