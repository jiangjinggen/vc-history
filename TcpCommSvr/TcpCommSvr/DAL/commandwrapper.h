// CommandWrapper.h: interface for the CCommandWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDWRAPPER_H__105F73F1_8EE1_42E5_A5D2_1B266DED5B14__INCLUDED_)
#define AFX_COMMANDWRAPPER_H__105F73F1_8EE1_42E5_A5D2_1B266DED5B14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Param.h"
//********************************************
//	执行命令的封装，包括执行语句和执行语句包含的参数
//********************************************
class CCommandWrapper  
{
public:
	CCommandWrapper();
	virtual ~CCommandWrapper();

public:
	//根据传入参数生成命令的封装体
	void Build(CString sp, int sqlType, CInParamList* pParams);
	CString m_sp;
	int		m_sqlType;
	CInParamList m_pParams; 
};

inline CCommandWrapper::CCommandWrapper()
{

}

inline CCommandWrapper::~CCommandWrapper()
{
	for(int i=0;i<m_pParams.size();i++)
	{
		CInParam* para = *(m_pParams.begin()+i);
		//if(para->m_pValue!=NULL)
		//	delete para->m_pValue;
		delete para;
	}
}

inline void CCommandWrapper::Build(CString sp, int sqlType, CInParamList* pParams)
{
	this->m_sp=sp;
	this->m_sqlType= sqlType;
	if(pParams==NULL)
		return;

	m_pParams.clear();	
	for(int i=0;i<pParams->size();i++)
	{
		CInParam* para = (*pParams)[i];
		CInParam* thisParam = new CInParam();
		thisParam->m_name = para->m_name;
		thisParam->m_type = para->m_type;
		if(thisParam->m_type == ENUM_String)
		{
			thisParam->m_strVal = para->m_strVal;
		}
		else if(thisParam->m_type == ENUM_Num)
		{
			thisParam->m_numVal = para->m_numVal;
		}
		else if(thisParam->m_type ==ENUM_Binary)
		{
			thisParam->m_size = para->m_size;
			thisParam->m_pValue= new char[para->m_size];	
			memcpy(thisParam->m_pValue, para->m_pValue,para->m_size);
		}
		m_pParams.push_back(thisParam);
	}
}

#endif // !defined(AFX_COMMANDWRAPPER_H__105F73F1_8EE1_42E5_A5D2_1B266DED5B14__INCLUDED_)
