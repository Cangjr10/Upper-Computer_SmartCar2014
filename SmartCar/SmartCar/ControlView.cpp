// ControlView.cpp : 实现文件
//

#include "stdafx.h"
#include "SmartCar.h"
#include "ControlView.h"
#include "SmartCarDoc.h"
#include "MainFrm.h"
#include "AxisConfigDlg.h"

IMPLEMENT_DYNCREATE(CControlView, CFormView)

CControlView::CControlView()
	: CFormView(CControlView::IDD)
	, m_nComNum(2)
	, m_nBaudrate(1)
	, m_nWidth(140)
	, m_nHeight(80)
	, m_nPortOpen(FALSE)
	,m_inFrame(FALSE)
	,m_datacount(0)
	, m_nThresh(0)
	,m_pHistogram(NULL)
	,m_inPlotData(0)
	,Initial_flag(FALSE)
	,clicked_flag(FALSE)
	, IsAxisFailed(FALSE)
{

}

CControlView::~CControlView()
{
	if (m_pHistogram!=NULL)
	{
		delete m_pHistogram;
	}
/*	delete[] s;*/
}

void CControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM, m_SCIModule);
	DDX_CBIndex(pDX, IDC_COMNUM, m_nComNum);
	DDX_CBIndex(pDX, IDC_BAUDRATE, m_nBaudrate);
	DDX_Control(pDX, IDC_COMNUM, m_ComNum);
	DDX_Control(pDX, IDC_BAUDRATE, m_Baudrate);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);

	DDX_Control(pDX, IDC_OPENCOM, m_PortSwitch);
	DDX_Control(pDX, IDC_BITMAPOPEN, m_BitmapOpen);
	DDX_Control(pDX, IDC_BITMAPSAVE, m_BitmapSave);
	DDX_Text(pDX, IDC_THRESH, m_nThresh);
	DDX_Control(pDX, IDC_PLOTDATA_TREE, m_PlotTree);
}

BEGIN_MESSAGE_MAP(CControlView, CFormView)
	ON_BN_CLICKED(IDC_OPENCOM, &CControlView::OnBnClickedOpencom)
	ON_BN_CLICKED(IDC_BITMAPOPEN, &CControlView::OnBnClickedBitmapopen)
	ON_BN_CLICKED(IDC_BINARY_BUTTON, &CControlView::OnBnClickedBinaryButton)
	ON_BN_CLICKED(IDC_HISTOGRAM_BUTTON, &CControlView::OnBnClickedHistogramButton)
	ON_BN_CLICKED(IDC_ONALGORITHM, &CControlView::OnBnClickedOnalgorithm)
	ON_BN_CLICKED(IDC_BITMAPSAVE, &CControlView::OnBnClickedBitmapsave)
	ON_BN_CLICKED(IDC_WAVECONFIG, &CControlView::OnBnClickedWaveconfig)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_PLOTDATA_TREE, &CControlView::OnSelchangedPlotdataTree)
	ON_NOTIFY(NM_KILLFOCUS, IDC_PLOTDATA_TREE, &CControlView::OnNMKillfocusPlotdataTree)
//	ON_NOTIFY(TVN_SINGLEEXPAND, IDC_PLOTDATA_TREE, &CControlView::OnTvnSingleExpandPlotdataTree)
	ON_NOTIFY(NM_CLICK, IDC_PLOTDATA_TREE, &CControlView::OnNMClickPlotdataTree)
	ON_NOTIFY(TVN_ITEMCHANGING, IDC_PLOTDATA_TREE, &CControlView::OnTvnItemChangingPlotdataTree)
	ON_NOTIFY(TVN_ITEMCHANGED, IDC_PLOTDATA_TREE, &CControlView::OnTvnItemChangedPlotdataTree)
	ON_BN_CLICKED(IDC_Axis, &CControlView::OnBnClickedAxis)
END_MESSAGE_MAP()


// CControlView 诊断

#ifdef _DEBUG
void CControlView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlView 消息处理程序
BEGIN_EVENTSINK_MAP(CControlView, CFormView)
	ON_EVENT(CControlView, IDC_MSCOMM, 1, CControlView::OnCommMscomm, VTS_NONE)
END_EVENTSINK_MAP()



void CControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//加载窗口信息
	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
	CPlotView *pPlotView_temp=(CPlotView*)pFrame->pPlotView;
	CFile file ;
	if(file.Open(_T ("Axis.txt"), CFile::modeRead))//打开了文档
	{
		try
		{
			CArchive ar (&file, CArchive::load);
			for (int i=0;i<3;i++)
			{
				pPlotView_temp->Window[i].Serialization(ar); 
			}
			ar.Flush();
			ar.Close();
		}
		catch (CArchiveException*)
		{
			//若反序列化失败
			AfxMessageBox(_T("未正确读取坐标，将采用默认配置"));
			IsAxisFailed = TRUE;
		}
		file.Close();
	}
	else//文档不存在，创建新文档
	{
		AfxMessageBox(_T("坐标配置文件不存在，创建默认文件"));
		IsAxisFailed = TRUE;
	}
	//若曲线信息文件加载失败，创建默认文件
	if (IsAxisFailed)
	{
		CAxisConfigDlg dlg_temp;
		dlg_temp.SaveAxisConfig();
	}
	if (((CSmartCarDoc*)m_pDocument)->IsFailedReadConfig)
	{
		CWavePlotDlg tem_dlg;
		tem_dlg.p_doc = (CSmartCarDoc*)m_pDocument;
		tem_dlg.saveconfig();
	}
	CString str;
	for (int i=1;i<25;i++)
	{
		str.Format(_T("COM%d"),i);
		m_ComNum.AddString(str);
	}
	str_baudrate.push_back(_T("4800"));
	str_baudrate.push_back(_T("9600"));
	str_baudrate.push_back(_T("19200"));
	str_baudrate.push_back(_T("38400"));
	str_baudrate.push_back(_T("57600"));
	str_baudrate.push_back(_T("115200"));//CString类型存储的是string变量的首地址，每个元素占四个字节
	for (int i=0;i < static_cast<int>(str_baudrate.size());i++)
	{
		m_Baudrate.AddString(str_baudrate[i]);
	}
	m_nBaudrate = str_baudrate.size()-1;
	UpdateData(FALSE);
	CSmartCarDoc * pDoc_temp=(CSmartCarDoc*)m_pDocument;
	//创建句柄时将曲线名称信息显示至树状图中
	hRoot = m_PlotTree.InsertItem(_T("WavePlot"), 0, 0); 
	hWindow[0] = m_PlotTree.InsertItem(_T("Window1"),  hRoot, TVI_LAST); 
	hWindow[1] = m_PlotTree.InsertItem(_T("Window2"),  hRoot, TVI_LAST);
	hWindow[2] = m_PlotTree.InsertItem(_T("Window3"),  hRoot, TVI_LAST);
	hPlot[0]=m_PlotTree.InsertItem(pDoc_temp->plot[0].GetTitle(), hWindow[0], TVI_LAST); 
	hPlot[1]=m_PlotTree.InsertItem(pDoc_temp->plot[1].GetTitle(), hWindow[0], TVI_LAST);
	hPlot[2]=m_PlotTree.InsertItem(pDoc_temp->plot[2].GetTitle(), hWindow[0], TVI_LAST);
	hPlot[3]=m_PlotTree.InsertItem(pDoc_temp->plot[3].GetTitle(), hWindow[1], TVI_LAST); 
	hPlot[4]=m_PlotTree.InsertItem(pDoc_temp->plot[4].GetTitle(), hWindow[1], TVI_LAST);
	hPlot[5]=m_PlotTree.InsertItem(pDoc_temp->plot[5].GetTitle(), hWindow[1], TVI_LAST);
	hPlot[6]=m_PlotTree.InsertItem(pDoc_temp->plot[6].GetTitle(), hWindow[2], TVI_LAST);
	hPlot[7]=m_PlotTree.InsertItem(pDoc_temp->plot[7].GetTitle(), hWindow[2], TVI_LAST);
	hPlot[8]=m_PlotTree.InsertItem(pDoc_temp->plot[8].GetTitle(), hWindow[2], TVI_LAST); 
	m_PlotTree.Expand(hRoot,TVE_EXPAND);
	m_PlotTree.Expand(hWindow[0],TVE_EXPAND);
	m_PlotTree.Expand(hWindow[1],TVE_EXPAND);
	m_PlotTree.Expand(hWindow[2],TVE_EXPAND);
	m_PlotTree.SetCheck(hRoot);
	for (int i=0;i<9;i++)
	{
		if(pDoc_temp->plot[i].GetShowFlag())
		{
			m_PlotTree.SetCheck(hPlot[i]);
		}
	}
	for (int i=0;i<3;i++)
	{
		if(pPlotView_temp->Window[i].GetShowFlag())
			m_PlotTree.SetCheck(hWindow[i]);
	}
	Initial_flag = TRUE;

//	OnBnClickedOpencom();//一开始就打开串口
	// TODO: 在此添加专用代码和/或调用基类

	
}


void CControlView::OnBnClickedOpencom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString Com_Config=_T("");
	Com_Config += str_baudrate[m_nBaudrate];
	Com_Config += _T(",n,8,1");
	m_nPortOpen=m_SCIModule.get_PortOpen();
	if (m_nPortOpen == FALSE)
	{
		m_SCIModule.put__CommPort(m_nComNum+1);//选择com口
		m_SCIModule.put_InputMode(1);//输入方式为二进制方式
		m_SCIModule.put_InBufferSize(1024);//输入缓冲区大小为1024byte
		m_SCIModule.put_OutBufferSize(512);//输出缓冲区大小为512byte
		m_SCIModule.put_Settings(Com_Config);//设置串口参数：9600波特率，无奇偶校验，8个数据位，1个停止位
		if(!m_SCIModule.get_PortOpen() )//获取串口的当前状态
		{
			m_SCIModule.put_PortOpen(1);//打开串口
			m_nPortOpen = TRUE;
		}
		m_SCIModule.put_RThreshold(1);//每当串口接收缓冲区有多余或等于1个字符时将引发一个接收数据的oncomm事件
		m_SCIModule.put_InputLen(0);//设置当前接收区数据长度为0
		m_SCIModule.get_Input();//预读缓冲区以清空残留数据
		SetDlgItemTextW(IDC_OPENCOM,_T("关闭串口"));
		m_BitmapOpen.EnableWindow(FALSE);
		m_BitmapSave.EnableWindow(FALSE);
		CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
		CPlotView *pPlotView_temp=(CPlotView*)pFrame->pPlotView;
		CImageView *pImageView_temp=(CImageView*)pFrame->pImageView;
//		pImageView_temp->SetTimer(0, 200, NULL);
//		pPlotView_temp->SetTimer(0,200,NULL);
		//定义图像尺寸
		((CSmartCarDoc*)m_pDocument)->image_buffer1.image_data.resize(m_nWidth*m_nHeight,255);
		((CSmartCarDoc*)m_pDocument)->image_buffer1.width=m_nWidth;
		((CSmartCarDoc*)m_pDocument)->image_buffer1.height=m_nHeight;
		((CSmartCarDoc*)m_pDocument)->image_buffer2.image_data.resize(m_nWidth*m_nHeight,255);
		((CSmartCarDoc*)m_pDocument)->image_buffer2.width=m_nWidth;
		((CSmartCarDoc*)m_pDocument)->image_buffer2.height=m_nHeight;
		((CSmartCarDoc*)m_pDocument)->Image_Tempt=&((CSmartCarDoc*)m_pDocument)->image_buffer1;
		((CSmartCarDoc*)m_pDocument)->Image_Complete=&((CSmartCarDoc*)m_pDocument)->image_buffer2;
	}
	else
	{
		m_SCIModule.put_PortOpen(0);//关闭串口
		m_nPortOpen = FALSE;
		CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
		CPlotView *pPlotView_temp=(CPlotView*)pFrame->pPlotView;
		CImageView *pImageView_temp=(CImageView*)pFrame->pImageView;
//		pImageView_temp->KillTimer(0);//关闭刷新视图定时器
//		pPlotView_temp->KillTimer(0);//关闭刷新视图定时器
		m_BitmapOpen.EnableWindow(TRUE);
		m_BitmapSave.EnableWindow(TRUE);
		SetDlgItemTextW(IDC_OPENCOM,_T("打开串口"));
	}

}



void CControlView::OnCommMscomm()
{
	// TODO: 在此处添加消息处理程序代码
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len;
	if(m_SCIModule.get_CommEvent()==2)//事件2表示接受缓冲区有字符
	{
		//接受摄像头图像数据
		variant_inp=m_SCIModule.get_Input();//读缓冲区
		safearray_inp=variant_inp;//variant数据转换成colesafearray型变量
		len = safearray_inp.GetOneDimSize();		//数据的有效长度
		for (long k = 0; k < len; k++)
		{
			//图像接收模块
			if (m_inFrame && !m_inPlotData)//正在采集数据
			{
				BYTE temp;
				safearray_inp.GetElement(&k, &temp);
				//容错处理，数组不能越界
				if (m_datacount < (((CSmartCarDoc*)m_pDocument)->Image_Tempt->image_data.size()))
				{
					((CSmartCarDoc*)m_pDocument)->Image_Tempt->image_data[m_datacount]=temp;
				}				
				m_datacount++;
				//一帧的采集结束
				if (m_datacount == m_nWidth*m_nHeight)
				{
					SwapImageBuffers();
					m_datacount = 0;
					m_inFrame = FALSE;
				}
			}
			else if(!m_inPlotData)//没有其他操作，等待下一帧图像开始
			{
				BYTE config = 0;
				safearray_inp.GetElement(&k, &config);
				// 平移校验位
				if (config == 1)
				{
					m_config = m_config << 1;
				} 
				else if (config == 254)
				{
					m_config = (m_config << 1) + 1;
				}
				else
				{
					m_config = 0;
				}
				//校验位
				if (m_config == 0x55)
				{
					m_datacount = 0;
					m_config = 0;
					m_inFrame = TRUE;
				}
			}
			//PlotView数据接收
			if (m_inPlotData && !m_inFrame)//正在采集数据
			{
				BYTE receive_tempt;
				safearray_inp.GetElement(&k, &receive_tempt);
				int RightNumber=0;//寻找正确的曲线编号
				for (int i=0;i<9;i++)
				{
					if (((CSmartCarDoc*)m_pDocument)->plot[i].GetDoorStatus()==1)
					{
						RightNumber = i;
						break;
					}
				}
				if(((CSmartCarDoc*)m_pDocument)->plot[RightNumber].GetDoorStatus()==1 && ((CSmartCarDoc*)m_pDocument)->plot[RightNumber].decode(receive_tempt))
				{
					for (int j=0;j<9;j++)
						((CSmartCarDoc*)m_pDocument)->plot[j].ClearALL();//采集完毕，清除所有信息
					m_inPlotData=FALSE;
					//更新PlotView视图
					CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
					CPlotView* pPlotView_tempt=(CPlotView*)pMain->pPlotView;
					pPlotView_tempt->Invalidate(FALSE);//这种方法效率比较高，不用全部重新绘图
				//	pPlotView_tempt->SetTimer(0,1000,NULL);
				}
 			}
			else if(!m_inFrame) //没有外部操作，等待下一帧数据开始
			{
				BYTE receive_tempt;
				safearray_inp.GetElement(&k, &receive_tempt);
				//检测曲线标志是否到达，到达则开启接收数据模式
				for (int i=0;i<9;i++)
				{
					if (((CSmartCarDoc*)m_pDocument)->plot[i].IsDoorOpen(receive_tempt))
					{
						m_inPlotData=TRUE;
						break;
					}
				}
			}
		}
	}		
}

void CControlView::SwapImageBuffers()//交换缓存并更新图像
{
	Image_class* tempt = ((CSmartCarDoc*)m_pDocument)->Image_Tempt;
	((CSmartCarDoc*)m_pDocument)->Image_Tempt = ((CSmartCarDoc*)m_pDocument)->Image_Complete;
	((CSmartCarDoc*)m_pDocument)->Image_Complete = tempt;
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CImageView* pPImageView_tempt=(CImageView*)pMain->pImageView;
	pPImageView_tempt->Invalidate(FALSE);//更新原始图像
	OnBnClickedOnalgorithm();//调用图像处理算法，实时显示处理效果

}

void CControlView::OnBnClickedConfigpicok()
{
	// TODO: 在此添加控件通知处理程序代码
}





void CControlView::OnBnClickedBitmapopen()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog BitmapOpen(
		TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"数据文件 (*.bmp) |*.bmp|所有文件 (*.*)|*.*||",AfxGetMainWnd());
	CString strPath=_T(""); 
	if (BitmapOpen.DoModal() == IDOK)
	{
 		strPath=BitmapOpen.GetPathName();
 		CFile file(strPath,CFile::modeReadWrite);
 		ULONGLONG len = file.GetLength();
		//获取位图大小信息
		BITMAPINFO Bitinfo;//定义位图信息
		file.Seek(14,CFile::begin);
		memset(&Bitinfo,0,sizeof(BITMAPINFO));
		file.Read(&Bitinfo,sizeof(BITMAPINFO));
		UINT x=Bitinfo.bmiHeader.biWidth;
		UINT y=Bitinfo.bmiHeader.biHeight;
		m_nWidth=x;
		m_nHeight=y;
		UpdateData(FALSE);
		int linewidth=(x+3)/4*4;
		((CSmartCarDoc*)m_pDocument)->image_buffer3.width=x;
		((CSmartCarDoc*)m_pDocument)->image_buffer3.height=y;
		//获取位图数据信息
 		file.Seek(54+256*4,CFile::begin);
 		BYTE* m_pBmpData = new BYTE[static_cast<UINT>(len-54-256*4)];
 		file.Read(m_pBmpData,static_cast<UINT>(len-54-256*4));//跳过信息头与颜色表
		((CSmartCarDoc*)m_pDocument)->image_buffer3.image_data.resize(x*y);
		((CSmartCarDoc*)m_pDocument)->Image_Complete=&((CSmartCarDoc*)m_pDocument)->image_buffer3;
		for (unsigned int i=0;i<y;i++)
		{
			for (UINT j=0;j<x;j++)
			{
				((CSmartCarDoc*)m_pDocument)->image_buffer3.image_data[x*(y-1-i)+j]=m_pBmpData[linewidth*i+j];
			}
		}	
 		delete[] m_pBmpData;
		CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
		CImageView* pPImageView_tempt=(CImageView*)pMain->pImageView;
		pPImageView_tempt->Invalidate(FALSE);//更新原始图像
	//	GetDocument()->UpdateAllViews(this);
	}
}


void CControlView::OnBnClickedBinaryButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UINT WIDTH = ((CSmartCarDoc*)m_pDocument)->Image_Complete->width;
	UINT HEIGHT = ((CSmartCarDoc*)m_pDocument)->Image_Complete->height;
	((CSmartCarDoc*)m_pDocument)->after_process = *((CSmartCarDoc*)m_pDocument)->Image_Complete;
	for (UINT i=0;i<WIDTH*HEIGHT;i++)
		((CSmartCarDoc*)m_pDocument)->after_process.image_data[i] = ((CSmartCarDoc*)m_pDocument)->Image_Complete->image_data[i] > m_nThresh ? 255 : 0;
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CProcessView* pProcessView_tempt=(CProcessView*)pMain->pProcessView;
	pProcessView_tempt->Invalidate(FALSE);
//	GetDocument()->UpdateAllViews(this);
}


void CControlView::OnBnClickedHistogramButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL != m_pHistogram)
	{
		delete m_pHistogram;
		m_pHistogram=NULL;
	}
	if (NULL == m_pHistogram)
	{
		m_pHistogram = new CHistogramDlg();
		m_pHistogram -> Create(IDD_HISTOGRAM_DIALOG,this);
	}
	m_pHistogram->ShowWindow(SW_SHOW);
}


void CControlView::OnBnClickedOnalgorithm()
{
	// TODO: 在此添加控件通知处理程序代码
	//若图像有更改则更新变化的部分
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CProcessView* pProcessView_tempt=(CProcessView*)pMain->pProcessView;
	pProcessView_tempt->OnImageprocess();
}


void CControlView::OnBnClickedBitmapsave()
{
	// TODO: 在此添加控件通知处理程序代码
	//打开保存对话框
	CFileDialog dlg(
		FALSE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"位图文件 (*.bmp) |*.bmp|数据文件 (*.txt)|*.txt||",AfxGetMainWnd());
	CString strPath=_T(""); 
	if (dlg.DoModal() == IDOK)
	{
		strPath = dlg.GetPathName();
		int a = dlg.m_ofn.nFilterIndex;
		if (a==1 && strPath.Right(4) != ".bmp")
			strPath += ".bmp";	
		if (a==2 && strPath.Right(4) != ".txt")
			strPath += ".txt";	
	}
	else	
	{
		return;
	}

	if(dlg.m_ofn.nFilterIndex == 1)//保存成BMP格式
	{
		//建立CFile对象准备保存图片
		CFile file;
		if(!file.Open(strPath,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
		{
			AfxMessageBox(_T("Can not write BMP file."));
			return;
		}
		//生成图像的信息头（包括位图文件头和位图信息）
		//位图文件头占14字节，位图信息头占40字节
		//灰度图具有调色板，所以图像的偏移为54+256*4字节
		UpdateData(TRUE);
		BITMAPFILEHEADER bf;
		bf.bfOffBits=54+256*4;
		bf.bfReserved1=0;
		bf.bfReserved2=0;
		//说明文件大小（位图文件头+位图信息头+图片像素所占字节数）
		bf.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+m_nHeight*m_nWidth;
		//说明文件类型为BM
		bf.bfType=((WORD)'M'<<8|'B');
		//定义位图信息头
		BITMAPINFOHEADER bi;
		//定义图像深度为8
		bi.biBitCount=8;
		//说明图像显示有重要影响的颜色索引的数目，如果是0，表示都重要
		bi.biClrImportant=0;
		//说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）
		bi.biClrUsed=0;
		//说明图像数据压缩的类型
		bi.biCompression=0L;
		//说明图像的宽度，以像素为单位
		bi.biWidth=m_nWidth;
		//说明图像的高度，以像素为单位
		bi.biHeight=m_nHeight;
		//目标设备说明位面数，其值总是被设为1
		bi.biPlanes=1;
		//说明BITMAPINFOHEADER结构所需要的字节数
		bi.biSize=sizeof(BITMAPINFOHEADER);
		//说明图像的大小，以字节为单位。当用BI_RGB格式时，可设置为0
		bi.biSizeImage=(m_nWidth+3)/4*4*m_nHeight*8;
		//说明图像的水平分辨率，用像素/米来表示
		bi.biXPelsPerMeter=0;
		//说明图像的垂直分辨率，用像素/米来表示
		bi.biYPelsPerMeter=0;
		//写入位图文件头
		file.Write(&bf,14);
		//写入位图信息头
		file.Write(&bi,sizeof(BITMAPINFOHEADER));
		//写入颜色表
		RGBQUAD _rgb[256];
		for (int i=0; i<256; i++)
		{
			_rgb[i].rgbBlue=_rgb[i].rgbGreen=_rgb[i].rgbRed=static_cast<BYTE>(i);
			_rgb[i].rgbReserved = 0;
		}																	
		RGBQUAD *rgb_quad = _rgb;
		file.Write(rgb_quad,256*sizeof(RGBQUAD));
		//获得图像每一行所占的字节数
		long lWidthBytes=(m_nWidth*8+31)/32*4;
		//写入整个的像素矩阵中的数据
		BYTE* p_bmpData = new BYTE[lWidthBytes*m_nHeight](); 
		if(((CSmartCarDoc*)m_pDocument)->Image_Complete->height > 0)
		{
			for (UINT i=0;i<m_nHeight;i++)
			{
				for (UINT j=0;j<m_nWidth;j++)
				{
					p_bmpData[lWidthBytes*(m_nHeight-1-i)+j]=((CSmartCarDoc*)m_pDocument)->Image_Complete->image_data[m_nWidth*i+j];
				}
			}
		}

		file.Write(p_bmpData,m_nHeight*lWidthBytes);
		//写入完毕，关闭文件
		file.Close();
		delete[] p_bmpData;
	}
	else//保存成TXT格式
	{
		UpdateData(TRUE);
		//建立CFile对象准备保存图片
		CStdioFile file;
		if(!file.Open(strPath,CFile::modeCreate|CFile::modeWrite))//|CFile::typeText
		{
			AfxMessageBox(_T("Can not write TXT file."));
			return;
		}
		BYTE* p_bmpData = new BYTE[m_nWidth*m_nHeight](); 
		CString str;
		if (((CSmartCarDoc*)m_pDocument)->Image_Complete->height > 0)
		{
			str.Format(_T("WIDTH%d,HEIGHT%d"),m_nWidth,m_nHeight);
			str +="\r\n";
			file.WriteString(str);
			for (UINT i=0; i<m_nWidth*m_nHeight; i++)
			{
				p_bmpData[i]=((CSmartCarDoc*)m_pDocument)->Image_Complete->image_data[i];
				str.Format(_T("%d"),static_cast<int>(((CSmartCarDoc*)m_pDocument)->Image_Complete->image_data[i]));
				str +="\r\n";
				file.WriteString(str);
			}
		}	

		file.Close();
		delete[] p_bmpData;
	}

}


void CControlView::OnBnClickedWaveconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CWavePlotDlg WaveDlg;
	WaveDlg.p_doc=(CSmartCarDoc*)m_pDocument;
	if(WaveDlg.DoModal() == IDOK)
	{
		//将曲线信息保存成文件以供别处调用
		WaveDlg.saveconfig();
		CSmartCarDoc * pDoc_temp=(CSmartCarDoc*)m_pDocument;
		m_PlotTree.SetItemText(hPlot[0],pDoc_temp->plot[0].GetTitle());
		m_PlotTree.SetItemText(hPlot[1],pDoc_temp->plot[1].GetTitle());
		m_PlotTree.SetItemText(hPlot[2],pDoc_temp->plot[2].GetTitle());
		m_PlotTree.SetItemText(hPlot[3],pDoc_temp->plot[3].GetTitle());
		m_PlotTree.SetItemText(hPlot[4],pDoc_temp->plot[4].GetTitle());
		m_PlotTree.SetItemText(hPlot[5],pDoc_temp->plot[5].GetTitle());
		m_PlotTree.SetItemText(hPlot[6],pDoc_temp->plot[6].GetTitle());
		m_PlotTree.SetItemText(hPlot[7],pDoc_temp->plot[7].GetTitle());
		m_PlotTree.SetItemText(hPlot[8],pDoc_temp->plot[8].GetTitle());
	}								  
	
}


//void CControlView::OnSelchangedPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CControlView::OnNMKillfocusPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CSmartCarDoc * pDoc_temp=(CSmartCarDoc*)m_pDocument;
	UpdateData(TRUE);
	for (int i=0;i<9;i++)
	{
		pDoc_temp->plot[i].SetShowFlag(m_PlotTree.GetCheck(hPlot[i]));
	}
	//序列化保存配置信息
	CFile file (_T ("abc.txt"), CFile::modeCreate | CFile::modeWrite);
	CArchive ar (&file, CArchive::store);
	for (int i=0;i<9;i++)
	{
		pDoc_temp->plot[i].Serialize(ar); 
	}
	ar.Flush();
	//读完毕，关闭文件流
	ar.Close();
	file.Close();
	*pResult = 0;
}


//void CControlView::OnTvnSingleExpandPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CControlView::OnNMClickPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	UpdateData(TRUE);
	clicked_flag = TRUE;
	CSmartCarDoc * pDoc_temp=(CSmartCarDoc*)m_pDocument;
	for (int i=0;i<9;i++)
	{
		BOOL hehe = m_PlotTree.GetCheck(hPlot[i]);
		pDoc_temp->plot[i].SetShowFlag(m_PlotTree.GetCheck(hPlot[i]));
	}
	//序列化保存配置信息
	CFile file (_T ("abc.txt"), CFile::modeCreate | CFile::modeWrite);
	CArchive ar (&file, CArchive::store);
	for (int i=0;i<9;i++)
	{
		pDoc_temp->plot[i].Serialize(ar); 
	}
	ar.Flush();
	//读完毕，关闭文件流
	ar.Close();
	file.Close();
	*pResult = 0;
}


void CControlView::OnTvnItemChangingPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE *>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}


void CControlView::OnTvnItemChangedPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	
	if (Initial_flag && clicked_flag)//防止bug
	{
		CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
		CPlotView* pPlotView_tempt=(CPlotView*)pMain->pPlotView;
		CSmartCarDoc * pDoc_temp=(CSmartCarDoc*)m_pDocument;
		for (int i=0; i<3; i++)
			pPlotView_tempt->Window[i].SetShowFlag(m_PlotTree.GetCheck(hWindow[i]));
		for (int i=0;i<9;i++)
			pDoc_temp->plot[i].SetShowFlag(m_PlotTree.GetCheck(hPlot[i]));

		//序列化保存配置信息
		CFile file (_T ("abc.txt"), CFile::modeCreate | CFile::modeWrite);
		CArchive ar (&file, CArchive::store);
		for (int i=0;i<9;i++)
		{
			pDoc_temp->plot[i].Serialize(ar); 
		}
		ar.Flush();
		//读完毕，关闭文件流
		ar.Close();
		file.Close();
		//保存窗口坐标轴信息
		CFile file1 (_T ("Axis.txt"), CFile::modeCreate | CFile::modeWrite);
		CArchive ar1 (&file1, CArchive::store);
		for (int i=0;i<3;i++)
		{
			pPlotView_tempt->Window[i].Serialization(ar1); 
		}
		ar1.Flush();
		//读完毕，关闭文件流
		ar1.Close();
		file1.Close();
		clicked_flag = FALSE;
		//更新视图

		pPlotView_tempt->Invalidate(FALSE);
	}
	
	*pResult = 0;
}


void CControlView::OnBnClickedAxis()
{
	// TODO: 在此添加控件通知处理程序代码
	CAxisConfigDlg Axis;
	if (Axis.DoModal()==IDOK)
	{
		Axis.SaveAxisConfig();
 		CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
 		CPlotView* pPlotView_tempt=(CPlotView*)pMain->pPlotView;
		pPlotView_tempt->Invalidate(FALSE);

	}
	
}
