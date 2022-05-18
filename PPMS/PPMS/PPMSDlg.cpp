// PPMSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PPMS.h"
#include "PPMSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//
#define WM_RMENU_CMD_TEST_1  WM_USER + 0x010
#define WM_RMENU_CMD_TEST_2  WM_USER + 0x011
#define WM_RMENU_CMD_TEST_3  WM_USER + 0x012
#define WM_RMENU_CMD_TEST_4  WM_USER + 0x013
#define WM_RMENU_CMD_TEST_5  WM_USER + 0x014
#define WM_RMENU_CMD_TEST_6  WM_USER + 0x015
//
const int r = 4;
const int c = 5;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPPMSDlg dialog




CPPMSDlg::CPPMSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPPMSDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPPMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_DEV, m_lstDev);
}

BEGIN_MESSAGE_MAP(CPPMSDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CPPMSDlg message handlers
void CPPMSDlg::InitHead()
{
	CRect picRect;
	CStatic* pWnd = NULL;
	//ͼ����ʾ
	picRect.left = 0;
	picRect.top = 0;
	picRect.right = m_cx;
	picRect.bottom = m_cy/5;
	//
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_HEAD); // 
	pWnd->MoveWindow(&picRect);
	CRect rect;
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // �޸���������Ϊλͼ
	pWnd->GetWindowRect(&rect);
	CString strPath = ".\\pic\\head.bmp";
	//HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\map.bmp"), 
	HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(strPath.GetBuffer()),
					IMAGE_BITMAP, 
					rect.Width(), 
					rect.Height(),
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	pWnd->SetBitmap(hPic);
	DeleteObject(hPic);
	pWnd->SetWindowText("");
	//
}
void CPPMSDlg::InitBody()
{
	CRect picRect;
	CStatic* pWnd = NULL;
	//ͼ����ʾ
	picRect.left = m_cx/5;
	picRect.top = m_cy/5;
	picRect.right = m_cx;
	picRect.bottom = m_cy;
	//
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_BODY); // 
	pWnd->MoveWindow(&picRect);
	CRect rect;
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // �޸���������Ϊλͼ
	pWnd->GetWindowRect(&rect);
	CString strPath = ".\\pic\\body.bmp";
	//HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\map.bmp"), 
	HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(strPath.GetBuffer()),
					IMAGE_BITMAP, 
					rect.Width(), 
					rect.Height(),
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	pWnd->SetBitmap(hPic);
	DeleteObject(hPic);
	pWnd->SetWindowText("");
	//
	map<unsigned int, stuCardRtInfo>::iterator iter;
	iter = m_mapCardRtInfo.begin();
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
		{
			if(m_mapCardRtInfo.end() == iter)return;
			DisplayMonitorUnit(i,j,iter->first);
			iter++;
		}
	}
	//
}
//
void CPPMSDlg::DisplayMonitorUnit(int i,int j,unsigned int card_no)
{
	string sname,spolice,spos;
	map<unsigned int, stuCardRtInfo>::iterator iter;
	iter = m_mapCardRtInfo.find(card_no);
	if(m_mapCardRtInfo.end() == iter)return;
	//���ޱ���
	bool bWarn = false;
	map<unsigned int,stuCardWarnInfo>::iterator iter_warn;
	iter_warn = m_mapWarnInfo.find(card_no);
	if(m_mapWarnInfo.end() != iter_warn)
	{
		if(iter->second.body_temp < iter_warn->second.body_temp_down || iter->second.body_temp > iter_warn->second.body_temp_up)
		{
			bWarn = true;
		}
		if(iter->second.heart_rate < iter_warn->second.heart_rate_down || iter->second.heart_rate > iter_warn->second.heart_rate_up)
		{
			bWarn = true;
		}
	}
	//
	CRect rect;
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_BODY); // 
	pWnd->GetWindowRect(&rect);
	CDC* pControlDC; //��ʾ�ؼ����ڵ��豸������.DC 
	pControlDC = pWnd->GetDC();
	if(NULL == pControlDC)return;
	if(NULL == pControlDC->m_hDC)return;
	int w = rect.Width();
	int h = rect.Height();
	int x = 10;
	int y = 10;
	int w_cld = (w - (c+1)*x)/c;
	int h_cld = (h - (r+1)*y)/r;
	//
	CBrush brush, *oldbrush; 
	if(bWarn)
	{
		brush.CreateSolidBrush(RGB(255, 255, 0));
	}
	else
	{
	    brush.CreateSolidBrush(RGB(255, 255, 255));
	}
	oldbrush = pControlDC->SelectObject(&brush);
	//
	CRect  rectCld;
	rectCld = CRect(CPoint(10 + j*(x + w_cld), 10 + i*(y + h_cld)), CSize(w_cld, h_cld));
	pControlDC->RoundRect(rectCld, CPoint(20,20));
	//��Բ
	DisWarnZone_text(i,j,card_no,rectCld,pControlDC);
	//д����
	//
	pControlDC->SelectObject(oldbrush);
	//DeleteObject(brush);
	brush.DeleteObject();
}
//
void CPPMSDlg::DisWarnZone_text(int m,int n,unsigned int card_no,CRect rect,CDC* pControlDC)
{
	string sname,spolice,spos;
	map<unsigned int, stuCardRtInfo>::iterator iter;
	iter = m_mapCardRtInfo.find(card_no);
	if(m_mapCardRtInfo.end() == iter)return;
	//��Ա��Ϣ
	map<unsigned int,stuCardBaseInfo>::iterator iter_man;
	iter_man = m_mapPeopleInfo.find(card_no);
	if(m_mapPeopleInfo.end() == iter_man)
	{
		sname = "δ֪";
		spolice = "δ֪"; 
	}
	else
	{
		sname = iter_man->second.name;
		spolice = iter_man->second.police;
	}
	//λ����Ϣ
	map<unsigned int,stuLblPosInfo>::iterator iter_lp;
	iter_lp = m_mapLblPosInfo.find(iter->second.lbl_id);
	if(m_mapLblPosInfo.end() == iter_lp)
	{
		map<unsigned int,stuDevPosInfo>::iterator iter_dp;
		iter_dp = m_mapDevPosInfo.find(iter->second.dev_id);
		if(m_mapDevPosInfo.end() == iter_dp)
		{
			spos = "δ֪";
		}
		else
		{
			spos = iter_dp->second.name;
		}
	}
	else
	{
		spos = iter_lp->second.name;
	}
	//���ޱ���
	bool bWarn = false;
	map<unsigned int,stuCardWarnInfo>::iterator iter_warn;
	iter_warn = m_mapWarnInfo.find(card_no);
	if(m_mapWarnInfo.end() != iter_warn)
	{
		if(iter->second.body_temp < iter_warn->second.body_temp_down || iter->second.body_temp > iter_warn->second.body_temp_up)
		{
			bWarn = true;
		}
		if(iter->second.heart_rate < iter_warn->second.heart_rate_down || iter->second.heart_rate > iter_warn->second.heart_rate_up)
		{
			bWarn = true;
		}
	}
	//
	int x = rect.left;
	int y = rect.top;
	int w = rect.Width();
	int h = rect.Height();
    //
	CFont font,*oldfont;
	font.CreateFont(
		h/6, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial") // nPitchAndFamily Arial
		);
    //
	oldfont = pControlDC->SelectObject(&font);
	//����
	CString strTmp = "������";
	strTmp += sname.c_str();
	CRect TextRect;
	TextRect.left = x+20;
	TextRect.right = x+w-10;
	TextRect.top = y+2;
	TextRect.bottom = y+h/6;
	//
	CRect temp = TextRect;
	if(bWarn)
	{
		pControlDC->SetBkColor(RGB(255, 255, 0));
	}
	else
	{
	    pControlDC->SetBkColor(RGB(255, 255, 255));
	}
	pControlDC->SetTextColor(RGB(255, 0, 0)); 
	int height = pControlDC->DrawText(strTmp, temp, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pControlDC->DrawText(strTmp, TextRect, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);
	//λ��
    strTmp = "λ�ã�";
	strTmp += spos.c_str();
	TextRect.left = x+20;
	TextRect.right = x+w-10;
	TextRect.top = y+h/6;
	TextRect.bottom = y+h/3;
	pControlDC->SetTextColor(RGB(255,0,0));
	//
	temp = TextRect;
	height = pControlDC->DrawText(strTmp, temp, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pControlDC->DrawText(strTmp, TextRect, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);
	//����
	//strTmp = "���ʣ�100��/��";
	int heart_r = iter->second.heart_rate;
	strTmp.Format("���ʣ�%d��/��",heart_r);
	TextRect.left = x+20;
	TextRect.right = x+w-10;
	TextRect.top = y+h/3;
	TextRect.bottom = y+h/2;
	pControlDC->SetTextColor(RGB(255,0,0));
	//
	temp = TextRect;
	height = pControlDC->DrawText(strTmp, temp, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pControlDC->DrawText(strTmp, TextRect, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);
	//����
	//strTmp = "���£�37��";
	int body_t = iter->second.body_temp;
	strTmp.Format("���£�%d��",body_t);
	TextRect.left = x+20;
	TextRect.right = x+w-10;
	TextRect.top = y+h/2;
	TextRect.bottom = y+h*2/3;
	pControlDC->SetTextColor(RGB(255,0,0));
	//
	temp = TextRect;
	height = pControlDC->DrawText(strTmp, temp, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pControlDC->DrawText(strTmp, TextRect, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);
	//״̬
    strTmp = "״̬������";
	TextRect.left = x+20;
	TextRect.right = x+w-10;
	TextRect.top = y+h*2/3;
	TextRect.bottom = y+h*5/6;
	pControlDC->SetTextColor(RGB(255,0,0));
	//
	temp = TextRect;
	height = pControlDC->DrawText(strTmp, temp, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pControlDC->DrawText(strTmp, TextRect, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);
	//������
    strTmp = "�����ˣ�";
	strTmp += spolice.c_str();
	TextRect.left = x+20;
	TextRect.right = x+w-10;
	TextRect.top = y+h*5/6;
	TextRect.bottom = y+h;
	pControlDC->SetTextColor(RGB(255,0,0));
	//
	temp = TextRect;
	height = pControlDC->DrawText(strTmp, temp, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pControlDC->DrawText(strTmp, TextRect, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);
	//
	pControlDC->SelectObject(oldfont);
	font.DeleteObject();
}
//
void CPPMSDlg::DisWarnZone(int m,int n,CRect rect,CDC* pControlDC)
{
	int x = rect.left;
	int y = rect.top;
	int w = rect.Width();
	int h = rect.Height();
	int dstR = w/5;
	int yd = (h/2 - dstR)/2;
	CBrush brush, *oldbrush; 
	brush.CreateSolidBrush(RGB(0, 255, 0)); 
	oldbrush = pControlDC->SelectObject(&brush);
	//
	for(int i = 0; i < 5; i++)
	{
		//int xx = 
        pControlDC->Ellipse(x + i * dstR, y+yd, x + (i + 1)*dstR, y + dstR + yd);
	}
	pControlDC->MoveTo(x,y+h/2);
	pControlDC->LineTo(x+w,y+h/2);
	//
	pControlDC->SelectObject(oldbrush);
	//DeleteObject(brush);
	brush.DeleteObject();
	//
	//
	CFont font,*oldfont;
	font.CreateFont(
		h/4, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial") // nPitchAndFamily Arial
		);
    //
	oldfont = pControlDC->SelectObject(&font);
	CString strTmp = "��С��";
	CRect TextRect;
	TextRect.left = x;
	TextRect.right = x+w;
	TextRect.top = y+h/2;
	TextRect.bottom = y+h;
	CBrush brush3, *oldbrush3;
	brush3.CreateSolidBrush(RGB(0, 255, 0)); 
	oldbrush3 = pControlDC->SelectObject(&brush3);
	pControlDC->SetTextColor(RGB(255,0,0));
	//
	//pControlDC->DrawText(strTmp, strTmp.GetLength(), &TextRect, DT_CENTER);
	CRect temp = TextRect;
	int height = pControlDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pControlDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//
	pControlDC->SelectObject(oldbrush3);
	brush3.DeleteObject();
	pControlDC->SelectObject(oldfont);
	font.DeleteObject();
	//
}
//
int CPPMSDlg::GetPointPos(CPoint point)
{
	//
	CRect rect;
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_BODY); // 
	pWnd->GetWindowRect(&rect);
	//
	int cx = point.x - m_cx/5;
	int cy = point.y - m_cy/5;
    //
	int w = rect.Width();
	int h = rect.Height();
	int x = 10;
	int y = 10;
	int w_cld = (w - (c+1)*x)/c;
	int h_cld = (h - (r+1)*y)/r;
	//
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
		{
			CRect  rectCld;
			rectCld = CRect(CPoint(40 + j*(x + w_cld), 30 + i*(y + h_cld)), CSize(w_cld, h_cld));
			if(cx > rectCld.left && cx < rectCld.right && cy > rectCld.top && cy < rectCld.bottom)
			{
				return i*c + j + 1;
			}
		}
	}
	return 0;
	//
}
//
void CPPMSDlg::ParseWatchData(int len,const unsigned char *data,string &desc_str,stuElderInfo &elderInfo)
{
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
	if(0 == w_tmp)
	{
        d_rate = 0;
	}
	else
	{
	    d_rate = 60 / (w_tmp * 0.02);
	}
	p += 1;
	//�Ƿ�����
	i_ok = p[0];
	p += 1;
	int hand = 1;
	string strTmp = "";
	if(i_ok > 127)
	{
		d_rate = 0;
		strTmp = "����";
	}
	else 
	{
		hand = 2;
		strTmp = "δ����";
	}
	//
	int i_rate = (int)d_rate;
	sprintf(stmp,"%.04f,%.04f\n%.04f,%.02f,%d,",d_x,d_y,d_z,d_temperature,i_rate);
	desc_str = stmp;
	desc_str += strTmp;
	elderInfo.x = d_x;
	elderInfo.y = d_y;
	elderInfo.z = d_z;
	elderInfo.temperature = d_temperature;
	elderInfo.heart_rate = i_rate;
	elderInfo.hand = hand;
}
//
void CPPMSDlg::PutElderInfo(stuElderInfo elderInfo)
{
	m_csElder.Lock();
	m_vecElderInfo.push_back(elderInfo);
	m_csElder.Unlock();
}
void CPPMSDlg::GetAllElderInfo(vector<stuElderInfo> &vecElderInfo)
{
	m_csElder.Lock();
    std::copy(m_vecElderInfo.begin(),m_vecElderInfo.end(),std::back_inserter(vecElderInfo));
	m_vecElderInfo.clear();
	m_csElder.Unlock();
}
//
BOOL CPPMSDlg::UpdateElderInfo_DB()
{
	//
    UpdateElderInfo_MSDB();
	//
	return TRUE;
}
//������Ա��Ϣ
BOOL CPPMSDlg::UpdateElderInfo_MSDB()
{
	vector<stuElderInfo> vecElderInfo;
	vecElderInfo.clear();
	GetAllElderInfo(vecElderInfo);
	vector<stuElderInfo>::iterator iter;
	for(iter = vecElderInfo.begin(); iter != vecElderInfo.end(); iter++)
	{
		//д���ݿ�
		char sqlBuf[1024];
		memset(sqlBuf,0,sizeof(sqlBuf));
				sprintf(sqlBuf,"insert into pm_db.people_health_inf (DEV_ID,CARD_ID,LBL_ID,X,Y,Z,TEMPERATURE,HEART_RATE,HAND,UP_FLG,CHECK_TIME) values ('%s','%s','%s','%f','%f','%f','%f','%d','%d','N','%s')",
			iter->dev_id,iter->card_id,iter->lbl_id,iter->x,iter->y,iter->z,iter->temperature,iter->heart_rate,iter->hand,iter->check_time);
		///mysql_query(&m_myCont,"start transaction;");
		int ret = -1;
		ret = mysql_query(&m_myCont,sqlBuf);
		if(0 == ret)
		{
			//�ɹ�
			//mysql_commit(&m_myCont);
		}
		else
		{
			//mysql_rollback(&m_myCont);
			//ʧ��
			return FALSE;
		}
	}
	return TRUE;
}
//
BOOL CPPMSDlg::StartLogPro()
{
	char stmp[256];
	int iCount = 0;
	int nIndex = 0;
    while(m_bolLog)
	{
	    stuLogContent msg;
	    if(!m_pLogMsg->GetLogMessage(&msg))
		{
			Sleep(1000);
			continue;
		}
		WriteLogMsg(msg);		
	}
    //
	return TRUE;				
}
UINT logProcess(LPVOID Ptr)
{
	CPPMSDlg *pp = (CPPMSDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartLogPro();
	return 1;
}
//
BOOL CPPMSDlg::StartDataPro()
{	
    //���ݴ���
	time_t nowtime;
	time_t fsttime = 0;
	time_t lsttime = 0;
	int frameLen = 0;
	int dataSize = 0;
	unsigned char tmpBuf[DATA_FRAME_LEN];
    CString strTmp;
	int ireaderNo = 0;
	unsigned int icardNo = 0;
	unsigned int ilblNo = 0;
	char stmp[256];
	char scontent[256];
	int cardnum = 0;
	unsigned char sdata[256];
	int  dlen = 0;
	char slog[256];
	while(m_bolData)
	{
		Sleep(3);
		time(&nowtime);
		while(!m_pDataQueue->EmptyQueue()) 
		{
			dataSize = m_pDataQueue->GetDataRecordSum();
			if(dataSize <= 0) 
				break;
			memset(tmpBuf,0,sizeof(tmpBuf));
			dataSize = 0;
			m_pDataQueue->ReadQueue(tmpBuf, &dataSize);
			//����������Ϣ
			if(0x0B == tmpBuf[0])
			{
				//��������֡
				stuCardInfo cardInfo;
				memset(&cardInfo,0,sizeof(stuCardInfo));
				//
				unsigned char *pp = tmpBuf;
				if(IS_NET == m_connType)
				{
				    pp += 2;
				}
				else
				{
				    pp += 1;
				}
				//�豸���
				ireaderNo = pp[0];
				pp += 3;
				cardnum = pp[0];
				pp += 1;
				//��cardnum��ˣ����ӽ�׳��
				int dataLen_1 = 0;
				int dataLen_2 = 0;
				if(IS_NET == m_connType)
				{
					dataLen_1 = 6+cardnum*8+1;
					dataLen_2 = 6+cardnum*18+1;
				}
				else
				{
					dataLen_1 = 5+cardnum*8+1;
					dataLen_2 = 5+cardnum*18+1;
				}
				if((dataLen_1 == dataSize)||(dataLen_2 == dataSize))
				{}
				else
				{
				    //���ݳ�������
					continue;
				}
				int card_data_len = 8;
				if(dataLen_2 == dataSize)card_data_len = 17;
				//
				unsigned char ustmp[256];
				CString strCardNo;
				CString strLblNo;
				for(int i = 0; i < cardnum; i++)
				{
					//����
					memset(ustmp,0,sizeof(ustmp));
					memcpy(ustmp,pp,4);
					icardNo = ustmp[0]*256*256*256 + ustmp[1]*256*256 + ustmp[2]*256 + ustmp[3];
					strCardNo.Format("%010u",icardNo);
					pp += 4;
					//λ�ñ�ǩ��
					memset(ustmp,0,sizeof(ustmp));
					memcpy(ustmp,pp,4);
					ilblNo = ustmp[0]*256*256*256 + ustmp[1]*256*256 + ustmp[2]*256 + ustmp[3];
					strLblNo.Format("%010u",ilblNo);
					pp += 4;
					//
					//map<unsigned int, stuCardInOutStatus> m_mapCardInOutStatus;
					map<unsigned int, stuCardInOutStatus>::iterator iter_ios;
					iter_ios = m_mapCardInOutStatus.find(icardNo);
					if(iter_ios == m_mapCardInOutStatus.end())
					{
					    if(17 == card_data_len)
					    {
							pp += 10;
						}
						else
						{
						    pp += 4;
						}
						continue;
					}
					//���
					if(17 == card_data_len)
					{
		                //m_mapReaderCardInfo[ireaderNo][icardNo].data_len = 9;
						//memcpy(m_mapReaderCardInfo[ireaderNo][icardNo].data,pp,9);
						memset(sdata,0,sizeof(sdata));
						dlen = 10;
                        memcpy(sdata,pp,10);
						//��������
						string desc_str = "";
						stuElderInfo elderInfo;
						memset(&elderInfo,0,sizeof(stuElderInfo));
						ParseWatchData(dlen,sdata,desc_str,elderInfo);
						sprintf(elderInfo.dev_id,"%03d",ireaderNo);
						sprintf(elderInfo.card_id,"%010u",icardNo);
						sprintf(elderInfo.lbl_id,"%010u",ilblNo);
						//ʱ��
						struct tm *now = NULL;
						time_t ttime = time(NULL);
						now = localtime(&ttime);
						sprintf(elderInfo.check_time,"%d-%02d-%02d %02d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
						PutElderInfo(elderInfo);
						//
						////strcpy(m_mapElderInfo[icardNo].card_id,elderInfo.card_id);
						////m_mapElderInfo[icardNo].x = elderInfo.x;
						////m_mapElderInfo[icardNo].y = elderInfo.y;
						////m_mapElderInfo[icardNo].z = elderInfo.z;
						////m_mapElderInfo[icardNo].temperature = elderInfo.temperature;
						////m_mapElderInfo[icardNo].heart_rate = elderInfo.heart_rate;
						////m_mapElderInfo[icardNo].hand = elderInfo.hand;
						////strcpy(m_mapElderInfo[icardNo].dev_id,elderInfo.dev_id);
						////strcpy(m_mapElderInfo[icardNo].lbl_id,elderInfo.lbl_id);
						////strcpy(m_mapElderInfo[icardNo].check_time,elderInfo.check_time);
						m_mapCardRtInfo[icardNo].card_no = icardNo;
						m_mapCardRtInfo[icardNo].body_temp = elderInfo.temperature;
						m_mapCardRtInfo[icardNo].heart_rate = elderInfo.heart_rate;
						m_mapCardRtInfo[icardNo].hand = elderInfo.hand;
						m_mapCardRtInfo[icardNo].dev_id = ireaderNo;
						m_mapCardRtInfo[icardNo].lbl_id = ilblNo;
						m_mapCardRtInfo[icardNo].lst_time = time(NULL);
						//
						memset(slog,0,sizeof(slog));
						sprintf(slog,"[%d][%u][%u][%s]",ireaderNo,icardNo,ilblNo,desc_str.c_str());
						AddLogMsg(GRADE_LOG_COMM,slog);
						//
						pp += 10;
					}
						
				}//end for
			}
			PostMessage(WM_PAINT);
		}
	}
    //
	return TRUE;				
}
UINT dataProcess(LPVOID Ptr)
{
	CPPMSDlg *pp = (CPPMSDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartDataPro();
	return 1;
}
//�����߳�
BOOL CPPMSDlg::StartHelper()
{	
	int iCount = 0;
	int iNum = 0;
	char stmp[256];
    while(m_bolHelper)
	{
		if(WAIT_OBJECT_0==WaitForSingleObject(m_hConnFreshEvent,INFINITE))
		{
			if(FALSE == m_bolHelper)break;
			iCount = 0;
			m_lstDev.DeleteAllItems();//���
			vector<stuSockInfo> vecSockInfo;
			m_netTcpSvr.GetAllSockInfo(vecSockInfo);
			vector<stuSockInfo>::iterator iter;
			for(iter = vecSockInfo.begin(); iter != vecSockInfo.end(); iter++)
			{
				iCount++;
				//���
				memset(stmp,0,sizeof(stmp));
				sprintf(stmp,"%03d",iCount);
				m_lstDev.InsertItem(iCount-1, _T(stmp));
				//�豸���
				memset(stmp,0,sizeof(stmp));
				sprintf(stmp,"%03d",iter->reader_no);
				m_lstDev.SetItemText(iCount-1, 1, _T(stmp));
				//IP:[�˿�]
				memset(stmp,0,sizeof(stmp));
				sprintf(stmp,"%s:[%d]",iter->ip.c_str(),iter->port);
				m_lstDev.SetItemText(iCount-1, 2, _T(stmp));
			}
		}
	}    
    //
	return TRUE;				
}
UINT helperThread(LPVOID Ptr)
{
	CPPMSDlg *pp = (CPPMSDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartHelper();
	return 1;
}
BOOL CPPMSDlg::StartDbFresh()
{	
	//
    while(m_bolDb)
	{
		//����
		mysql_ping(&m_myCont);
		Sleep(1000);
		UpdateElderInfo_DB();
	}    
    //
	return TRUE;				
}
UINT dbFresh(LPVOID Ptr)
{
	CPPMSDlg *pp = (CPPMSDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartDbFresh();
	
	return 1;
}
//
BOOL CPPMSDlg::AddLogMsg(int grade, const char *sbody)
{
	stuLogContent msg;
    msg.item           = 0;
	msg.type           = TYPE_LOG_DAT;
	msg.grade          = grade;
	msg.body           = sbody;
	msg.title          = "DAT";
	msg.datetime       = time(NULL);
	m_pLogMsg->AddLogMessage(msg);
	return TRUE;
}
//
BOOL CPPMSDlg::WriteLogMsg(stuLogContent msg)
{
	//ʱ��
	char spath[256],scontent[512];
	memset(spath,0,sizeof(spath));
	memset(scontent,0,sizeof(scontent));
	struct tm *now = NULL;
	now = localtime(&msg.datetime); 
	sprintf(spath,".\\log\\%d-%02d-%02d.log",now->tm_year+1900,now->tm_mon+1,now->tm_mday);
	sprintf(scontent,"[%d-%02d-%02d %d:%02d:%02d][%d][%s]\n",
		now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec,msg.grade,msg.body.c_str());
	//	
	FILE *fp = NULL;
	fp = fopen(spath,"a+");
	if(NULL != fp)
	{
		fputs(scontent,fp);
		fclose(fp);
		fp = NULL;
	}
	return TRUE;
}
//
BOOL CPPMSDlg::LoadCardFromMSDb()
{
	//��ʼ����Ƭ�ڿ�״̬
	m_mapCardInOutStatus.clear();
    try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //���ñ����ʽ
        int ret = mysql_query(&m_myCont, "select CARD_ID,IO_FLG,NOW_POS,CHG_DATE from pm_db.card_inf");//��ѯ
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
                while (m_sqlRow = mysql_fetch_row(m_myResult))//��ȡ���������
                {
					unsigned int iCardNo = strtoul(m_sqlRow[0],NULL,10);
					m_mapCardInOutStatus[iCardNo].card_no = iCardNo;
					m_mapCardInOutStatus[iCardNo].io_flg = atoi(m_sqlRow[1]);
					m_mapCardInOutStatus[iCardNo].room_no = atoi(m_sqlRow[2]);
					m_mapCardInOutStatus[iCardNo].pre_room_no = atoi(m_sqlRow[2]);
					m_mapCardInOutStatus[iCardNo].lst_time = StrToTime(m_sqlRow[3]);
                }
				mysql_free_result(m_myResult);
            }
        }
        else
        {
			MessageBox("mysql��ʼ����Ƭʧ��1��");
			SendMessage(WM_CLOSE);
			return FALSE;
        }
	}
	catch(...)
	{
		//
		MessageBox("mysql��ʼ����Ƭʧ��2��");
		SendMessage(WM_CLOSE);
		return FALSE;
	}
	return TRUE;
}
//������Ա��Ϣ
BOOL CPPMSDlg::LoadPeopleFromMSDb()
{
	//��ʼ����Ա��Ϣ
	m_mapPeopleInfo.clear();
    try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //���ñ����ʽ
        int ret = mysql_query(&m_myCont, "select CARD_ID,POLICE,PEPOLE_NAME,ID_NO,PHONE_NO,ADDR from pm_db.card_pepole_inf");//��ѯ
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
                while (m_sqlRow = mysql_fetch_row(m_myResult))//��ȡ���������
                {
					unsigned int iCardNo = strtoul(m_sqlRow[0],NULL,10);
					m_mapPeopleInfo[iCardNo].card_no = iCardNo;
					m_mapPeopleInfo[iCardNo].police = m_sqlRow[1];
					m_mapPeopleInfo[iCardNo].name = m_sqlRow[2];
					m_mapPeopleInfo[iCardNo].id_no = m_sqlRow[3];
					m_mapPeopleInfo[iCardNo].phone_no = m_sqlRow[4];
					m_mapPeopleInfo[iCardNo].addr = m_sqlRow[5];
                }
				mysql_free_result(m_myResult);
            }
        }
        else
        {
			MessageBox("mysql��ʼ����Ա��Ϣʧ��1��");
			SendMessage(WM_CLOSE);
			return FALSE;
        }
	}
	catch(...)
	{
		//
		MessageBox("mysql��ʼ����Ա��Ϣʧ��2��");
		SendMessage(WM_CLOSE);
		return FALSE;
	}
	return TRUE;
}
//���ر���������
BOOL CPPMSDlg::LoadWarnFromMSDb()
{
	//��ʼ������������
	m_mapWarnInfo.clear();
    try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //���ñ����ʽ
        int ret = mysql_query(&m_myCont, "select CARD_ID,POS_UP,POS_DOWN,BODY_TEMP_UP,BODY_TEMP_DOWN,HEART_RATE_UP,HEART_RATE_DOWN,HAND_UP,HAND_DOWN from pm_db.card_warn_inf");//��ѯ
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
                while (m_sqlRow = mysql_fetch_row(m_myResult))//��ȡ���������
                {
					unsigned int iCardNo = strtoul(m_sqlRow[0],NULL,10);
					m_mapWarnInfo[iCardNo].card_no = iCardNo;
					m_mapWarnInfo[iCardNo].pose_up = atoi(m_sqlRow[1]);
					m_mapWarnInfo[iCardNo].pose_down = atoi(m_sqlRow[2]);
					m_mapWarnInfo[iCardNo].body_temp_up = atof(m_sqlRow[3]);
					m_mapWarnInfo[iCardNo].body_temp_down = atof(m_sqlRow[4]);
					m_mapWarnInfo[iCardNo].heart_rate_up = atof(m_sqlRow[5]);
					m_mapWarnInfo[iCardNo].heart_rate_down = atof(m_sqlRow[6]);
					m_mapWarnInfo[iCardNo].hand_up = atoi(m_sqlRow[7]);
					m_mapWarnInfo[iCardNo].hand_down = atoi(m_sqlRow[8]);
                }
				mysql_free_result(m_myResult);
            }
        }
        else
        {
			MessageBox("mysql��ʼ������������ʧ��1��");
			SendMessage(WM_CLOSE);
			return FALSE;
        }
	}
	catch(...)
	{
		//
		MessageBox("mysql��ʼ������������ʧ��2��");
		SendMessage(WM_CLOSE);
		return FALSE;
	}
	return TRUE;
}
//�����豸λ��
BOOL CPPMSDlg::LoadDevPosFromMSDb()
{
	//��ʼ���豸λ��
	m_mapDevPosInfo.clear();
    try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //���ñ����ʽ
        int ret = mysql_query(&m_myCont, "select LBL_ID,POS_NAME from pm_db.dev_pos_inf");//��ѯ
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
                while (m_sqlRow = mysql_fetch_row(m_myResult))//��ȡ���������
                {
					unsigned int iDevNo = strtoul(m_sqlRow[0],NULL,10);
					m_mapDevPosInfo[iDevNo].dev_id = iDevNo;
					m_mapDevPosInfo[iDevNo].name = m_sqlRow[1];
                }
				mysql_free_result(m_myResult);
            }
        }
        else
        {
			MessageBox("mysql��ʼ���豸λ��ʧ��1��");
			SendMessage(WM_CLOSE);
			return FALSE;
        }
	}
	catch(...)
	{
		//
		MessageBox("mysql��ʼ���豸λ��ʧ��2��");
		SendMessage(WM_CLOSE);
		return FALSE;
	}
	return TRUE;
}
//���ر�ǩλ��
BOOL CPPMSDlg::LoadLblPosFromMSDb()
{
	//��ʼ����ǩλ��
	m_mapLblPosInfo.clear();
    try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //���ñ����ʽ
        int ret = mysql_query(&m_myCont, "select LBL_ID,POS_NAME from pm_db.lbl_pos_inf");//��ѯ
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
                while (m_sqlRow = mysql_fetch_row(m_myResult))//��ȡ���������
                {
					unsigned int iLblNo = strtoul(m_sqlRow[0],NULL,10);
					m_mapLblPosInfo[iLblNo].lbl_id = iLblNo;
					m_mapLblPosInfo[iLblNo].name = m_sqlRow[1];
                }
				mysql_free_result(m_myResult);
            }
        }
        else
        {
			MessageBox("mysql��ʼ����ǩλ��ʧ��1��");
			SendMessage(WM_CLOSE);
			return FALSE;
        }
	}
	catch(...)
	{
		//
		MessageBox("mysql��ʼ����ǩλ��ʧ��2��");
		SendMessage(WM_CLOSE);
		return FALSE;
	}
	return TRUE;
}
//������Ա��Ϣ
BOOL CPPMSDlg::LoadPeopleFromIni()
{
	m_mapPeopleInfo.clear();
	//��ʼ����Ա��Ϣ
	string filePath = ".\\cfg\\people.ini";
	if (access(filePath.c_str(), 0) == -1)
	{
		return FALSE;
	}
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return FALSE;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if ("��Ա��Ϣ" != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "������" || str_first == "��·��")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 6)
			{
				continue;
			}
			int itmpNo = atoi(str_id.c_str());
			//
			m_mapPeopleInfo[itmpNo].card_no = itmpNo;
			m_mapPeopleInfo[itmpNo].police = vStr[1];
			m_mapPeopleInfo[itmpNo].name = vStr[2];
			m_mapPeopleInfo[itmpNo].id_no = vStr[3];
			m_mapPeopleInfo[itmpNo].phone_no = vStr[4];
			m_mapPeopleInfo[itmpNo].addr = vStr[5];
		}
	}
	//
	return TRUE;
}
//���ر���������
BOOL CPPMSDlg::LoadWarnFromIni()
{
	//��ʼ������������
	m_mapWarnInfo.clear();
	string filePath = ".\\cfg\\warn.ini";
	if (access(filePath.c_str(), 0) == -1)
	{
		return FALSE;
	}
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return FALSE;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if ("����������" != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "������" || str_first == "��·��")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 9)
			{
				continue;
			}
			int itmpNo = atoi(str_id.c_str());
			//
			m_mapWarnInfo[itmpNo].card_no = itmpNo;
			m_mapWarnInfo[itmpNo].pose_up = atoi(vStr[1].c_str());
			m_mapWarnInfo[itmpNo].pose_down = atoi(vStr[2].c_str());
			m_mapWarnInfo[itmpNo].body_temp_down = atof(vStr[3].c_str());
			m_mapWarnInfo[itmpNo].body_temp_up = atof(vStr[4].c_str());
			m_mapWarnInfo[itmpNo].heart_rate_down = atof(vStr[5].c_str());
			m_mapWarnInfo[itmpNo].heart_rate_up = atof(vStr[6].c_str());
			m_mapWarnInfo[itmpNo].hand_up = atoi(vStr[7].c_str());
			m_mapWarnInfo[itmpNo].hand_down = atoi(vStr[8].c_str());
		}
	}
	//
	return TRUE;
}
//�����豸λ��
BOOL CPPMSDlg::LoadDevPosFromIni()
{
	//��ʼ���豸λ��
	m_mapDevPosInfo.clear();
	string filePath = ".\\cfg\\dev_pos.ini";
	if (access(filePath.c_str(), 0) == -1)
	{
		return FALSE;
	}
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return FALSE;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if ("�豸λ����Ϣ" != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "������" || str_first == "��·��")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 2)
			{
				continue;
			}
			int itmpNo = atoi(str_id.c_str());
			//
			m_mapDevPosInfo[itmpNo].dev_id = itmpNo;
			m_mapDevPosInfo[itmpNo].name = vStr[1];
		}
	}
	//
	return TRUE;
}
//���ر�ǩλ��
BOOL CPPMSDlg::LoadLblPosFromIni()
{
	//��ʼ����ǩλ��
	m_mapLblPosInfo.clear();
   	string filePath = ".\\cfg\\lbl_pos.ini";
	if (access(filePath.c_str(), 0) == -1)
	{
		return FALSE;
	}
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return FALSE;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if ("��ǩλ����Ϣ" != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "������" || str_first == "��·��")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 2)
			{
				continue;
			}
			int itmpNo = atoi(str_id.c_str());
			//
			m_mapLblPosInfo[itmpNo].lbl_id = itmpNo;
			m_mapLblPosInfo[itmpNo].name = vStr[1];
		}
	}
	//
	return TRUE;
}
//
BOOL CPPMSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	int cx = GetSystemMetrics(SM_CXSCREEN);//��Ļ���ؿ��
	int cy = GetSystemMetrics(SM_CYSCREEN);//��Ļ���ظ߶�
	m_cx = cx;
	m_cy = cy;
    //���ر�����
    DWORD dwStyle = GetStyle();//��ȡ����ʽ  
    DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE| WS_SYSMENU |WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;  
    dwNewStyle&=dwStyle;//��λ�뽫����ʽȥ��  
    SetWindowLong(m_hWnd,GWL_STYLE,dwNewStyle);//���ó��µ���ʽ  
    DWORD dwExStyle = GetExStyle();//��ȡ����չ��ʽ  
    DWORD dwNewExStyle = WS_EX_LEFT |WS_EX_LTRREADING |WS_EX_RIGHTSCROLLBAR;  
    dwNewExStyle&=dwExStyle;//��λ�뽫����չ��ʽȥ��  
    SetWindowLong(m_hWnd,GWL_EXSTYLE,dwNewExStyle);//�����µ���չ��ʽ  
    SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);//����windows���ҵ���ʽ�ı��ˣ�����λ�úʹ�С����ԭ�����䣡
	//
    ShowWindow(SW_MAXIMIZE);
	//
	//m_lstDev.InsertColumn(0, _T(""), LVCFMT_LEFT, 450,0);	
	long ExStyle = LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT;//�������ǻ������ߵ�
	m_lstDev.SetExtendedStyle(ExStyle);

	m_lstDev.SetHeaderHeight(1.1);//2.0				//����ͷ���߶�
	m_lstDev.SetRowHeigt(30);//25					//�����и߶�
	m_lstDev.SetHeaderFontHW(18,8);				//����ͷ������߶�,�Ϳ��,0��ʾȱʡ������Ӧ 
	m_lstDev.SetFontHW(15,7);					//��������߶ȣ��Ϳ��,0��ʾȱʡ���

	m_lstDev.SetHeaderBKColor(0,255,255,0);		//����ͷ������ɫ
	m_lstDev.SetHeaderTextColor(RGB(255,0,255));	//����ͷ��������ɫ

	m_lstDev.SetColColor(0,RGB(189,252,201));	//�����б���ɫ
	m_lstDev.SetColColor(1,RGB(245,222,179));	//�����б���ɫ

	m_lstDev.SetColTextColor(0,RGB(210,105,30));	//�������ı���ɫ
	m_lstDev.SetColTextColor(1,RGB(0,0,255));	//�������ı���ɫ
	//
	CRect eRect;
	eRect.left = 0;
	eRect.top = m_cy/5;
	eRect.right = m_cx/5;
	eRect.bottom = m_cy; 
	GetDlgItem(IDC_LST_DEV)->MoveWindow(&eRect);
	//
	CRect rect;
	dwStyle = m_lstDev.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstDev.SetExtendedStyle(dwStyle);
	m_lstDev.GetClientRect(&rect); 
    m_lstDev.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/6);
    m_lstDev.InsertColumn(1, _T("�豸���"), LVCFMT_LEFT, rect.Width()/4);
	m_lstDev.InsertColumn(2, _T("IP��ַ"), LVCFMT_LEFT, rect.Width()*7/12);
	//
	char stmp[256];
	CString strPath(".\\cfg\\syscfg.ini");
	//
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("��������", "ip","",stmp,256,strPath.GetBuffer());
	m_svrIp = stmp;
	m_svrPort = GetPrivateProfileInt("��������", "port",0,strPath.GetBuffer());
	m_freshTime = 10;
	m_logGrade = 1;
    m_protocolType = 1;
	m_useDb = 3;
	//
	memset(&m_mySqlCfg,0,sizeof(stuMySqlCfg));
	m_mySqlCfg.my_port = GetPrivateProfileInt("mysql����", "my_port",0,strPath.GetBuffer());
	if(m_mySqlCfg.my_port <= 0)m_mySqlCfg.my_port = 3306;
	GetPrivateProfileString("mysql����", "my_host","",m_mySqlCfg.my_host,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql����", "my_db","",m_mySqlCfg.my_db,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql����", "my_user","",m_mySqlCfg.my_user,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql����", "my_password","",m_mySqlCfg.my_password,256,strPath.GetBuffer());
	m_connType = GetPrivateProfileInt("������������", "conn_type",0,strPath.GetBuffer());
	//
	m_bolLog = TRUE;
	m_bolData = TRUE;
	m_bolDb = TRUE;
	m_bolHelper = TRUE;
	//
	m_pDataQueue = new CDataQueue;
	m_pLogMsg    = new CLogMessage;
	m_pDataQueue->RemoveAll();
	m_pLogMsg->RemoveAll();
	//
	////�������ݿ�
	if(3 == m_useDb)
	{
		MYSQL *tmpRet = NULL;
		tmpRet = mysql_init(&m_myCont);
		if(NULL == tmpRet)
		{
			MessageBox("mysql���ݿ�����ʧ��1��");
			SendMessage(WM_CLOSE);
		}
		tmpRet = mysql_real_connect(&m_myCont, m_mySqlCfg.my_host, m_mySqlCfg.my_user, m_mySqlCfg.my_password, m_mySqlCfg.my_db, m_mySqlCfg.my_port, NULL, CLIENT_MULTI_STATEMENTS);
		if(NULL == tmpRet)
		{
			MessageBox("mysql���ݿ�����ʧ��2��");
			SendMessage(WM_CLOSE);
		}
	}
	//
	BOOL bol = LoadCardFromMSDb();
	if(FALSE == bol)
	{
		//MessageBox("���ؿ�Ƭʧ�ܣ�");
		//SendMessage(WM_CLOSE);
		return FALSE;
	}
	//////LoadPeopleFromMSDb();
 //////   LoadWarnFromMSDb();
	//////LoadDevPosFromMSDb();
	//////LoadLblPosFromMSDb();
	LoadPeopleFromIni();
    LoadWarnFromIni();
	LoadDevPosFromIni();
	LoadLblPosFromIni();
	//
	m_mapCardRtInfo.clear();
	//
    InitHead();
	InitBody();
	//
	m_hConnFreshEvent = CreateEvent(NULL,FALSE,FALSE,NULL);//
	//��־�����߳�
	m_logThread = AfxBeginThread(logProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_logThread->m_bAutoDelete = TRUE;
	m_logThread->ResumeThread();
	//���ݴ����߳�
	m_dataThread = AfxBeginThread(dataProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_dataThread->m_bAutoDelete = TRUE;
	m_dataThread->ResumeThread();
	//�������ݿ��߳�
	m_dbThread = AfxBeginThread(dbFresh,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_dbThread->m_bAutoDelete = TRUE;
	m_dbThread->ResumeThread();
	//�����߳�
	m_helperThread = AfxBeginThread(helperThread,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_helperThread->m_bAutoDelete = TRUE;
	m_helperThread->ResumeThread();
	//
	bol = m_netTcpSvr.StartTcpServer(m_svrIp.GetBuffer(),m_svrPort,m_pDataQueue,m_pLogMsg,&m_hConnFreshEvent,m_connType);
	if(FALSE == bol)
	{
		MessageBox("TCP����������ʧ�ܣ�");
		return FALSE;
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPPMSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPPMSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
		//
		//InitHead();
		InitBody();
		//
	}
	else
	{
		//
		//InitHead();
		InitBody();
		//
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPPMSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL CPPMSDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch(LOWORD(wParam))
    {
    case WM_RMENU_CMD_TEST_1:
		{
			//��������
			CBaseInfoDlg bid;
			bid.DoModal();
			break;
		}
    case WM_RMENU_CMD_TEST_2:
		{
			//������������
			CWarnUdDlg wud;
			wud.DoModal();
			break;
		}
    case WM_RMENU_CMD_TEST_3:
		{
            //����
			CPoseHisDlg phd;
			phd.DoModal();
            break;
		}
    case WM_RMENU_CMD_TEST_4:
		{
			//����
			CTempHisDlg thd;
			thd.DoModal();
			break;
		}
    case WM_RMENU_CMD_TEST_5:
		{
            //����
			CHrateHisDlg hhd;
			hhd.DoModal();
            break;
		}
    case WM_RMENU_CMD_TEST_6:
		{
			//����
			CHandHisDlg hhd;
			hhd.DoModal();
			break;
		}
    default:
        break;
    }

    return CDialog::OnCommand(wParam, lParam);
}
BOOL CPPMSDlg::DestroyWindow() 
{
	m_bolLog = FALSE;
	m_bolData = FALSE;
	m_bolDb = FALSE;
	m_bolHelper = FALSE;
	SetEvent(m_hConnFreshEvent);
	//
	m_netTcpSvr.StopTcpServer();
	//
	Sleep(3000);
	//
	if(m_pDataQueue != NULL)
	{
		m_pDataQueue->RemoveAll();
		delete m_pDataQueue;
		m_pDataQueue = NULL;
	}
	//
	if(m_pLogMsg != NULL)
	{
		delete m_pLogMsg;
		m_pLogMsg = NULL;
	}
	if(3 == m_useDb)
	{
		mysql_close (&m_myCont);
	}
	//
	return CDialog::DestroyWindow();
}
void CPPMSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnTimer(nIDEvent);
}

void CPPMSDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    int n = GetPointPos(point);
	CString strTmp;
	if(n > 0)
	{
		//��������
		CBaseInfoDlg bid;
		bid.DoModal();
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CPPMSDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int n = GetPointPos(point);
	if(n <= 0)return;
    CMenu menu;
    menu.CreatePopupMenu();
    menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_1, "�������ϲ�ѯ����");
	menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_2, "����������������");
    menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_3, "������ʷ���ݲ�ѯ");
    menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_4, "������ʷ��������");
	menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_5, "������ʷ��������");
	menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_6, "������ʷ���ݲ�ѯ");

    POINT tpoint;
    tpoint.x = point.x;
    tpoint.y = point.y;
    ClientToScreen(&tpoint);
    menu.TrackPopupMenu(TPM_LEFTALIGN, tpoint.x, tpoint.y, this);
	//
	CDialog::OnRButtonDown(nFlags, point);
}

void CPPMSDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnLButtonDown(nFlags, point);
}

void CPPMSDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnMouseMove(nFlags, point);
}
