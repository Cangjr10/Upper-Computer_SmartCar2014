#pragma once

#include "SmartCarDoc.h"
// CProcessView ��ͼ

class CProcessView : public CView
{
	DECLARE_DYNCREATE(CProcessView)

protected:
	CProcessView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CProcessView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	void Plot_Image(const Image_class _image);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImageprocess();
};


