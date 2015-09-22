
// SmartCarDoc.cpp : CSmartCarDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "SmartCar.h"
#endif

#include "SmartCarDoc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSmartCarDoc

IMPLEMENT_DYNCREATE(CSmartCarDoc, CDocument)

BEGIN_MESSAGE_MAP(CSmartCarDoc, CDocument)
//	ON_COMMAND(ID_FILE_OPEN, &CSmartCarDoc::OnFileOpen)
END_MESSAGE_MAP()


// CSmartCarDoc 构造/析构

CSmartCarDoc::CSmartCarDoc()
{
	// TODO: 在此添加一次性构造代码
	Image_Tempt=&image_buffer1;
	Image_Complete=&image_buffer2;
	IsFailedReadConfig = FALSE;
	LoadPlotConfig();
}

CSmartCarDoc::~CSmartCarDoc()
{
}

BOOL CSmartCarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CSmartCarDoc 序列化

void CSmartCarDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码

	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CSmartCarDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CSmartCarDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CSmartCarDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSmartCarDoc 诊断

#ifdef _DEBUG
void CSmartCarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSmartCarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSmartCarDoc 命令




void CSmartCarDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: 在此添加专用代码和/或调用基类
	CDocument::SetTitle(_T("第九届智能车摄像头上位机"));
}
//读取曲线的基本配置
void CSmartCarDoc::LoadPlotConfig()
{
	//细节问题以后再说，如果没有文件怎么办？先创建文件再默认初始化
	CFile file ;
	if(file.Open(_T ("abc.txt"), CFile::modeRead))//打开了文档
	{
		try
		{
			CArchive ar (&file, CArchive::load);
			ULONGLONG len = file.GetLength();
			for (int i=0;i<9;i++)
			{
				plot[i].Serialize(ar); 
			}
			ar.Flush();
			ar.Close();
		}
		catch (CArchiveException*)
		{
			//若反序列化失败
			AfxMessageBox(_T("未正确读取曲线配置信息，将采用默认配置"));
			IsFailedReadConfig = TRUE;
		}
		file.Close();
	}
	else//文档不存在，创建新文档
	{
		AfxMessageBox(_T("配置文件不存在，创建默认文件"));
		IsFailedReadConfig = TRUE;
	}
	
// 	CWavePlotDlg init;
// 	init.DoModal();
// 	init.saveconfig();
}
