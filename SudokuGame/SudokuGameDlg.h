﻿
// SudokuGameDlg.h: 头文件
//

#pragma once
#include "ZSudoku.h"
#define IDC_EDIT_START 4000


// CSudokuGameDlg 对话框
class CSudokuGameDlg : public CDialogEx,public ZSudoku::AutoCalcCallback
{
// 构造
public:
	CSudokuGameDlg(CWnd* pParent = nullptr);	// 标准构造函数
	virtual ~CSudokuGameDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUDOKUGAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	std::vector<CFont*> m_vec_p_font;
	std::vector<CEdit*> m_vec_p_edit;
	CComboBox m_comboNumber;
	CString m_sz_strDataOri[ZSUDOKU_TOTAL_SIZE];
	ZSudoku m_sudoku;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonBackup();
	afx_msg void OnBnClickedButtonRestore();
	afx_msg void OnBnClickedButtonLock();
	afx_msg void OnBnClickedButtonUnlock();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonCheck();
	afx_msg void OnBnClickedButtonCandidate();
	afx_msg void OnBnClickedButtonBasiccalc();
	afx_msg void OnBnClickedButtonAutocalc();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedCheckHighlight();
	afx_msg void OnCbnSelchangeComboNumber();
	afx_msg void OnEnUpdateEditData(UINT nID);
	afx_msg void OnEnSetfocusEditData(UINT nID);
	afx_msg void OnEnKillfocusEditData(UINT nID);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	virtual void OnAutoCalcCountUpdate(int nCount);
	virtual void OnAutoCalcResult(bool bResult);
	void InitUI();
	void GetDataFromEdit();
	void SetDataToEdit();
	void SetEditFont(UINT nID, const CString& strEdit);
	void EnableButton(BOOL nEnable);
	CString GetRuleErrorInfo();
	void SetTimerInfo(const CString& strInfo, UINT nElapse);
};
