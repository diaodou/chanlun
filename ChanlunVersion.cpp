#include "stdafx.h"
#include "ChanlunVersion.h"

// ��̬����
ChanlunVersion* ChanlunVersion::instance = NULL;

//�汾��Ϣ
//{���汾�ţ��ΰ汾�ţ����99�������а汾�����9�����ڲ��汾�ţ������������99��������������}
//1, 02, 1, 1, 20101228
//1, 02, 1, 2, 20110101
//2, 01, 1, 1, 20110102
const int ChanlunVersion::iVersion[5] = {2, 01, 1, 1, 20110101};

ChanlunVersion::ChanlunVersion()
{
	// nothing
}

ChanlunVersion::~ChanlunVersion()
{
	if(NULL != instance)
	{
		delete instance;
		instance = NULL;
	}
}

// ��ȡΨһʵ��
ChanlunVersion* ChanlunVersion::GetInstance()
{
	if(NULL == instance)
	{
		instance = new ChanlunVersion();
	}

	return instance;
}

float ChanlunVersion::GetVersion()
{
	return (float)iVersion[0] + (float)iVersion[1] / 100 + (float)iVersion[2] / 1000;
}

int ChanlunVersion::GetDate()
{
	return iVersion[4];
}

char* ChanlunVersion::GetAuthor()
{
	return "goodpp2012@gmail.com";
}

char* ChanlunVersion::GetEmail()
{
	return "goodpp2012@gmail.com";
}

char* ChanlunVersion::GetCopyRight()
{
	return "goodpp2012@gmail.com";
}