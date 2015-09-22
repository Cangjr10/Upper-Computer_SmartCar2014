
// ImageView.cpp : CImageView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "SmartCar.h"
#endif

#include "SmartCarDoc.h"
#include "ImageView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageView

IMPLEMENT_DYNCREATE(CImageView, CView)

BEGIN_MESSAGE_MAP(CImageView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CImageView 构造/析构

CImageView::CImageView()
{
	// TODO: 在此处添加构造代码

}

CImageView::~CImageView()
{
}

BOOL CImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImageView 绘制

void CImageView::OnDraw(CDC* /*pDC*/)
{
	CSmartCarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
// 	if (((CSmartCarDoc*)m_pDocument)->Image_Complete->image_data.size()!=0)
// 	{
// 		assert(((CSmartCarDoc*)m_pDocument)->Image_Complete->image_data.size() == 74*80);
// 	}
	
	Plot_Image(*((CSmartCarDoc*)m_pDocument)->Image_Complete);
	
	// TODO: 在此处为本机数据添加绘制代码
}


// CImageView 打印


void CImageView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImageView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageView 诊断

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CView::AssertValid();
}

void CImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSmartCarDoc* CImageView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSmartCarDoc)));
	return (CSmartCarDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageView 消息处理程序
void CImageView::Plot_Image(const Image_class _image)
{
	BITMAPINFO Bitinfo;//定义位图信息
	memset(&Bitinfo,0,sizeof(BITMAPINFO));
	Bitinfo.bmiHeader.biSize=40;
	Bitinfo.bmiHeader.biWidth=_image.width;
	Bitinfo.bmiHeader.biHeight=_image.height;
	Bitinfo.bmiHeader.biPlanes=1;
	Bitinfo.bmiHeader.biBitCount=24;
	Bitinfo.bmiHeader.biCompression=0;
	Bitinfo.bmiHeader.biSizeImage=(_image.width*24+31)/32*4*_image.height;
	Bitinfo.bmiHeader.biXPelsPerMeter=10000;
	Bitinfo.bmiHeader.biYPelsPerMeter=10000;
	Bitinfo.bmiHeader.biClrUsed=0;
	Bitinfo.bmiHeader.biClrImportant=0;
	//分配RGB数据存储空间
	BYTE * graph_tempt=new BYTE [Bitinfo.bmiHeader.biSizeImage];
	memset(graph_tempt,0,Bitinfo.bmiHeader.biSizeImage);
	//灰度图与RGB三色图转化
	int width_adjust=(_image.width*24+31)/32*4;//保证每行字节数为4的倍数
	for (unsigned int line=0;line < _image.height;line++)
	{
		for (unsigned int col=0;col<_image.width;col++)
		{
			graph_tempt[width_adjust*line+3*col]=_image.image_data[(_image.height-line-1)*_image.width+col];
			graph_tempt[width_adjust*line+3*col+1]=_image.image_data[(_image.height-line-1)*_image.width+col];
			graph_tempt[width_adjust*line+3*col+2]=_image.image_data[(_image.height-line-1)*_image.width+col];
		}
	}

	CRect rect;
	GetClientRect(&rect);
	StretchDIBits(GetDC()->m_hDC,0,0,(rect.right-rect.left),(rect.bottom-rect.top),0,0,_image.width,_image.height,graph_tempt,&Bitinfo,DIB_RGB_COLORS,SRCCOPY);
	delete[] graph_tempt;
}

void CImageView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//   	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
// 	CProcessView *pProcessView_temp=(CProcessView*)pFrame->pProcessView;
// 	Invalidate(FALSE);
// 	//调用动态链接库中的处理算法实时显示处理效果
// 	typedef void (*IMAGEPROCESS) ( const Image_class &_image_input,Image_class &_image_output);
// 	FARPROC lpfn = NULL;
// 	HINSTANCE hinst = NULL;
// 	hinst = LoadLibrary(_T("ImageProcess.dll"));
// 	ASSERT(hinst!=NULL);
// 	lpfn = GetProcAddress(hinst,"ImageProcess");
// 	ASSERT(lpfn!=NULL);
// 	IMAGEPROCESS image_process = (IMAGEPROCESS) lpfn;
// 	(*image_process)(*((CSmartCarDoc*)m_pDocument)->Image_Complete,((CSmartCarDoc*)m_pDocument)->after_process);
// 	FreeLibrary(hinst);//释放动态链接库
// 	pProcessView_temp->Invalidate(FALSE);//更新视图
	CView::OnTimer(nIDEvent);
}
