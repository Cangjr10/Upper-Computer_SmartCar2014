
// SmartCarDoc.h : CSmartCarDoc ��Ľӿ�
//


#pragma once
#include "..\ImageProcess\ImageProcess.h"
#include "WavePlotDlg.h"
// #include  <vector>
// using std::vector;
// typedef struct //����ͼ����Ϣ�ṹ��
// {
// 	vector<BYTE> image_data;
// 	UINT width;
// 	UINT height;
// 	vector<BYTE> image_color;//���ڷ�����Ҫ��ʾ����ɫ��Ϣ
// }Image_class;
/*class CWavePlotDlg;*/
class CSmartCarDoc : public CDocument
{
	friend class CWavePlotDlg;
protected: // �������л�����
	CSmartCarDoc();
	DECLARE_DYNCREATE(CSmartCarDoc)

// ����
public:
	void LoadPlotConfig();//���ļ��е�������������Ϣ
	Image_class image_buffer1;//���ڴӴ��ڽ���ͼ�����ݣ�ͼ���Сm_nWidth*m_nHeight
	Image_class image_buffer2;//���ڴӴ��ڽ���ͼ�����ݣ�ͼ���Сm_nWidth*m_nHeight
	Image_class* Image_Tempt;//����ͼ��ɼ����̵�ָ��
	Image_class* Image_Complete;//����ָ��ɼ���ɵ�ͼ��ָ��
	Image_class image_buffer3;//���ڴ��е�ȡͼ������
	Image_class after_process;//��ʾ������ͼ��ProcessView����ʾ
	//PlotView������
	WavePlot plot[9];//����9������
	BOOL IsFailedReadConfig;//�ж��ļ������Ƿ���ȷ
private:
// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CSmartCarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
//	afx_msg void OnFileOpen();
	virtual void SetTitle(LPCTSTR lpszTitle);
};
