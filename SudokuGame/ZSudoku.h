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
public:
	enum DIRECTION
	{
		DIR_ROW = 0,
		DIR_COLUMN,
	};
	enum BLOCK_INDEX
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
	enum RULE_ERROR_TYPE
	{
		RULE_ERR_NONE=0,
		RULE_ERR_ROW,
		RULE_ERR_COLUMN,
		RULE_ERR_BLOCK
	};
	class RuleErrorInfo
	{
	public:
		RULE_ERROR_TYPE m_type;
		int m_nIndex;
		int m_nNumber;
	public:
		RuleErrorInfo()
			: m_type(RULE_ERR_NONE)
			, m_nIndex(0)
			, m_nNumber(0)
		{

		}
		RuleErrorInfo(RULE_ERROR_TYPE type, int nIndex,int nNumber)
			: m_type(type)
			, m_nIndex(nIndex)
			, m_nNumber(nNumber)
		{

		}
	};
	class AutoCalcCallback
	{
	public:
		virtual void OnAutoCalcCountUpdate(int nCount) = 0;
		virtual void OnAutoCalcResult(bool bResult) = 0;
	};
private:
	int m_sz2_nData[ZSUDOKU_SIZE][ZSUDOKU_SIZE];
	int m_sz2_nDataFreeze[ZSUDOKU_SIZE][ZSUDOKU_SIZE];
	int m_nTotal;
	int m_nTotalFreeze;
	std::vector<int> m_sz2_vec_nData[ZSUDOKU_SIZE][ZSUDOKU_SIZE];
	std::thread m_threadAutoCalc;
	bool m_bIsAutoCalcRun;
	RuleErrorInfo m_rei;
private:
	int GetNumCount(int nNum, DIRECTION emDir, int nIndex);
	int GetNumCount(int nNum, BLOCK_INDEX emBlock);
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
	const RuleErrorInfo & GetLastRuleError();
	void BasicCalc();
	void StartAutoCalc(AutoCalcCallback* p_callback);
	void StopAutoCalc();
};
