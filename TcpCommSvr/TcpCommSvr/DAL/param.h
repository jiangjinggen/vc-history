// Param.h: interface for the CInParam class.
//	DAL向外暴露的数据结构，调用DAL时需要用到这些结构
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAM_H__556A0E5A_4ECD_401E_AC63_69BCA4039FAF__INCLUDED_)
#define AFX_PARAM_H__556A0E5A_4ECD_401E_AC63_69BCA4039FAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
//********************************************
//	参数结构体，用于接口中的参数传入
//	使用接口时，先构造该类型参数，传入DAL接口
//********************************************

class CInParam  
{
public:
	CInParam(){m_pValue=NULL;m_size=0;};
	CInParam(CString name,CString value);
	CInParam(CString name,float value);
	CInParam(CString name,void* value ,int size);
	virtual ~CInParam(){ delete []m_pValue; };

	CString m_name;		//参数名称
	int m_type;			//参数类型
	
	//
	char* m_pValue;		//参数值
	int m_size;			//参数大小，只用于不定长类型
	//
	CString m_strVal;
	//
	float m_numVal;
};
typedef std::vector<CInParam*> CInParamList;

//********************************************
//	参数数据类型：
//********************************************
enum EParamType
{
	ENUM_String=1,	//字符串
	ENUM_Binary=2,	//二进制
	ENUM_Num=3,
};

//********************************************
//	执行语句类型
//********************************************
enum ESqlType
{
	ENUM_Text=1,		//sql语句
	ENUM_Stored=2	//存储过程
};

//********************************************
//	数据库连接控制策略
//********************************************
enum EConnStrategy
{
	ENUM_Auto =1,	//由DAL自动控制连接，每个command取一次连接
	ENUM_User =2		//由用户设定、打开、关闭DAL层的连接，
};

inline CInParam::CInParam(CString name,CString value)
{
	m_pValue=NULL;m_size=0;

	this->m_name =name;
	this->m_type =EParamType(ENUM_String);
	m_strVal = value;
}

inline CInParam::CInParam(CString name,float value)
{
	m_pValue=NULL;m_size=0;
	
	this->m_name =name;
	this->m_type =EParamType(ENUM_Num);
	m_numVal = value;
}

//构造Binary参数
inline CInParam::CInParam(CString name,void* value ,int size)
{
	m_size=0;

	this->m_name =name;
	this->m_type =EParamType(ENUM_Binary);;
	//this->m_pValue = value;
	this->m_pValue = new char[size];
	memcpy(m_pValue, value, size);
	this->m_size=size;
}

#endif // !defined(AFX_PARAM_H__556A0E5A_4ECD_401E_AC63_69BCA4039FAF__INCLUDED_)
