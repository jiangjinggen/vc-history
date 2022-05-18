// DlgZoneMng.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgZoneMng.h"

// CDlgZoneMng �Ի���

IMPLEMENT_DYNAMIC(CDlgZoneMng, CDialog)

CDlgZoneMng::CDlgZoneMng(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZoneMng::IDD, pParent)
	, m_zoneNo(_T(""))
	, m_zoneName(_T(""))
{

}

CDlgZoneMng::~CDlgZoneMng()
{
}

void CDlgZoneMng::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ZONE_NO, m_zoneNo);
	DDX_Text(pDX, IDC_EDIT_ZONE_NAME, m_zoneName);
	DDX_Control(pDX, IDC_LIST_ZONE, m_lstZone);
}


BEGIN_MESSAGE_MAP(CDlgZoneMng, CDialog)
	ON_BN_CLICKED(IDC_BTN_ZONE_SAVE, &CDlgZoneMng::OnBnClickedBtnZoneSave)
	ON_BN_CLICKED(IDC_BTN_ZONE_UPD, &CDlgZoneMng::OnBnClickedBtnZoneUpd)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CDlgZoneMng::OnHdnItemchangedListZone)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ZONE, &CDlgZoneMng::OnLvnItemchangedListZone)
END_MESSAGE_MAP()

//���ض�����������ӳ���ϵ
BOOL CDlgZoneMng::LoadReaderAreaMapping()
{
	m_lstZone.DeleteAllItems();//���
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
			m_lstZone.InsertItem(iCount-1, _T(stmp));
			//������
			m_lstZone.SetItemText(iCount-1, 1, _T(vStr[1].c_str()));
			//��������
			m_lstZone.SetItemText(iCount-1, 2, _T(vStr[2].c_str()));
			//����
			map<int,string>::iterator iter;
			int zNo = atoi(vStr[1].c_str());
			iter = m_totAreaMap.find(zNo);
			if(m_totAreaMap.end() == iter)
			{
				m_totAreaMap[zNo] = "��";
				m_lstZone.SetItemText(iCount-1, 3, _T("��"));
			}
			else
			{
				m_lstZone.SetItemText(iCount-1, 3, _T(iter->second.c_str()));
			}
			BOOL bol = WritePrivateProfileString("���Ҷ��������ӳ���ϵ",vStr[1].c_str(),m_totAreaMap[zNo].c_str(),".\\cfg\\devcfg.ini");
		}
	}
	return TRUE;
}
//�����������Ҷ�ӳ���ϵ
BOOL CDlgZoneMng::LoadAreaTotMapping()
{
	m_totAreaMap.clear();
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
		if("���Ҷ��������ӳ���ϵ" != str_mapping_type)continue; 
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
			int id = atoi(str_id.GetBuffer());
			m_totAreaMap[id] = pos_ini.Data().second;
			//
		}
	}
	return TRUE;
}
// CDlgZoneMng ��Ϣ�������
BOOL CDlgZoneMng::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
    DWORD dwStyle = m_lstZone.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstZone.SetExtendedStyle(dwStyle);	
	CRect rect;
	m_lstZone.GetClientRect(&rect); //��õ�ǰ�ͻ�����Ϣ 
    m_lstZone.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/8);
    m_lstZone.InsertColumn(1, _T("������"), LVCFMT_LEFT, rect.Width()/4);
	m_lstZone.InsertColumn(2, _T("��������"), LVCFMT_LEFT, rect.Width()/4);
	m_lstZone.InsertColumn(3, _T("����"), LVCFMT_LEFT, rect.Width()/4);
    ////
	LoadAreaTotMapping();
	LoadReaderAreaMapping();
	//
	GetDlgItem(IDC_EDIT_ZONE_NO)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ZONE_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ZONE_SAVE)->EnableWindow(FALSE);
	return TRUE;
}

void CDlgZoneMng::OnBnClickedBtnZoneSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgZoneMng::OnBnClickedBtnZoneUpd()
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
    int len = m_zoneNo.GetLength();
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
	int lstCount = m_lstZone.GetItemCount();
	strcpy(spath,".\\cfg\\devcfg.ini");
	CString strName;
	if(1 == ((CButton *)GetDlgItem(IDC_RADIO_LEFT))->GetCheck())
	{
		strName = "��";
	}
	else
	{
		strName = "�Ҷ�";
	}
	BOOL bFind = FALSE;
	for(int i = 0; i < lstCount; i++)
	{
		CString strZoneNo = m_lstZone.GetItemText(i,1);
		if(strZoneNo == m_zoneNo)
		{
			m_lstZone.SetItemText(i, 3,strName);
			bFind = TRUE;
			sprintf(sitem,"%s",m_zoneNo.GetBuffer());
			sprintf(stmp,"%s",strName.GetBuffer());
			BOOL bol = WritePrivateProfileString("���Ҷ��������ӳ���ϵ",sitem,stmp,spath);
			break;
		}
	}
	if(FALSE == bFind)
	{
		MessageBox("�������Ų����ڣ�");
		//
		return;
	}
	MessageBox("���³ɹ���");
}

void CDlgZoneMng::OnHdnItemchangedListZone(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}

void CDlgZoneMng::OnLvnItemchangedListZone(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION ps;
	int nIndex;
	ps=m_lstZone.GetFirstSelectedItemPosition();
	nIndex=m_lstZone.GetNextSelectedItem(ps);
	m_zoneNo = m_lstZone.GetItemText(nIndex,1);
	m_zoneName   = m_lstZone.GetItemText(nIndex,2);
	CString strTmp = m_lstZone.GetItemText(nIndex,3);
	if("��" == strTmp)
	{
		((CButton*)GetDlgItem(IDC_RADIO_LEFT))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_RADIO_RIGHT))->SetCheck(BST_UNCHECKED);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_RIGHT))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_RADIO_LEFT))->SetCheck(BST_UNCHECKED);
	}
	UpdateData(FALSE);
	//
	*pResult = 0;
}
