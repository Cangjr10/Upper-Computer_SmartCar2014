// ProcessView.cpp : 实现文件
//

#include "stdafx.h"
#include "SmartCar.h"
#include "ProcessView.h"
#include "MainFrm.h"
#include "SmartCarDoc.h"
/*#include "..\ImageProcess\ImageProcess.h"*/
//extern "C" __declspec(dllexport) void ImageProcess(const Image_class image_input,Image_class &image_output);

// CProcessView

IMPLEMENT_DYNCREATE(CProcessView, CView)

CProcessView::CProcessView()
{

}

CProcessView::~CProcessView()
{
}

BEGIN_MESSAGE_MAP(CProcessView, CView)
	ON_COMMAND(ID_ImageProcess, &CProcessView::OnImageprocess)
END_MESSAGE_MAP()


// CProcessView 绘图

void CProcessView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
// 	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
// 	CImageView *peView=(CImageView*)pFrame->pImageView;
	Plot_Image(((CSmartCarDoc*)m_pDocument)->after_process);
}


// CProcessView 诊断

#ifdef _DEBUG
void CProcessView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CProcessView 消息处理程序
void CProcessView::Plot_Image(const Image_class _image)//注意颜色信息的显示
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
	if (_image.image_color.size()==_image.image_data.size())
	{
		for (unsigned int line=0;line < _image.height;line++)
		{
			for (unsigned int col=0;col<_image.width;col++)
			{
				if (_image.image_color[(_image.height-line-1)*_image.width+col]==1)
				{
					graph_tempt[width_adjust*line+3*col]=0;
					graph_tempt[width_adjust*line+3*col+1]=0;
					graph_tempt[width_adjust*line+3*col+2]=255;
				}
			}
		}
	}
	

	CRect rect;
	GetClientRect(&rect);
	StretchDIBits(GetDC()->m_hDC,0,0,(rect.right-rect.left),(rect.bottom-rect.top),0,0,_image.width,_image.height,graph_tempt,&Bitinfo,DIB_RGB_COLORS,SRCCOPY);
	delete[] graph_tempt;
}

void CProcessView::OnImageprocess()
{
	// TODO: 在此添加命令处理程序代码
// 	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
// 	CProcessView *pProcessView_temp=(CProcessView*)pFrame->pProcessView;
//	Invalidate(FALSE);
	//调用动态链接库中的处理算法实时显示处理效果
	typedef void (*IMAGEPROCESS) ( const Image_class &_image_input,Image_class &_image_output);
	FARPROC lpfn = NULL;
	HINSTANCE hinst = NULL;
	hinst = LoadLibrary(_T("ImageProcess.dll"));
	ASSERT(hinst!=NULL);
	if(hinst!=NULL)
	{
		lpfn = GetProcAddress(hinst,"ImageProcess");
		ASSERT(lpfn!=NULL);
		IMAGEPROCESS image_process = (IMAGEPROCESS) lpfn;
		(*image_process)(*((CSmartCarDoc*)m_pDocument)->Image_Complete,((CSmartCarDoc*)m_pDocument)->after_process);
	}
	else
	{
		MessageBox(_T("找不到DLL文件！"));
	}
	
	FreeLibrary(hinst);//释放动态链接库
	Invalidate(FALSE);//更新视图
}
