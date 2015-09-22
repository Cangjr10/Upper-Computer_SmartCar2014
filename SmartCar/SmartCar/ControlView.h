#pragma once
//#include "cmscomm.h"
#include "CMSComm.h"
#include "afxwin.h"
#include "HistogramDlg.h"
#include "WavePlotDlg.h"
#include  <vector>
#include "afxcmn.h"
using std::vector;
// CControlView ������ͼ
//enum data_style{s_NULL,s_byte,s_int16,s_uint16,s_float,s_long};
class CControlView : public CFormView
{
	DECLARE_DYNCREATE(CControlView)
	friend class CHistogramDlg;
//	friend class CWavePlotDlg;
protected:
	CControlView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
private:
	CHistogramDlg *m_pHistogram;
	CMSComm m_SCIModule;
	DECLARE_EVENTSINK_MAP()
	BOOL m_nPortOpen;
	BOOL m_inFrame;//�����Ƿ�ʼ�ɼ�ͼ������
	BYTE m_config;
	void OnCommMscomm();//������Ϣ��Ӧ������
	int m_nComNum;
	int m_nBaudrate;
	CComboBox m_ComNum;
	CComboBox m_Baudrate;
	UINT m_nWidth;//ͼ����
	UINT m_nHeight;//ͼ��߶�
	UINT m_datacount;//��¼ͼ�����ݸ���
	void SwapImageBuffers();
	vector<CString> str_baudrate ;//���沨����
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
	//PlotView�е����ݽ��ձ�������
	BOOL m_inPlotData;//�Ƿ����ڽ��ܲ������ݱ�־
	BOOL IsAxisFailed;

	afx_msg void OnBnClickedBitmapsave();
public:
	afx_msg void OnBnClickedWaveconfig();
	CTreeCtrl m_PlotTree;
	HTREEITEM hRoot;     // ���ĸ��ڵ�ľ��   
	HTREEITEM hWindow[3];  // ���ڽڵ�   
	HTREEITEM hPlot[9];  // ����1~9
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

