// DlgTblMng.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgTblMng.h"

// CDlgTblMng �Ի���

IMPLEMENT_DYNAMIC(CDlgTblMng, CDialog)

CDlgTblMng::CDlgTblMng(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTblMng::IDD, pParent)
{

}

CDlgTblMng::~CDlgTblMng()
{
}

void CDlgTblMng::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TBL1, m_lstTbl);
}


BEGIN_MESSAGE_MAP(CDlgTblMng, CDialog)
	ON_BN_CLICKED(IDC_BTN_TBL_SAVE, &CDlgTblMng::OnBnClickedBtnTblSave)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TBL1, &CDlgTblMng::OnLvnItemchangedListTbl1)
END_MESSAGE_MAP()

//
BOOL CDlgTblMng::LoadTblTitleMapping()
{
	m_lstTbl.DeleteAllItems();//���
	m_mapTblTitle.clear();
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
		if("��ϸ��Ϣ������" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		//pos_ini.Next();	
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
			if("Y" == vStr[2])
			{
			    ((CButton *)GetDlgItem(IDC_CHECK_TBL1+iCount))->SetCheck(1);
			}
			//
			iCount++;
			//���
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%d",iCount);
			m_lstTbl.InsertItem(iCount-1, _T(vStr[0].c_str()));
			//����
			m_lstTbl.SetItemText(iCount-1, 1, _T(vStr[1].c_str()));
			//���
			m_lstTbl.SetItemText(iCount-1, 2, _T(vStr[2].c_str()));
		}
	}
	return TRUE;
}
// CDlgTblMng ��Ϣ�������
BOOL CDlgTblMng::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
    DWORD dwStyle = m_lstTbl.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstTbl.SetExtendedStyle(dwStyle);	
	CRect rect;
	m_lstTbl.GetClientRect(&rect); //��õ�ǰ�ͻ�����Ϣ 
    m_lstTbl.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/8);
    m_lstTbl.InsertColumn(1, _T("����"), LVCFMT_LEFT, rect.Width()/4);
	m_lstTbl.InsertColumn(2, _T("��Ч��"), LVCFMT_LEFT, rect.Width()/4);
	//
	LoadTblTitleMapping();
	//
	return TRUE;
}

void CDlgTblMng::OnBnClickedBtnTblSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(int i = 0; i < 6; i++)
	{
		int chk = ((CButton *)GetDlgItem(IDC_CHECK_TBL1+i))->GetCheck();
		if(1 == chk)
		{
			m_lstTbl.SetItemText(i, 2, _T("Y"));
		}
		else
		{
			m_lstTbl.SetItemText(i, 2, _T("N"));
		}
		CString strItem = m_lstTbl.GetItemText(i,0);
		CString strName = m_lstTbl.GetItemText(i,1);
		CString strFlg = m_lstTbl.GetItemText(i,2);
		char stmp[256];
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%s,%s,%s",strItem.GetBuffer(),strName.GetBuffer(),strFlg.GetBuffer());
		BOOL bol = WritePrivateProfileString("��ϸ��Ϣ������",strItem.GetBuffer(),stmp,".\\cfg\\devcfg.ini");

	}
}

void CDlgTblMng::OnLvnItemchangedListTbl1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
