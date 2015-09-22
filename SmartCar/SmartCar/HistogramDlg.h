#pragma once
#include "SmartCarDoc.h"

// CHistogramDlg 对话框

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistogramDlg();

// 对话框数据
	enum { IDD = IDD_HISTOGRAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	vector<BYTE> histo_graph;
	UINT histo[256];
	UINT* pHisto;
	int m_nSel;
	BOOL m_LButtonDown;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	void ShowHistogram(UINT* Histo_ptr);
	void OnStatistic(const vector<BYTE> &graph_ptr,UINT* Histo_ptr);//
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
