/************************************************************************/
/* 
���ۿɵ������õĺ����ڴ�ʵ��
����ģʽ   
����Chanlun�ӿ��пɵ������õĺ����ڴ�ʵ��                                                                 
 */
/************************************************************************/
#ifndef __CHANLUNTOOLS_H_INCLUDE
#define __CHANLUNTOOLS_H_INCLUDE
/*
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
*/
#include "FxjFunc.h"

class ChanlunTools
{
private:
	ChanlunTools(); // ���캯��
	virtual ~ChanlunTools(); // ��������
	static ChanlunTools* instance; // ����ʵ��
	
	static const int cjunx[8]; // ���۾���1-8��

public:
	static ChanlunTools* GetInstance(); // ��ȡ����ʵ��
	
	/*
	 *	���̼۾��� 
	 */
	float ma_close(CALCINFO* pData, int num, int zq); 

	/*
	 *	�������� 1-9�� 1Ϊȫ�ھ���֮�� 9Ϊȫ�ھ���֮��
	 */
	int jxzt(CALCINFO* pData, int num); 

	/*
	 *	����ѹ�� 9Ϊ��ѹ�ƾ��� ��ͷ���� ����1-8�����
	 */
	int jxyl(CALCINFO* pData, int num);

	/*
	 *	����֧�� 0Ϊ���κ�֧�� ��ͷ���� ����1-8�����
	 */
	int jxzc(CALCINFO* pData, int num); 

	/*
	 *	����ѹ�� 0Ϊ��ѹ�ƾ��� ��ͷ���� ����5 13 21 34 55 89 144 233
	 */
	int jxyl2(CALCINFO* pData, int num);

	/*
	 *	����֧�� 0Ϊ���κ�֧�� ��ͷ����  ����5 13 21 34 55 89 144 233
	 */
	int jxzc2(CALCINFO* pData, int num); 

};

/*
#ifdef __cplusplus
}
#endif //__cplusplus
*/
#endif // __CHANLUNTOOLS_H_INCLUDE
