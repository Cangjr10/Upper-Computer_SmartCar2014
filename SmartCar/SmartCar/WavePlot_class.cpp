#include "stdafx.h"
#include "WavePlot_class.h"
#include <cassert>
//�������ã�������ʼ��־
//˼·����һ������Ҫ����Կ�ף���������Կ���������ģ���θ�����֪���õ���ȷ��Կ�ף�����ȷ����ʵ��־��
BOOL WavePlot::IsDoorOpen(BYTE NewReceivedData)
{
	assert(!DoorOpen && Key.size()<3);//��û����ȥ���ţ����ſ��˾ͱ��ٿ���
	if (Key.size()==KeyNumber)//�Ѿ����㹻��Կ�ף����򲻿�����Ҫ����һ��Կ��
	{
		Key[0]=Key[1];
		Key[1]=NewReceivedData;
		if (Key[0]==RightKey[0] && Key[1]==RightKey[1])//Կ��ƥ��
		{
			DoorOpen = TRUE;
			return TRUE;
		}
		else
			return FALSE;
	}
	else if (Key.size()<KeyNumber)//Կ����Ŀ������������ȡԿ��
	{
		Key.push_back(NewReceivedData);
		if (Key.size()==KeyNumber)//����һ��Կ��֮��Կ����Ŀ�㹻�������Ƿ�ƥ��
		{
			if (Key[0]==RightKey[0] && Key[1]==RightKey[1])//Կ��ƥ��
			{
				DoorOpen = TRUE;
				return TRUE;//Կ����ȷ������
			}
			else
				return FALSE;//Կ�ײ���ȷ
		}
		else
			return FALSE;//����һ��Կ��Կ�ף���Ŀ���ɲ���������
	}
	else
		return FALSE;
}

BOOL WavePlot::decode(BYTE NewReceivedData)
{
	assert(DoorOpen);//ȷ�����п��ŵģ��������ݽ���
	ByteStream.push_back(NewReceivedData);
	if (ByteStream.size()==DataNumber*BytesPerData)//���ݽ�����ȫ
	{
		DoorOpen = FALSE;//�����Ѿ�ȫ��������ϣ����ſ�ʼ��������
		switch(data_style)
		{
		case s_BYTE:
			for (UINT i=0;i<DataNumber;i++)
				plotdata_byte.push_back(ByteStream[i]);
			break;
		case s_INT16:
			for (UINT i=0;i<DataNumber;i++)
			{
				INT16 a = 0;
				char* hehe = (char*)&a;
				*hehe = ByteStream[2*i+0];
				*(hehe+1)=ByteStream[2*i+1];
				plotdata_int16.push_back(a);
			}
			break;
		case s_UINT16:
			for (UINT i=0;i<DataNumber;i++)
			{
				UINT16 a = 0;
				char* hehe = (char*)&a;
				*hehe = ByteStream[2*i+0];
				*(hehe+1)=ByteStream[2*i+1];
				plotdata_uint16.push_back(a);
			}
			break;
		case s_FLOAT:
			for (UINT i=0;i<DataNumber;i++)
			{
				float a = 0;
				char* hehe = (char*)&a;
				*hehe = ByteStream[4*i+0];
				*(hehe+1)=ByteStream[4*i+1];
				*(hehe+2) = ByteStream[4*i+2];
				*(hehe+3)=ByteStream[4*i+3];
				plotdata_float.push_back(a);
			}
			break;
		default:
			break;
		}
		ByteStream.resize(0);//���������ݽ����ջ���������
		return TRUE;
	}
	return FALSE;
}
//���ý�����ȫ���ó���ֻ���ó���Ҫ��ʾ��x_range��������
// void WavePlot::getdata(vector<BYTE> &_plotdata_byte)
// {
// 	assert(data_style == s_BYTE);
// 	if (x_range < plotdata_byte.size())//���ݱ���ʾ��Ҫ�࣬ȡ������Ŀ��ʾ
// 	{
// 		_plotdata_byte.resize(x_range);
// 		for (UINT i=0;i<_plotdata_byte.size();i++)
// 		{
// 			_plotdata_byte[i]=plotdata_byte[plotdata_byte.size()-x_range+i];
// 		}
// 	}
// 	else//������Ŀ����
// 	{
// 		_plotdata_byte.resize(plotdata_byte.size());
// 		for (UINT i=0;i<_plotdata_byte.size();i++)
// 		{
// 			_plotdata_byte[i]=plotdata_byte[i];
// 		}
// 	}
// }
// void WavePlot::getdata(vector<INT16> &_plotdata_int16)
// {
// 	assert(data_style == s_INT16);
// 	
// 	if (x_range<plotdata_int16.size())//���ݱ���ʾ��Ҫ�࣬ȡ������Ŀ��ʾ
// 	{
// 		_plotdata_int16.resize(x_range);//������0����
// 		for (UINT i=0;i<_plotdata_int16.size();i++)
// 		{
// 			_plotdata_int16[i]=plotdata_int16[plotdata_int16.size()-x_range+i];
// 		}
// 	}
// 	else//������Ŀ����
// 	{
// 		_plotdata_int16.resize(plotdata_int16.size());//������0����
// 		for (UINT i=0;i<_plotdata_int16.size();i++)
// 		{
// 			_plotdata_int16[i]=plotdata_int16[i];
// 		}
// 	}
// }
// void WavePlot::getdata(vector<UINT16> &_plotdata_uint16)
// {
// 	assert(data_style == s_UINT16);
// 	if (x_range<plotdata_uint16.size())//���ݱ���ʾ��Ҫ�࣬ȡ������Ŀ��ʾ
// 	{
// 		_plotdata_uint16.resize(x_range);//ֻȡx_range��
// 		for (UINT i=0;i<_plotdata_uint16.size();i++)
// 		{
// 			_plotdata_uint16[i]=plotdata_uint16[plotdata_uint16.size()-x_range+i];
// 		}
// 	}
// 	else//������Ŀ����
// 	{
// 		_plotdata_uint16.resize(plotdata_uint16.size());
// 		for (UINT i=0;i<_plotdata_uint16.size();i++)
// 		{
// 			_plotdata_uint16[i]=plotdata_uint16[i];
// 		}
// 	}
// 
// }
// void WavePlot::getdata(vector<float> &_plotdata_float)
// {
// 	assert(data_style == s_FLOAT);
// 	if (x_range<plotdata_float.size())//���ݱ���ʾ��Ҫ�࣬ȡ������Ŀ��ʾ
// 	{
// 		_plotdata_float.resize(x_range);
// 		for (UINT i=0;i<_plotdata_float.size();i++)
// 		{
// 			_plotdata_float[i]=plotdata_float[plotdata_float.size()-x_range+i];
// 		}
// 	}
// 	else//������Ŀ����
// 	{
// 		_plotdata_float.resize(plotdata_float.size());
// 		for (UINT i=0;i<_plotdata_float.size();i++)
// 		{
// 			_plotdata_float[i]=plotdata_float[i];
// 		}
// 	}
// }
	/************************************************************************/
	/* ��ȡ��������Ĵ�С                                                                     */
	/************************************************************************/
UINT WavePlot::GetDataSize()
{
	switch(data_style)
	{
	case s_BYTE:
		return plotdata_byte.size();
	case s_INT16:
		return plotdata_int16.size();
	case s_UINT16:
		return plotdata_uint16.size();
	case s_FLOAT:
		return plotdata_float.size();
	default:
		return 0;
	}
}

	/************************************************************************/
	/* ��ȡ���ݣ��洢��ͳһ����PlotData�У������ͼ��ͳһ����                                                                     */
	/************************************************************************/
void WavePlot::GetPlotData(vector<float> &_PlotData,UINT x_range)
{
	UINT data_size = GetDataSize();
	//��_PlotData�����ڴ�ռ�
	if (data_size < x_range)//���ݸ�������
		_PlotData.resize(data_size);
	else
		_PlotData.resize(x_range);
	//��������
	switch(data_style)
	{
	case s_BYTE:
		for (UINT index = 0;index<_PlotData.size();index++)//get the last few elements
			_PlotData[index] = static_cast<float>(plotdata_byte[data_size-_PlotData.size()+index]);
		break;
	case s_INT16:
		for (UINT index = 0;index<_PlotData.size();index++)//get the last few elements
			_PlotData[index] = static_cast<float>(plotdata_int16[data_size-_PlotData.size()+index]);
		break;
	case s_UINT16:
		for (UINT index = 0;index<_PlotData.size();index++)//get the last few elements
			_PlotData[index] = static_cast<float>(plotdata_uint16[data_size-_PlotData.size()+index]);
		break;
	case s_FLOAT:
		for (UINT index = 0;index<_PlotData.size();index++)//get the last few elements
			_PlotData[index] = plotdata_float[data_size-_PlotData.size()+index];
		break;
	default:
		break;
	}

	
}



IMPLEMENT_SERIAL (WavePlot, CObject, 1)

void WavePlot::Serialize (CArchive & ar)
{
	CObject::Serialize (ar);
	if (ar.IsStoring ())
	{
		int a=0;
		switch(data_style)
		{
		case s_NULL:
			a = 0 ;
			ar << a ;break;
		case s_BYTE:
			a = 1 ;
			ar << a ;break;
		case s_INT16:
			a = 2 ;
			ar << a ;break;
		case s_UINT16:
			a = 3 ;
			ar << a ;break;
		case s_FLOAT:
			a = 4 ;
			ar << a ;break;
		default:
			break;
		}
		ar<<IsShow;
		ar<<BytesPerData;
		ar<<DataNumber;
		ar<<title;
		assert(RightKey.size()==2);
		ar<<RightKey[0];
		ar<<RightKey[1];

		
	}	
	else
	{
		int a=0;
		ar>>a;
		switch(a)
		{
		case 0:
			data_style=s_NULL;break;
		case 1:
			data_style=s_BYTE;break;
		case 2:
			data_style=s_INT16;break;
		case 3:
			data_style=s_UINT16;break;
		case 4:
			data_style=s_FLOAT;break;
		default:
			break;
		}
		ar>>IsShow;
		ar>>BytesPerData;
		ar>>DataNumber;
		ar>>title;
		RightKey.resize(2);
		ar>>RightKey[0];
		ar>>RightKey[1];
	}
}
