
// ImageView.cpp : CImageView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CImageView ����/����

CImageView::CImageView()
{
	// TODO: �ڴ˴���ӹ������

}

CImageView::~CImageView()
{
}

BOOL CImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CImageView ����

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
	
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CImageView ��ӡ


void CImageView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CImageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CImageView ���

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CView::AssertValid();
}

void CImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSmartCarDoc* CImageView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSmartCarDoc)));
	return (CSmartCarDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageView ��Ϣ�������
void CImageView::Plot_Image(const Image_class _image)
{
	BITMAPINFO Bitinfo;//����λͼ��Ϣ
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
	//����RGB���ݴ洢�ռ�
	BYTE * graph_tempt=new BYTE [Bitinfo.bmiHeader.biSizeImage];
	memset(graph_tempt,0,Bitinfo.bmiHeader.biSizeImage);
	//�Ҷ�ͼ��RGB��ɫͼת��
	int width_adjust=(_image.width*24+31)/32*4;//��֤ÿ���ֽ���Ϊ4�ı���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//   	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
// 	CProcessView *pProcessView_temp=(CProcessView*)pFrame->pProcessView;
// 	Invalidate(FALSE);
// 	//���ö�̬���ӿ��еĴ����㷨ʵʱ��ʾ����Ч��
// 	typedef void (*IMAGEPROCESS) ( const Image_class &_image_input,Image_class &_image_output);
// 	FARPROC lpfn = NULL;
// 	HINSTANCE hinst = NULL;
// 	hinst = LoadLibrary(_T("ImageProcess.dll"));
// 	ASSERT(hinst!=NULL);
// 	lpfn = GetProcAddress(hinst,"ImageProcess");
// 	ASSERT(lpfn!=NULL);
// 	IMAGEPROCESS image_process = (IMAGEPROCESS) lpfn;
// 	(*image_process)(*((CSmartCarDoc*)m_pDocument)->Image_Complete,((CSmartCarDoc*)m_pDocument)->after_process);
// 	FreeLibrary(hinst);//�ͷŶ�̬���ӿ�
// 	pProcessView_temp->Invalidate(FALSE);//������ͼ
	CView::OnTimer(nIDEvent);
}
