
// MFC_GrayedoutListViewDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MFC_GrayedoutListView.h"
#include "MFC_GrayedoutListViewDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCGrayedoutListViewDlg ダイアログ



CMFCGrayedoutListViewDlg::CMFCGrayedoutListViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_GRAYEDOUTLISTVIEW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hdcDisabledList = NULL;
}

CMFCGrayedoutListViewDlg::~CMFCGrayedoutListViewDlg()
{
	if (m_hdcDisabledList) {
		DeleteDC(m_hdcDisabledList);
		m_hdcDisabledList = NULL;
	}
}

void CMFCGrayedoutListViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnGrayedout);
	DDX_Control(pDX, IDC_LIST1, m_lvList);
}

BEGIN_MESSAGE_MAP(CMFCGrayedoutListViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCGrayedoutListViewDlg::OnBnClickedButton1)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCGrayedoutListViewDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CMFCGrayedoutListViewDlg メッセージ ハンドラー

BOOL CMFCGrayedoutListViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	SetDataListView();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMFCGrayedoutListViewDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		// 透過リスト画像の描画
		if (m_hdcDisabledList) {
			CRect rc;
			m_lvList.GetWindowRect(&rc);
			ScreenToClient(&rc);
			BitBlt(dc.GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(), m_hdcDisabledList, 0, 0, SRCCOPY);
		}
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMFCGrayedoutListViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// リストビューにデータをセット
void CMFCGrayedoutListViewDlg::SetDataListView()
{
	// チェックボックス有効化
	DWORD dwStyle = m_lvList.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT;
	m_lvList.SetExtendedStyle(dwStyle);

	LPTSTR lpszHeader[] = { _T("ID"), _T("Val"), _T("Text1"), _T("Text2"), NULL };
	for (int i = 0; lpszHeader[i] != NULL; i++) {

		m_lvList.InsertColumn(i, lpszHeader[i], LVCFMT_LEFT, 80);
	}

	for (int iRow = 0; iRow < 100; iRow++) {

		// ID
		TCHAR szTxt[256];
		_stprintf_s(szTxt, _T("%d"), iRow);
		m_lvList.InsertItem(iRow, szTxt);

		// Val
		_stprintf_s(szTxt, _T("%d"), iRow * 5);
		m_lvList.SetItemText(iRow, 1, szTxt);

		// Text1
		m_lvList.SetItemText(iRow, 2, _T("foo"));

		// Text2
		m_lvList.SetItemText(iRow, 3, _T("bar"));
	}
}


// Enable/Disableボタン押下処理
void CMFCGrayedoutListViewDlg::OnBnClickedButton1()
{
	static bool m_bGrayedout = false;
	if (m_bGrayedout) {
		// リスト有効化処理
		m_bGrayedout = false;
		m_btnGrayedout.SetWindowTextW(_T("Disable"));

		if (m_hdcDisabledList) {
			DeleteDC(m_hdcDisabledList);
			m_hdcDisabledList = NULL;
		}

		m_lvList.ShowWindow(SW_SHOW);
		m_lvList.EnableWindow(TRUE);
		m_lvList.SetFocus();
	}
	else {
		// リスト無効化処理
		m_bGrayedout = true;
		m_btnGrayedout.SetWindowTextW(_T("Enable"));

		if (m_hdcDisabledList) {
			DeleteDC(m_hdcDisabledList);
			m_hdcDisabledList = NULL;
		}

		CRect rc;
		m_lvList.GetWindowRect(&rc);
		ScreenToClient(&rc);

		CBitmap alphaBmp;
		CDC	*pSrcDC = m_lvList.GetDC();
		alphaBmp.CreateCompatibleBitmap(pSrcDC, rc.Width(), rc.Height());

		m_hdcDisabledList = CreateCompatibleDC(pSrcDC->GetSafeHdc());
		/*m_objOldBmp = */SelectObject(m_hdcDisabledList, alphaBmp);

		CBrush brush(GetSysColor(COLOR_BTNFACE));
		CRect rcFill(0, 0, rc.Width(), rc.Height());
		FillRect(m_hdcDisabledList, rcFill, brush);

		// 透過処理
		BLENDFUNCTION bf;
		bf.AlphaFormat = 0;
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = 100;
		::AlphaBlend(m_hdcDisabledList, 0, 0, rc.Width(), rc.Height(), pSrcDC->GetSafeHdc(), 0, 0, rc.Width(), rc.Height(), bf);
		ReleaseDC(pSrcDC);

		m_lvList.EnableWindow(FALSE);
		m_lvList.ShowWindow(SW_HIDE);
	}

}
