#include "pch.h"
#include "ZSudoku.h"


ZSudoku::ZSudoku()
	: m_sz2_nData()
	, m_sz2_nDataFreeze()
	, m_nTotal(0)
	, m_nTotalFreeze(0)
	, m_bIsAutoCalcRun(false)
{
}


ZSudoku::~ZSudoku()
{
}


int ZSudoku::GetNumCount(int nNum, DIRECTION emDir, int nIndex)
{
	int nCount = 0;
	if (nIndex < ZSUDOKU_SIZE)
	{
		for (int i = 0; i < ZSUDOKU_SIZE; ++i)
		{
			switch (emDir)
			{
			case DIR_ROW:
			{
				if (m_sz2_nData[nIndex][i] == nNum)
					nCount++;
			}
			break;
			case DIR_COLUMN:
			{
				if (m_sz2_nData[i][nIndex] == nNum)
					nCount++;
			}
			break;
			default:break;
			}
		}
	}
	return nCount;
}


int ZSudoku::GetNumCount(int nNum, BLOCK_INDEX emBlock)
{
	int nCount = 0;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (m_sz2_nData[(emBlock / 3) * 3 + i][(emBlock % 3) * 3 + j] == nNum)
				nCount++;
		}
	}
	return nCount;
}


int ZSudoku::GetFirstNumReverseIndex(int nNum, DIRECTION emDir, int nIndex)
{
	for (int i = 0; i < ZSUDOKU_SIZE; ++i)
	{
		switch (emDir)
		{
		case DIR_ROW:
		{
			if (m_sz2_nData[nIndex][i] == nNum)
				return i;
		}
		break;
		case DIR_COLUMN:
		{
			if (m_sz2_nData[i][nIndex] == nNum)
				return i;
		}
		break;
		default:break;
		}
	}
	return -1;
}


void ZSudoku::GetCandidateNum(int nX, int nY, std::vector<int>& vec_nNum)
{
	vec_nNum.clear();
	BLOCK_INDEX emBlock = BLOCK_INDEX(nX / 3 * 3 + nY / 3);
	for (int nNum = 1; nNum <= 9; ++nNum)
	{
		if (GetNumCount(nNum, DIR_ROW, nX) == 0 && GetNumCount(nNum, DIR_COLUMN, nY) == 0 && GetNumCount(nNum, emBlock) == 0)
			vec_nNum.push_back(nNum);
	}
}


int ZSudoku::ThreeLinesModule(DIRECTION emDir, int nStartIndex)
{
	int nCount = 0;
	for (int nNum = 1; nNum <= 9; ++nNum)
	{
		std::vector<int> vec_nIndexExist, vec_nIndexNone, vec_nBlock;
		for (int i = 0; i < 3; ++i)
		{
			if (GetNumCount(nNum, emDir, nStartIndex + i) == 1)
			{
				vec_nIndexExist.push_back(nStartIndex + i);
				vec_nBlock.push_back(GetFirstNumReverseIndex(nNum, emDir, nStartIndex + i) / 3);
			}
			else
				vec_nIndexNone.push_back(nStartIndex + i);
		}
		if (vec_nIndexExist.size() == 2 && vec_nIndexNone.size() == 1)
		{
			CPoint pt;
			std::vector<CPoint> vec_pt;
			for (int nBlock = 0; nBlock < 3; ++nBlock)
			{
				if (std::find(vec_nBlock.begin(), vec_nBlock.end(), nBlock) == vec_nBlock.end())
				{
					for (int i = 0; i < 3; ++i)
					{
						switch (emDir)
						{
						case DIR_ROW:
						{
							pt.x = vec_nIndexNone[0];
							pt.y = nBlock * 3 + i;
							if (m_sz2_nData[pt.x][pt.y] == 0 && GetNumCount(nNum, DIR_COLUMN, pt.y) == 0)
								vec_pt.push_back(pt);
						}
						break;
						case DIR_COLUMN:
						{
							pt.x = nBlock * 3 + i;
							pt.y = vec_nIndexNone[0];
							if (m_sz2_nData[pt.x][pt.y] == 0 && GetNumCount(nNum, DIR_ROW, pt.x) == 0)
								vec_pt.push_back(pt);
						}
						break;
						default:break;
						}
					}
					break;
				}
			}
			if (vec_pt.size() == 1)
			{
				m_sz2_nData[vec_pt[0].x][vec_pt[0].y] = nNum;
				nCount++;
			}
		}
	}
	return nCount;
}


int ZSudoku::CandidateModule(int nX, int nY)
{
	int nCount = 0;
	if (m_sz2_nData[nX][nY] == 0)
	{
		std::vector<int> vec_nNum;
		GetCandidateNum(nX, nY, vec_nNum);
		if (vec_nNum.size() == 1)
		{
			m_sz2_nData[nX][nY] = vec_nNum[0];
			nCount++;
		}
	}
	return nCount;
}


int ZSudoku::ThreeLinesAlgorithm()
{
	int nCount = 0;
	for (int i = 0; i < 3; ++i)
	{
		nCount += ThreeLinesModule(DIR_ROW, i * 3);
		nCount += ThreeLinesModule(DIR_COLUMN, i * 3);
	}
	return nCount;
}


int ZSudoku::CandidateAlgorithm()
{
	int nCount = 0;
	for (int i = 0; i < ZSUDOKU_SIZE; ++i)
	{
		for (int j = 0; j < ZSUDOKU_SIZE; ++j)
			nCount += CandidateModule(i, j);
	}
	return nCount;
}


int ZSudoku::BasicAlgorithm()
{
	return (ThreeLinesAlgorithm() + CandidateAlgorithm());
}


void ZSudoku::DataFreeze()
{
	for (int i = 0; i < ZSUDOKU_SIZE; ++i)
	{
		for (int j = 0; j < ZSUDOKU_SIZE; ++j)
			m_sz2_nDataFreeze[i][j] = m_sz2_nData[i][j];
	}
	m_nTotalFreeze = m_nTotal;
}


void ZSudoku::DataUnfreeze()
{
	for (int i = 0; i < ZSUDOKU_SIZE; ++i)
	{
		for (int j = 0; j < ZSUDOKU_SIZE; ++j)
			m_sz2_nData[i][j] = m_sz2_nDataFreeze[i][j];
	}
	m_nTotal = m_nTotalFreeze;
}


void ZSudoku::DataConvert()
{
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		int nX = i / ZSUDOKU_SIZE;
		int nY = i % ZSUDOKU_SIZE;
		m_sz2_vec_nData[nX][nY].clear();
		if (m_sz2_nData[nX][nY])
			m_sz2_vec_nData[nX][nY].push_back(m_sz2_nData[nX][nY]);
		else
			GetCandidateNum(nX, nY, m_sz2_vec_nData[nX][nY]);
	}
}


bool ZSudoku::CandidateSel(int nCandidateSize)
{
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		int nX = i / ZSUDOKU_SIZE;
		int nY = i % ZSUDOKU_SIZE;
		if (m_sz2_vec_nData[nX][nY].size() == nCandidateSize)
		{
			m_sz2_nData[nX][nY] = m_sz2_vec_nData[nX][nY][rand() % nCandidateSize];
			m_nTotal++;
			return true;
		}
	}
	return false;
}


bool ZSudoku::RandomCalc()
{
	while (m_nTotal < ZSUDOKU_TOTAL_SIZE)
	{
		int nCount = BasicAlgorithm();
		m_nTotal += nCount;
		if (!nCount)
		{
			DataConvert();
			bool bFind = false;
			for (int i = 2; i <= 9; ++i)
			{
				if (CandidateSel(i))
				{
					bFind = true;
					break;
				}
			}
			if (!bFind)
				break;
		}
	}
	return (m_nTotal == ZSUDOKU_TOTAL_SIZE && RuleCheck());
}


void ZSudoku::AutoCalcThread(std::promise<void> prom, AutoCalcCallback* p_callback)
{
	m_bIsAutoCalcRun = true;
	prom.set_value();
	bool bResult = false;
	int nCount = 0;
	DataFreeze();
	srand((unsigned int)time(nullptr));
	do
	{
		if (p_callback)
			p_callback->OnAutoCalcCountUpdate(++nCount);
		if (RandomCalc())
		{
			bResult = true;
			break;
		}
		else
			DataUnfreeze();
	} while (m_bIsAutoCalcRun);
	if (p_callback)
		p_callback->OnAutoCalcResult(bResult);
	m_bIsAutoCalcRun = false;
}


void ZSudoku::SetData(const int* p_nData)
{
	m_nTotal = 0;
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		m_sz2_nData[i / ZSUDOKU_SIZE][i % ZSUDOKU_SIZE] = p_nData[i];
		if (m_sz2_nData[i / ZSUDOKU_SIZE][i % ZSUDOKU_SIZE])
			m_nTotal++;
	}
}


void ZSudoku::GetData(int* p_nData)
{
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
		p_nData[i] = m_sz2_nData[i / ZSUDOKU_SIZE][i % ZSUDOKU_SIZE];
}


bool ZSudoku::RuleCheck()
{
	for (int nNum = 1; nNum <= 9; ++nNum)
	{
		for (int i = 0; i < ZSUDOKU_SIZE; ++i)
		{
			if (GetNumCount(nNum, DIR_ROW, i) > 1)
			{
				m_rei = RuleErrorInfo(RULE_ERR_ROW, i, nNum);
				return false;
			}
			if (GetNumCount(nNum, DIR_COLUMN, i) > 1)
			{
				m_rei = RuleErrorInfo(RULE_ERR_COLUMN, i, nNum);
				return false;
			}
			if (GetNumCount(nNum, (BLOCK_INDEX)i) > 1)
			{
				m_rei = RuleErrorInfo(RULE_ERR_BLOCK, i, nNum);
				return false;
			}
		}
	}
	m_rei = RuleErrorInfo(RULE_ERR_NONE, 0, 0);
	return true;
}


const  ZSudoku::RuleErrorInfo & ZSudoku::GetLastRuleError()
{
	return m_rei;
}


void ZSudoku::BasicCalc()
{
	while (m_nTotal < ZSUDOKU_TOTAL_SIZE)
	{
		int nCount = BasicAlgorithm();
		m_nTotal += nCount;
		if (!nCount)
			break;
	}
}


void ZSudoku::StartAutoCalc(AutoCalcCallback* p_callback)
{
	if (!m_bIsAutoCalcRun)
	{
		if (m_threadAutoCalc.joinable())
			m_threadAutoCalc.join();
		std::promise<void> prom;
		std::future<void> f = prom.get_future();
		m_threadAutoCalc = std::thread(&ZSudoku::AutoCalcThread, this, std::move(prom), p_callback);
		f.wait();
	}
}


void ZSudoku::StopAutoCalc()
{
	m_bIsAutoCalcRun = false;
	if (m_threadAutoCalc.joinable())
		m_threadAutoCalc.join();
}


CString ZSudoku::GetCandidateNum(int nX, int nY)
{
	CString strCandidateNum;
	if (m_sz2_nData[nX][nY] == 0)
	{
		CString strNum;
		std::vector<int> vec_nNum;
		GetCandidateNum(nX, nY, vec_nNum);
		if (!vec_nNum.empty())
		{
			for (int nNum = 1; nNum <= 9; ++nNum)
			{
				if (std::find(vec_nNum.begin(), vec_nNum.end(), nNum) != vec_nNum.end())
				{
					strNum.Format(_T("%d"), nNum);
					strCandidateNum += strNum;
				}
				else
					strCandidateNum += _T(" ");
				switch (nNum)
				{
				case 1:
				case 2:
				case 4:
				case 5:
				case 7:
				case 8:
					strCandidateNum += _T(" ");
					break;
				case 3:
				case 6:
					strCandidateNum += _T("\r\n");
					break;
				default:break;
				}
			}
		}
	}
	return strCandidateNum;
}
