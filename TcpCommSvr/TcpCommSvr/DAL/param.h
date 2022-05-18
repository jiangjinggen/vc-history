// Param.h: interface for the CInParam class.
//	DAL���Ⱪ¶�����ݽṹ������DALʱ��Ҫ�õ���Щ�ṹ
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAM_H__556A0E5A_4ECD_401E_AC63_69BCA4039FAF__INCLUDED_)
#define AFX_PARAM_H__556A0E5A_4ECD_401E_AC63_69BCA4039FAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
//********************************************
//	�����ṹ�壬���ڽӿ��еĲ�������
//	ʹ�ýӿ�ʱ���ȹ�������Ͳ���������DAL�ӿ�
//********************************************

class CInParam  
{
public:
	CInParam(){m_pValue=NULL;m_size=0;};
	CInParam(CString name,CString value);
	CInParam(CString name,float value);
	CInParam(CString name,void* value ,int size);
	virtual ~CInParam(){ delete []m_pValue; };

	CString m_name;		//��������
	int m_type;			//��������
	
	//
	char* m_pValue;		//����ֵ
	int m_size;			//������С��ֻ���ڲ���������
	//
	CString m_strVal;
	//
	float m_numVal;
};
typedef std::vector<CInParam*> CInParamList;

//********************************************
//	�����������ͣ�
//********************************************
enum EParamType
{
	ENUM_String=1,	//�ַ���
	ENUM_Binary=2,	//������
	ENUM_Num=3,
};

//********************************************
//	ִ���������
//********************************************
enum ESqlType
{
	ENUM_Text=1,		//sql���
	ENUM_Stored=2	//�洢����
};

//********************************************
//	���ݿ����ӿ��Ʋ���
//********************************************
enum EConnStrategy
{
	ENUM_Auto =1,	//��DAL�Զ��������ӣ�ÿ��commandȡһ������
	ENUM_User =2		//���û��趨���򿪡��ر�DAL������ӣ�
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

//����Binary����
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
