// ControlView.cpp : ʵ���ļ�
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


// CControlView ���

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


// CControlView ��Ϣ�������
BEGIN_EVENTSINK_MAP(CControlView, CFormView)
	ON_EVENT(CControlView, IDC_MSCOMM, 1, CControlView::OnCommMscomm, VTS_NONE)
END_EVENTSINK_MAP()



void CControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//���ش�����Ϣ
	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
	CPlotView *pPlotView_temp=(CPlotView*)pFrame->pPlotView;
	CFile file ;
	if(file.Open(_T ("Axis.txt"), CFile::modeRead))//�����ĵ�
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
			//�������л�ʧ��
			AfxMessageBox(_T("δ��ȷ��ȡ���꣬������Ĭ������"));
			IsAxisFailed = TRUE;
		}
		file.Close();
	}
	else//�ĵ������ڣ��������ĵ�
	{
		AfxMessageBox(_T("���������ļ������ڣ�����Ĭ���ļ�"));
		IsAxisFailed = TRUE;
	}
	//��������Ϣ�ļ�����ʧ�ܣ�����Ĭ���ļ�
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
	str_baudrate.push_back(_T("115200"));//CString���ʹ洢����string�������׵�ַ��ÿ��Ԫ��ռ�ĸ��ֽ�
	for (int i=0;i < static_cast<int>(str_baudrate.size());i++)
	{
		m_Baudrate.AddString(str_baudrate[i]);
	}
	m_nBaudrate = str_baudrate.size()-1;
	UpdateData(FALSE);
	CSmartCarDoc * pDoc_temp=(CSmartCarDoc*)m_pDocument;
	//�������ʱ������������Ϣ��ʾ����״ͼ��
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

//	OnBnClickedOpencom();//һ��ʼ�ʹ򿪴���
	// TODO: �ڴ����ר�ô����/����û���

	
}


void CControlView::OnBnClickedOpencom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString Com_Config=_T("");
	Com_Config += str_baudrate[m_nBaudrate];
	Com_Config += _T(",n,8,1");
	m_nPortOpen=m_SCIModule.get_PortOpen();
	if (m_nPortOpen == FALSE)
	{
		m_SCIModule.put__CommPort(m_nComNum+1);//ѡ��com��
		m_SCIModule.put_InputMode(1);//���뷽ʽΪ�����Ʒ�ʽ
		m_SCIModule.put_InBufferSize(1024);//���뻺������СΪ1024byte
		m_SCIModule.put_OutBufferSize(512);//�����������СΪ512byte
		m_SCIModule.put_Settings(Com_Config);//���ô��ڲ�����9600�����ʣ�����żУ�飬8������λ��1��ֹͣλ
		if(!m_SCIModule.get_PortOpen() )//��ȡ���ڵĵ�ǰ״̬
		{
			m_SCIModule.put_PortOpen(1);//�򿪴���
			m_nPortOpen = TRUE;
		}
		m_SCIModule.put_RThreshold(1);//ÿ�����ڽ��ջ������ж�������1���ַ�ʱ������һ���������ݵ�oncomm�¼�
		m_SCIModule.put_InputLen(0);//���õ�ǰ���������ݳ���Ϊ0
		m_SCIModule.get_Input();//Ԥ������������ղ�������
		SetDlgItemTextW(IDC_OPENCOM,_T("�رմ���"));
		m_BitmapOpen.EnableWindow(FALSE);
		m_BitmapSave.EnableWindow(FALSE);
		CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
		CPlotView *pPlotView_temp=(CPlotView*)pFrame->pPlotView;
		CImageView *pImageView_temp=(CImageView*)pFrame->pImageView;
//		pImageView_temp->SetTimer(0, 200, NULL);
//		pPlotView_temp->SetTimer(0,200,NULL);
		//����ͼ��ߴ�
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
		m_SCIModule.put_PortOpen(0);//�رմ���
		m_nPortOpen = FALSE;
		CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
		CPlotView *pPlotView_temp=(CPlotView*)pFrame->pPlotView;
		CImageView *pImageView_temp=(CImageView*)pFrame->pImageView;
//		pImageView_temp->KillTimer(0);//�ر�ˢ����ͼ��ʱ��
//		pPlotView_temp->KillTimer(0);//�ر�ˢ����ͼ��ʱ��
		m_BitmapOpen.EnableWindow(TRUE);
		m_BitmapSave.EnableWindow(TRUE);
		SetDlgItemTextW(IDC_OPENCOM,_T("�򿪴���"));
	}

}



void CControlView::OnCommMscomm()
{
	// TODO: �ڴ˴������Ϣ����������
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len;
	if(m_SCIModule.get_CommEvent()==2)//�¼�2��ʾ���ܻ��������ַ�
	{
		//��������ͷͼ������
		variant_inp=m_SCIModule.get_Input();//��������
		safearray_inp=variant_inp;//variant����ת����colesafearray�ͱ���
		len = safearray_inp.GetOneDimSize();		//���ݵ���Ч����
		for (long k = 0; k < len; k++)
		{
			//ͼ�����ģ��
			if (m_inFrame && !m_inPlotData)//���ڲɼ�����
			{
				BYTE temp;
				safearray_inp.GetElement(&k, &temp);
				//�ݴ������鲻��Խ��
				if (m_datacount < (((CSmartCarDoc*)m_pDocument)->Image_Tempt->image_data.size()))
				{
					((CSmartCarDoc*)m_pDocument)->Image_Tempt->image_data[m_datacount]=temp;
				}				
				m_datacount++;
				//һ֡�Ĳɼ�����
				if (m_datacount == m_nWidth*m_nHeight)
				{
					SwapImageBuffers();
					m_datacount = 0;
					m_inFrame = FALSE;
				}
			}
			else if(!m_inPlotData)//û�������������ȴ���һ֡ͼ��ʼ
			{
				BYTE config = 0;
				safearray_inp.GetElement(&k, &config);
				// ƽ��У��λ
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
				//У��λ
				if (m_config == 0x55)
				{
					m_datacount = 0;
					m_config = 0;
					m_inFrame = TRUE;
				}
			}
			//PlotView���ݽ���
			if (m_inPlotData && !m_inFrame)//���ڲɼ�����
			{
				BYTE receive_tempt;
				safearray_inp.GetElement(&k, &receive_tempt);
				int RightNumber=0;//Ѱ����ȷ�����߱��
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
						((CSmartCarDoc*)m_pDocument)->plot[j].ClearALL();//�ɼ���ϣ����������Ϣ
					m_inPlotData=FALSE;
					//����PlotView��ͼ
					CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
					CPlotView* pPlotView_tempt=(CPlotView*)pMain->pPlotView;
					pPlotView_tempt->Invalidate(FALSE);//���ַ���Ч�ʱȽϸߣ�����ȫ�����»�ͼ
				//	pPlotView_tempt->SetTimer(0,1000,NULL);
				}
 			}
			else if(!m_inFrame) //û���ⲿ�������ȴ���һ֡���ݿ�ʼ
			{
				BYTE receive_tempt;
				safearray_inp.GetElement(&k, &receive_tempt);
				//������߱�־�Ƿ񵽴����������������ģʽ
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

void CControlView::SwapImageBuffers()//�������沢����ͼ��
{
	Image_class* tempt = ((CSmartCarDoc*)m_pDocument)->Image_Tempt;
	((CSmartCarDoc*)m_pDocument)->Image_Tempt = ((CSmartCarDoc*)m_pDocument)->Image_Complete;
	((CSmartCarDoc*)m_pDocument)->Image_Complete = tempt;
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CImageView* pPImageView_tempt=(CImageView*)pMain->pImageView;
	pPImageView_tempt->Invalidate(FALSE);//����ԭʼͼ��
	OnBnClickedOnalgorithm();//����ͼ�����㷨��ʵʱ��ʾ����Ч��

}

void CControlView::OnBnClickedConfigpicok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}





void CControlView::OnBnClickedBitmapopen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog BitmapOpen(
		TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"�����ļ� (*.bmp) |*.bmp|�����ļ� (*.*)|*.*||",AfxGetMainWnd());
	CString strPath=_T(""); 
	if (BitmapOpen.DoModal() == IDOK)
	{
 		strPath=BitmapOpen.GetPathName();
 		CFile file(strPath,CFile::modeReadWrite);
 		ULONGLONG len = file.GetLength();
		//��ȡλͼ��С��Ϣ
		BITMAPINFO Bitinfo;//����λͼ��Ϣ
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
		//��ȡλͼ������Ϣ
 		file.Seek(54+256*4,CFile::begin);
 		BYTE* m_pBmpData = new BYTE[static_cast<UINT>(len-54-256*4)];
 		file.Read(m_pBmpData,static_cast<UINT>(len-54-256*4));//������Ϣͷ����ɫ��
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
		pPImageView_tempt->Invalidate(FALSE);//����ԭʼͼ��
	//	GetDocument()->UpdateAllViews(this);
	}
}


void CControlView::OnBnClickedBinaryButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ͼ���и�������±仯�Ĳ���
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CProcessView* pProcessView_tempt=(CProcessView*)pMain->pProcessView;
	pProcessView_tempt->OnImageprocess();
}


void CControlView::OnBnClickedBitmapsave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�򿪱���Ի���
	CFileDialog dlg(
		FALSE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"λͼ�ļ� (*.bmp) |*.bmp|�����ļ� (*.txt)|*.txt||",AfxGetMainWnd());
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

	if(dlg.m_ofn.nFilterIndex == 1)//�����BMP��ʽ
	{
		//����CFile����׼������ͼƬ
		CFile file;
		if(!file.Open(strPath,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
		{
			AfxMessageBox(_T("Can not write BMP file."));
			return;
		}
		//����ͼ�����Ϣͷ������λͼ�ļ�ͷ��λͼ��Ϣ��
		//λͼ�ļ�ͷռ14�ֽڣ�λͼ��Ϣͷռ40�ֽ�
		//�Ҷ�ͼ���е�ɫ�壬����ͼ���ƫ��Ϊ54+256*4�ֽ�
		UpdateData(TRUE);
		BITMAPFILEHEADER bf;
		bf.bfOffBits=54+256*4;
		bf.bfReserved1=0;
		bf.bfReserved2=0;
		//˵���ļ���С��λͼ�ļ�ͷ+λͼ��Ϣͷ+ͼƬ������ռ�ֽ�����
		bf.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+m_nHeight*m_nWidth;
		//˵���ļ�����ΪBM
		bf.bfType=((WORD)'M'<<8|'B');
		//����λͼ��Ϣͷ
		BITMAPINFOHEADER bi;
		//����ͼ�����Ϊ8
		bi.biBitCount=8;
		//˵��ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ
		bi.biClrImportant=0;
		//˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ���
		bi.biClrUsed=0;
		//˵��ͼ������ѹ��������
		bi.biCompression=0L;
		//˵��ͼ��Ŀ�ȣ�������Ϊ��λ
		bi.biWidth=m_nWidth;
		//˵��ͼ��ĸ߶ȣ�������Ϊ��λ
		bi.biHeight=m_nHeight;
		//Ŀ���豸˵��λ��������ֵ���Ǳ���Ϊ1
		bi.biPlanes=1;
		//˵��BITMAPINFOHEADER�ṹ����Ҫ���ֽ���
		bi.biSize=sizeof(BITMAPINFOHEADER);
		//˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0
		bi.biSizeImage=(m_nWidth+3)/4*4*m_nHeight*8;
		//˵��ͼ���ˮƽ�ֱ��ʣ�������/������ʾ
		bi.biXPelsPerMeter=0;
		//˵��ͼ��Ĵ�ֱ�ֱ��ʣ�������/������ʾ
		bi.biYPelsPerMeter=0;
		//д��λͼ�ļ�ͷ
		file.Write(&bf,14);
		//д��λͼ��Ϣͷ
		file.Write(&bi,sizeof(BITMAPINFOHEADER));
		//д����ɫ��
		RGBQUAD _rgb[256];
		for (int i=0; i<256; i++)
		{
			_rgb[i].rgbBlue=_rgb[i].rgbGreen=_rgb[i].rgbRed=static_cast<BYTE>(i);
			_rgb[i].rgbReserved = 0;
		}																	
		RGBQUAD *rgb_quad = _rgb;
		file.Write(rgb_quad,256*sizeof(RGBQUAD));
		//���ͼ��ÿһ����ռ���ֽ���
		long lWidthBytes=(m_nWidth*8+31)/32*4;
		//д�����������ؾ����е�����
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
		//д����ϣ��ر��ļ�
		file.Close();
		delete[] p_bmpData;
	}
	else//�����TXT��ʽ
	{
		UpdateData(TRUE);
		//����CFile����׼������ͼƬ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWavePlotDlg WaveDlg;
	WaveDlg.p_doc=(CSmartCarDoc*)m_pDocument;
	if(WaveDlg.DoModal() == IDOK)
	{
		//��������Ϣ������ļ��Թ��𴦵���
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}


void CControlView::OnNMKillfocusPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSmartCarDoc * pDoc_temp=(CSmartCarDoc*)m_pDocument;
	UpdateData(TRUE);
	for (int i=0;i<9;i++)
	{
		pDoc_temp->plot[i].SetShowFlag(m_PlotTree.GetCheck(hPlot[i]));
	}
	//���л�����������Ϣ
	CFile file (_T ("abc.txt"), CFile::modeCreate | CFile::modeWrite);
	CArchive ar (&file, CArchive::store);
	for (int i=0;i<9;i++)
	{
		pDoc_temp->plot[i].Serialize(ar); 
	}
	ar.Flush();
	//����ϣ��ر��ļ���
	ar.Close();
	file.Close();
	*pResult = 0;
}


//void CControlView::OnTvnSingleExpandPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}


void CControlView::OnNMClickPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	UpdateData(TRUE);
	clicked_flag = TRUE;
	CSmartCarDoc * pDoc_temp=(CSmartCarDoc*)m_pDocument;
	for (int i=0;i<9;i++)
	{
		BOOL hehe = m_PlotTree.GetCheck(hPlot[i]);
		pDoc_temp->plot[i].SetShowFlag(m_PlotTree.GetCheck(hPlot[i]));
	}
	//���л�����������Ϣ
	CFile file (_T ("abc.txt"), CFile::modeCreate | CFile::modeWrite);
	CArchive ar (&file, CArchive::store);
	for (int i=0;i<9;i++)
	{
		pDoc_temp->plot[i].Serialize(ar); 
	}
	ar.Flush();
	//����ϣ��ر��ļ���
	ar.Close();
	file.Close();
	*pResult = 0;
}


void CControlView::OnTvnItemChangingPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE *>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}


void CControlView::OnTvnItemChangedPlotdataTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(TRUE);
	
	if (Initial_flag && clicked_flag)//��ֹbug
	{
		CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
		CPlotView* pPlotView_tempt=(CPlotView*)pMain->pPlotView;
		CSmartCarDoc * pDoc_temp=(CSmartCarDoc*)m_pDocument;
		for (int i=0; i<3; i++)
			pPlotView_tempt->Window[i].SetShowFlag(m_PlotTree.GetCheck(hWindow[i]));
		for (int i=0;i<9;i++)
			pDoc_temp->plot[i].SetShowFlag(m_PlotTree.GetCheck(hPlot[i]));

		//���л�����������Ϣ
		CFile file (_T ("abc.txt"), CFile::modeCreate | CFile::modeWrite);
		CArchive ar (&file, CArchive::store);
		for (int i=0;i<9;i++)
		{
			pDoc_temp->plot[i].Serialize(ar); 
		}
		ar.Flush();
		//����ϣ��ر��ļ���
		ar.Close();
		file.Close();
		//���洰����������Ϣ
		CFile file1 (_T ("Axis.txt"), CFile::modeCreate | CFile::modeWrite);
		CArchive ar1 (&file1, CArchive::store);
		for (int i=0;i<3;i++)
		{
			pPlotView_tempt->Window[i].Serialization(ar1); 
		}
		ar1.Flush();
		//����ϣ��ر��ļ���
		ar1.Close();
		file1.Close();
		clicked_flag = FALSE;
		//������ͼ

		pPlotView_tempt->Invalidate(FALSE);
	}
	
	*pResult = 0;
}


void CControlView::OnBnClickedAxis()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAxisConfigDlg Axis;
	if (Axis.DoModal()==IDOK)
	{
		Axis.SaveAxisConfig();
 		CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
 		CPlotView* pPlotView_tempt=(CPlotView*)pMain->pPlotView;
		pPlotView_tempt->Invalidate(FALSE);

	}
	
}
