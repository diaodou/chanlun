/************************************************************************/
/* 
���۰汾��Ϣ
����ģʽ                                                                   
 */
/************************************************************************/
#ifndef __CHANLUNVERSION_H_INCLUDE
#define __CHANLUNVERSION_H_INCLUDE

/*
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
*/
class ChanlunVersion
{
private:
	ChanlunVersion(); // ���캯��
	~ChanlunVersion(); // ��������
	static ChanlunVersion* instance; // Ψһʵ��
	static const int iVersion[5]; // �汾��Ϣ

public:
	static ChanlunVersion* GetInstance(); // ��ȡʵ�� 
	
	/*
	 *	�汾��Ϣ
	 *  �� 1.013 1���汾�� 01 С�汾�� 
	 *  ����λС�� 1 alpha(�ڲ��) 2 beta(�����) 3 standard��׼�� 4-5��׼������ 6 ��ʱ�� 7 ע��� 8 �ر�� 9 release���հ�
	 */
	float GetVersion();
	
	/*
	 *	��������
	 */
	int GetDate();
	
	/*
	 *	����
	 */
	char* GetAuthor();
	
	/*
	 *	Email
	 */
	char* GetEmail();
	
	/*
	 *	��Ȩ
	 */
	char* GetCopyRight();
};

/*	
#ifdef __cplusplus
}
#endif //__cplusplus
*/
#endif // __CHANLUNTOOLS_H_INCLUDE