// DlgReaderQry.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgReaderQry.h"
#include "TcpCommSvrDlg.h"

//
extern CTcpCommSvrDlg *g_pMainDlg;
// CDlgReaderQry �Ի���

IMPLEMENT_DYNAMIC(CDlgReaderQry, CDialog)

CDlgReaderQry::CDlgReaderQry(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReaderQry::IDD, pParent)
	, m_sendCmd(_T(""))
{

}

CDlgReaderQry::~CDlgReaderQry()
{
}

void CDlgReaderQry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbReaderNo);
	DDX_Control(pDX, IDC_LIST1, m_lstCardNo);
	DDX_Text(pDX, IDC_EDIT_CMD, m_sendCmd);
	DDX_Control(pDX, IDC_COMBO2, m_cbLength);
	DDX_Control(pDX, IDC_COMBO3, m_cbFName);
	DDX_Control(pDX, IDC_LIST2, m_lstCmd);
}


BEGIN_MESSAGE_MAP(CDlgReaderQry, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgReaderQry::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgReaderQry::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_READER_QRY, &CDlgReaderQry::OnBnClickedBtnReaderQry)
	ON_BN_CLICKED(IDC_BTN_SEND, &CDlgReaderQry::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SAVE_ONE, &CDlgReaderQry::OnBnClickedBtnSaveOne)
	ON_BN_CLICKED(IDC_BTN_SEND_ONE, &CDlgReaderQry::OnBnClickedBtnSendOne)
	ON_BN_CLICKED(IDC_LOAD_DOC, &CDlgReaderQry::OnBnClickedLoadDoc)
	ON_BN_CLICKED(IDC_BTN_SAVE_ALL, &CDlgReaderQry::OnBnClickedBtnSaveAll)
	ON_BN_CLICKED(IDC_BTN_SEND_ALL, &CDlgReaderQry::OnBnClickedBtnSendAll)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CDlgReaderQry::OnLvnItemchangedList2)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CDlgReaderQry::OnNMClickList2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgReaderQry::OnCbnSelchangeCombo3)
	ON_CBN_SELENDOK(IDC_COMBO3, &CDlgReaderQry::OnCbnSelendokCombo3)
END_MESSAGE_MAP()
//���ؾ�������ӳ���ϵ
BOOL CDlgReaderQry::LoadLenCmdMapping()
{
	CString filePath = _T(".\\cfg\\����.ini");
	TIniFile iniFile(filePath);
	TIniFile::SectionIterator pos_ini;
    m_mapLenCmd.clear();
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if(section_count <= 0)
	{
		return FALSE;
	}
	for(size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{	
		std::string& str_mapping_type = v_section[sec_idx];
		if("����������ӳ���ϵ" != str_mapping_type)continue; 
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
			if (vStr.size() != 2)
			{
				continue;
			}
			m_mapLenCmd[vStr[0]] = atoi(vStr[1].c_str());
			m_cbLength.AddString(vStr[0].c_str());
		}
	}
	return TRUE;
}
BOOL CDlgReaderQry::LoadFName()
{
	CString filePath = _T(".\\cfg\\����.ini");
	TIniFile iniFile(filePath);
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if(section_count <= 0)
	{
		return FALSE;
	}
	for(size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{	
		std::string& str_mapping_type = v_section[sec_idx];
		if("����" != str_mapping_type)continue; 
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
			m_cbFName.AddString(str_dcqk.GetBuffer());
		}
	}
	return TRUE;
}
//
//BOOL CDlgReaderQry::LoadReaderNo()
//{
//	CString filePath = _T(".\\cfg\\�豸.ini");
//	TIniFile iniFile(filePath);
//	TIniFile::SectionIterator pos_ini;
//	std::vector<std::string> v_section;
//	iniFile.GetIniSections(v_section);
//	size_t section_count = v_section.size();
//	if(section_count <= 0)
//	{
//		return FALSE;
//	}
//	for(size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
//	{	
//		std::string& str_mapping_type = v_section[sec_idx];
//		if("����" != str_mapping_type)continue; 
//		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
//		{
//			continue;
//		}
//		pos_ini.Next();	
//		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
//		{
//			CString str_first = pos_ini.Data().first.c_str();
//			if(str_first == "������" || str_first == "��·��" )
//				continue;
//
//			CString str_id = pos_ini.Data().first.c_str();
//			CString str_dcqk = pos_ini.Data().second.c_str();
//			m_cbFName.AddString(str_dcqk.GetBuffer());
//		}
//	}
//	return TRUE;
//}
//
BOOL CDlgReaderQry::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
    DWORD dwStyle = m_lstCardNo.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstCardNo.SetExtendedStyle(dwStyle);	
	CRect rect;
	m_lstCardNo.GetClientRect(&rect); //��õ�ǰ�ͻ�����Ϣ 
    m_lstCardNo.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/20);
    //m_lstCardNo.InsertColumn(1, _T("�豸���"), LVCFMT_LEFT, rect.Width()/6);
    m_lstCardNo.InsertColumn(1, _T("����"), LVCFMT_LEFT, rect.Width()*3/20);
	//m_lstCardNo.InsertColumn(2, _T("��ѹ(V)"), LVCFMT_LEFT, rect.Width()/7);
	//m_lstCardNo.InsertColumn(3, _T("����(A)"), LVCFMT_LEFT, rect.Width()/7);
	//m_lstCardNo.InsertColumn(4, _T("�¶�(��)"), LVCFMT_LEFT, rect.Width()/7);
	m_lstCardNo.InsertColumn(2, _T("x��"), LVCFMT_LEFT, rect.Width()/10);
	m_lstCardNo.InsertColumn(3, _T("y��"), LVCFMT_LEFT, rect.Width()/10);
	m_lstCardNo.InsertColumn(4, _T("z��"), LVCFMT_LEFT, rect.Width()/10);
	m_lstCardNo.InsertColumn(5, _T("�¶�"), LVCFMT_LEFT, rect.Width()/10);
	m_lstCardNo.InsertColumn(6, _T("����"), LVCFMT_LEFT, rect.Width()/10);
	m_lstCardNo.InsertColumn(7, _T("״̬"), LVCFMT_LEFT, rect.Width()/10);
	m_lstCardNo.InsertColumn(8, _T("����"), LVCFMT_LEFT, rect.Width()/10);
    m_lstCardNo.InsertColumn(9, _T("ʱ��"), LVCFMT_LEFT, rect.Width()/10); 
    ////
    dwStyle = m_lstCmd.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstCmd.SetExtendedStyle(dwStyle);	
	rect;
	m_lstCmd.GetClientRect(&rect); //��õ�ǰ�ͻ�����Ϣ 
    m_lstCmd.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/6);
    m_lstCmd.InsertColumn(1, _T("�豸���"), LVCFMT_LEFT, rect.Width()/4);
    m_lstCmd.InsertColumn(2, _T("����"), LVCFMT_LEFT, rect.Width()/4);
	m_lstCmd.InsertColumn(3, _T("״̬"), LVCFMT_LEFT, rect.Width()/3); 
    ////
    BOOL bol = FALSE;
	bol = LoadLenCmdMapping();
	if(FALSE == bol)
	{
		MessageBox("����.ini�����ڣ�");
	}
	m_cbLength.SetCurSel(0);
	bol = LoadFName();
	if(FALSE == bol)
	{
		MessageBox("����.ini�����ڣ�");
	}
	m_cbFName.SetCurSel(0);
	char stmp[256];
	memset(stmp,0,sizeof(stmp));
    for(int i = 1; i < 256; i++)
	{
		sprintf(stmp,"%03d",i);
		m_cbReaderNo.AddString(stmp);
	}
	m_cbReaderNo.SetCurSel(0);
	return TRUE;
}
// CDlgReaderQry ��Ϣ�������

void CDlgReaderQry::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CDlgReaderQry::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CDlgReaderQry::OnBnClickedBtnReaderQry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lstCardNo.DeleteAllItems();//���
	UpdateData(TRUE);
	char stmp[256];
	char scontents[256];
	memset(stmp,0,sizeof(stmp));
	m_cbReaderNo.GetWindowTextA(stmp,255);
	int readerNo = atoi(stmp);
	if(readerNo <= 0)
	{
		MessageBox("�������ݲ��Ϸ���");
		return ;
	}
	//
	CString strPath(".\\data\\%03d.txt",readerNo);
	FILE *fp = fopen(strPath.GetBuffer(),"w");
	//
	map<unsigned int,stuCardInfo> mapCardInfo;
	mapCardInfo.clear();
	g_pMainDlg->GetCardInfo(readerNo,mapCardInfo);
	//
	int iCount = 0;
	CString strCardNo;
	map<unsigned int,stuCardInfo>::iterator iter;
	for(iter = mapCardInfo.begin();iter != mapCardInfo.end();iter++)
	{
		memset(scontents,0,sizeof(scontents));
		iCount++;
		//���
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%03d",iCount);
		m_lstCardNo.InsertItem(iCount-1, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,",");
		//�豸���
		//memset(stmp,0,sizeof(stmp));
		//sprintf(stmp,"%03d",readerNo);
		//m_lstCardNo.SetItemText(iCount-1, 1, _T(stmp));
		//strcat(scontents,stmp);
		//strcat(scontents,",");
		//����
		memset(stmp,0,sizeof(stmp));
		strCardNo.Format("%010u",iter->first);
		sprintf(stmp,"%s",strCardNo.GetBuffer());
		m_lstCardNo.SetItemText(iCount-1, 1, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,",");
		//////��ѹ
		////memset(stmp,0,sizeof(stmp));
		////sprintf(stmp,"%.02f",iter->second.voltage);
		////m_lstCardNo.SetItemText(iCount-1, 2, _T(stmp));
		////strcat(scontents,stmp);
		////strcat(scontents,",");
		//////����
		////memset(stmp,0,sizeof(stmp));
		////sprintf(stmp,"%.02f",iter->second.current);
		////m_lstCardNo.SetItemText(iCount-1, 3, _T(stmp));
		////strcat(scontents,stmp);
		////strcat(scontents,",");
		//////�¶�
		////memset(stmp,0,sizeof(stmp));
		////sprintf(stmp,"%.02f",iter->second.temperature);
		////m_lstCardNo.SetItemText(iCount-1, 4, _T(stmp));
		////strcat(scontents,stmp);
		////strcat(scontents,",");
		//////////////////
		stuWatchInfo stu_wi;
		memset(&stu_wi,0,sizeof(stuWatchInfo));
		ParseWatchData(iter->second.data_len,iter->second.data,stu_wi);
		//x��
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%.04f",stu_wi.d_x);
		m_lstCardNo.SetItemText(iCount-1, 2, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,",");
		//y��
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%.04f",stu_wi.d_y);
		m_lstCardNo.SetItemText(iCount-1, 3, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,",");
		//z��
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%.04f",stu_wi.d_z);
		m_lstCardNo.SetItemText(iCount-1, 4, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,",");
        //�¶�
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%.02f",stu_wi.d_temperature);
		m_lstCardNo.SetItemText(iCount-1, 5, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,",");
		//����
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",stu_wi.i_rate);
		m_lstCardNo.SetItemText(iCount-1, 6, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,",");
		//״̬
		memset(stmp,0,sizeof(stmp));
		strcpy(stmp,stu_wi.status);
		m_lstCardNo.SetItemText(iCount-1, 7, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,",");
		//��������
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",iter->second.readCount);
		m_lstCardNo.SetItemText(iCount-1, 8, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,",");
		//ʱ��
		struct tm *now = NULL;
		now = localtime(&iter->second.lstTime);
		sprintf(stmp,"%d-%02d-%02d %d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
		m_lstCardNo.SetItemText(iCount-1, 9, _T(stmp));
		strcat(scontents,stmp);
		strcat(scontents,"\n");
		if(NULL != fp)
		{
			fputs(scontents,fp);
		}
	}
	if(NULL != fp)
	{
		fclose(fp);
		fp = NULL;
	}
}
//
void CDlgReaderQry::ParseWatchData(int len,const unsigned char *data,stuWatchInfo &stuWI)
{
	string desc_str;
	char stmp[256];
	memset(stmp,0,sizeof(stmp));
	double d_x = 0;
	double d_y = 0;
	double d_z = 0;
	double d_temperature = 0;
	double d_rate = 0;
	int    i_ok = 0;
	short w_tmp;
	const unsigned char *p = data;
	//x
	w_tmp = p[0] * 256 + p[1];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_x = w_tmp * 3.9 / 1000;
	p += 2;
	//y
	w_tmp = p[0] * 256 + p[1];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_y = w_tmp * 3.9 / 1000;
	p += 2;
	//z
	w_tmp = p[0] * 256 + p[1];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_z = w_tmp * 3.9 / 1000;
	p += 2;
	//�¶�
	w_tmp = p[0] * 256 + p[1];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_temperature = w_tmp / 10;
	p += 2;
	//����
	w_tmp = p[0];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_rate = 60 / (w_tmp * 0.02);
	p += 1;
	//�Ƿ�����
	i_ok = p[0];
	p += 1;
	string strTmp = "";
	if(i_ok > 127)strTmp = "����";
	else strTmp = "δ����";
	//
	int i_rate = d_rate;
	sprintf(stmp,"%.04f,%.04f\n%.04f,%.02f,%d,",d_x,d_y,d_z,d_temperature,i_rate);
	desc_str = stmp;
	desc_str += strTmp;
	stuWI.d_x = d_x;
	stuWI.d_y = d_y;
	stuWI.d_z = d_z;
	stuWI.d_temperature = d_temperature;
	stuWI.i_rate = i_rate;
	strcpy(stuWI.status,strTmp.c_str());


}
//
 /******************************************************************************
 * Purpose : ascii��ת16����
 * Argument: source:ascii������
 *           result:16�������
 *           len:result����ĳ���
 * Author  : jjg 2011/08/30
 *****************************************************************************/
void CDlgReaderQry::StrToHex(char *pbDest, char *pbSrc, int nLen)
{
	char h1,h2;
	char s1,s2;
	int i;

	for (i=0; i<nLen; i++)
	{
		h1 = pbSrc[2*i];
		h2 = pbSrc[2*i+1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9) 
		s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9) 
		s2 -= 7;

		pbDest[i] = s1*16 + s2;
	}
}

int CDlgReaderQry::isHex( char ch ) //�ж��ַ�ch�Ƿ�16�����ַ����Ƿ���1���񷵻�0 
{ 
    if ( ch >='0' && ch <='9' ) //����0-9���ϣ������� 
        return 1; 
    if ( ch >='A' && ch <='F' ) //����A-F���ϣ������� 
        return 1; 
    if ( ch >='a' && ch <='f' ) //����a-f���ϣ������� 
        return 1; 
    return 0; //���򣬷��ز��� 
}  

void CDlgReaderQry::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	char stmp[256];
	char sndBuf[256];
	char ch = 0;
	int len = 0;
	memset(stmp,0,sizeof(stmp));
	m_cbReaderNo.GetWindowTextA(stmp,255);
	int readerNo = atoi(stmp);
	if(readerNo <= 0)
	{
		MessageBox("�������������");
		return ;
	}
	memset(stmp,0,sizeof(stmp));
	memset(sndBuf,0,sizeof(sndBuf));
	len = m_sendCmd.GetLength();
	if(len < 256)
	{
		memcpy(stmp,m_sendCmd.GetBuffer(),len);
		if(0 != len%2)
		{
			MessageBox("���������ӦΪż����");
			return;
		}
		for(int i = 0;i < len; i++)
		{
			ch  = stmp[i];
			if(!isHex(ch))
			{
				MessageBox("�����зǷ��ַ���");
				return;
			}
		}
		StrToHex(sndBuf,stmp,len/2);
		stuSockInfo sockinfo;
		BOOL bol = g_pMainDlg->FindSockInfoByReaderNo(readerNo,sockinfo);
        if(TRUE == bol)
		{
			int ret = send(sockinfo.sock,sndBuf,len/2,0);
			if(ret == len/2)
			{
				MessageBox("���ͳɹ���");
			}
		}
		else
		{
			MessageBox("δ�ҵ��ö�������Ч���ӣ�");
		}
	}
	else
	{
		MessageBox("����̫�������ܳ���256�ַ���");
	}

}

void CDlgReaderQry::OnBnClickedBtnSaveOne()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	char stmp[256];
	char spath[256];
	memset(stmp,0,sizeof(stmp));
	memset(spath,0,sizeof(spath));
	m_cbFName.GetWindowTextA(stmp,255);
	//д�����ļ�
	strcpy(spath,".\\cfg\\");
	strcat(spath,stmp);
	strcat(spath,".ini");
	memset(stmp,0,sizeof(stmp));
	m_cbReaderNo.GetWindowTextA(stmp,255);
	string strReadNo = stmp;
	int readerNo = atoi(stmp);
	if(readerNo <= 0)
	{
		MessageBox("�������������");
		return ;
	}
	memset(stmp,0,sizeof(stmp));
	m_cbLength.GetWindowTextA(stmp,255);
	BOOL bol = WritePrivateProfileString("�������������",strReadNo.c_str(),stmp,spath);
	if(bol)
	{
		int lstCount  = m_lstCmd.GetItemCount();
		BOOL bFind = FALSE;
		for(int i = 0; i < lstCount; i++)
		{
			CString strDevNo = m_lstCmd.GetItemText(i,1);
			if(0 == strcmp(strDevNo.GetBuffer(),strReadNo.c_str()))
			{
				m_lstCmd.SetItemText(i, 2,_T(stmp));
				m_lstCmd.SetItemText(i, 3,_T("����ɹ���")); 
				bFind = TRUE;
				break;
			}
		}
		if(FALSE == bFind)
		{
			//���
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%03d",lstCount+1);
			m_lstCmd.InsertItem(lstCount, _T(stmp));
			//�豸���
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%03d",readerNo);
			m_lstCmd.SetItemText(lstCount, 1, _T(stmp));
			//����
			memset(stmp,0,sizeof(stmp));
			m_cbLength.GetWindowTextA(stmp,255);
			m_lstCmd.SetItemText(lstCount, 2, _T(stmp));
			//
			m_lstCmd.SetItemText(lstCount, 3,_T("����ɹ���")); 
		}
		MessageBox("����ɹ���");
	}
	else
	{
		MessageBox("����ʧ�ܣ�");
	}
}
//
int CDlgReaderQry::PackCmdBuf(char *sndBuf,CString strDevNo,CString strLen)
{
	int len = 0;
	unsigned char cc = 0;
	sndBuf[len] = 0x0A;
	cc += sndBuf[len];
	len += 1;
	//�豸���
	sndBuf[len] = (char)atoi(strDevNo.GetBuffer());
	cc += sndBuf[len];
	len += 1;
	//
	sndBuf[len] = 0x04;
	cc += sndBuf[len];
	len += 1;
	sndBuf[len] = 0x23;
	cc += sndBuf[len];
	len += 1;
	sndBuf[len] = (char)0xAE;
	cc += sndBuf[len];
	len += 1;
	//
	string str = strLen.GetBuffer();
	map<string,int>::iterator iter;
	iter = m_mapLenCmd.find(str);
	if(iter != m_mapLenCmd.end())
	{
		sndBuf[len] = (char)iter->second;
		cc += sndBuf[len];
		len += 1;
	}
	else
	{
		return -1;
	}
	cc = ~cc;
	cc += 1;
	sndBuf[len] = (char)cc;
	len += 1;
	return len;
}
//
void CDlgReaderQry::OnBnClickedBtnSendOne()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	char stmp[256];
	char sndBuf[256];
	char ch = 0;
	int len = 0;
	memset(stmp,0,sizeof(stmp));
	m_cbReaderNo.GetWindowTextA(stmp,255);
	int readerNo = atoi(stmp);
	if(readerNo <= 0)
	{
		MessageBox("�������������");
		return ;
	}
	memset(sndBuf,0,sizeof(sndBuf));
	//���
	CString strDevNo;
	strDevNo.Format("%03d",readerNo);
	//����
	memset(stmp,0,sizeof(stmp));
	m_cbLength.GetWindowTextA(stmp,255);
	CString strLen = stmp;
	int ret = PackCmdBuf(sndBuf,strDevNo,strLen);
	if(ret <= 0)
	{
		MessageBox("���ʧ�ܣ�");
		return ;
	}
	len = ret;
	//
	stuSockInfo sockinfo;
	BOOL bol = g_pMainDlg->FindSockInfoByReaderNo(readerNo,sockinfo);
    if(TRUE == bol)
	{
		int ret = send(sockinfo.sock,sndBuf,len,0);
		if(ret == len)
		{
			MessageBox("���ͳɹ���");
		}
		else
		{
            MessageBox("����ʧ�ܣ�");
		}
	}
	else
	{
		MessageBox("δ�ҵ��ö�������Ч���ӣ�");
	}
}

void CDlgReaderQry::OnBnClickedLoadDoc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lstCmd.DeleteAllItems();//���
	UpdateData(TRUE);
	int iCount = 0;
	char stmp[256];
	char spath[256];
	memset(stmp,0,sizeof(stmp));
	memset(spath,0,sizeof(spath));
	//m_cbFName.GetWindowTextA(stmp,255);
	int index = m_cbFName.GetCurSel();
	m_cbFName.GetLBText(index,stmp);
	strcpy(spath,".\\cfg\\");
	strcat(spath,stmp);
	strcat(spath,".ini");
	if (access(spath, 0) == -1)
	{
		memset(stmp,0,sizeof(stmp));
		strcpy(stmp,spath);
		strcat(stmp,"�ļ������ڣ�");
		MessageBox(stmp);
		return ;
	}
	CString filePath = _T(spath);
	TIniFile iniFile(filePath);
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if(section_count <= 0)
	{
		memset(stmp,0,sizeof(stmp));
		strcpy(stmp,spath);
		strcat(stmp,"�ļ�����");
		MessageBox(stmp);
		return ;
	}
	for(size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{	
		std::string& str_mapping_type = v_section[sec_idx];
		if("�������������" != str_mapping_type)continue; 
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
			CString strTmp = pos_ini.Data().second.c_str();
			iCount++;
			//���
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%03d",iCount);
			m_lstCmd.InsertItem(iCount-1, _T(stmp));
			//�豸���
			int readerNo = atoi(str_id.GetBuffer());
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%03d",readerNo);
			m_lstCmd.SetItemText(iCount-1, 1, _T(stmp));
			//����
			m_lstCmd.SetItemText(iCount-1, 2, _T(strTmp.GetBuffer()));

		}
	}
}

void CDlgReaderQry::OnBnClickedBtnSaveAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	char stmp[256];
	char spath[256];
	memset(stmp,0,sizeof(stmp));
	memset(spath,0,sizeof(spath));
	m_cbFName.GetWindowTextA(stmp,255);
	//д�����ļ�
	strcpy(spath,".\\cfg\\");
	strcat(spath,stmp);
	strcat(spath,".ini");
	//memset(stmp,0,sizeof(stmp));
	//m_cbReaderNo.GetWindowTextA(stmp,255);
	//string strReadNo = stmp;
	//int readerNo = atoi(stmp);
	//if(readerNo <= 0)
	//{
	//	MessageBox("�������������");
	//	return ;
	//}
	memset(stmp,0,sizeof(stmp));
	m_cbLength.GetWindowTextA(stmp,255);
	int lstCount  = m_lstCmd.GetItemCount();
	for(int i = 0; i < lstCount; i++)
	{
		memset(stmp,0,sizeof(stmp));
		//m_cbLength.GetWindowTextA(stmp,255);_
		CString strDevNo = m_lstCmd.GetItemText(i,1);
		CString strLen   = m_lstCmd.GetItemText(i,2);
		BOOL bol = WritePrivateProfileString("�������������",strDevNo.GetBuffer(),strLen.GetBuffer(),spath);
		if(bol)
		{
			m_lstCmd.SetItemText(i, 3,_T("����ɹ���"));
		}
		else
		{
			m_lstCmd.SetItemText(i, 3,_T("����ʧ�ܣ�"));
		}
	}
}

void CDlgReaderQry::OnBnClickedBtnSendAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	char stmp[256];
	char sndBuf[256];
	int len = 0;
	int readerNo = 0;
	int lstCount  = m_lstCmd.GetItemCount();
	for(int i = 0; i < lstCount; i++)
	{
		memset(stmp,0,sizeof(stmp));
		memset(sndBuf,0,sizeof(sndBuf));
		CString strDevNo = m_lstCmd.GetItemText(i,1);
		CString strLen   = m_lstCmd.GetItemText(i,2);
		//���
		int ret = PackCmdBuf(sndBuf,strDevNo,strLen);
		if(ret <= 0)
		{
			m_lstCmd.SetItemText(i, 3,_T("���ʧ�ܣ�"));
			continue;
		}
		len = ret;
		//
		stuSockInfo sockinfo;
		BOOL bol = g_pMainDlg->FindSockInfoByReaderNo(readerNo,sockinfo);
		if(TRUE == bol)
		{
			ret = send(sockinfo.sock,sndBuf,len,0);
			if(ret == len)
			{
				m_lstCmd.SetItemText(i, 3,_T("���ͳɹ���"));
			}
			else
			{
				m_lstCmd.SetItemText(i, 3,_T("����ʧ�ܣ�"));
			}
		}
		else
		{
			m_lstCmd.SetItemText(i, 3,_T("δ�ҵ��ö�������Ч���ӣ�"));
			//MessageBox("δ�ҵ��ö�������Ч���ӣ�");
		}
	}

}

void CDlgReaderQry::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION ps;
	int nIndex;
	ps=m_lstCmd.GetFirstSelectedItemPosition();
	nIndex=m_lstCmd.GetNextSelectedItem(ps);
	CString strDevNo = m_lstCmd.GetItemText(nIndex,1);
	CString strLen   = m_lstCmd.GetItemText(nIndex,2);
	m_cbReaderNo.SetWindowTextA(strDevNo.GetBuffer());
	m_cbLength.SetWindowTextA(strLen.GetBuffer());
	*pResult = 0;
}

void CDlgReaderQry::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CDlgReaderQry::OnCbnSelchangeCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnBnClickedLoadDoc();
	//UpdateData(TRUE);
	//char stmp[256];
	//memset(stmp,0,sizeof(stmp));
	//int index = m_cbFName.GetCurSel();
	//m_cbFName.GetLBText(index,stmp);
}

void CDlgReaderQry::OnCbnSelendokCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(TRUE);
	//char stmp[256];
	//memset(stmp,0,sizeof(stmp));
	//int index = m_cbFName.GetCurSel();
	OnBnClickedLoadDoc();
}
