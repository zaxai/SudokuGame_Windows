
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



CSudokuGameDlg::CSudokuGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SUDOKUGAME_DIALOG, pParent)
	, m_sz2_nDataOri()
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
	ON_BN_CLICKED(IDC_BUTTON_CANDIDATE, &CSudokuGameDlg::OnBnClickedButtonCandidate)
	ON_BN_CLICKED(IDC_BUTTON_BASICCALC, &CSudokuGameDlg::OnBnClickedButtonBasiccalc)
	ON_BN_CLICKED(IDC_BUTTON_AUTOCALC, &CSudokuGameDlg::OnBnClickedButtonAutocalc)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSudokuGameDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_CHECK_HIGHLIGHT, &CSudokuGameDlg::OnBnClickedCheckHighlight)
	ON_CBN_SELCHANGE(IDC_COMBO_NUMBER, &CSudokuGameDlg::OnCbnSelchangeComboNumber)
	ON_CONTROL_RANGE(EN_UPDATE, IDC_EDIT_START, IDC_EDIT_START + ZSUDOKU_TOTAL_SIZE, &CSudokuGameDlg::OnEnUpdateEditData)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
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
		m_sz2_nDataOri[i / ZSUDOKU_SIZE][i % ZSUDOKU_SIZE] = GetDlgItemInt(IDC_EDIT_START + i);
}


void CSudokuGameDlg::OnBnClickedButtonRestore()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		if (m_sz2_nDataOri[i / ZSUDOKU_SIZE][i % ZSUDOKU_SIZE])
			SetDlgItemInt(IDC_EDIT_START + i, m_sz2_nDataOri[i / ZSUDOKU_SIZE][i % ZSUDOKU_SIZE]);
		else
			SetDlgItemText(IDC_EDIT_START + i, _T(""));
		if (!m_vec_p_edit[i]->IsWindowEnabled())
			m_vec_p_edit[i]->EnableWindow(TRUE);
	}
}


void CSudokuGameDlg::OnBnClickedButtonLock()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		int nNum = GetDlgItemInt(IDC_EDIT_START + i);
		if (nNum >= 1 && nNum <= 9)
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
			m_vec_p_edit[i]->SetWindowText(_T(""));
	}
}


void CSudokuGameDlg::OnBnClickedButtonCandidate()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDataFromEdit();
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		if (GetDlgItemInt(IDC_EDIT_START + i) == 0)
		{
			CString strEdit, strNum;
			std::vector<int> vec_nNum;
			m_sudoku.GetCandidateNum(i / ZSUDOKU_SIZE, i % ZSUDOKU_SIZE, vec_nNum);
			for (const int& nNum : vec_nNum)
			{
				strNum.Format(_T("%d"), nNum);
				strEdit += strNum;
			}
			SetDlgItemText(IDC_EDIT_START + i, strEdit);
		}
	}
}


void CSudokuGameDlg::OnBnClickedButtonBasiccalc()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDataFromEdit();
	if (!m_sudoku.RuleCheck())
	{
		AfxMessageBox(_T("单行、单列或九宫格存在重复数字！"));
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
		AfxMessageBox(_T("单行、单列或九宫格存在重复数字！"));
		return;
	}
	EnableButton(FALSE);
	m_sudoku.StartAutoCalc(this);
}


void CSudokuGameDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sudoku.StopAutoCalc();
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
	SetEditFont(nID);
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
	CRect rc;
	GetClientRect(&rc);
	rc.bottom = rc.top;
	const int nStartLeft = rc.left + 15;
	const int nWidth = 50, nHeight = 50;
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
		p_edit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_CENTER | ES_NUMBER | ES_WANTRETURN, rc, this, IDC_EDIT_START + i);
		p_edit->SetFont(p_font);
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
		sz_nData[i] = GetDlgItemInt(IDC_EDIT_START + i);
	m_sudoku.SetData(sz_nData);
}


void CSudokuGameDlg::SetDataToEdit()
{
	int sz_nData[ZSUDOKU_TOTAL_SIZE];
	m_sudoku.GetData(sz_nData);
	for (int i = 0; i < ZSUDOKU_TOTAL_SIZE; ++i)
	{
		if (sz_nData[i])
			SetDlgItemInt(IDC_EDIT_START + i, sz_nData[i]);
	}
}


void CSudokuGameDlg::SetEditFont(UINT nID)
{
	LOGFONT logfont;
	GetFont()->GetLogFont(&logfont);
	CString strEdit;
	GetDlgItemText(nID, strEdit);
	if (strEdit.GetLength() > 4)
		logfont.lfHeight = 15;
	else if (strEdit.GetLength() > 1)
		logfont.lfHeight = 20;
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
	GetDlgItem(IDC_BUTTON_CANDIDATE)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_BASICCALC)->EnableWindow(nEnable);
	GetDlgItem(IDC_BUTTON_AUTOCALC)->EnableWindow(nEnable);
}
