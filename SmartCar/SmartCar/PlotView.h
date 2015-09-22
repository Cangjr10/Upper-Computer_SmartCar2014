#pragma once

#include "PlotWindow.h"
// CPlotView 视图

class CPlotView : public CView
{
	DECLARE_DYNCREATE(CPlotView)
public:
	//void GetPlotData()
protected:
	CPlotView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPlotView();
private:
	vector<float> PlotData[9];
public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	CPlotWindow Window[3];
	CRect rect1_1;
	CRect rect2_[2];
	CRect rect3_[3];
	COLORREF color[3];
	COLORREF PenColor[3];
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


