// HistogramDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SmartCar.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "SmartCarDoc.h"
#include "ControlView.h"

// CHistogramDlg �Ի���

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistogramDlg::IDD, pParent)
{
	pHisto = histo;
	m_nSel = 0;
	m_LButtonDown=FALSE;
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CHistogramDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CHistogramDlg ��Ϣ�������


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	OnStatistic(histo_graph,pHisto);
	ShowHistogram(pHisto);
}


BOOL CHistogramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	 CControlView* pView=(CControlView*)pMain->GetActiveView(); 
	 histo_graph = ((CSmartCarDoc*)pView->GetDocument())->Image_Complete->image_data;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CHistogramDlg::OnStatistic(const vector<BYTE> &graph_ptr,UINT* Histo_ptr)//nΪ���ݴ�С
{
	memset(Histo_ptr,0,sizeof(histo));
	for (UINT i=0;i < graph_ptr.size();i++)
	{
		Histo_ptr[graph_ptr[i]]++;
	}
}

void CHistogramDlg::ShowHistogram(UINT* Histo_ptr)
{
	CWnd* pWnd=GetDlgItem(IDC_PICTURE);
	CDC* pDC=pWnd->GetDC();//ָ��
	CRect rect;
	pWnd->GetClientRect(&rect);
	CString str;
	CPen* pPenGreen = new CPen;//�������ʶ���
	pPenGreen->CreatePen(PS_SOLID,2,RGB(0,160,0));//��ɫ����
	CPen* pPenBlue = new CPen;
	pPenBlue->CreatePen(PS_SOLID,2,RGB(0,0,255));//��ɫ����
	CGdiObject* pOldPen = pDC->SelectObject(pPenGreen);
	pDC->MoveTo(10,10);
	pDC->LineTo(10,280);
	pDC->LineTo(300,280);
	str.Format(_T("0"));
	pDC->TextOutW(10,283,str);
	str.Format(_T("50"));
	pDC->TextOutW(60,283,str);
	str.Format(_T("100"));
	pDC->TextOutW(110,283,str);
	str.Format(_T("150"));
	pDC->TextOutW(160,283,str);
	str.Format(_T("200"));
	pDC->TextOutW(210,283,str);
	str.Format(_T("255"));
	pDC->TextOutW(265,283,str);
	for (int i=0;i<256;i+=5)
	{
		if ((i & 1)==0)
		{
			pDC->MoveTo(i+10,280);
			pDC->LineTo(i+10,284);
		}
		else
		{
			pDC->MoveTo(i+10,280);
			pDC->LineTo(i+10,282);
		}
	}
	pDC->MoveTo(295,275);
	pDC->LineTo(300,280);
	pDC->LineTo(295,285);
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	UINT lMaxcount=0;
	for (int i =0;i<256;i++)
	{
		if (Histo_ptr[i]>lMaxcount)
		{
			lMaxcount=Histo_ptr[i];
		}
	}
	pDC->MoveTo(10,25);
	pDC->LineTo(14,25);
	str.Format(_T("%d"),lMaxcount);
	pDC->TextOutW(11,26,str);
	pDC->SelectObject(pPenBlue);
	if (lMaxcount>0)
	{
		for (int i=0;i<256;i++)
		{
			pDC->MoveTo(i+10,280);
			pDC->LineTo(i+10,281-(UINT)(Histo_ptr[i]*256/lMaxcount));
		}
	}
	pDC->SelectObject(pOldPen);
	delete pPenBlue;
	delete pPenGreen;
}




void CHistogramDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_LButtonDown = TRUE;
	CWnd* pWnd=GetDlgItem(IDC_PICTURE);
	CDC* pDC=pWnd->GetDC();//ָ��
	CRect rect;
	pWnd->GetClientRect(&rect);
	pDC->SetROP2(R2_NOT);   //����
	if (m_nSel)
	{
		pDC->MoveTo(m_nSel,10);
		pDC->LineTo(m_nSel,280);
	}


	int nX=point.x;
	if (nX<20)
	{
		nX=20;
	}
	if (nX>275)
	{
		nX=275;
	}
	pDC->MoveTo(nX-10,10);         //���ƹ��
	pDC->LineTo(nX-10,280);
	m_nSel = nX-10;//�������ͼλ�ã�������һ�β���
	CString str;
	str.Format(_T("        "));
	pDC->TextOutW(rect.right-30,rect.top+10,str);
	str.Format(_T("%4d"),m_nSel-10);
	pDC->TextOutW(rect.right-30,rect.top+10,str);
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CProcessView* peView=(CProcessView*)pMain->pProcessView;
	CControlView* pControlView_tempt=(CControlView*)pMain->pControlView;
	pControlView_tempt->m_nThresh = m_nSel-10;
	pControlView_tempt->UpdateData(FALSE);
	pControlView_tempt->OnBnClickedBinaryButton();//���ö�ֵ���㷨
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CHistogramDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDialogEx::OnOK();
}


void CHistogramDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_LButtonDown)
	{
		CWnd* pWnd=GetDlgItem(IDC_PICTURE);
		CDC* pDC=pWnd->GetDC();//ָ��
		CRect rect;
		pWnd->GetClientRect(&rect);
		pDC->SetROP2(R2_NOT);   //����
		if (m_nSel)
		{
			pDC->MoveTo(m_nSel,10);
			pDC->LineTo(m_nSel,280);
		}
		int nX=point.x;
		if (nX<20)
		{
			nX=20;
		}
		if (nX>275)
		{
			nX=275;
		}
		pDC->MoveTo(nX-10,10);         //���ƹ��
		pDC->LineTo(nX-10,280);
		m_nSel = nX-10;
		CString str;
		str.Format(_T("        "));
		pDC->TextOutW(rect.right-30,rect.top+10,str);
		str.Format(_T("%4d"),m_nSel-10);
		pDC->TextOutW(rect.right-30,rect.top+10,str);
		CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
		CProcessView* peView=(CProcessView*)pMain->pProcessView;
		CControlView* pControlView_tempt=(CControlView*)pMain->pControlView;
		pControlView_tempt->m_nThresh = m_nSel-10;
		pControlView_tempt->UpdateData(FALSE);
		pControlView_tempt->OnBnClickedBinaryButton();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CHistogramDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_LButtonDown = FALSE;
	CDialogEx::OnLButtonUp(nFlags, point);
}



void CHistogramDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CHistogramDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		/*OnChar()*/
		CWnd* pWnd=GetDlgItem(IDC_PICTURE);
		CDC* pDC=pWnd->GetDC();//ָ��
		CRect rect;
		pWnd->GetClientRect(&rect);
		
		switch (pMsg->wParam)
		{
		case VK_LEFT:
			{
				if (m_nSel)
				{
					pDC->SetROP2(R2_NOT);   //����
					pDC->MoveTo(m_nSel,10);
					pDC->LineTo(m_nSel,280);
				}
				if(m_nSel<11)	m_nSel=11;
				if(m_nSel>264)	m_nSel=264;
				m_nSel--;
				pDC->MoveTo(m_nSel,10);
				pDC->LineTo(m_nSel,280);
				CString str;
				str.Format(_T("        "));
				pDC->TextOutW(rect.right-30,rect.top+10,str);
				str.Format(_T("%4d"),m_nSel-10);
				pDC->TextOutW(rect.right-30,rect.top+10,str);
				CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
				CProcessView* peView=(CProcessView*)pMain->pProcessView;
				CControlView* pControlView_tempt=(CControlView*)pMain->pControlView;
				pControlView_tempt->m_nThresh = m_nSel-10;
				pControlView_tempt->UpdateData(FALSE);
				pControlView_tempt->OnBnClickedBinaryButton();
			}
			
			break;
		case VK_RIGHT:
			{
				if (m_nSel)
				{
					pDC->SetROP2(R2_NOT);   //����
					pDC->MoveTo(m_nSel,10);
					pDC->LineTo(m_nSel,280);
				}
				if(m_nSel<11) m_nSel=11;
				if(m_nSel>264)	m_nSel=264;
				m_nSel++;
				pDC->MoveTo(m_nSel,10);
				pDC->LineTo(m_nSel,280);
				CString str;
				str.Format(_T("        "));
				pDC->TextOutW(rect.right-30,rect.top+10,str);
				str.Format(_T("%4d"),m_nSel-10);
				pDC->TextOutW(rect.right-30,rect.top+10,str);
				CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
				CProcessView* peView=(CProcessView*)pMain->pProcessView;
				CControlView* pControlView_tempt=(CControlView*)pMain->pControlView;
				pControlView_tempt->m_nThresh = m_nSel-10;
				pControlView_tempt->UpdateData(FALSE);
				pControlView_tempt->OnBnClickedBinaryButton();
			}

			break;
		default:
			break;

		}
	
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CHistogramDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
