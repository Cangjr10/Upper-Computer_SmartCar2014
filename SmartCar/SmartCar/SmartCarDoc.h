
// SmartCarDoc.h : CSmartCarDoc 类的接口
//


#pragma once
#include "..\ImageProcess\ImageProcess.h"
#include "WavePlotDlg.h"
// #include  <vector>
// using std::vector;
// typedef struct //保存图像信息结构体
// {
// 	vector<BYTE> image_data;
// 	UINT width;
// 	UINT height;
// 	vector<BYTE> image_color;//用于返回需要显示的颜色信息
// }Image_class;
/*class CWavePlotDlg;*/
class CSmartCarDoc : public CDocument
{
	friend class CWavePlotDlg;
protected: // 仅从序列化创建
	CSmartCarDoc();
	DECLARE_DYNCREATE(CSmartCarDoc)

// 特性
public:
	void LoadPlotConfig();//从文件中调用曲线配置信息
	Image_class image_buffer1;//用于从串口接受图像数据，图像大小m_nWidth*m_nHeight
	Image_class image_buffer2;//用于从串口接受图像数据，图像大小m_nWidth*m_nHeight
	Image_class* Image_Tempt;//用于图像采集过程的指针
	Image_class* Image_Complete;//用于指向采集完成的图像指针
	Image_class image_buffer3;//从内存中调取图像数据
	Image_class after_process;//显示处理后的图像，ProcessView中显示
	//PlotView中数据
	WavePlot plot[9];//定义9条曲线
	BOOL IsFailedReadConfig;//判断文件加载是否正确
private:
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CSmartCarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
//	afx_msg void OnFileOpen();
	virtual void SetTitle(LPCTSTR lpszTitle);
};
