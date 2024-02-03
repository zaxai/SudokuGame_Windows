/*
** Version  1.0.0.1
** Date     2023.11.24
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com All Rights Reserved
*/
#pragma once
#include <vector>
#include <thread>
#include <future>

#define ZSUDOKU_SIZE 9
#define ZSUDOKU_TOTAL_SIZE (ZSUDOKU_SIZE*ZSUDOKU_SIZE)

class ZSudoku
{
private:
	int m_sz2_nData[ZSUDOKU_SIZE][ZSUDOKU_SIZE];
	int m_sz2_nDataFreeze[ZSUDOKU_SIZE][ZSUDOKU_SIZE];
	int m_nTotal;
	int m_nTotalFreeze;
	std::vector<int> m_sz2_vec_nData[ZSUDOKU_SIZE][ZSUDOKU_SIZE];
	std::thread m_threadAutoCalc;
	bool m_bIsAutoCalcRun;
public:
	enum DIRECTION
	{
		DIR_ROW = 0,
		DIR_COLUMN,
	};
	enum BLOCKINDEX
	{
		BLOCK_1 = 0,
		BLOCK_2,
		BLOCK_3,
		BLOCK_4,
		BLOCK_5,
		BLOCK_6,
		BLOCK_7,
		BLOCK_8,
		BLOCK_9
	};
	class AutoCalcCallback
	{
	public:
		virtual void OnAutoCalcCountUpdate(int nCount) = 0;
		virtual void OnAutoCalcResult(bool bResult) = 0;
	};
private:
	int GetNumCount(int nNum, DIRECTION emDir, int nIndex);
	int GetNumCount(int nNum, BLOCKINDEX emBlock);
	int GetFirstNumReverseIndex(int nNum, DIRECTION emDir, int nIndex);
	int ThreeLinesModule(DIRECTION emDir, int nStartIndex);
	int CandidateModule(int nX, int nY);
	int ThreeLinesAlgorithm();
	int CandidateAlgorithm();
	int BasicAlgorithm();
	void DataFreeze();
	void DataUnfreeze();
	void DataConvert();
	bool CandidateSel(int nCandidateSize);
	bool RandomCalc();
	void AutoCalcThread(std::promise<void> prom, AutoCalcCallback* p_callback);
public:
	ZSudoku();
	virtual ~ZSudoku();
	void SetData(const int * p_nData);
	void GetData(int* p_nData);
	void GetCandidateNum(int nX, int nY, std::vector<int>& vec_nNum);
	bool RuleCheck();
	void BasicCalc();
	void StartAutoCalc(AutoCalcCallback* p_callback);
	void StopAutoCalc();
};
