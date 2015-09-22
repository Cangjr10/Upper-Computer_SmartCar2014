
// SmartCarDoc.cpp : CSmartCarDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CSmartCarDoc ����/����

CSmartCarDoc::CSmartCarDoc()
{
	// TODO: �ڴ����һ���Թ������
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

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSmartCarDoc ���л�

void CSmartCarDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����

	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CSmartCarDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CSmartCarDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CSmartCarDoc ���

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


// CSmartCarDoc ����




void CSmartCarDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: �ڴ����ר�ô����/����û���
	CDocument::SetTitle(_T("�ھŽ����ܳ�����ͷ��λ��"));
}
//��ȡ���ߵĻ�������
void CSmartCarDoc::LoadPlotConfig()
{
	//ϸ�������Ժ���˵�����û���ļ���ô�죿�ȴ����ļ���Ĭ�ϳ�ʼ��
	CFile file ;
	if(file.Open(_T ("abc.txt"), CFile::modeRead))//�����ĵ�
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
			//�������л�ʧ��
			AfxMessageBox(_T("δ��ȷ��ȡ����������Ϣ��������Ĭ������"));
			IsFailedReadConfig = TRUE;
		}
		file.Close();
	}
	else//�ĵ������ڣ��������ĵ�
	{
		AfxMessageBox(_T("�����ļ������ڣ�����Ĭ���ļ�"));
		IsFailedReadConfig = TRUE;
	}
	
// 	CWavePlotDlg init;
// 	init.DoModal();
// 	init.saveconfig();
}
