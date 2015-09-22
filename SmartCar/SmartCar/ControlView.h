#pragma once
//#include "cmscomm.h"
#include "CMSComm.h"
#include "afxwin.h"
#include "HistogramDlg.h"
#include "WavePlotDlg.h"
#include  <vector>
#include "afxcmn.h"
using std::vector;
// CControlView 窗体视图
//enum data_style{s_NULL,s_byte,s_int16,s_uint16,s_float,s_long};
class CControlView : public CFormView
{
	DECLARE_DYNCREATE(CControlView)
	friend class CHistogramDlg;
//	friend class CWavePlotDlg;
protected:
	CControlView();           // 动态创建所使用的受保护的构造函数
	virtual ~CControlView();

public:
	enum { IDD = IDD_CONTROL_DIALOG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
private:
	CHistogramDlg *m_pHistogram;
	CMSComm m_SCIModule;
	DECLARE_EVENTSINK_MAP()
	BOOL m_nPortOpen;
	BOOL m_inFrame;//表征是否开始采集图像数据
	BYTE m_config;
	void OnCommMscomm();//串口消息响应处理函数
	int m_nComNum;
	int m_nBaudrate;
	CComboBox m_ComNum;
	CComboBox m_Baudrate;
	UINT m_nWidth;//图像宽度
	UINT m_nHeight;//图像高度
	UINT m_datacount;//记录图像数据个数
	void SwapImageBuffers();
	vector<CString> str_baudrate ;//保存波特率
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedOpencom();
	CButton m_PortSwitch;
	afx_msg void OnBnClickedConfigpicok();
	afx_msg void OnBnClickedBitmapopen();
	CButton m_BitmapOpen;
	CButton m_BitmapSave;
	BYTE	m_nThresh;
	afx_msg void OnBnClickedBinaryButton();
	afx_msg void OnBnClickedHistogramButton();
	afx_msg void OnBnClickedOnalgorithm();
	//PlotView中的数据接收变量定义
	BOOL m_inPlotData;//是否正在接受波形数据标志
	BOOL IsAxisFailed;

	afx_msg void OnBnClickedBitmapsave();
public:
	afx_msg void OnBnClickedWaveconfig();
	CTreeCtrl m_PlotTree;
	HTREEITEM hRoot;     // 树的根节点的句柄   
	HTREEITEM hWindow[3];  // 窗口节点   
	HTREEITEM hPlot[9];  // 曲线1~9
	BOOL Initial_flag;
	BOOL clicked_flag;


//	afx_msg void OnSelchangedPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTvnSingleExpandPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemChangingPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemChangedPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAxis();
};

