
// ImageView.h : CImageView 类的接口
//

#pragma once
#include "SmartCarDoc.h"

class CImageView : public CView
{
protected: // 仅从序列化创建
	CImageView();
	DECLARE_DYNCREATE(CImageView)

// 特性
public:
	CSmartCarDoc* GetDocument() const;

// 操作
public:
	void Plot_Image(const Image_class _image);
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // ImageView.cpp 中的调试版本
inline CSmartCarDoc* CImageView::GetDocument() const
   { return reinterpret_cast<CSmartCarDoc*>(m_pDocument); }
#endif

