// WavePlotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SmartCar.h"
#include "WavePlotDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CWavePlotDlg 对话框

IMPLEMENT_DYNAMIC(CWavePlotDlg, CDialogEx)

CWavePlotDlg::CWavePlotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWavePlotDlg::IDD, pParent)
	, m_W1Name1(_T("plot1_1"))
	, m_W1Name2(_T("plot1_2"))
	, m_W1Name3(_T("plot1_3"))
	, m_W2Name1(_T("plot2_1"))
	, m_W2Name2(_T("plot2_3"))
	, m_W2Name3(_T("plot2_3"))
	, m_W3Name1(_T("plot3_1"))
	, m_W3Name2(_T("plot3_2"))
	, m_W3Name3(_T("plot3_3"))
	, m_W1DataNum2(1)
	, m_W1DataNum1(1)
	, m_W1DataNum3(1)
	, m_W2DataNum1(1)
	, m_W2DataNum2(1)
	, m_W2DataNum3(1)
	, m_W3DataNum1(1)
	, m_W3DataNum2(1)
	, m_W3DataNum3(1)
	, m_W1Start1(0xFF)
	, m_W1Start2(0xFE)
	, m_W1Start3(0xFD)
	, m_W2Start1(0xFC)
	, m_W2Start2(0xFB)
	, m_W2Start3(0xFA)
	, m_W3Start1(0xF9)
	, m_W3Start2(0xF8)
	, m_W3Start3(0xF7)
	, m_nW1Style1(0)
	, m_nW1Style2(0)
	, m_nW1Style3(0)
	, m_nW2Style1(0)
	, m_nW2Style2(0)
	, m_nW2Style3(0)
	, m_nW3Style1(0)
	, m_nW3Style2(0)
	, m_nW3Style3(0)
	, m_W1Start1_1(0)
	, m_W1Start2_1(0)
	, m_W1Start3_1(0)
	, m_W2Start1_1(0)
	, m_W2Start2_1(0)
	, m_W2Start3_1(0)
	, m_W3Start1_1(0)
	, m_W3Start2_1(0)
	, m_W3Start3_1(0)
{
	
}

CWavePlotDlg::~CWavePlotDlg()
{
}

void CWavePlotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_W1STYLE1, m_W1Style1);
	DDX_Text(pDX, IDC_W1NAME1, m_W1Name1);
	DDX_Text(pDX, IDC_W1NAME2, m_W1Name2);
	DDX_Text(pDX, IDC_W1NAME3, m_W1Name3);
	DDX_Text(pDX, IDC_W2NAME1, m_W2Name1);
	DDX_Text(pDX, IDC_W2NAME2, m_W2Name2);
	DDX_Text(pDX, IDC_W2NAME3, m_W2Name3);
	DDX_Text(pDX, IDC_W3NAME1, m_W3Name1);
	DDX_Text(pDX, IDC_W3NAME2, m_W3Name2);
	DDX_Text(pDX, IDC_W3NAME3, m_W3Name3);
	DDX_Control(pDX, IDC_W1STYLE2, m_W1Style2);
	DDX_Control(pDX, IDC_W1STYLE3, m_W1Style3);
	DDX_Control(pDX, IDC_W2STYLE1, m_W2Style1);
	DDX_Control(pDX, IDC_W2STYLE2, m_W2Style2);
	DDX_Control(pDX, IDC_W2STYLE3, m_W2Style3);
	DDX_Control(pDX, IDC_W3STYLE1, m_W3Style1);
	DDX_Control(pDX, IDC_W3STYLE2, m_W3Style2);
	DDX_Control(pDX, IDC_W3STYLE3, m_W3Style3);
	//  DDX_Control(pDX, IDC_W1DATACNT1, m_W1DataNum1);
	DDX_Text(pDX, IDC_W1DATACNT2, m_W1DataNum2);
	DDX_Text(pDX, IDC_W1DATACNT1, m_W1DataNum1);
	DDX_Text(pDX, IDC_W1DATACNT3, m_W1DataNum3);
	DDX_Text(pDX, IDC_W2DATACNT1, m_W2DataNum1);
	DDX_Text(pDX, IDC_W2DATACNT2, m_W2DataNum2);
	DDX_Text(pDX, IDC_W2DATACNT3, m_W2DataNum3);
	DDX_Text(pDX, IDC_W3DATACNT1, m_W3DataNum1);
	DDX_Text(pDX, IDC_W3DATACNT2, m_W3DataNum2);
	DDX_Text(pDX, IDC_W3DATACNT3, m_W3DataNum3);
	DDX_Text(pDX, IDC_W1START1, m_W1Start1);
	DDX_Text(pDX, IDC_W1START2, m_W1Start2);
	DDX_Text(pDX, IDC_W1START3, m_W1Start3);
	DDX_Text(pDX, IDC_W2START1, m_W2Start1);
	DDX_Text(pDX, IDC_W2START2, m_W2Start2);
	DDX_Text(pDX, IDC_W2START3, m_W2Start3);
	DDX_Text(pDX, IDC_W3START1, m_W3Start1);
	DDX_Text(pDX, IDC_W3START2, m_W3Start2);
	DDX_Text(pDX, IDC_W3START3, m_W3Start3);
	DDX_CBIndex(pDX, IDC_W1STYLE1, m_nW1Style1);
	DDX_CBIndex(pDX, IDC_W1STYLE2, m_nW1Style2);
	DDX_CBIndex(pDX, IDC_W1STYLE3, m_nW1Style3);
	DDX_CBIndex(pDX, IDC_W2STYLE1, m_nW2Style1);
	DDX_CBIndex(pDX, IDC_W2STYLE2, m_nW2Style2);
	DDX_CBIndex(pDX, IDC_W2STYLE3, m_nW2Style3);
	DDX_CBIndex(pDX, IDC_W3STYLE1, m_nW3Style1);
	DDX_CBIndex(pDX, IDC_W3STYLE2, m_nW3Style2);
	DDX_CBIndex(pDX, IDC_W3STYLE3, m_nW3Style3);
	//  DDX_Text(pDX, IDC_W1START1_1, m_W1Start1_1);
	DDX_Text(pDX, IDC_W1START1_1, m_W1Start1_1);
	DDX_Text(pDX, IDC_W1START2_1, m_W1Start2_1);
	DDX_Text(pDX, IDC_W1START3_1, m_W1Start3_1);
	DDX_Text(pDX, IDC_W2START1_1, m_W2Start1_1);
	DDX_Text(pDX, IDC_W2START2_1, m_W2Start2_1);
	DDX_Text(pDX, IDC_W2START3_1, m_W2Start3_1);
	DDX_Text(pDX, IDC_W3START1_1, m_W3Start1_1);
	DDX_Text(pDX, IDC_W3START2_1, m_W3Start2_1);
	DDX_Text(pDX, IDC_W3START3_1, m_W3Start3_1);
}


BEGIN_MESSAGE_MAP(CWavePlotDlg, CDialogEx)
END_MESSAGE_MAP()


// CWavePlotDlg 消息处理程序


BOOL CWavePlotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString str[4];
	str[0].Format(_T("BYTE"));
	str[1].Format(_T("INT16"));
	str[2].Format(_T("UINT16"));
	str[3].Format(_T("FLOAT"));
	for (int i=0;i<4;i++)
	{
		m_W1Style1.AddString(str[i]);
		m_W1Style2.AddString(str[i]);
		m_W1Style3.AddString(str[i]);
		m_W2Style1.AddString(str[i]);
		m_W2Style2.AddString(str[i]);
		m_W2Style3.AddString(str[i]);
		m_W3Style1.AddString(str[i]);
		m_W3Style2.AddString(str[i]);
		m_W3Style3.AddString(str[i]);
	}
	loadconfig();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CWavePlotDlg::loadconfig()
{
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CControlView* pView=(CControlView*)pMain->GetActiveView(); 
	p_doc = ((CSmartCarDoc*)pView->GetDocument());
	m_W1Name1=p_doc->plot[0].GetTitle();
	m_W1Name2=p_doc->plot[1].GetTitle();
	m_W1Name3=p_doc->plot[2].GetTitle();
	m_W2Name1=p_doc->plot[3].GetTitle();
	m_W2Name2=p_doc->plot[4].GetTitle();
	m_W2Name3=p_doc->plot[5].GetTitle();
	m_W3Name1=p_doc->plot[6].GetTitle();
	m_W3Name2=p_doc->plot[7].GetTitle();
	m_W3Name3=p_doc->plot[8].GetTitle();
	m_W1DataNum1=p_doc->plot[0].GetDataNumber();
	m_W1DataNum2=p_doc->plot[1].GetDataNumber();
	m_W1DataNum3=p_doc->plot[2].GetDataNumber();
	m_W2DataNum1=p_doc->plot[3].GetDataNumber();
	m_W2DataNum2=p_doc->plot[4].GetDataNumber();
	m_W2DataNum3=p_doc->plot[5].GetDataNumber();
	m_W3DataNum1=p_doc->plot[6].GetDataNumber();
	m_W3DataNum2=p_doc->plot[7].GetDataNumber();
	m_W3DataNum3=p_doc->plot[8].GetDataNumber();
	m_W1Start1=p_doc->plot[0].GetRightKey()[0];
	m_W1Start2=p_doc->plot[1].GetRightKey()[0];
	m_W1Start3=p_doc->plot[2].GetRightKey()[0];
	m_W2Start1=p_doc->plot[3].GetRightKey()[0];
	m_W2Start2=p_doc->plot[4].GetRightKey()[0];
	m_W2Start3=p_doc->plot[5].GetRightKey()[0];
	m_W3Start1=p_doc->plot[6].GetRightKey()[0];
	m_W3Start2=p_doc->plot[7].GetRightKey()[0];
	m_W3Start3=p_doc->plot[8].GetRightKey()[0];
	m_nW1Style1=(int)(p_doc->plot[0].GetDataStyle()-1);
	m_nW1Style2=(int)(p_doc->plot[1].GetDataStyle()-1);
	m_nW1Style3=(int)(p_doc->plot[2].GetDataStyle()-1);
	m_nW2Style1=(int)(p_doc->plot[3].GetDataStyle()-1);
	m_nW2Style2=(int)(p_doc->plot[4].GetDataStyle()-1);
	m_nW2Style3=(int)(p_doc->plot[5].GetDataStyle()-1);
	m_nW3Style1=(int)(p_doc->plot[6].GetDataStyle()-1);
	m_nW3Style2=(int)(p_doc->plot[7].GetDataStyle()-1);
	m_nW3Style3=(int)(p_doc->plot[8].GetDataStyle()-1);
	m_W1Start1_1=p_doc->plot[0].GetRightKey()[1];
	m_W1Start2_1=p_doc->plot[1].GetRightKey()[1];
	m_W1Start3_1=p_doc->plot[2].GetRightKey()[1];
	m_W2Start1_1=p_doc->plot[3].GetRightKey()[1];
	m_W2Start2_1=p_doc->plot[4].GetRightKey()[1];
	m_W2Start3_1=p_doc->plot[5].GetRightKey()[1];
	m_W3Start1_1=p_doc->plot[6].GetRightKey()[1];
	m_W3Start2_1=p_doc->plot[7].GetRightKey()[1];
	m_W3Start3_1=p_doc->plot[8].GetRightKey()[1];
}
void CWavePlotDlg::saveconfig()
{
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CControlView* pView=(CControlView*)pMain->GetActiveView(); 
	p_doc = ((CSmartCarDoc*)pView->GetDocument());
	//设置标题
	p_doc->plot[0].SetTitle(m_W1Name1);
	p_doc->plot[1].SetTitle(m_W1Name2);
	p_doc->plot[2].SetTitle(m_W1Name3);
	p_doc->plot[3].SetTitle(m_W2Name1);
	p_doc->plot[4].SetTitle(m_W2Name2);
	p_doc->plot[5].SetTitle(m_W2Name3);
	p_doc->plot[6].SetTitle(m_W3Name1);
	p_doc->plot[7].SetTitle(m_W3Name2);
	p_doc->plot[8].SetTitle(m_W3Name3);
	//设置数据类型
	p_doc->plot[0].SetDataStyle((DATASTYLE)(m_nW1Style1+1));
	p_doc->plot[1].SetDataStyle((DATASTYLE)(m_nW1Style2+1));
	p_doc->plot[2].SetDataStyle((DATASTYLE)(m_nW1Style3+1));
	p_doc->plot[3].SetDataStyle((DATASTYLE)(m_nW2Style1+1));
	p_doc->plot[4].SetDataStyle((DATASTYLE)(m_nW2Style2+1));
	p_doc->plot[5].SetDataStyle((DATASTYLE)(m_nW2Style3+1));
	p_doc->plot[6].SetDataStyle((DATASTYLE)(m_nW3Style1+1));
	p_doc->plot[7].SetDataStyle((DATASTYLE)(m_nW3Style2+1));
	p_doc->plot[8].SetDataStyle((DATASTYLE)(m_nW3Style3+1));
	//设置接收数目
	p_doc->plot[0].SetDataNumber(m_W1DataNum1);
	p_doc->plot[1].SetDataNumber(m_W1DataNum2);
	p_doc->plot[2].SetDataNumber(m_W1DataNum3);
	p_doc->plot[3].SetDataNumber(m_W2DataNum1);
	p_doc->plot[4].SetDataNumber(m_W2DataNum2);
	p_doc->plot[5].SetDataNumber(m_W2DataNum3);
	p_doc->plot[6].SetDataNumber(m_W3DataNum1);
	p_doc->plot[7].SetDataNumber(m_W3DataNum2);
	p_doc->plot[8].SetDataNumber(m_W3DataNum3);
	//设置起始位
	vector<BYTE>key;
	key.resize(0);
	key.push_back(m_W1Start1);
	key.push_back(m_W1Start1_1);
	p_doc->plot[0].SetRightKey(key);
	key.resize(0);
	key.push_back(m_W1Start2);
	key.push_back(m_W1Start2_1);
	p_doc->plot[1].SetRightKey(key);
	key.resize(0);
	key.push_back(m_W1Start3);
	key.push_back(m_W1Start3_1);
	p_doc->plot[2].SetRightKey(key);
	key.resize(0);
	key.push_back(m_W2Start1);
	key.push_back(m_W2Start1_1);
	p_doc->plot[3].SetRightKey(key);
	key.resize(0);
	key.push_back(m_W2Start2);
	key.push_back(m_W2Start2_1);
	p_doc->plot[4].SetRightKey(key);
	key.resize(0);
	key.push_back(m_W2Start3);
	key.push_back(m_W2Start3_1);
	p_doc->plot[5].SetRightKey(key);
	key.resize(0);
	key.push_back(m_W3Start1);
	key.push_back(m_W3Start1_1);
	p_doc->plot[6].SetRightKey(key);
	key.resize(0);
	key.push_back(m_W3Start2);
	key.push_back(m_W3Start2_1);
	p_doc->plot[7].SetRightKey(key);
	key.resize(0);
	key.push_back(m_W3Start3);
	key.push_back(m_W3Start3_1);
	p_doc->plot[8].SetRightKey(key);
	//序列化保存配置信息
	CFile file (_T ("abc.txt"), CFile::modeCreate | CFile::modeWrite);
	CArchive ar (&file, CArchive::store);
	for (int i=0;i<9;i++)
	{
		p_doc->plot[i].Serialize(ar); 
	}
	ar.Flush();
	//读完毕，关闭文件流
	ar.Close();
	file.Close();
}