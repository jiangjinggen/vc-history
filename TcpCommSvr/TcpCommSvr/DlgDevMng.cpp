// DlgDevMng.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgDevMng.h""

// CDlgDevMng �Ի���

IMPLEMENT_DYNAMIC(CDlgDevMng, CDialog)

CDlgDevMng::CDlgDevMng(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevMng::IDD, pParent)
	, m_devNo(_T(""))
	, m_zoneNo(_T(""))
	, m_zoneName(_T(""))
{

}

CDlgDevMng::~CDlgDevMng()
{
}

void CDlgDevMng::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEV_NO, m_devNo);
	DDX_Text(pDX, IDC_EDIT_DEV_ZONE_NO, m_zoneNo);
	DDX_Text(pDX, IDC_EDIT_DEV_ZONE_NAME, m_zoneName);
	DDX_Control(pDX, IDC_LIST_DEV, m_lstDev);
}


BEGIN_MESSAGE_MAP(CDlgDevMng, CDialog)
	ON_BN_CLICKED(IDC_BTN_DEV_ADD, &CDlgDevMng::OnBnClickedBtnDevAdd)
	ON_BN_CLICKED(IDC_BTN_DEV_DEL, &CDlgDevMng::OnBnClickedBtnDevDel)
	ON_BN_CLICKED(IDC_BTN_DEV_UPD, &CDlgDevMng::OnBnClickedBtnDevUpd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DEV, &CDlgDevMng::OnLvnItemchangedListDev)
END_MESSAGE_MAP()


//���ض�����������ӳ���ϵ
BOOL CDlgDevMng::LoadReaderAreaMapping()
{
	m_lstDev.DeleteAllItems();//���
	m_raMap.clear();
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
	int iCount = 0;
	char stmp[256];
	for(int sec_idx = 0; sec_idx < section_count; sec_idx++)
	{	
		std::string& str_mapping_type = v_section[sec_idx];
		if("������������ӳ���ϵ" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		pos_ini.Next();	
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "������" || str_first == "��·��" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_dcqk = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_dcqk, ",", vStr);
			if (vStr.size() != 3)
			{
				continue;
			}
			stuReaderAreaMapping readerAreaMapping;
			memset(&readerAreaMapping,0,sizeof(stuReaderAreaMapping));
			readerAreaMapping.readerNo = atoi(vStr[0].c_str());
			int len = strlen(vStr[1].c_str());
			if(len > 3) len = 3;
			//memcpy(readerAreaMapping.areaNo,vStr[1].c_str(),len);
			readerAreaMapping.areaNo = atoi(vStr[1].c_str());
			len = strlen(vStr[2].c_str());
			if(len > 255) len = 255;
			memcpy(readerAreaMapping.areaName,vStr[2].c_str(),len);
			//
			readerAreaMapping.man_cnt = 0;
			//
			memcpy(&m_raMap[readerAreaMapping.readerNo],&readerAreaMapping,sizeof(stuReaderAreaMapping));
			//
			iCount++;
			//���
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%d",iCount);
			m_lstDev.InsertItem(iCount-1, _T(stmp));
			//������
			m_lstDev.SetItemText(iCount-1, 1, _T(vStr[0].c_str()));
			//��������
			m_lstDev.SetItemText(iCount-1, 2, _T(vStr[1].c_str()));
			//����
			m_lstDev.SetItemText(iCount-1, 3, _T(vStr[2].c_str()));
		}
	}
	return TRUE;
}

// CDlgZoneMng ��Ϣ�������
BOOL CDlgDevMng::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
    DWORD dwStyle = m_lstDev.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstDev.SetExtendedStyle(dwStyle);	
	CRect rect;
	m_lstDev.GetClientRect(&rect); //��õ�ǰ�ͻ�����Ϣ 
    m_lstDev.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/8);
    m_lstDev.InsertColumn(1, _T("�豸���"), LVCFMT_LEFT, rect.Width()/4);
	m_lstDev.InsertColumn(2, _T("������"), LVCFMT_LEFT, rect.Width()/4);
	m_lstDev.InsertColumn(3, _T("��������"), LVCFMT_LEFT, rect.Width()/4);
    ////
	LoadReaderAreaMapping();
	//
	GetDlgItem(IDC_BTN_DEV_DEL)->EnableWindow(FALSE);
	return TRUE;
}


void CDlgDevMng::OnBnClickedBtnDevAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
	char stmp[256];
	char sitem[16];
	char spath[256];
	memset(stmp,0,sizeof(stmp));
	memset(sitem,0,sizeof(sitem));
	memset(spath,0,sizeof(spath));
	//�Ϸ����ж�
	int len = m_devNo.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("�豸��Ų��Ϸ���");
		return ;
	}
    len = m_zoneNo.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("�����Ų��Ϸ���");
		return ;
	}
    len = m_zoneName.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("�������Ʋ��Ϸ���");
		return ;
	}
	//д�����ļ�
	int lstCount = m_lstDev.GetItemCount();
	sprintf(sitem,"%d",lstCount+1);
	strcpy(spath,".\\cfg\\devcfg.ini");
	sprintf(stmp,"%s,%s,%s",m_devNo.GetBuffer(),m_zoneNo.GetBuffer(),m_zoneName.GetBuffer());
	
	BOOL bFind = FALSE;
	for(int i = 0; i < lstCount; i++)
	{
		CString strDevNo = m_lstDev.GetItemText(i,1);
		if(strDevNo == m_devNo)
		{
			bFind = TRUE;
			break;
		}
	}
	if(FALSE == bFind)
	{
		BOOL bol = WritePrivateProfileString("������������ӳ���ϵ",sitem,stmp,spath);
		//���
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",lstCount+1);
		m_lstDev.InsertItem(lstCount, _T(stmp));
		//�豸���
		m_lstDev.SetItemText(lstCount, 1, m_devNo);
		//������
		m_lstDev.SetItemText(lstCount, 2, m_zoneNo);
		//��������
		m_lstDev.SetItemText(lstCount, 3, m_zoneName);
		//
	}
	else
	{
		MessageBox("���豸����Ѵ��ڣ�");
		return;
	}
	MessageBox("��ӳɹ���");
}

void CDlgDevMng::OnBnClickedBtnDevDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgDevMng::OnBnClickedBtnDevUpd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
	char stmp[256];
	char sitem[16];
	char spath[256];
	memset(stmp,0,sizeof(stmp));
	memset(sitem,0,sizeof(sitem));
	memset(spath,0,sizeof(spath));
	//�Ϸ����ж�
	int len = m_devNo.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("�豸��Ų��Ϸ���");
		return ;
	}
    len = m_zoneNo.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("�����Ų��Ϸ���");
		return ;
	}
    len = m_zoneName.GetLength();
	if(len < 1 || len > 20)
	{
		MessageBox("�������Ʋ��Ϸ���");
		return ;
	}
	//д�����ļ�
	int lstCount = m_lstDev.GetItemCount();
	strcpy(spath,".\\cfg\\devcfg.ini");
	sprintf(stmp,"%s,%s,%s",m_devNo.GetBuffer(),m_zoneNo.GetBuffer(),m_zoneName.GetBuffer());
	
	BOOL bFind = FALSE;
	for(int i = 0; i < lstCount; i++)
	{
		CString strDevNo = m_lstDev.GetItemText(i,1);
		if(strDevNo == m_devNo)
		{
			m_lstDev.SetItemText(i, 2,m_zoneNo);
			m_lstDev.SetItemText(i, 3,m_zoneName);
			bFind = TRUE;
			sprintf(sitem,"%d",i+1);
			BOOL bol = WritePrivateProfileString("������������ӳ���ϵ",sitem,stmp,spath);
			break;
		}
	}
	if(FALSE == bFind)
	{
		MessageBox("���豸��Ų����ڣ�");
		//
		return;
	}
	MessageBox("���³ɹ���");
}

void CDlgDevMng::OnLvnItemchangedListDev(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION ps;
	int nIndex;
	ps=m_lstDev.GetFirstSelectedItemPosition();
	nIndex=m_lstDev.GetNextSelectedItem(ps);
	m_devNo = m_lstDev.GetItemText(nIndex,1);
	m_zoneNo   = m_lstDev.GetItemText(nIndex,2);
	m_zoneName = m_lstDev.GetItemText(nIndex,3);
	UpdateData(FALSE);
	*pResult = 0;
}
