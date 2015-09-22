// PlotView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SmartCar.h"
#include "PlotView.h"
#include "SmartCarDoc.h"
#include "MainFrm.h"

// CPlotView

IMPLEMENT_DYNCREATE(CPlotView, CView)

CPlotView::CPlotView()
{
}

CPlotView::~CPlotView()
{
}

BEGIN_MESSAGE_MAP(CPlotView, CView)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CPlotView ��ͼ

void CPlotView::OnDraw(CDC* pDC)
{
		// TODO: �ڴ���ӻ��ƴ���
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CSmartCarDoc* pSmartCarDoc = (CSmartCarDoc*)m_pDocument;
	//ȷ�������ӷָ�
	int PartNumber = 0;
	for (int i=0;i<3;i++)
	{
		PartNumber +=Window[i].GetShowFlag();
	}
	switch (PartNumber)
	{
	case 1:
		for(int i=0; i<3; i++)
		{
			if (Window[i].GetShowFlag())
			{
				Window[i].SetRect(rect1_1);
				Window[i].SetBackColor(color[0]);
			}
		}
		break;
	case 2:
		if (!Window[0].GetShowFlag())//��һ�����ڲ���ʾ
		{
			Window[1].SetRect(rect2_[0]);
			Window[1].SetBackColor(color[0]);
			Window[2].SetRect(rect2_[1]);
			Window[2].SetBackColor(color[1]);
		}
		else if(!Window[1].GetShowFlag())
		{
			Window[0].SetRect(rect2_[0]);
			Window[2].SetRect(rect2_[1]);
			Window[0].SetBackColor(color[0]);
			Window[2].SetBackColor(color[1]);
		}
		else 
		{
			Window[0].SetRect(rect2_[0]);
			Window[1].SetRect(rect2_[1]);
			Window[0].SetBackColor(color[0]);
			Window[1].SetBackColor(color[1]);
		}
		break;
	case 3:
		for (int i=0;i<3;i++)
		{
			Window[i].SetRect(rect3_[i]);
			Window[i].SetBackColor(color[i]);
		}
		break;
	default://Ĭ�����ȷ�
		for (int i=0;i<3;i++)
		{
			Window[i].SetRect(rect3_[i]);
			Window[i].SetBackColor(color[i]);
			pDC->FillSolidRect(rect1_1,RGB(255,255,255));
		}
		break;
	}
	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL); 
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, rect1_1.right-rect1_1.left, rect1_1.bottom-rect1_1.top);
	MemDC.SelectObject(&MemBitmap); 
	for (int i=0;i<3;i++)
	{
		if (Window[i].GetShowFlag())
		{
			Window[i].DrawAxis(MemDC);
		}
	}
	for (int index=0;index<9;index++)
	{
		pSmartCarDoc->plot[index].GetPlotData(PlotData[index],Window[index/3].GetXrange());
		if(Window[index/3].GetShowFlag() && pSmartCarDoc->plot[index].GetShowFlag())
		{
			//Drawlines
			Window[index/3].PlotLine(MemDC,PlotData[index],PenColor[index%3]);
		}
	}
	pDC->BitBlt(0, 0,rect1_1.right-rect1_1.left, rect1_1.bottom-rect1_1.top, &MemDC, 0, 0, SRCCOPY); 


	//CSplitButton split;
// 	CRect rc(100,100,500,105);
// 	rc.InflateRect(-2,-2); 
// 	pDC->FillSolidRect(rc,RGB(128,0,0));
// 	pDC->Draw3dRect(rc,RGB(200,200,200),RGB(200,200,200)); 
// //	pDC->Draw3dRect(rc,RGB(255,0,0),RGB(255,0,0)); 

}



// CPlotView ���

#ifdef _DEBUG
void CPlotView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPlotView ��Ϣ�������



void CPlotView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnTimer(nIDEvent);
}

void CPlotView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	GetClientRect(rect1_1);
	for (int i=0;i<2;i++)
	{
		rect2_[i].left=rect1_1.left;
		rect2_[i].top=rect1_1.left+i*(rect1_1.bottom-rect1_1.top)/2;
		rect2_[i].right=rect1_1.right;
		rect2_[i].bottom=rect1_1.top+(i+1)*(rect1_1.bottom-rect1_1.top)/2;
	}
	for (int i=0;i<3;i++)
	{
		rect3_[i].left=rect1_1.left;
		rect3_[i].top=rect1_1.left+i*(rect1_1.bottom-rect1_1.top)/3;
		rect3_[i].right=rect1_1.right;
		rect3_[i].bottom=rect1_1.top+(i+1)*(rect1_1.bottom-rect1_1.top)/3;
	}
	color[0]=RGB(0,0,0);
	color[1]=RGB(0,0,0);
	color[2]=RGB(0,0,0);
	PenColor[0]=RGB(255,255,100);
	PenColor[1]=RGB(45,45,255);
	PenColor[2]=RGB(255,0,128);
// 	CSmartCarDoc* pSmartCarDoc = (CSmartCarDoc*)m_pDocument;
// 	for (int i=0;i<9;i++)
// 	{
// 		if(pSmartCarDoc->plot[i].GetShowFlag())
// 			Window[i/3].SetShowFlag(TRUE);
// 	}
	// TODO: �ڴ����ר�ô����/����û���
}


void CPlotView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	GetClientRect(rect1_1);
	for (int i=0;i<2;i++)
	{
		rect2_[i].left=rect1_1.left;
		rect2_[i].top=rect1_1.left+i*(rect1_1.bottom-rect1_1.top)/2;
		rect2_[i].right=rect1_1.right;
		rect2_[i].bottom=rect1_1.top+(i+1)*(rect1_1.bottom-rect1_1.top)/2;
	}
	for (int i=0;i<3;i++)
	{
		rect3_[i].left=rect1_1.left;
		rect3_[i].top=rect1_1.left+i*(rect1_1.bottom-rect1_1.top)/3;
		rect3_[i].right=rect1_1.right;
		rect3_[i].bottom=rect1_1.top+(i+1)*(rect1_1.bottom-rect1_1.top)/3;
	}
	color[0]=RGB(0,0,0);
	color[1]=RGB(0,0,0);
	color[2]=RGB(0,0,0);
	PenColor[0]=RGB(255,255,100);
	PenColor[1]=RGB(45,45,255);
	PenColor[2]=RGB(255,0,128);
	// TODO: �ڴ˴������Ϣ����������
}


void CPlotView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnLButtonDown(nFlags, point);
}


void CPlotView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnLButtonUp(nFlags, point);
}


void CPlotView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnMouseMove(nFlags, point);
}


void CPlotView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
