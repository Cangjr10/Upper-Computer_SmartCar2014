
// ImageView.h : CImageView ��Ľӿ�
//

#pragma once
#include "SmartCarDoc.h"

class CImageView : public CView
{
protected: // �������л�����
	CImageView();
	DECLARE_DYNCREATE(CImageView)

// ����
public:
	CSmartCarDoc* GetDocument() const;

// ����
public:
	void Plot_Image(const Image_class _image);
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // ImageView.cpp �еĵ��԰汾
inline CSmartCarDoc* CImageView::GetDocument() const
   { return reinterpret_cast<CSmartCarDoc*>(m_pDocument); }
#endif

