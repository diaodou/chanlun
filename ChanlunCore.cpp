#include "stdafx.h"
#include "ChanlunCore.h"


// ��ʼ����̬����
ChanlunCore* ChanlunCore::instance = NULL;

const int ChanlunCore::DIR_0 = 0; 
const int ChanlunCore::DIR_UP = 1;
const int ChanlunCore::DIR_DN = -1;
const int ChanlunCore::DIR_XBH = -2;
const int ChanlunCore::DIR_SBH = 2;
const int ChanlunCore::QK_N = 0; // ������ȱ��
const int ChanlunCore::QK_Y = 1; // ����ȱ��

ChanlunCore::ChanlunCore()
{
	biQuekou = 0;
	firstDuanDir = DIR_0;
}

ChanlunCore::~ChanlunCore()
{
	if(NULL != instance)
	{
		delete instance;
		instance = NULL;
	}
}

ChanlunCore* ChanlunCore::GetInstance()
{
	if(NULL == instance)
	{
		instance = new ChanlunCore();
	}

	return instance;
}

void ChanlunCore::initBiQK(CALCINFO* pData)
{
	const qk = 0.005;
	if(NULL == pData) return;

	switch(pData->m_dataType)
	{
	case MIN1_DATA: // 1F
		biQuekou = qk;
	case MIN5_DATA:					//5������		
		biQuekou = qk * 3;
	case MIN15_DATA:					//15������
		biQuekou = qk * 6;
	case MIN30_DATA:					//30������
		biQuekou = qk * 9;
	case MIN60_DATA:					//60������
		biQuekou = qk * 18;
	case DAY_DATA:					//����
		biQuekou = qk * 30;
	case WEEK_DATA:					//����
		biQuekou = qk * 90;
	case MONTH_DATA:					//����
		biQuekou = qk * 250;
		//case YEAR_DATA:				//û�����ߣ���
		//	biQuekou = qk * 1000;
		
	default:
		biQuekou = qk * 1000;
	}
}

void ChanlunCore::initKx(CALCINFO* pData)
{
	if(!kxData.empty()) kxData.clear();

	if (NULL != pData && pData->m_nNumData>0) 
	{
		initBiQK(pData);


		float h=0, l=0, h1=0, l1=0;
		int dir = 0;
		
		int tj_jg = 5;
		
		/* ������ȡ
		int dt = pData->m_dataType;
		h = pData->m_pData[dt, i].m_fHigh;
		l = pData->m_pData[dt, i].m_fLow;
		*/
		// ���������ϵ
		for(int i=0; i<pData->m_nNumData; i++)
		{
			h = pData->m_pData[i].m_fHigh;
			l = pData->m_pData[i].m_fLow;
			
			if (dir > DIR_0)
			{
				// ���ڰ�����ϵ
				if ((h>=h1 && l<=l1) 
					|| (h<=h1 && l>=l1))
				{
					// �ߵ�ĸߵ� �͵�ĸߵ�
					h = h>h1 ? h : h1;
					l = l>l1 ? l : l1;
					
					dir = DIR_SBH;
				}
				
				
			}
			else if (dir < DIR_0)
			{
				
				// ���ڰ�����ϵ
				if ((h>=h1 && l<=l1) 
					|| (h<=h1 && l>=l1))
				{
					// �ߵ�ĵ͵� �͵�ĵ͵�
					h = h>h1 ? h1 : h;
					l = l>l1 ? l1 : l;
					
					dir = DIR_XBH;
				}
				
				
			}
			
			if(h>h1 && l>l1)
			{
				dir = DIR_UP;
			} else if (h<h1 && l<l1)
			{
				dir = DIR_DN;
			}
			
			ckx kx;
			kx.low = l;
			kx.high = h;
			kx.dir = dir;
			kx.flag = DIR_0;
			kx.fxqj = 0;
			kx.bi = DIR_0;
			kx.duan = DIR_0;
			kx.no = i + 1;

			kx.rhigh = h;
			kx.rlow = l;
			h1 = h;
			l1 = l;
			
			kxData.push_back(kx);
		}
	}
}

void ChanlunCore::initFX()
{
		int kxnum = kxData.size();
		if(kxnum <= 5) return;

		int i = 0, j = 0, k = 0;

		float h = 0, h11 = 0, h12 = 0, h13 = 0, h21 = 0;
		float l = 0, l11 = 0, l12 = 0, l13 = 0, l21 = 0;
		float p31 = 0, p32 = 0, p33 = 0, quekou = 0;
		bool tjg1 = false, tjd1 = false, tjc = false;
		
		int jg = 0, jg2 = 0; // ����5��K��
		int gdnum = 0;  //��������

		int tj_jg = 3, tj_jg2 = 3;
		//if (pData->m_dataType > WEEK_DATA) tj_jg = 4;

		CKXIT kx, kxt, kxl, kxlg, kxld;
		
		kx = kxData.begin();
		kx = getCKX(2);

		// ������׷���
		for(i=2; i<kxnum-1; i++, kx++)
		{
			jg++;
			//if(DIR_DN==kx->dir || DIR_UP==kx->dir) jg2++; // ���������ϵ��ļ��

			h11 = 0, h12 = 0, h13 = 0, h21 = 0, l11 = 0, l12 = 0, l13 = 0,  l21 = 0, p31 = 0, p32 = 0, p33 = 0;
			
			tjg1 = false, tjd1 = false;

			h = kx->high;
			l = kx->low;
			
			j = i;
			kxt = kx;			
			// ǰ��һ���ߵ�
			do {
				j = j - 1;
				if(j>0) 
				{
					kxt--;
					h11 = kxt->high;
					l11 = kxt->low;
				}			
			} while(h11 == h && j>0);
			// ǰ�ڶ����ߵ�
				j = j - 1;
				if(j>0) 
				{
					kxt--;
					h12 = kxt->high;	
				}			
			
			// ǰ�������ߵ�
				j = j - 1;
				if(j>0) 
				{
					kxt--;
					h13 = kxt->high;	
				}			

			k = i;
			kxt = kx;
			// ���һ���ߵ�
			do {
				k = k + 1;
				if(k<kxnum-1) 
				{
					kxt++;
					h21 = kxt->high;	
					l21 = kxt->low;
				}			
			} while(h21 == h && k<kxnum-1);
			
			// ���ж� ���۶��� �Լ� ���>���ֵ�ǰ2��K�ߵĸߵ�
			tjg1 = h>h11 && h>h21 && h>h12 && h>h13 ;
			
			// �Ƕ�
			if(!tjg1)
			{				
				j = i;
				kxt = kx;			
				// ǰ��һ���͵�
				do {
					j = j - 1;
					if(j>0) 
					{
						kxt--;
						h11 = kxt->high;	
						l11 = kxt->low;
					}			
				} while(l11 == l && j>0);
				// ǰ��2���͵�
				j = j - 1;
				if(j>0) 
				{
					kxt--;
					h12 = kxt->high;	
					l12 = kxt->low;
				}			
			
				// ǰ��3���͵�
				j = j - 1;
				if(j>0) 
				{
					kxt--;
					h13 = kxt->high;
					l13 = kxt->low;
				}			

				
				k = i;
				kxt = kx;
				// ���һ���͵�
				do {
					k = k + 1;
					if(k<kxnum-1) 
					{
						kxt++;
						h21 = kxt->high;	
						l21 = kxt->low;
					}			
				} while(l21 == l && k<kxnum-1);
				
				// ���ж� ���۶��� �Լ� ���<�׷ֵ�ǰ2��K�ߵĵ͵�
				tjd1 = l<l11 && l<l21 && l<l13 && l<l12;
			} // �Ƕ�
			
			
			// ������׷���
			if (tjg1 || tjd1)
			{
				if (0 == gdnum)
				{
					// ��һ������
					if (tjg1)
					{
						kx->flag = DIR_UP;
						kxlg = kx;
					}
					else if (tjd1)
					{
						kx->flag = DIR_DN;
						kxld = kx;
					}
					kxl = kx;
					gdnum++;
					jg = 1;
					jg2 = 1;
				}
				else
				{
					if (tjg1)
					{
						// ����߽�
						kxt = kx;
						kxt--;
						p31 = kxt->low;
						kx->fxqj = p31;
						// �������ȱ�� ȱ�ڱ߽��δ��K�ߵĶ��� (����ȱ��)
						
						if (i>1)
						{
							j = i - 1;
							//quekou = pData->m_pData[i].m_fLow - pData->m_pData[j].m_fHigh;
							quekou = kx->rlow - kxt->rhigh;
							if(quekou >= biQuekou)
							{
								kx->fxqj = kx->low;
							}
						}
						

						// ���Ӷ� �۸�Ϊ�¶�
						if(DIR_UP == kxl->flag)
						{
							if(kx->high > kxl->high)
							{
								kx->flag = DIR_UP;
								gdnum++;
								jg = 1;
								//jg2 = 1;
								
								kxl->flag = DIR_SBH;
								kxl = kx;		
								kxlg = kx;
							}
							else
							{
								//kx->flag = DIR_SBH;
							}
						}
						else
						{
							// �׽Ӷ� 
							// �������K������3�� ���ɶ��� ���ֵ�������ǰ�ͷָߣ����ٶ��ķ��������ڣ�
							if(jg >= tj_jg && kx->high > kxl->fxqj && kx->fxqj > kxl->low)
							{
								kx->flag = DIR_UP;
								gdnum++;
								jg = 1;
								jg2 = 1;
								
								kxl = kx;
								kxlg = kx;
							}
							else if(jg == 2)
							{
								
								// 1 ����ȱ��(����) 2 ����ǰ��
								
								if(i>1 && quekou  >= biQuekou)
								{
									// ����ȱ�� ���ֳ��� ǰ�׷�Ҳ��Ч 
									kx->flag = DIR_UP;
									gdnum++;
									jg = 1;
									jg2 = 1;
									
									kxl = kx;
									kxlg = kx;
								}
								else if(gdnum>=4 && quekou  < biQuekou && kx->high > kxlg->high)
								{
									// ����ǰ���� ǰ��ʧЧ ǰ��ʧЧ
									kx->flag = DIR_UP;
									kxlg->flag = DIR_SBH;
									kxld->flag = DIR_0;

									gdnum++;
									jg = 1;
									jg2 = 1;
									
									kxl = kx;
									kxlg = kx;
									
								}
							}
						}
					}
					else if (tjd1)
					{
						// ����߽�
						kxt = kx;
						kxt--;
						p31 = kxt->high;
						kx->fxqj = p31;
						// �������ȱ�� ȱ�ڱ߽��δ��K�ߵĶ��� (�µ�ȱ��)
						if(i>1)
						{
							j = i - 1;
							//quekou = -(pData->m_pData[i].m_fHigh - pData->m_pData[j].m_fLow);
							quekou = -(kx->rhigh - kxt->rlow);
							if(quekou>=biQuekou)
							{
								kx->fxqj = kx->high;
							}
						}


						// �׽ӵ� �۵�Ϊ�µ�
						if(DIR_DN == kxl->flag)
						{
							if(kx->low < kxl->low)
							{
								kx->flag = DIR_DN;
								gdnum++;
								jg = 1;
								jg2 = 1;
								
								kxl->flag = DIR_XBH;
								kxl = kx;
								kxld = kx;
							}
							else
							{
								//kx->flag = DIR_XBH;
							}
						}
						else
						{
							// ���ӵ� �������K������3 ���ɵ׷� ���ֵ����С��ǰ���� ���ٶ�������
							if(jg >= tj_jg && kx->low < kxl->fxqj && kx->fxqj < kxl->high)
							{
								kx->flag = DIR_DN;
								gdnum++;
								jg = 1;
								jg2 = 1;
								
								kxl = kx;
								kxld = kx;
							}
							else if(jg == 2)
							{
								// 1 ����ȱ��(�µ�) 2 С��ǰ��
								if(i>1 && quekou  >= biQuekou)
								{
									// ����ȱ�� ���ֳ��� ǰ�׷�Ҳ��Ч 
									kx->flag = DIR_DN;
									gdnum++;
									jg = 1;
									jg2 = 1;
									
									kxl = kx;
									kxld = kx;
								}
								
								else if(gdnum>=4 && quekou  < biQuekou  && kx->low < kxld->low)
								{
									// ����ǰ���� ǰ��ʧЧ ǰ��ʧЧ
									kx->flag = DIR_DN;
									kxld->flag = DIR_XBH;
									kxlg->flag = DIR_0;
									
									gdnum++;
									jg = 1;
									jg2 = 1;
									
									kxl = kx;
									kxld = kx;
								}
							}
						}
					}					
				}
			} // end ����
			else 
			{
				// do nothing
			} // end else  �Ƕ���


		} // end for ������׷���

		
		// ���� 
		kxt = kxData.end();
		kxt--; // ���һ��K��
		// �Ƕ� �ǵ� �����жϵ��� ����δ��֮ǰ
		// ���¸� �� ��Ȼ�����µĸߵ�͵͵� ȥ��ǰһ���ߵ͵� ע������Ǹ߸� �͵�
		if (DIR_UP == kxl->flag)
		{
			if(kxt->high > kxl->high)
			{
				kxl->flag = DIR_SBH;
			}
		}
		else if(DIR_DN == kxl->flag)
		{
			if (kxt->low < kxl->low)
			{
				kxl->flag = DIR_XBH;
			}
		}	
}

CKXIT ChanlunCore::getCKX(int num)
{
	int i = 0;
	CKXIT it;
	for(it = kxData.begin(); it!=kxData.end(); it++)
	{
		if (num == i)
		{
			return  it;	
		}
		i++;
	}
	
	return it;
}

list<ckx> ChanlunCore::getCkxData()
{
	return kxData;
}

list<cbi> ChanlunCore::getXbData()
{
	return xbData;
}

list<cbi> ChanlunCore::getSbData()
{
	return sbData;
}

list<cduan> ChanlunCore::getDuanData()
{
	return dData;
}

list<czhongshu> ChanlunCore::getZsData()
{
	return zsData;
}

void ChanlunCore::initBi()
{
	int kxnum = kxData.size();

	if (kxnum>5)
	{ 
		int i = 0, binum = 0, bignum = 0, bidnum = 0, jg = 1, jg2 = 2;


		int tj_jg = 5, tj_jg2 = 4;

		CKXIT fx, fxl, fxt, fxlg, fxld;

		float quekou = 0;

		fx = kxData.begin();
		fx = getCKX(2);

		// �����
		for(i=2; i<kxnum-2; i++, fx++)
		{			
			jg++;
			if(DIR_UP == fx->dir || DIR_DN == fx->dir) jg2++; // ��������


			if (binum > 0)
			{
				if (DIR_UP == fx->flag)
				{
					// �׽Ӷ�
					if (DIR_DN == fxl->bi)
					{
						// �������ȱ�� ����һ��K�� jg ��JG2 ��+1 (����)
						/*
						quekou = (pData->m_pData[i].m_fLow - pData->m_pData[i-1].m_fHigh);
						if(quekou>=biQuekou)
						{
							jg2++; 
							jg = jg+2;
						}*/

						if(jg >= tj_jg && jg2 >= tj_jg2)
						{
							fx->bi = DIR_UP;
							
							binum++;
							jg = 1;
							jg2 = 1;
							fxl = fx;
							bignum++;
							fxlg = fx;
						}
						else
						{
							// �������� ȡ���еĸߵ���еĵ͵�
							if(bignum>0)
							{
								if(fx->high > fxlg->high)
								{
									fx->bi = DIR_UP;
									fxlg->bi = DIR_SBH;
									fxld->bi = DIR_XBH; // add
									
									binum++;
									jg = 1;
									jg2 = 1;
									fxl = fx;
									bignum++;
									fxlg = fx;
								}
							}
						}
					}
					else if (DIR_UP == fxl->bi)
					{
						// ���Ӷ� 
						// ȡ�߶�
						if (fx->high > fxl->high)
						{
							fx->bi = DIR_UP;
							fxl->bi = DIR_SBH;
							binum++;
							jg = 1;
							jg2 = 1;
							fxl = fx;
							bignum++;
							fxlg = fx;
						}
					}
				} // end if (DIR_UP == fx->flag)
				else if (DIR_DN == fx->flag)
				{
					// ���ӵ�
					if (DIR_UP == fxl->bi)
					{
						/*
						// �������ȱ�� ����һ��K�� jg ��JG2 ��+1 (�µ�)
						quekou = -(pData->m_pData[i].m_fHigh - pData->m_pData[i-1].m_fLow);
						if(quekou>=biQuekou)
						{
							jg2++; 
							jg = jg+2;
						}*/

						if(jg >= tj_jg && jg2 >= tj_jg2)
						{
							fx->bi = DIR_DN;
							
							binum++;
							jg = 1;
							jg2 = 1;
							fxl = fx;
							bidnum++;
							fxld = fx;
						}
						else
						{
							// �������� ȡ���еĵ͵�
							if(bidnum>0)
							{
								if(fx->low < fxld->low)
								{
									fx->bi = DIR_DN;
									fxld->bi = DIR_XBH;
									fxlg->bi = DIR_SBH; // add
									
									binum++;
									jg = 1;
									jg2 = 1;
									fxl = fx;
									bidnum++;
									fxld = fx;
								}
							}
						}
					}
					else if (DIR_DN == fxl->bi)
					{
						// �׽ӵ� 
						// ȡ�͵�
						if (fx->low < fxl->low)
						{
							fx->bi = DIR_DN;
							fxl->bi = DIR_XBH;
							binum++;
							jg = 1;
							jg2 = 1;
							fxl = fx;
							bidnum++;
							fxld = fx;
						}
					}
				} // end else if (DIR_DN == fx->flag)
			} // end binum>0
			else 
			{
				// ��һ��
				if (DIR_UP == fx->flag)
				{
					fx->bi = DIR_UP;
					fxl = fx;
					binum++;
					jg = 1;
					jg2 = 1;
					bignum++;
					fxlg = fx;
				}
				else if (DIR_DN == fx->flag)
				{
					fx->bi = DIR_DN;
					fxl = fx;
					binum++;
					jg = 1;
					jg2 = 1;
					bidnum++;
					fxld = fx;
				}
			}
		} // end �����


		// ���� 
		fxt = kxData.end();
		fxt--; // ���һ��K��
		// �Ƕ� �ǵ� �����жϵ��� ����δ��֮ǰ
		// ���¸� �� ��Ȼ�����µĸߵ�͵͵� ȥ��ǰһ���ߵ͵� ע������Ǹ߸� �͵�
		if (DIR_UP == fxl->bi)
		{
			if(fxt->high > fxl->high)
			{
				fxl->bi = DIR_SBH;
			}
		}
		else if(DIR_DN == fxl->bi)
		{
			if (fxt->low < fxl->low)
			{
				fxl->bi = DIR_XBH;
			}
		}

	}
}

void ChanlunCore::initTZXL()
{
	if(!xbData.empty()) xbData.clear();
	if(!sbData.empty()) sbData.clear();
	
	CKXIT kx, kxl;
	
	int kxnum = kxData.size();
	int i = 0, begin = 2, binum = 0, sbnum = 0, xbnum = 0;
	
	kx = getCKX(begin);
	
	// ����������������
	for (i = begin; i < kxnum - 1; i++, kx++)
	{
		if(DIR_UP == kx->bi)
		{
			if(binum > 0) 
			{
				if(DIR_DN == kxl->bi)
				{
					// �׽Ӷ�
					binum ++;
					sbnum ++;
					cbi tz;
					tz.dir = DIR_UP; // ���ϱ�
					tz.high = kx->high;
					tz.noh = kx->no;
					tz.low = kxl->low;
					tz.no = binum;
					tz.nol = kxl->no;
					tz.flag = DIR_0;
					tz.qk = QK_N;
					// ���ϱ� 
					sbData.push_back(tz);
					//	bList.push_back(tz);
					
					kxl = kx;
				}
				
			}
			else
			{
				// ��һ��
				kxl = kx;
				binum++;
			}
		}
		else if(DIR_DN == kx->bi)
		{
			if(binum > 0) 
			{
				// ���ӵ�
				if(DIR_UP == kxl->bi)
				{
					binum ++;
					xbnum ++;
					cbi tz;
					tz.dir = DIR_DN; // ���±�
					tz.high = kxl->high;
					tz.noh = kxl->no;
					tz.low = kx->low;
					tz.nol = kx->no;
					tz.no = binum;
					tz.flag = DIR_0;
					tz.qk = QK_N;
					// ���±� 
					xbData.push_back(tz);
					//	bList.push_back(tz);
					
					kxl = kx;
				}
				
			}
			else
			{
				// ��һ��
				kxl = kx;
				binum++;
			}
		}
	} // END ����������������
}

BIIT ChanlunCore::findTZG(int fromNo)
{
		BIIT ret = xbData.end();

		int kxnum = kxData.size();
		if(fromNo >= kxnum) return ret;
		

		int bnum = xbData.size();
		if(bnum >= 3)
		{
			BIIT bi, bit, btz2 = xbData.end();
			cbi tz1, tz2, tz3; // ����Ԫ��1 2

			bool doTZ1 = true, doTZ2 = false, doTZ3 = false;
			int i=0;

			for(bi=xbData.begin(); bi!=xbData.end(); bi++)
			{
				if(bi->noh < fromNo)
				{
					continue;
				}
				
				if(doTZ1)
				{
					// ����1
					tz1 = (*bi);
					doTZ1 = false;
					doTZ2 = true;
					continue;
					/*
					if(0 == i)
					{
						tz1 = (*bi);
						doTZ1 = false;
						doTZ2 = true;
						continue;
					}
					else
					{
						bit = bi;
						bit--;
						tz1 = (*bit);
						doTZ1 = false;
						doTZ2 = true;
					}
					*/
					// ok ��ʼTZ2
				}
				else if(doTZ2)
				{
					// ����2
					if(bi->high > tz1.high && bi->low > tz1.low)
					{
						// ����
						btz2 = bi;
						tz2 = (*bi);
						doTZ2 = false;
						doTZ3 = true;
						continue;
						// ok ��ʼTZ3

					}
					else if (bi->high < tz1.high && bi->low < tz1.low)
					{
						// �µ�
						tz1 = (*bi);
						// ����TZ2
					}
					else
					{
						//TZ1 TZ2 ������ͬһ����������İ�����ϵ
						if(tz1.high > bi->high)
						{
							// ǰ����
							tz1.low = bi->low;
							tz1.nol = bi->nol;
						}
						else
						{
							// ���ǰ (���ƻ�������)
							btz2 = bi;
							tz2 = (*bi);
							doTZ2 = false;
							doTZ3 = true;
							continue;

						}					
						/*
						// ���ڰ�����ϵ 2�����
						// tz1 ȡ�ߵ�ĸߵ� �͵�ĸߵ�
						if(tz1.high > bi->high)
						{
							// ǰ����
							tz1.low = bi->low;
							tz1.nol = bi->nol;
						}
						else
						{
							// ���ǰ
							tz2 = (*bi);
							tz2.low = tz1.low;
							tz1 = tz2;
						}
						// ����TZ2*/
					}

				}
				else if(doTZ3)
				{
					// ����3
					if (bi->high < tz2.high && bi->low < tz2.low)
					{
						// �µ� ���ֳ���
						//��� tz2
						//btz2->flag = DIR_UP;
						// �������ȱ��
						if(tz2.low > tz1.high)
						{
							btz2->qk = QK_Y;
						}
						
						return btz2;
						//break; // �ҵ��˽���
						// OK
					}
					else if(bi->high > tz2.high && bi->low > tz2.low)
					{
						// ����
						tz1 = tz2;
						tz2 = (*bi);
						// ����TZ3
						
					}
					else
					{
						// ����3 �� ����2 ���ڰ�����ϵ
						// tz1 ���� tz2 ��������
						if(tz2.high > bi->high)
						{
							// ǰ����
							tz2.low = bi->low;
							tz2.nol = bi->nol;

						}
						else
						{
							// ���ǰ
							btz2 = bi;
							tz3 = (*bi);
							tz3.low = tz2.low;
							tz2 = tz3;
						}
						// ����TZ3
					}

				}
			} // end for 
		} // end (bnum >= 3)
		
	return ret;
}

BIIT ChanlunCore::findTZD(int fromNo)
{
	BIIT ret = sbData.end();

	int kxnum = kxData.size();
	if(fromNo >= kxnum) return ret;
	
	int bnum = sbData.size();
	if(bnum >= 3)
	{
		BIIT bi, btz2=sbData.end(), bit;
		cbi tz1, tz2, tz3; // ����Ԫ��1 2

		bool doTZ1 = true, doTZ2 = false, doTZ3 = false;
		int i=0;

		for(bi=sbData.begin(); bi!=sbData.end(); bi++, i++)
		{
			if(bi->nol < fromNo)
			{
				// do nothing
				continue;
			}
			
			if(doTZ1)
			{
				// ����1
				tz1 = (*bi);
				doTZ1 = false;
				doTZ2 = true;
				continue;
				
				/*
				if(0 == i)
				{
					
					tz1 = (*bi);
					doTZ1 = false;
					doTZ2 = true;
					continue;
					
				}
				else
				{
					bit = bi;
					bit--;
					tz1 = (*bit);
					doTZ1 = false;
					doTZ2 = true;
				}*/

				// ok ��ʼTZ2
			}
			else if(doTZ2)
			{
				// ����2
				if (bi->high < tz1.high && bi->low < tz1.low)
				{
					// �µ�
					btz2 = bi;
					tz2 = (*bi);
					doTZ2 = false;
					doTZ3 = true;
					continue;
					// ok ��ʼTZ3
				}
				else if(bi->high > tz1.high && bi->low > tz1.low)
				{
					// ����
					tz1 = (*bi);
					// ����TZ2
				}
				else
				{
					//TZ1 TZ2 ������ͬһ����������İ�����ϵ
					if( tz1.low < bi->low)
					{
						// ǰ����
						tz1.high = bi->high;
						tz1.noh = bi->noh;
					}
					else
					{
						// ���ǰ (���ƻ�������)
						btz2 = bi;
						tz2 = (*bi);
						doTZ2 = false;
						doTZ3 = true;
						continue;
					}


					/*
					// tz2 tz1 ���ڰ�����ϵ 2�����
					// tz1 ȡ�ߵ�ĵ͵� �͵�ĵ͵�
					if( tz1.low < bi->low)
					{
						// ǰ����
						tz1.high = bi->high;
						tz1.noh = bi->noh;
					}
					else
					{
						// ���ǰ
						tz2 = (*bi);
						tz2.high = tz1.high;
						tz1 = tz2;
					}
					// ����TZ2
					*/
				}

			}
			else if(doTZ3)
			{
				// ����3
				if(bi->high > tz2.high && bi->low > tz2.low)
				{
					// ���� �׷ֳ���
					//��� tz2
				//	btz2->flag = DIR_DN;
					// �������ȱ��
					if(tz2.high > tz1.low)
					{
						btz2->qk = QK_Y;
					}
					
					return btz2;
					//break; // �ҵ��˽���
					// OK
					
				}
				else if (bi->high < tz2.high && bi->low < tz2.low)
				{
					// �µ�
					tz1 = tz2;
					tz2 = (*bi);
					// ����TZ3
				}
				else
				{
					// ����3 �� ����2 ���ڰ�����ϵ
					// tz1 ���� tz2 ��������
					if(tz2.low < bi->low)
					{
						// ǰ����
						tz2.high = bi->high;
						tz2.noh = bi->noh;
					}
					else
					{
						// ���ǰ
						btz2 = bi;

						tz3 = (*bi);
						tz3.high = tz2.high;
						tz2 = tz3;
					}
					// ����TZ3
				}

			}
		}
	} // end (bnum >= 3)

	return ret;
}

void ChanlunCore::initDuan()
{
	int kxnum = kxData.size();
	
	if (kxnum>21)
	{
		initTZXL();	
		
		int dnum = 0, gnum = 0, next = 0, num = 0;
		BIIT tzd, tzg, tzl;
		
		// ����ʣ��Ķ���֪������
		bool isOver = false;

		do {
			dnum = kxnum;
			gnum = kxnum;

			tzd = findTZD(next);
			tzg = findTZG(next);

			if(sbData.end() != tzd)
			{
				dnum = tzd->nol;
			}

			if(xbData.end() != tzg)
			{
				gnum = tzg->noh;
			}

			if(dnum < gnum)
			{

				
				// �� tzd
				if(num > 0)
				{
					if(DIR_UP == tzl->flag)
					{
						// ���ӵ�
						tzd->flag = DIR_DN;
						tzl = tzd;
						num++;
					}
					else if(DIR_DN == tzl->flag)
					{
						// �׽ӵ� �����͵ĵ�
						if(tzd->low < tzl->low)
						{
							tzd->flag = DIR_DN;
							tzl->flag = DIR_XBH;
							tzl = tzd;
							num++;
						}
					}
				}
				else
				{
					// ��һ����OR��
					tzd->flag = DIR_DN;
					tzl = tzd;
					num++;
				}

				next = dnum;
			} // end if dnum < gnums
			else if (dnum > gnum)
			{
				// �� tzg
				if(num > 0)
				{
					if(DIR_DN == tzl->flag)
					{
						// �׽Ӷ�
						tzg->flag = DIR_UP;
						tzl = tzg;
						num++;
					}
					else if(DIR_UP == tzl->flag)
					{
						// ���Ӷ� ȡ�ߵĶ�
						if(tzg->high > tzl->high)
						{
							tzg->flag = DIR_UP;
							tzl->flag = DIR_SBH;
							tzl = tzg;
							num++;
						}
					}
				}
				else
				{
					// ��һ����OR��
					tzg->flag = DIR_UP;
					tzl = tzg;
					num++;
				}

				next = gnum;

			} // end if (dnum > gnum + 1)
			else
			{
				//����
				// do nothing
				isOver = true;
			}
			
		} while(!isOver);
		
		CKXIT kx, kxl;
		
		// ��Ƕ�
		// ��
		for (tzd = sbData.begin(); tzd != sbData.end(); tzd++)
		{
			if(DIR_DN == tzd->flag)
			{
				num = tzd->nol - 1;
				kx = getCKX(num);
				kx->duan = DIR_DN;
			}
		}
		// ��
		for (tzg = xbData.begin(); tzg != xbData.end(); tzg++)
		{
			if(DIR_UP == tzg->flag)
			{
				num = tzg->noh - 1;
				kx = getCKX(num);
				kx->duan = DIR_UP;
			}
		}

	} // end kxnum>21 
}

void ChanlunCore::initDuanList()
{
	if (!dData.empty()) {
		dData.clear();
	}
	
	CKXIT kx, kxl;
	
	int kxnum = kxData.size();
	int i = 0, begin = 2, num = 0;
	
	kx = getCKX(begin);
	
	// �������ж�
	for (i = begin; i < kxnum - 1; i++, kx++)
	{
		if(DIR_UP == kx->duan)
		{
			if(num > 0) 
			{
				if(DIR_DN == kxl->duan)
				{
					// �׽Ӷ�
					num ++;
					cduan d;
					d.flag = DIR_UP;
					d.no = num;
					d.noh = kx->no;
					d.high = kx->high;
					d.nol = kxl->no;
					d.low = kxl->low;
					dData.push_back(d);
					
					kxl = kx;
				}
			}
			else
			{
				// ��һ��
				num++;
				kxl = kx;				
			}
		}
		else if(DIR_DN == kx->duan)
		{
			if(num > 0) 
			{
				// ���ӵ�
				if(DIR_UP == kxl->duan)
				{
					num ++;
					cduan d;
					d.flag = DIR_DN;
					d.no = num;
					d.noh = kxl->no;
					d.high = kxl->high;
					d.nol = kx->no;
					d.low = kx->low;
					dData.push_back(d);
					
					kxl = kx;
				}
			}
			else
			{
				// ��һ��
				num++;
				kxl = kx;
			}
		}
	} // END �������ж�
}

void ChanlunCore::findHuiTiaoZS(int duanno, int begin, int end, int high, int low)
{
	if(xbData.size() >= 2)
	{
		BIIT zn, znl = xbData.end();
		ZSIT zsit;
		bool findZSNew = true;
		int gg=0, dd=0, num=0;
		for(zn = xbData.begin(); zn != xbData.end(); zn++)
		{
			if (zn->noh > begin && zn->noh < end)
			{	
				if(num > 0)
				{
					if(zn->low < znl->high)
					{ 
						if(findZSNew)
						{
							gg = znl->high > zn->high ? znl->high : zn->high;
							dd = znl->low < zn->low ? znl->low : zn->low;
							if(high > gg && low < dd)
							{
								// ZN�ص� ������
								czhongshu zs;
								zs.flag = DIR_UP;
								zs.duanno = duanno;
								zs.znnum = 2;
								zs.zg = znl->high < zn->high ? znl->high : zn->high;	
								zs.zd = znl->low > zn->low ? znl->low : zn->low;
								zs.ksno = znl->noh;
								zs.jsno = zn->nol;
								zs.gg = gg;
								zs.dd = dd;
								zs.zz = zs.zd + (zs.zg-zs.zd)/2;

								// �������ȫ���ڶ���
								zsData.push_back(zs);
								findZSNew = false;
							}
							else
							{
								findZSNew = true;
							}

						}
						else
						{
							zsit = zsData.end();
							zsit--;
							
							if(zn->low > zsit->zg || zn->high < zsit->zd)
							{
								// �뿪����
								findZSNew = true;
							}
							else
							{
								// ��������
								zsit->jsno = zn->nol;
								zsit->znnum ++;
								if(zn->high > zsit->gg)
								{
									zsit->gg = zn->high;
								}
								if(zn->low < zsit->dd)
								{
									zsit->dd = zn->low;
								}
							}
						}
					}
					else
					{
						// �뿪����
						findZSNew = true;
					}
				}// end if(num > 0)
				else
				{
					// �����һ���ƻ������϶β�����
					/*
					if(zn->high > low)
					{
						break;
					}*/
				}
				
				num++;
				znl = zn;
			}			
			else if (zn->noh >= end)
			{
				break;
			}
		} // end for
	}
}

void ChanlunCore::findFanTanZS(int duanno, int begin, int end, int high, int low)
{
	if(sbData.size() >= 2)
	{
		BIIT zn, znl = sbData.end();
		ZSIT zsit;
		bool findZSNew = true;
		int gg=0, dd=0, num = 0;
		for(zn = sbData.begin(); zn != sbData.end(); zn++)
		{
			if (zn->nol > begin && zn->nol < end)
			{	
				if(num>0)
				{
					if(zn->high > znl->low)
					{ 
						if(findZSNew)
						{
							gg = znl->high > zn->high ? znl->high : zn->high;
							dd = znl->low < zn->low ? znl->low : zn->low;
							if(high > gg && low < dd)
							{
								// ZN�ص� ������
								czhongshu zs;
								zs.flag = DIR_DN;
								zs.duanno = duanno;
								zs.znnum = 2;
								zs.zg = znl->high < zn->high ? znl->high : zn->high;	
								zs.zd = znl->low > zn->low ? znl->low : zn->low;
								zs.ksno = znl->nol;
								zs.jsno = zn->noh;
								zs.gg = gg;
								zs.dd = dd;
								zs.zz = zs.zd + (zs.zg-zs.zd)/2;

								// �������ȫ���ڶ���
								zsData.push_back(zs);
								findZSNew = false;
							}
							else
							{
								findZSNew = true;
							}						
						}
						else
						{
							zsit = zsData.end();
							zsit--;
							
							if(zn->low > zsit->zg || zn->high < zsit->zd)
							{
								// �뿪����
								findZSNew = true;
							}
							else
							{
								// ��������
								findZSNew = false;
								zsit->jsno = zn->noh;
								zsit->znnum ++;
								if(zn->high > zsit->gg)
								{
									zsit->gg = zn->high;
								}
								if(zn->low < zsit->dd)
								{
									zsit->dd = zn->low;
								}
							}
						}
					}
					else
					{
						// �뿪����
						findZSNew = true;
					}
				}
				else
				{
					// �����һ���ƻ������¶β�����
					/*
					if(zn->low < low)
					{
						break;
					}*/
				}

				num++;
				znl = zn;
			} 		
			else if (zn->noh >= end)
			{
				break;
			}
		} // end for
	}	
}

void ChanlunCore::initZhongshu()
{
	if(!zsData.empty()) zsData.clear();
	
	initDuanList();
	
	if(dData.size() > 0)
	{
		DUANIT dit;
		int i = 0;
		for(dit = dData.begin(); dit != dData.end(); dit++)
		{
			i++;
			if(DIR_UP == dit->flag)
			{
				// ���϶�
				findHuiTiaoZS(i, dit->nol, dit->noh, dit->high, dit->low);
				
			}
			else if(DIR_DN == dit->flag)
			{
				// ���¶�
				findFanTanZS(i, dit->noh, dit->nol, dit->high, dit->low);
			}
			
		}
	} // if(dData.size() > 0)
}