#pragma once


// CAxisConfigDlg �Ի���

class CAxisConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAxisConfigDlg)

public:
	CAxisConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAxisConfigDlg();

// �Ի�������
	enum { IDD = IDD_AXIS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nW1Xrange;
//	int m_nW1Y1;
	long m_nW1Y2;
	long m_nW1Y1;
	UINT m_nW2Xrange;
	long m_nW2Y1;
	long m_nW2Y2;
	UINT m_nW3Xrange;
	long m_nW3Y1;
	long m_nW3Y2;
	void SaveAxisConfig();
	virtual BOOL OnInitDialog();
};
