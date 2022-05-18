/**
* @file MyString.h
* @brief Routines about string operation
* @author DuYuesheng
* @date 2005.12.15
*/

#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#include <afxole.h>
#endif

#include <assert.h>
#include <string>
#include <sstream>
#include <vector>

/**
* Thread safe. I design it by staitc member variable. It is a dangrous class. 
* Warning: you should not store the return pointer for the future use. 
*/
#ifdef _MSC_VER

class I2STR
{
public:
	I2STR(int n)
		: m_data(n)
	{
		::memset(m_buf, 0, 11);
	}
	operator LPCSTR()
	{
		::itoa(m_data, m_buf, 10);
		return m_buf; 
	}
	
private:
	__declspec(thread) static char m_buf[11];
	int m_data;
};

__declspec(selectany) char I2STR::m_buf[11] = "\0";

/**
* @name copy-paste text to clipboard
*/
//@{
inline void CopyToClipboard(const char* pBuf, size_t nLen)
{
	ASSERT(pBuf);
	COleDataSource* psource = new COleDataSource();
	HGLOBAL hMem = GlobalAlloc(GHND | GMEM_SHARE, 1 + nLen);   
	memcpy(GlobalLock(hMem), pBuf, nLen);
	GlobalUnlock(hMem);
	psource->CacheGlobalData(CF_TEXT, hMem);
	psource->SetClipboard();
	psource->FlushClipboard();
}

inline void PasteFromClipboard(std::vector<char>& vBuf)
{
	vBuf.clear();
	COleDataObject obj;
	if (!obj.AttachClipboard())
		return;
	if (!obj.IsDataAvailable(CF_TEXT))
		return;
	HGLOBAL hMem = obj.GetGlobalData(CF_TEXT);
	vBuf.resize(GlobalSize(hMem));
	memcpy(&vBuf[0], GlobalLock(hMem), GlobalSize(hMem));
	GlobalUnlock(hMem);
}
//@}

#endif

/**
* @name powerful and platform independent. These functions can convert not only integer but also float etc.
*/
//@{
template<typename _T> 
inline std::string X2A(const _T& tNumber, int nWidth = 0) 
{ 
	std::ostringstream sOutStream; 
	sOutStream.width(nWidth); 
	sOutStream.fill('0'); 
	sOutStream << tNumber; 
	return sOutStream.str(); 
}

template<typename _T> 
inline _T A2X(const std::string& strNumber, _T def = (_T)0)
{
	_T tResult = def;
	std::stringstream sTranslation;
	sTranslation << strNumber;
	sTranslation >> tResult;
	return tResult;
}

template<typename _T>
inline _T A2X(const char* pszNumber, _T def = (_T)0)
{
	assert(pszNumber);
	std::string strTmp(pszNumber);
	return A2X<_T>(strTmp, def);
}
//@}

inline bool stristr(const char* pszStr, const char* pszSub)
{
	assert(pszStr && pszSub);

	char* pszStrDummy = new char[strlen(pszStr) + 1];
	strcpy(pszStrDummy, pszStr);
	strupr(pszStrDummy);

	char* pszSubDummy = new char[strlen(pszSub) + 1];
	strcpy(pszSubDummy, pszSub);
	strupr(pszSubDummy);

	char* p = strstr(pszStrDummy, pszSubDummy);
	delete []pszStrDummy;
	delete []pszSubDummy;
	return p != NULL;
}

inline size_t SpliteStr(const char* pszSource, const char* pszSeparator, std::vector<std::string>& vStr)
{
	assert(pszSource && pszSeparator);
	vStr.clear();
	std::string strDummy(pszSource);
	std::string::size_type old = 0;
	std::string::size_type idx;
	while (TRUE)
	{
		if ((idx = strDummy.find_first_of(pszSeparator, old)) == std::string::npos)
		{
			if (old < strDummy.size())
				vStr.push_back(strDummy.substr(old));
			break;
		}
		if (old < idx)
			vStr.push_back(strDummy.substr(old, idx - old));
		old = 1 + idx;
	}
	std::vector<std::string>(vStr).swap(vStr);
	return vStr.size();
}

/**
* "12A34F" ---> 0x12A34F
* Note:the caller must check the return value first. If return false,you should ingore the out param res!
*/
inline bool HexString2Int(const char* pszHexStr, unsigned int& res)
{
	assert(pszHexStr);
	const size_t len = strlen(pszHexStr);
	// avoid overflow
	if (len > 8)
		return false;
	res = 0; 
	unsigned int tmp = 0;
	for (size_t i = 0; i < len; i++)
	{
		if (pszHexStr[i] >= '0' && pszHexStr[i] <= '9')   
			tmp = pszHexStr[i] - '0'; 
		else if (pszHexStr[i] >= 'a' && pszHexStr[i] <= 'f') 
			tmp = pszHexStr[i] - 'a' + 10;   
		else if (pszHexStr[i] >= 'A' && pszHexStr[i] <= 'F')   
			tmp = pszHexStr[i] - 'A' + 10;   
		else 
			return false;
		tmp <<= ((len - i - 1) << 2);    
		res |= tmp;   
	}
	return true;
}

/**
* "10 00 00 00 3F 51 44 5A" ---> buff
* return the item count in pOut, the caller should release pOut!
*/
inline int HexMemory2Buff(const char* pszHexMemory, char*& pOut)
{
	assert(pszHexMemory);
	pOut = NULL;
	std::vector<std::string> vStr;
	SpliteStr(pszHexMemory, " ", vStr);
	if (vStr.empty())
		return 0;
	int idx = 0;
	pOut = new char[vStr.size()];
	std::vector<std::string>::iterator pos;
	for (pos = vStr.begin(); pos != vStr.end(); ++pos)
	{
		if (pos->size() > 2)
		{
			delete []pOut;
			pOut = NULL;
			return 0;
		}
		unsigned int res;
		if (!HexString2Int(pos->c_str(), res))
		{
			delete []pOut;
			pOut = NULL;
			return 0;
		}
		pOut[idx++] = res;
	}
	return idx;
}
//将"YYYY-MM-DD HH:MM:SS"格式的时间串转化成time_t
inline time_t StrToTime(const char* str_time,int type = 0)
{
	if(NULL == str_time)return time(NULL);
	if(strlen(str_time) < 19)return time(NULL);
	struct tm tmp_tm;
	//////
	sscanf(str_time, "%4d-%2d-%2d %2d:%2d:%2d",      
          &tmp_tm.tm_year,   
          &tmp_tm.tm_mon,   
          &tmp_tm.tm_mday,   
          &tmp_tm.tm_hour,   
          &tmp_tm.tm_min,  
          &tmp_tm.tm_sec);            
    tmp_tm.tm_year -= 1900;  
    tmp_tm.tm_mon--;   
    tmp_tm.tm_isdst=-1; 
	time_t tmp_time = mktime(&tmp_tm);
	if(tmp_time <= 0)return time(NULL);
	return tmp_time;
}
//将time_t转化成"YYYY-MM-DD HH:MM:SS"格式的时间串
inline bool TimeToStr(char* str_time,time_t itime,int type = 0)
{
	if(NULL == str_time)return false;
	if(itime <= 0)return false;
	struct tm *tmp_tm;
	tmp_tm = localtime(&itime);
	//tmp_tm->tm_year += 1900;  
    //tmp_tm->tm_mon++;   
    //tmp_tm->tm_isdst=-1; 
	//////
	sprintf(str_time, "%4d-%02d-%02d %02d:%02d:%02d",      
          tmp_tm->tm_year + 1900,   
          tmp_tm->tm_mon + 1,   
          tmp_tm->tm_mday,   
          tmp_tm->tm_hour,   
          tmp_tm->tm_min,  
          tmp_tm->tm_sec); 
	return true;
}
#endif