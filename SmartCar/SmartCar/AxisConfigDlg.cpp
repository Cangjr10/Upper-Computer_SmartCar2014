// AxisConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SmartCar.h"
#include "AxisConfigDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CAxisConfigDlg �Ի���

IMPLEMENT_DYNAMIC(CAxisConfigDlg, CDialogEx)

CAxisConfigDlg::CAxisConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAxisConfigDlg::IDD, pParent)
	, m_nW1Xrange(16)
	, m_nW1Y1(0)
	, m_nW1Y2(255)
/*	, m_nW1Y1(0)*/
	, m_nW2Xrange(16)
	, m_nW2Y1(0)
	, m_nW2Y2(255)
	, m_nW3Xrange(16)
	, m_nW3Y1(0)
	, m_nW3Y2(255)
{

}

CAxisConfigDlg::~CAxisConfigDlg()
{
}

void CAxisConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_W1XPOINT, m_nW1Xrange);
	DDX_Text(pDX, IDC_W1Y2, m_nW1Y2);
	DDX_Text(pDX, IDC_W1Y1, m_nW1Y1);
	DDX_Text(pDX, IDC_W2XPOINT, m_nW2Xrange);
	DDX_Text(pDX, IDC_W2Y1, m_nW2Y1);
	DDX_Text(pDX, IDC_W2Y2, m_nW2Y2);
	DDX_Text(pDX, IDC_W3XPOINT, m_nW3Xrange);
	DDX_Text(pDX, IDC_W3Y1, m_nW3Y1);
	DDX_Text(pDX, IDC_W3Y2, m_nW3Y2);
}


BEGIN_MESSAGE_MAP(CAxisConfigDlg, CDialogEx)
END_MESSAGE_MAP()


// CAxisConfigDlg ��Ϣ�������
void CAxisConfigDlg::SaveAxisConfig()
{
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CPlotView* pPlotView_tempt=(CPlotView*)pMain->pPlotView;
	pPlotView_tempt->Window[0].SetXrange(m_nW1Xrange);
	pPlotView_tempt->Window[1].SetXrange(m_nW2Xrange);
	pPlotView_tempt->Window[2].SetXrange(m_nW3Xrange);
	pPlotView_tempt->Window[0].SetYrange(m_nW1Y1,m_nW1Y2);
	pPlotView_tempt->Window[1].SetYrange(m_nW2Y1,m_nW2Y2);
	pPlotView_tempt->Window[2].SetYrange(m_nW3Y1,m_nW3Y2);
	CFile file1 (_T ("Axis.txt"), CFile::modeCreate | CFile::modeWrite);
	CArchive ar1 (&file1, CArchive::store);
	for (int i=0;i<3;i++)
	{
		pPlotView_tempt->Window[i].Serialization(ar1); 
	}
	ar1.Flush();
	//����ϣ��ر��ļ���
	ar1.Close();
	file1.Close();
}

BOOL CAxisConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CPlotView* pPlotView_tempt=(CPlotView*)pMain->pPlotView;
	m_nW1Xrange = pPlotView_tempt->Window[0].GetXrange();
	m_nW1Y1= pPlotView_tempt->Window[0].GetY1();
	m_nW1Y2= pPlotView_tempt->Window[0].GetY2();
	m_nW2Xrange= pPlotView_tempt->Window[1].GetXrange();
	m_nW2Y1= pPlotView_tempt->Window[1].GetY1();
	m_nW2Y2= pPlotView_tempt->Window[1].GetY2();
	m_nW3Xrange= pPlotView_tempt->Window[2].GetXrange();
	m_nW3Y1= pPlotView_tempt->Window[2].GetY1();
	m_nW3Y2= pPlotView_tempt->Window[2].GetY2();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
