#pragma once
#include "afxwin.h"
#include "SmartCarDoc.h"

// CWavePlotDlg 对话框

class CWavePlotDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWavePlotDlg)
	friend class CSmartCarDoc;//声明为友元，读取配置信息以保存成文件
public:
	CWavePlotDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWavePlotDlg();
	void loadconfig();//读取设置并显示
	void saveconfig();//保存设置
// 对话框数据
	enum { IDD = IDD_WAVEPLOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_W1Name1;
	CString m_W1Name2;
	CString m_W1Name3;
	CString m_W2Name1;
	CString m_W2Name2;
	CString m_W2Name3;
	CString m_W3Name1;
	CString m_W3Name2;
	CString m_W3Name3;
	CComboBox m_W1Style1;
	CComboBox m_W1Style2;
	CComboBox m_W1Style3;
	CComboBox m_W2Style1;
	CComboBox m_W2Style2;
	CComboBox m_W2Style3;
	CComboBox m_W3Style1;
	CComboBox m_W3Style2;
	CComboBox m_W3Style3;
	int m_W1DataNum1;
	int m_W1DataNum2;
	int m_W1DataNum3;
	int m_W2DataNum1;
	int m_W2DataNum2;
	int m_W2DataNum3;
	int m_W3DataNum1;
	int m_W3DataNum2;
	int m_W3DataNum3;
	BYTE m_W1Start1;
	BYTE m_W1Start2;
	BYTE m_W1Start3;
	BYTE m_W2Start1;
	BYTE m_W2Start2;
	BYTE m_W2Start3;
	BYTE m_W3Start1;
	BYTE m_W3Start2;
	BYTE m_W3Start3;
	BYTE m_W1Start1_1;
	BYTE m_W1Start2_1;
	BYTE m_W1Start3_1;
	BYTE m_W2Start1_1;
	BYTE m_W2Start2_1;
	BYTE m_W2Start3_1;
	BYTE m_W3Start1_1;
	BYTE m_W3Start2_1;
	BYTE m_W3Start3_1;
	int m_nW1Style1;
	int m_nW1Style2;
	int m_nW1Style3;
	int m_nW2Style1;
	int m_nW2Style2;
	int m_nW2Style3;
	int m_nW3Style1;
	int m_nW3Style2;
	int m_nW3Style3;
	CSmartCarDoc* p_doc;
};
