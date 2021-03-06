
// MFC_GrayedoutListViewDlg.h : ヘッダー ファイル
//

#pragma once


// CMFCGrayedoutListViewDlg ダイアログ
class CMFCGrayedoutListViewDlg : public CDialogEx
{
// コンストラクション
public:
	CMFCGrayedoutListViewDlg(CWnd* pParent = nullptr);	// 標準コンストラクター
	~CMFCGrayedoutListViewDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_GRAYEDOUTLISTVIEW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

	void SetDataListView();
// 実装
protected:
	HICON m_hIcon;

	HDC	m_hdcDisabledList;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnGrayedout;
	CListCtrl m_lvList;
	afx_msg void OnBnClickedButton1();
};
