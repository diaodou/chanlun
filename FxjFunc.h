#ifndef __FXJFUNC_H_INCLUDE
#define __FXJFUNC_H_INCLUDE

/*
///////////////////////////////////////////////////////////////////////////
��������չ�����淶V3.10
1.���淶�����ڷ�����3.10��׼���רҵ�湫ʽϵͳ.
2.��չ��������ʵ��ϵͳ��������ʵ�ֵ������㷨.
3.��չ������windows 32λ��̬���ӿ�ʵ��,����ʹ��Microsoft Visual C++���.
4.����ʱ�ڹ�ʽ�༭����д"��̬������@��������"(������)����,�������溯������дΪ"FXJFUNC@MYCMALOSE"(5)
5.��̬���ӿ����ƺͺ������ƿ����Լ�����.
6.ʹ��ʱ���Խ���̬�⿽����������Ŀ¼��ʹ��.
*/

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

///////////////////////////////////////////////////////////////////////////
//��������
enum DATA_TYPE
{
	TICK_DATA=2,				//�ֱʳɽ�
	MIN1_DATA,					//1������
	MIN5_DATA,					//5������					
	MIN15_DATA,					//15������
	MIN30_DATA,					//30������
	MIN60_DATA,					//60������
	DAY_DATA,					//����
	WEEK_DATA,					//����
	MONTH_DATA,					//����
	MULTI_DATA					//������
};

///////////////////////////////////////////////////////////////////////////
//��������

typedef struct tagSTKDATA	
{
	time_t	m_time;			//ʱ��,UCT
	float	m_fOpen;		//����
	float	m_fHigh;		//���
	float	m_fLow;			//���
	float	m_fClose;		//����
	float	m_fVolume;		//�ɽ���
	float	m_fAmount;		//�ɽ���
	WORD	m_wAdvance;		//���Ǽ���(��������Ч)
	WORD	m_wDecline;		//�µ�����(��������Ч)
} STKDATA;


////////////////////////////////////////////////////////////////////////////
//��չ����,���������ֱʳɽ����ݵ�������

typedef union tagSTKDATAEx
{
	struct
	{
		float m_fBuyPrice[3];		//��1--��3��
		float m_fBuyVol[3];			//��1--��3��
		float m_fSellPrice[3];		//��1--��3��	
		float m_fSellVol[3];		//��1--��3��
	};
	float m_fDataEx[12];			//����
} STKDATAEx;

/////////////////////////////////////////////////////////////////////////////
/*��������˳��(m_pfFinData����)

	���	����

	0	�ܹɱ�(���),
	1	���ҹ�,
	2	�����˷��˹�,
	3	���˹�,
	4	B��,
	5	H��,
	6	��ͨA��,
	7	ְ����,
	8	A2ת���,
	9	���ʲ�(ǧԪ),
	10	�����ʲ�,
	11	�̶��ʲ�,
	12	�����ʲ�,
	13	����Ͷ��,
	14	������ծ,
	15	���ڸ�ծ,
	16	�ʱ�������,
	17	ÿ�ɹ�����,
	18	�ɶ�Ȩ��,
	19	��Ӫ����,
	20	��Ӫ����,
	21	��������,
	22	Ӫҵ����,
	23	Ͷ������,
	24	��������,
	25	Ӫҵ����֧,
	26	�����������,
	27	�����ܶ�,
	28	˰������,
	29	������,
	30	δ��������,
	31	ÿ��δ����,
	32	ÿ������,
	33	ÿ�ɾ��ʲ�,
	34	����ÿ�ɾ���,
	35	�ɶ�Ȩ���,
	36	����������
*/

/////////////////////////////////////////////////////////////////////////////
//�������ݽṹ

typedef struct tagCALCINFO
{
	const DWORD			m_dwSize;				//�ṹ��С
	const DWORD			m_dwVersion;			//��������汾(V2.10 : 0x210)
	const DWORD			m_dwSerial;				//����������к�
	const char*			m_strStkLabel;			//��Ʊ����
	const BOOL			m_bIndex;				//����

	const int			m_nNumData;				//��������(pData,pDataEx,pResultBuf��������)
	const STKDATA*		m_pData;				//��������,ע��:��m_nNumData==0ʱ����Ϊ NULL
	const STKDATAEx*	m_pDataEx;				//��չ����,�ֱʳɽ�������,ע��:����Ϊ NULL

	const int			m_nParam1Start;			//����1��Чλ��
	const float*		m_pfParam1;				//���ò���1	
	const float*		m_pfParam2;				//���ò���2
	const float*		m_pfParam3;				//���ò���3
	const float*		m_pfParam4;				//���ò���3

	float*				m_pResultBuf;			//���������
	const DATA_TYPE		m_dataType;				//��������
	const float*		m_pfFinData;			//��������
} CALCINFO;

/* 
ע: 
	1.�������ò�����m_pfParam1--m_pfParam4����,��ΪNULL���ʾ�ò�����Ч.
	2.��һ��������Чʱ,���������в�������Ч.
		��:m_pfParam2ΪNULL,��m_pfParam3,m_pfParam4һ��ΪNULL.
	3.����1�����ǳ�������������������,�������ֻ��Ϊ��������.
	4.��m_nParam1Start<0, �����1Ϊ��������,��������*m_pfParam1;
	5.��m_nParam1Start>=0,�����1Ϊ����������,m_pfParam1ָ��һ������������,
		�����СΪm_nNumData,������Ч��ΧΪm_nParam1Start--m_nNumData.
		��ʱ����m_pData[x] �� m_pfParam1[x]��һ�µ�
*/


///////////////////////////////////////////////////////////////////////////////////
/* �������

__declspec(dllexport) int xxxxxxxx(CALCINFO* pData);	---------- A
__declspec(dllexport) int xxxxxxxxVAR(CALCINDO* pData);	---------- B

1.����������ȫ����д.
2.��������������A,B������ʽ֮һ����,����ʵ�ʺ����������xxxxxxxx;
	����C++����������� extern "C" {   } ������.
3.������ʽA������������������ȫ������Ϊ�����ĺ���;
	��ʽB������������1Ϊ�������ĺ���;���ֺ������������ں�����VAR��β.
4.������������pData->m_pResultBuf����.
5.��������-1��ʾ�����ȫ��������Ч,���򷵻ص�һ����Чֵλ��,��:
	m_pResultBuf[����ֵ] -- m_pResultBuf[m_nNumData-1]��Ϊ��Чֵ.
6.�������Ƴ��Ȳ��ܳ���15�ֽ�,��̬���ӿ��ļ������ܳ���9�ֽ�(��������չ��),��̬�����Ʋ��ܽ�SYSTEM,EXPLORER
7.����ʱ����ѡ��1�ֽڶ���

*/

//ʾ������,ʹ��ʱ��ʵ�������滻
__declspec(dllexport) int WINAPI MYMACLOSE(CALCINFO* pData);
__declspec(dllexport) int WINAPI MYMAVAR(CALCINFO* pData);

__declspec(dllexport) int WINAPI MYMACLOSE_CALC_PREV(CALCINFO* pData);
__declspec(dllexport) int WINAPI MYMAVAR_CALC_PREV(CALCINFO* pData);


// added by pp
// �Զ��庯��
// ��дΪ�ɵ������õĺ��� СдΪ���ɵ������õĺ���
__declspec(dllexport) int WINAPI TT(CALCINFO* pData); // ������
__declspec(dllexport) int WINAPI VERSION(CALCINFO* pData); // �汾 �� 1.013 1���汾�� 01 С�汾�� ����λС�� 1 alpha(�ڲ��) 2 beta(�����) 3 standard��׼�� 4-5��׼������ 9 release���հ�
__declspec(dllexport) int WINAPI DATE(CALCINFO* pData); // �������� ��20101231 1.013.20101231

__declspec(dllexport) int WINAPI JXZT(CALCINFO* pData); // ����״̬ @since 1.01
__declspec(dllexport) int WINAPI JXYL(CALCINFO* pData); // ����ѹ�� @since 1.01 ����1-8�����
__declspec(dllexport) int WINAPI JXZC(CALCINFO* pData); // ����֧�� @since 1.01
__declspec(dllexport) int WINAPI JXYL2(CALCINFO* pData); // ����ѹ�� @since 1.02 ����5 13 21 34 55 89 144 233
__declspec(dllexport) int WINAPI JXZC2(CALCINFO* pData); // ����֧�� @since 1.02

__declspec(dllexport) int WINAPI kxian(CALCINFO* pData); // ��ʼ��K�� ���� �Լ���������  @since 1.01 @update 1.02
__declspec(dllexport) int WINAPI kxg(CALCINFO* pData); // ��K�ߵ� @since 1.01
__declspec(dllexport) int WINAPI kxd(CALCINFO* pData); // ��K�͵� @since 1.01
__declspec(dllexport) int WINAPI kxfx(CALCINFO* pData); // K�߷��� @since 1.01

__declspec(dllexport) int WINAPI bi(CALCINFO* pData); // �� @since 1.01 @update 1.02
__declspec(dllexport) int WINAPI bilast(CALCINFO* pData); // ������һ�ʽ��������� @since 1.02
__declspec(dllexport) int WINAPI bigglast(CALCINFO* pData); // �����ϵ�1���ʸߵ������ @since 1.02 
__declspec(dllexport) int WINAPI biddlast(CALCINFO* pData); // �����ϵ�1���ʵ͵������ @since 1.02

__declspec(dllexport) int WINAPI duan(CALCINFO* pData); // �� @since 1.02
__declspec(dllexport) int WINAPI duanlast(CALCINFO* pData); // ������һ�ν��������� @since 1.02
__declspec(dllexport) int WINAPI duangglast(CALCINFO* pData); // �����ϵ�1���θߵ������ @since 1.02 
__declspec(dllexport) int WINAPI duanddlast(CALCINFO* pData); // �����ϵ�1���ε͵������ @since 1.02

__declspec(dllexport) int WINAPI zs(CALCINFO* pData); // �������� @since 1.02
__declspec(dllexport) int WINAPI zszg(CALCINFO* pData); // ����ZG @since 1.02
__declspec(dllexport) int WINAPI zszd(CALCINFO* pData); // ����ZD @since 1.02
__declspec(dllexport) int WINAPI zsgg(CALCINFO* pData); // ����GG @since 1.02
__declspec(dllexport) int WINAPI zsdd(CALCINFO* pData); // ����DD @since 1.02
__declspec(dllexport) int WINAPI zszz(CALCINFO* pData); // ��������(�𵴼�����) @since 1.02
__declspec(dllexport) int WINAPI zskslast(CALCINFO* pData); // ������һ�����࿪ʼ������ @since 1.02
__declspec(dllexport) int WINAPI zsjslast(CALCINFO* pData); // ������һ��������������� @since 1.02

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__FXJFUNC_H_INCLUDE
