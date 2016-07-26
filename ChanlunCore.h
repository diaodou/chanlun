/************************************************************************/
/* 
���ۺ���ģ��ʵ��
����ģʽ   
����Chanlun�ӿ��в��ۺ��ĺ����ڴ�ʵ��                                                                 
 */
/************************************************************************/
#ifndef __CHANLUNCORE_H_INCLUDE
#define __CHANLUNCORE_H_INCLUDE

#include "FxjFunc.h"
#include <list>
	
using namespace std;

//�����������
// ����K�߽ṹ �����������ϵ��K��
typedef struct chankx
{
	int no;				// K����� ��1��ʼ��
	float rhigh;		// ��ֵ
	float rlow;			// ��ֵ
	float high;		//���������ĸ�ֵ
	float low;		//���������ĵ�ֵ
	int	flag;			//1�� -1�� 0 �Ƕ���
	float fxqj;		// �������� ���Ϊ���� ��¼����߽�
	int dir;            //K�߷��� 1�� -1�� 2 �ϰ��� -2 �°���
	int bi;				//�� 1�� -1�� 2 �ϰ��� -2 �°���
	int duan;			//�� 1�� -1�� 2 �ϰ��� -2 �°���
} ckx;

// �� (��������)
typedef struct chanbi
{
	int no; // ���
	int noh; // �ߵ�K�߱��
	int nol;  // �͵�K�߱��
	float high; // �ߵ�
	float low; // �͵�
	int dir; // ���� ���� 1�� -1�� 2 �ϰ��� -2 �°���
	int flag; // 1�� -1��
	int qk; // ����1 2 ֮���Ƿ����ȱ�� 
} cbi;

// ��
typedef struct chanduan
{
	int no; // ���
	int noh; // �ߵ�K�߱��
	int nol;  // �͵�K�߱��
	float high; // �ߵ�
	float low; // �͵�
	int flag; //  1�� -1��
	int binum; // ��������
} cduan;

// ��������
typedef struct chanzhongshu
{
	int no; // ���
	int duanno; // �����
	int flag; // ���Ʒ��� 1�� -1��
	int ksno; // zg����K��NO (��zg����zd)
	int jsno; // zd����K��NO
	int znnum; // ����zn��
	float zg; // ZG=min(g1��g2)
	float zd; // ZD=max(d1��d2)
	float gg; // GG=max(gn);
	float dd; // dd=min(dn);
	float zz; // ������(������)
} czhongshu;
//����������� END

typedef list<ckx> KXDATA;
typedef list<cbi> BIDATA;
typedef list<cduan> DUANDATA;
typedef list<czhongshu> ZSDATA;

typedef list<ckx>::iterator CKXIT;
typedef list<cbi>::iterator BIIT;
typedef list<cduan>::iterator DUANIT;
typedef list<czhongshu>::iterator ZSIT;

typedef list<ckx>::const_iterator C_CKXIT;
typedef list<cbi>::const_iterator C_BIIT;
typedef list<cduan>::const_iterator C_DUANIT;
typedef list<czhongshu>::const_iterator C_ZSIT;

// ���ۺ���ʵ��
class ChanlunCore
{
private:
	ChanlunCore(); // ���캯��
	~ChanlunCore(); // ��������

	static ChanlunCore* instance;


	KXDATA kxData; // ���ݲ��۴������K��
	BIDATA xbData; // ���±� �����ϱʿ�ʼ�Ķε��������У�
	BIDATA sbData; // ���ϱ� �����±ʿ�ʼ�Ķε��������У�
	DUANDATA dData; // ��
	ZSDATA zsData; // ����
	
	float biQuekou;
	int firstDuanDir;

	void initBiQK(CALCINFO* pData); // ��ʼ��ȱ��
	void initTZXL(); // ��ʼ����������
	void initDuanList(); //��ʼ����

	BIIT findTZG(int fromNo); // �����������еĶ�����
	BIIT findTZD(int fromNo); // �����������еĵ׷���
	
	void findFanTanZS(int duanno, int begin, int end, int high, int low); // �����µ�������
	void findHuiTiaoZS(int duanno, int begin, int end, int high, int low); // ��������������

public:
	static ChanlunCore* GetInstance(); //��ȡ Ψһʵ��
	
	void initKx(CALCINFO* pData); // ��ʼ������K��
	void initFX(); // ��ʼ������
	void initBi();	// ��ʼ����
	void initDuan(); // ��ʼ����
	void initZhongshu(); // ��ʼ������

	CKXIT getCKX(int num);

	// Getter
	KXDATA getCkxData();
	BIDATA getXbData();
	BIDATA getSbData(); 
	DUANDATA getDuanData();
	ZSDATA getZsData();

	// �Զ��峣��
	// ���� 1���� -1����
	static const int DIR_0;
	static const int DIR_UP;
	static const int DIR_DN;
	static const int DIR_SBH;
	static const int DIR_XBH;
	
	static const int QK_N; // ������ȱ��
	static const int QK_Y; // ����ȱ��
};

#endif // __CHANLUNTOOLS_H_INCLUDE