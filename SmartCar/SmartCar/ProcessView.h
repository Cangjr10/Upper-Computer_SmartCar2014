#pragma once

#include "SmartCarDoc.h"
// CProcessView 视图

class CProcessView : public CView
{
	DECLARE_DYNCREATE(CProcessView)

protected:
	CProcessView();           // 动态创建所使用的受保护的构造函数
	virtual ~CProcessView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
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


