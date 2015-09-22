//WavePlot_class.h
#pragma once
// #ifndef _WAVEPLOTCLASS_H
// #define _WAVEPLOTCLASS_H

// #ifndef __AFXWIN_H__
// #error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
// #endif
#include <cassert>
#include <vector>
using std::vector;
enum DATASTYLE{s_NULL, s_BYTE, s_INT16, s_UINT16, s_FLOAT};
class WavePlot : public CObject//为了实现串行化保存数据
{
	//friend class CSmartCarDoc;
	DECLARE_SERIAL (WavePlot);

public:
	WavePlot()
	{
		data_style = s_NULL;
		KeyNumber=2;
		DoorOpen=FALSE;
		IsShow = FALSE;
		DataNumber=0;
		BytesPerData=0;
	}
	//画图相关
// 	void SetXrange(UINT x=100) {x_range = x;}//设置横坐标范围
// 	UINT GetXrange() {return x_range; }//设置横坐标范围
// 	void SetYrange(long _y1=0, long _y2=255){y1=_y1;y2=_y2;}//设置纵坐标范围
/*	void GetData(void* p_data);//返回最近的需要显示的数据*/
	//针对不同数据类型，重载获取数据函数
// 	void getdata(vector<BYTE> &_plotdata_byte);
// 	void getdata(vector<INT16> &_plotdata_int16);
// 	void getdata(vector<UINT16> &_plotdata_uint16);
// 	void getdata(vector<float> &_plotdata_float);
	/************************************************************************/
	/* 尝试用float类型的数去存储所有的数据，但会造成一定内存的开销                                                                     */
	/************************************************************************/
	void GetPlotData(vector<float> &_PlotData,UINT x_range);
	UINT GetDataSize();
	//曲线识别
	void SetTitle(CString _title){title = _title;}
	CString GetTitle(){return title;}
	BOOL GetShowFlag(){return IsShow;}
	void SetShowFlag(BOOL flag){IsShow=flag;}
	void SetDataStyle(DATASTYLE _data_style)
	{
		data_style=_data_style;
		switch(data_style)
		{
		case s_NULL:BytesPerData=0;break;
		case s_BYTE:BytesPerData=1;break;
		case s_INT16:BytesPerData=2;break;
		case s_UINT16:BytesPerData=2;break;
		case s_FLOAT:BytesPerData=4;break;
		}
	}
	DATASTYLE GetDataStyle(){return data_style;}
	void SetDataNumber(UINT _datanumber){DataNumber=_datanumber;}
	UINT GetDataNumber() {return DataNumber;}
	void SetRightKey(vector<BYTE> _key) {assert(_key.size()==2); RightKey = _key;}
	vector<BYTE> GetRightKey(){return RightKey;}
	BOOL IsDoorOpen(BYTE NewReceivedData);//判断是否拿到正确的钥匙
	BOOL GetDoorStatus(){return DoorOpen;}
	BOOL decode(BYTE NewReceivedData);//开门之后进行数据的解帧操作，接受完数据将门关上
	void ClearALL()//清空所有标志
	{
		DoorOpen=FALSE;
		Key.resize(0);
		ByteStream.resize(0);
	}
	void Serialize (CArchive & ar);
private:
	CString title;
	DATASTYLE data_style;
	BOOL IsShow;//保存曲线是否显示标志
// 	UINT x_range;//横坐标显示的数据点数
// 	long y1,y2;//纵坐标的范围为y1~y2;
	vector<BYTE> Key;//打开一扇门需要两把钥匙，此处为拿到的钥匙
	UINT KeyNumber;//钥匙数目
	BOOL DoorOpen;
	vector<BYTE> RightKey;//正确的两把钥匙
	UINT DataNumber;//接收的数据个数
	UINT BytesPerData;//一个数据所占字节数
	//每接收BytesPerData个字节存储一个数据，共接收DataCount*BytesPerData个字节后退出
	vector<BYTE> ByteStream;//用于接收数据流管理
	//用于接受数据存储
	vector<BYTE> plotdata_byte;
	vector<INT16> plotdata_int16;
	vector<UINT16> plotdata_uint16;
	vector<float> plotdata_float;
};

/*#endif*/


