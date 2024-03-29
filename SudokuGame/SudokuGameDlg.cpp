﻿
// SudokuGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SudokuGame.h"
#include "SudokuGameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSudokuGameDlg 对话框
const COLORREF crHighLight = RGB(246, 166, 56);
const int nTimerInfoClear = 1;



CSudokuGameDlg::CSudokuGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SUDOKUGAME_DIALOG, pParent)
	, m_sz_strDataOri()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSudokuGameDlg::~CSudokuGameDlg()
{
	for (CFont* p_font : m_vec_p_font)
	{
		p_font->DeleteObject();
		delete p_font;
	}
	for (CEdit* p_edit : m_vec_p_edit)
	{
		p_edit->DestroyWindow();
		delete p_edit;
	}
}

void CSudokuGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NUMBER, m_comboNumber);
}

BEGIN_MESSAGE_MAP(CSudokuGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MSGRECVPRO, &CSudokuGameDlg::OnMsgrecvpro)
	ON_BN_CLICKED(IDC_BUTTON_BACKUP, &CSudokuGameDlg::OnBnClickedButtonBackup)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, &CSudokuGameDlg::OnBnClickedButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_LOCK, &CSudokuGameDlg::OnBnClickedButtonLock)
	ON_BN_CLICKED(IDC_BUTTON_UNLOCK, &CSudokuGameDlg::OnBnClickedButtonUnlock)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSudokuGameDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CSudokuGameDlg::OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDC_BUTTON_CANDIDATE, &CSudokuGameDlg::OnBnClickedButtonCandidate)
	ON_BN_CLICKED(IDC_BUTTON_BASICCALC, &CSudokuGameDlg::OnBnClickedButtonBasiccalc)
	ON_BN_CLICKED(IDC_BUTTON_AUTOCALC, &CSudokuGameDlg::OnBnClickedButtonAutocalc)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSudokuGameDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_CHECK_HIGHLIGHT, &CSudokuGameDlg::OnBnClickedCheckHighlight)
	ON_CBN_SELCHANGE(IDC_COMBO_NUMBER, &CSudokuGameDlg::OnCbnSelchangeComboNumber)
	ON_CONTROL_RANGE(EN_UPDATE, IDC_EDIT_START, IDC_EDIT_START + ZSUDOKU_TOTAL_SIZE, &CSudokuGameDlg::OnEnUpdateEditData)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT_START, IDC_EDIT_START + ZSUDOKU_TOTAL_SIZE, &CSudokuGameDlg::OnEnSetfocusEditData)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT_START, IDC_EDIT_START + ZSUDOKU_TOTAL_SIZE, &CSudokuGameDlg::OnEnKillfocusEditData)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSudokuGameDlg 消息处理程序

BOOL CSudokuGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitUI();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSudokuGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSudokuGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSudokuGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT CSudokuGameDlg::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_AUTOCALCCOUNT:
	{
		SetDlgItemInt(IDC_STATIC_COUNT, wParam);
	}
	break;
	case MSGUSER_AUTOCALCRESULT:
	{
		if (wParam)
			SetDataToEdit();
		EnableButton(TRUE);
	}
	break;
	default:break;
	}
	return 0;
}


void CSudokuGameDlg::OnBnClickedButtonBackup()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		CString strEdit;
		GetDlgItemText(IDC_EDIT_START + i, strEdit);
		if (strEdit.GetLength() == 1)
			m_sz_strDataOri[i] = strEdit;
		else
			m_sz_strDataOri[i] = _T("");
	}
	SetTimerInfo(_T("已标记起始"), 3000);
}


void CSudokuGameDlg::OnBnClickedButtonRestore()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		SetDlgItemText(IDC_EDIT_START + i, m_sz_strDataOri[i]);
		SetEditFont(IDC_EDIT_START + i, m_sz_strDataOri[i]);
		if (!m_vec_p_edit[i]->IsWindowEnabled())
			m_vec_p_edit[i]->EnableWindow(TRUE);
	}
	SetTimerInfo(_T("已恢复起始"), 3000);
}


void CSudokuGameDlg::OnBnClickedButtonLock()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		CString strEdit;
		GetDlgItemText(IDC_EDIT_START + i, strEdit);
		if (strEdit.GetLength() == 1&& m_vec_p_edit[i]->IsWindowEnabled())
			m_vec_p_edit[i]->EnableWindow(FALSE);
	}
}


void CSudokuGameDlg::OnBnClickedButtonUnlock()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		if (!m_vec_p_edit[i]->IsWindowEnabled())
			m_vec_p_edit[i]->EnableWindow(TRUE);
	}
}


void CSudokuGameDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		if (m_vec_p_edit[i]->IsWindowEnabled())
		{
			m_vec_p_edit[i]->SetWindowText(_T(""));
			SetEditFont(IDC_EDIT_START + i, _T(""));
		}
	}
	SetTimerInfo(_T("已清空"), 3000);
}


void CSudokuGameDlg::OnBnClickedButtonCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDataFromEdit();
	m_sudoku.RuleCheck();
	SetTimerInfo(GetRuleErrorInfo(), 3000);
}


void CSudokuGameDlg::OnBnClickedButtonCandidate()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDataFromEdit();
	if (!m_sudoku.RuleCheck())
	{
		SetTimerInfo(GetRuleErrorInfo(), 3000);
		return;
	}
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		CString strEdit;
		GetDlgItemText(IDC_EDIT_START + i, strEdit);
		if (strEdit.GetLength() != 1)
		{
			CString strCandidateNum = m_sudoku.GetCandidateNum(i / ZSUDOKU_SIZE, i % ZSUDOKU_SIZE);
			if (strCandidateNum != strEdit)
			{
				SetDlgItemText(IDC_EDIT_START + i, strCandidateNum);
				SetEditFont(IDC_EDIT_START + i, strCandidateNum);
			}
		}
	}
}


void CSudokuGameDlg::OnBnClickedButtonBasiccalc()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDataFromEdit();
	if (!m_sudoku.RuleCheck())
	{
		SetTimerInfo(GetRuleErrorInfo(), 3000);
		return;
	}
	m_sudoku.BasicCalc();
	SetDataToEdit();
}


void CSudokuGameDlg::OnBnClickedButtonAutocalc()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDataFromEdit();
	if (!m_sudoku.RuleCheck())
	{
		SetTimerInfo(GetRuleErrorInfo(), 3000);
		return;
	}
	EnableButton(FALSE);
	m_sudoku.StartAutoCalc(this);
}


void CSudokuGameDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sudoku.StopAutoCalc();
	SetTimerInfo(_T("已停止计算"), 3000);
}


void CSudokuGameDlg::OnBnClickedCheckHighlight()
{
	// TODO: 在此添加控件通知处理程序代码
	for (CEdit* p_edit : m_vec_p_edit)
		p_edit->Invalidate();
}


void CSudokuGameDlg::OnCbnSelchangeComboNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_HIGHLIGHT))->GetCheck())
	{
		for (CEdit* p_edit : m_vec_p_edit)
			p_edit->Invalidate();
	}
}


void CSudokuGameDlg::OnEnUpdateEditData(UINT nID)
{
	CString strEdit;
	GetDlgItemText(nID, strEdit);
	SetEditFont(nID, strEdit);
}


void CSudokuGameDlg::OnEnSetfocusEditData(UINT nID)
{
	CString strEdit;
	GetDlgItemText(nID, strEdit);
	if (strEdit.GetLength() > 1)
	{
		strEdit.Remove(_T(' '));
		strEdit.Remove(_T('\r'));
		strEdit.Remove(_T('\n'));
		SetDlgItemText(nID, strEdit);
		SetEditFont(nID, strEdit);
	}
	m_vec_p_edit[nID - IDC_EDIT_START]->SetSel(0, -1);
}


void CSudokuGameDlg::OnEnKillfocusEditData(UINT nID)
{
	CString strEdit;
	GetDlgItemText(nID, strEdit);
	if (strEdit.GetLength() > 1)
	{
		CString strCandidateNum, strNum;
		for (int nNum = 1; nNum <= 9; ++nNum)
		{
			strNum.Format(_T("%d"), nNum);
			if (strEdit.Find(strNum) != -1)
				strCandidateNum += strNum;
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
		if (strCandidateNum != strEdit)
		{
			SetDlgItemText(nID, strCandidateNum);
			SetEditFont(nID, strCandidateNum);
		}
	}
}


HBRUSH CSudokuGameDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (((CButton*)GetDlgItem(IDC_CHECK_HIGHLIGHT))->GetCheck())
	{
		if (nCtlColor == CTLCOLOR_EDIT)
		{
			CString strEdit, strNum;
			pWnd->GetWindowText(strEdit);
			m_comboNumber.GetWindowText(strNum);
			if (strEdit.Find(strNum) != -1)
			{
				pDC->SetBkColor(crHighLight);
				return CreateSolidBrush(crHighLight);
			}
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSudokuGameDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_sudoku.StopAutoCalc();
}


void CSudokuGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case nTimerInfoClear:
	{
		SetDlgItemText(IDC_STATIC_INFO, _T("就绪"));
		KillTimer(nTimerInfoClear);
	}
	break;
	default:break;
	}
	__super::OnTimer(nIDEvent);
}


void CSudokuGameDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	return;
	CDialogEx::OnOK();
}


void CSudokuGameDlg::OnAutoCalcCountUpdate(int nCount)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)nCount, MSGUSER_AUTOCALCCOUNT);
}


void CSudokuGameDlg::OnAutoCalcResult(bool bResult)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)bResult, MSGUSER_AUTOCALCRESULT);
}


void CSudokuGameDlg::InitUI()
{
	LOGFONT logfont;
	GetFont()->GetLogFont(&logfont);
	logfont.lfHeight = 40;
	CFont* p_font = nullptr;
	CEdit* p_edit = nullptr;
	CRect rc,rcBackup,rcClear;
	GetDlgItem(IDC_BUTTON_BACKUP)->GetWindowRect(&rcBackup);
	ScreenToClient(&rcBackup);
	GetDlgItem(IDC_BUTTON_CLEAR)->GetWindowRect(&rcClear);
	ScreenToClient(&rcClear);
	const int nTotalWidth = rcClear.right - rcBackup.left;
	const int nFixedWidth = 5 * 6 + 15 * 2;
	const int nWidth = (nTotalWidth - nFixedWidth) / 9;
	const int nHeight = nWidth;
	const int nStartLeft = rcBackup.left + (nTotalWidth - nFixedWidth - nWidth * 9) / 2;
	rc.top = rcBackup.top - nTotalWidth - 15 * 2;
	rc.bottom = rc.top;
	for (int i = 0; i < 81; ++i)
	{
		if (i % 9 == 0)
		{
			rc.left = nStartLeft;
			if ((i / 9) % 3 == 0)
				rc.top = rc.bottom + 15;
			else
				rc.top = rc.bottom + 5;
			rc.bottom = rc.top + nHeight;
			rc.right = rc.left + nWidth;
		}
		else
		{
			if (i % 3 == 0)
				rc.left = rc.right + 15;
			else
				rc.left = rc.right + 5;
			rc.right = rc.left + nWidth;
		}
		p_font = new CFont();
		p_font->CreateFontIndirect(&logfont);
		p_edit = new CEdit();
		p_edit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_CENTER | ES_NUMBER | ES_MULTILINE, rc, this, IDC_EDIT_START + i);
		p_edit->SetFont(p_font);
		GetDynamicLayout()->AddItem(p_edit->GetSafeHwnd(), CMFCDynamicLayout::MoveHorizontalAndVertical(50, 50), CMFCDynamicLayout::SizeNone());
		m_vec_p_font.push_back(p_font);
		m_vec_p_edit.push_back(p_edit);
	}
	((CButton*)GetDlgItem(IDC_CHECK_HIGHLIGHT))->SetCheck(TRUE);
	CString strNum;
	for (int nNum = 1; nNum <= 9; ++nNum)
	{
		strNum.Format(_T("%d"), nNum);
		m_comboNumber.AddString(strNum);
	}
	m_comboNumber.SetCurSel(0);
}


void CSudokuGameDlg::GetDataFromEdit()
{
	int sz_nData[ZSUDOKU_TOTAL_SIZE];
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		CString strEdit;
		GetDlgItemText(IDC_EDIT_START + i, strEdit);
		if (strEdit.GetLength() == 1)
			sz_nData[i] = _ttoi(strEdit);
		else
			sz_nData[i] = 0;
	}
	m_sudoku.SetData(sz_nData);
}


void CSudokuGameDlg::SetDataToEdit()
{
	int sz_nData[ZSUDOKU_TOTAL_SIZE];
	m_sudoku.GetData(sz_nData);
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		if (sz_nData[i])
		{
			CString strEdit;
			strEdit.Format(_T("%d"), sz_nData[i]);
			SetDlgItemText(IDC_EDIT_START + i, strEdit);
			SetEditFont(IDC_EDIT_START + i, strEdit);
		}
	}
}


void CSudokuGameDlg::SetEditFont(UINT nID, const CString& strEdit)
{
	LOGFONT logfont;
	GetFont()->GetLogFont(&logfont);
	if (strEdit.GetLength() > 1)
		logfont.lfHeight = 15;
	else
		logfont.lfHeight = 40;
	m_vec_p_font[nID - IDC_EDIT_START]->DeleteObject();
	m_vec_p_font[nID - IDC_EDIT_START]->CreateFontIndirect(&logfont);
	m_vec_p_edit[nID - IDC_EDIT_START]->SetFont(m_vec_p_font[nID - IDC_EDIT_START]);
}


void CSudokuGameDlg::EnableButton(BOOL nEnable)
{
	GetDlgItem(IDC_BUTTON_BACKUP)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_RESTORE)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_LOCK)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_UNLOCK)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_CHECK)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_CANDIDATE)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_BASICCALC)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_AUTOCALC)->EnableWindow(nEnable);
}


CString CSudokuGameDlg::GetRuleErrorInfo()
{
	CString strError;
	const ZSudoku::RuleErrorInfo& rei = m_sudoku.GetLastRuleError();
	switch (rei.m_type)
	{
	case ZSudoku::RULE_ERR_NONE:strError.Format(_T("检查成功，已确认：%d个，未确认：%d个"),m_sudoku.GetConfirmedCount(),m_sudoku.GetUnconfirmedCount()); break;
	case ZSudoku::RULE_ERR_ROW:strError.Format(_T("第%d行存在多个数字\"%d\""), rei.m_nIndex + 1, rei.m_nNumber); break;
	case ZSudoku::RULE_ERR_COLUMN:strError.Format(_T("第%d列存在多个数字\"%d\""), rei.m_nIndex + 1, rei.m_nNumber); break;
	case ZSudoku::RULE_ERR_BLOCK:strError.Format(_T("第%d宫存在多个数字\"%d\""), rei.m_nIndex + 1, rei.m_nNumber); break;
	default:break;
	}
	return strError;
}


void CSudokuGameDlg::SetTimerInfo(const CString& strInfo, UINT nElapse)
{
	SetDlgItemText(IDC_STATIC_INFO, strInfo);
	SetTimer(nTimerInfoClear, nElapse, nullptr);
}
