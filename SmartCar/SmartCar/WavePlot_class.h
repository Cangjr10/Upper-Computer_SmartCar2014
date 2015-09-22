//WavePlot_class.h
#pragma once
// #ifndef _WAVEPLOTCLASS_H
// #define _WAVEPLOTCLASS_H

// #ifndef __AFXWIN_H__
// #error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
// #endif
#include <cassert>
#include <vector>
using std::vector;
enum DATASTYLE{s_NULL, s_BYTE, s_INT16, s_UINT16, s_FLOAT};
class WavePlot : public CObject//Ϊ��ʵ�ִ��л���������
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
	//��ͼ���
// 	void SetXrange(UINT x=100) {x_range = x;}//���ú����귶Χ
// 	UINT GetXrange() {return x_range; }//���ú����귶Χ
// 	void SetYrange(long _y1=0, long _y2=255){y1=_y1;y2=_y2;}//���������귶Χ
/*	void GetData(void* p_data);//�����������Ҫ��ʾ������*/
	//��Բ�ͬ�������ͣ����ػ�ȡ���ݺ���
// 	void getdata(vector<BYTE> &_plotdata_byte);
// 	void getdata(vector<INT16> &_plotdata_int16);
// 	void getdata(vector<UINT16> &_plotdata_uint16);
// 	void getdata(vector<float> &_plotdata_float);
	/************************************************************************/
	/* ������float���͵���ȥ�洢���е����ݣ��������һ���ڴ�Ŀ���                                                                     */
	/************************************************************************/
	void GetPlotData(vector<float> &_PlotData,UINT x_range);
	UINT GetDataSize();
	//����ʶ��
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
	BOOL IsDoorOpen(BYTE NewReceivedData);//�ж��Ƿ��õ���ȷ��Կ��
	BOOL GetDoorStatus(){return DoorOpen;}
	BOOL decode(BYTE NewReceivedData);//����֮��������ݵĽ�֡���������������ݽ��Ź���
	void ClearALL()//������б�־
	{
		DoorOpen=FALSE;
		Key.resize(0);
		ByteStream.resize(0);
	}
	void Serialize (CArchive & ar);
private:
	CString title;
	DATASTYLE data_style;
	BOOL IsShow;//���������Ƿ���ʾ��־
// 	UINT x_range;//��������ʾ�����ݵ���
// 	long y1,y2;//������ķ�ΧΪy1~y2;
	vector<BYTE> Key;//��һ������Ҫ����Կ�ף��˴�Ϊ�õ���Կ��
	UINT KeyNumber;//Կ����Ŀ
	BOOL DoorOpen;
	vector<BYTE> RightKey;//��ȷ������Կ��
	UINT DataNumber;//���յ����ݸ���
	UINT BytesPerData;//һ��������ռ�ֽ���
	//ÿ����BytesPerData���ֽڴ洢һ�����ݣ�������DataCount*BytesPerData���ֽں��˳�
	vector<BYTE> ByteStream;//���ڽ�������������
	//���ڽ������ݴ洢
	vector<BYTE> plotdata_byte;
	vector<INT16> plotdata_int16;
	vector<UINT16> plotdata_uint16;
	vector<float> plotdata_float;
};

/*#endif*/


