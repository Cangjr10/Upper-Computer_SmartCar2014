// ImageProcess.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_BoderTATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PABoderCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_BoderTATE(AfxGetBodertaticModuleBodertate());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CImageProcessApp

BEGIN_MESSAGE_MAP(CImageProcessApp, CWinApp)
END_MESSAGE_MAP()


// CImageProcessApp ����

CImageProcessApp::CImageProcessApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CImageProcessApp ����

CImageProcessApp theApp;


// CImageProcessApp ��ʼ��

BOOL CImageProcessApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
extern "C" void ImageProcess( const Image_class &_image_input,Image_class &_image_output)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int WIDTH = _image_input.width;
	int HEIGHT = _image_input.height;
	_image_output.image_data.resize(WIDTH*HEIGHT);
	_image_output.image_color.resize(WIDTH*HEIGHT);
	_image_output.width=WIDTH;
	_image_output.height=HEIGHT;
	//
	//
//#define RAMP_TRESHOLD 20
#define COLOR_DIF 40
//#define WHITE_MAX 220
//#define BLACK_MIN 40
	struct Boder{
		int x;
		int y;
	}LBoder[500],RBoder[500],Centre[500];
	bool inflectionR = false,inflectionL = false;  UINT InflectionPoint = 0;  // �жϡ���¼�յ�
    char CountMissL = 0, CountMissR = 0;// ��¼���ұ߽綪ʧ
    bool Cross = true;// �ж�ʮ�ֲ����Ƿ�ɹ�
    int i,j,k = 0;
	int TripR[2]={0},TripL[2]={0};
	int midgraph = WIDTH/2;//ͼ���е�
	//int s,k,m;
	int widemax = 65;//arg[3];  //��һ��ͼ���һ��
	int widemin = 15;//arg[4];     //���һ��ͼ���һ��
    int ValidPoint = 0; // ��¼���һ����Ч�е�λ��,��ʼ��ȫ����Ч
	int RValid[2]={0},LValid[2]={0};//��¼�������ߵ���Ч��
	int MID[500] = {0};
	int Temp[240] = {0};//�洢�����е㣨�����ã�
	int temp,temp2;  //Ѳ��ʱÿ�еļ����е� //�����е���ʱ����
	int CentreValid[2];
	int wide[140];//ÿ�е�������ȵ�һ��
	int OneBoder=0;//�ж��Ƿ�ֻ�ҵ���һ���߽�
	//
	//�ڴ˴���Ӵ����㷨
	vector<BYTE> source_image(_image_input.image_data);//ԭʼͼ����������image_origin[]
	vector<BYTE> Graph_process(WIDTH*HEIGHT);//����֮���ͼ������image_out[]
	vector<BYTE> dest_color(WIDTH*HEIGHT);//����֮���ͼ����ɫ����
	vector<BYTE> temp_graph(WIDTH*HEIGHT);

	for(i = 0;i < HEIGHT; i++)
		wide[i] = (widemin - widemax) * i / HEIGHT + widemax;
  
	RValid[1]=HEIGHT-1;
	LValid[1]=HEIGHT-1;

	for (i=0;i<WIDTH*HEIGHT;i++)
		Graph_process[i]= source_image[i];
	
	//
	for(i=0;i<=HEIGHT-1;i++){
		Centre[i].x=i;
		LBoder[i].x=i;
		RBoder[i].x=i;
	}
	// �ҵ�������һ�еı߽��λ��
    // ��߽��
    for(j = WIDTH/2-8;j>0;j--){
		if(j==1)LBoder[HEIGHT-1].y = 1;
		else if(Graph_process[(HEIGHT-1)*WIDTH + j + 1+1]-Graph_process[(HEIGHT - 1) * WIDTH + j]>=COLOR_DIF && 
			Graph_process[(HEIGHT-1)*WIDTH + j + 2+1]-Graph_process[(HEIGHT - 1) * WIDTH + j]>=COLOR_DIF &&
			Graph_process[(HEIGHT-1)*WIDTH + j + 1+1]-Graph_process[(HEIGHT - 1) * WIDTH + j-1]>=COLOR_DIF){// >= RAMP_TRESHOLD && Graph_process[(HEIGHT - 1) * WIDTH + j + 1]-Graph_process[(HEIGHT - 1) * WIDTH + j] <= ){
			LBoder[HEIGHT - 1].y = j+1;
			break;
        }
    }
    // �ұ߽��
    for(j = WIDTH/2;j<WIDTH;j++){
		if(j==WIDTH-1)RBoder[HEIGHT-1].y = WIDTH-1;
		else if(Graph_process[(HEIGHT-1) * WIDTH + j - 1-1]-Graph_process[(HEIGHT-1)*WIDTH + j]>=COLOR_DIF &&
			Graph_process[(HEIGHT-1) * WIDTH + j - 1-1]-Graph_process[(HEIGHT-1)*WIDTH + j + 1]>=COLOR_DIF && 
			Graph_process[(HEIGHT-1) * WIDTH + j - 2-1]-Graph_process[(HEIGHT-1)*WIDTH + j]>=COLOR_DIF){
			RBoder[HEIGHT - 1].y = j-1;
			break;
        }
    }
	Temp[HEIGHT-1]=(LBoder[HEIGHT-1].y+RBoder[HEIGHT-1].y)/2;
    for(i = HEIGHT - 2;;){		
		// �������
		temp = (LBoder[i+1].y+RBoder[i+1].y)/2;
		Temp[i]=temp;
		if(abs(Temp[i+1]-temp)>=4)temp=Temp[i+1];
		for(j = temp;j>0;j--) {
			if(j == 1){
				LBoder[i].y = 1; // ����߽磬��¼1
				break;
			}
			else if(Graph_process[i*WIDTH + j + 1]-Graph_process[i * WIDTH + j]>=COLOR_DIF && 
				Graph_process[i*WIDTH + j + 1]-Graph_process[i * WIDTH + j - 1]>=COLOR_DIF &&
				Graph_process[i*WIDTH + j + 2]-Graph_process[i * WIDTH + j]>=COLOR_DIF) {
				LBoder[i].y = j+1;      					
				break;
			}
		}
        
		// �ұ�����
		for(j = temp;j<=WIDTH-1;j++) {
			if(j == WIDTH - 1){
				RBoder[i].y = WIDTH -1;    // ���ұ߽磬��¼WIDTH-1
				break;
			}
			else if(Graph_process[i * WIDTH + j-1] -Graph_process[i * WIDTH + j] >= COLOR_DIF && 
				Graph_process[i * WIDTH + j-1] -Graph_process[i * WIDTH + j + 1] >= COLOR_DIF &&
				Graph_process[i * WIDTH + j-2] -Graph_process[i * WIDTH + j] >= COLOR_DIF) {
				RBoder[i].y = j-1;
				break;
			}
		}
		if(i==0)break;
		i--;
    }	
	
    //�����һ����Ч��λ��
    for (i = HEIGHT - 1;;){
		if(RBoder[i].y - LBoder[i].y <= WIDTH/15){
            ValidPoint = i;
			break;
        } 
		if(i==0)break;
		i--;
    }
	
	//�йյ�&����
	//�ұ�
	
	for(temp2=HEIGHT-1;temp2>ValidPoint+3;){
		for(i = temp2-1;i >= ValidPoint+3;i--){
			if(i==HEIGHT-1 && (RBoder[HEIGHT-2].y - RBoder[HEIGHT-1].y)>=8
				||(RBoder[i+1].y - RBoder[i-1].y)*(RBoder[i-1].y-RBoder[i-3].y)<=0 
				&& abs(RBoder[i+1].y -2* RBoder[i-1].y+ RBoder[i-3].y)>=6
				||abs(RBoder[i+1].y - RBoder[i-1].y)>=8&&abs(RBoder[i-1].y-RBoder[i-3].y)<=5){
				if(RBoder[i-1].y<RBoder[i-3].y){
					inflectionR = true;
					TripR[0]=i-1;
					break;
				}
			}
		}
		temp2 = i ;
		if(inflectionR){
			inflectionR=false;
			for(i = TripR[0]-5;i >= ValidPoint+1;i--){
				if(abs(RBoder[i+3].y+RBoder[i-1].y-2*RBoder[i+1].y)>=4){//||RBoder[i-1].y-RBoder[i+1].y==0&&abs(RBoder[i+3].y-RBoder[i+1].y)>=2){
					if(abs(abs(RBoder[i+1].y-RBoder[i-1].y)-abs(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y))<=1
						&&abs(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)<=1
						&&abs((double)(RBoder[i+1].y-RBoder[TripR[0]].y)/(double)(TripR[0]-i-1)-(double)(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)/2)<=1
						&&(RBoder[i+1].y-RBoder[TripR[0]].y)*(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)>=0
						||abs(abs(RBoder[i+1].y-RBoder[i-1].y)-abs(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y))<=2
						&&abs(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)>=2
						&&abs((double)(RBoder[i+1].y-RBoder[TripR[0]].y)/(double)(TripR[0]-i-1)-(double)(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)/(double)2)<=1
						&&(RBoder[i+1].y-RBoder[TripR[0]].y)*(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)>=0)
					{
						TripR[1]=i+1;
						inflectionR=true;
						break;									
					}
				}
			}
			if(!inflectionR){
				for( i = ValidPoint; i <= TripR[0]; i++){
					for (j=LBoder[i].y;j<=WIDTH-1;j++){
						if(j==WIDTH-1){
							RBoder[i].y=WIDTH-1;
						}
						else if(Graph_process[i * WIDTH + j-1] -Graph_process[i * WIDTH + j] >= COLOR_DIF && 
							Graph_process[i * WIDTH + j-1] -Graph_process[i * WIDTH + j + 1] >= COLOR_DIF &&
							Graph_process[i * WIDTH + j-2] -Graph_process[i * WIDTH + j] >= COLOR_DIF){
							RBoder[i].y=j-1;
							break;
						}

					}
				}
				for (i = HEIGHT - 1;;){
					if(LBoder[i].y >= RBoder[i].y-3){
						ValidPoint = i;
						break;
					} 
					if(i==0)break;
					i--;
				}
				for(i = TripR[0]-5;i >= ValidPoint+1;i--){
					if(abs(abs(RBoder[i+1].y-RBoder[i-1].y)-abs(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y))<=1
						&&abs(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)<=1
						&&abs((double)(RBoder[i+1].y-RBoder[TripR[0]].y)/(double)(TripR[0]-i-1)-(double)(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)/(double)2)<=1
						&&(RBoder[i+1].y-RBoder[TripR[0]].y)*(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)>=0
						||abs(abs(RBoder[i+1].y-RBoder[i-1].y)-abs(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y))<=2
						&&abs(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)>=2
						&&abs((double)(RBoder[i+1].y-RBoder[TripR[0]].y)/(double)(TripR[0]-i-1)-(double)(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)/(double)2)<=1
						&&(RBoder[i+1].y-RBoder[TripR[0]].y)*(RBoder[TripR[0]].y-RBoder[TripR[0]+2].y)>=0){
						TripR[1]=i+1;
						inflectionR=true;
						break;
					}
				}
			}
		}
		//  ����
		if(inflectionR){
			for(i=TripR[1];i<=TripR[0];i++){
				RBoder[i].y=(RBoder[TripR[0]].y-RBoder[TripR[1]].y)*(i-TripR[1])/(TripR[0]-TripR[1])+RBoder[TripR[1]].y;
			}
		}
		if(inflectionR)temp2 = TripR[1]-3;
		inflectionR = false;
	}
	//���
	for(temp2=HEIGHT-1;temp2>ValidPoint+3;){
		for(i = temp2-1;i >= ValidPoint+3;i--){
			if(i==HEIGHT-1 && (LBoder[HEIGHT-2].y - LBoder[HEIGHT-1].y)<=-8
				||(LBoder[i+1].y - LBoder[i-1].y)*(LBoder[i-1].y-LBoder[i-3].y)<=0 
				&& abs(LBoder[i+1].y -2* LBoder[i-1].y+ LBoder[i-3].y)>=6
				||abs(LBoder[i+1].y - LBoder[i-1].y)>=8&&abs(LBoder[i-1].y-LBoder[i-3].y)<=5){
				if(LBoder[i-1].y>LBoder[i-3].y){
					inflectionL = true;
					TripL[0]=i-1;
					break;
				}
			}
		}
		temp2 = i;
		if(inflectionL){
			inflectionL=false;
			for(i = TripL[0]-5;i >= ValidPoint+1;i--){
				if(abs(LBoder[i+3].y+LBoder[i-1].y-2*LBoder[i+1].y)>=3||LBoder[i-1].y-LBoder[i+1].y==0&&abs(LBoder[i+3].y-LBoder[i+1].y)>=2){
					if(abs(abs(LBoder[i+1].y-LBoder[i-1].y)-abs(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y))<=1
						&&abs(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)<=1
						&&abs((double)(LBoder[i+1].y-LBoder[TripL[0]].y)/(double)(TripL[0]-i-1)-(double)(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)/2)<=1
						&&(LBoder[i+1].y-LBoder[TripL[0]].y)*(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)>=0
						||abs(abs(LBoder[i+1].y-LBoder[i-1].y)-abs(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y))<=2
						&&abs(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)>=2
						&&abs((double)(LBoder[i+1].y-LBoder[TripL[0]].y)/(double)(TripL[0]-i-1)-(double)(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)/2)<=1
						&&(LBoder[i+1].y-LBoder[TripL[0]].y)*(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)>=0){	//LBoder[i+3]-LBoder[i+1]<0&&
						TripL[1]=i+1;
						inflectionL=true;
						break;									
					}
				}
			}
			if(!inflectionL){
				for( i = ValidPoint; i <= TripL[0]; i++){
					for (j=RBoder[i].y;j>=1;j--){
						if(Graph_process[i*WIDTH + j + 1]-Graph_process[i * WIDTH + j]>=COLOR_DIF && 
							Graph_process[i*WIDTH + j + 1]-Graph_process[i * WIDTH + j - 1]>=COLOR_DIF &&
							Graph_process[i*WIDTH + j + 2]-Graph_process[i * WIDTH + j]>=COLOR_DIF){
							LBoder[i].y=j+1;
							break;
						}
						if(j==1){
							LBoder[i].y=1;
						}
					}
				}
				for (i = HEIGHT - 1;;){
					if(LBoder[i].y >= RBoder[i].y-3){
						ValidPoint = i;
						break;
					} 
					if(i==0)break;
					i--;
				}
				for(i = TripL[0]-5;i >= ValidPoint+1;i--){
					if(abs(abs(LBoder[i+1].y-LBoder[i-1].y)-abs(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y))<=1
						&&abs(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)<=1
						&&abs((double)(LBoder[i+1].y-LBoder[TripL[0]].y)/(double)(TripL[0]-i-1)-(double)(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)/2)<=1
						&&(LBoder[i+1].y-LBoder[TripL[0]].y)*(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)>=0
						||abs(abs(LBoder[i+1].y-LBoder[i-1].y)-abs(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y))<=2
						&&abs(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)>=2
						&&abs((double)(LBoder[i+1].y-LBoder[TripL[0]].y)/(double)(TripL[0]-i-1)-(double)(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)/2)<=1
						&&(LBoder[i+1].y-LBoder[TripL[0]].y)*(LBoder[TripL[0]].y-LBoder[TripL[0]+2].y)>=0){
						TripL[1]=i+1;
						inflectionL=true;
						break;
					}
				}
			}
		}
		//  ����
		if(inflectionL){
			for(i=TripL[1];i<=TripL[0];i++){
				LBoder[i].y=(LBoder[TripL[0]].y-LBoder[TripL[1]].y)*(i-TripL[1])/(TripL[0]-TripL[1])+LBoder[TripL[1]].y;
			}
		}
		if(inflectionL)temp2 = TripL[1]-3;
		inflectionL = false;
	}
	//�ж����ұ߽���Ч�з�Χ
	LValid[0]=HEIGHT-1;
	RValid[0]=HEIGHT-1;
	for(i=HEIGHT-1;i>=ValidPoint;i--){
		if(LBoder[i].y != 1&&LBoder[i-1].y != 1&&LBoder[i-2].y != 1||i==ValidPoint){
			LValid[0]=i;
			break;
		}
	}
	for(i=LValid[0];i>=ValidPoint;i--){
		if(LBoder[i].y == 1&&LBoder[i-1].y == 1&&LBoder[i-2].y == 1||i==ValidPoint||abs(LBoder[i-1].y-LBoder[i].y)>=WIDTH/5){
			LValid[1]=i+1;
			break;
		}
	}
	for(i=HEIGHT-1;i>=ValidPoint;i--){
		if(RBoder[i].y != WIDTH-1&&RBoder[i-1].y != WIDTH-1&& RBoder[i-2].y != WIDTH-1||i==ValidPoint){
			RValid[0]=i;
			break;
		}
	}
	for(i=RValid[0];i>=ValidPoint;i--){
		if(RBoder[i].y == WIDTH-1&& RBoder[i-1].y == WIDTH-1&& RBoder[i-2].y == WIDTH-1||i==ValidPoint||abs(RBoder[i-1].y-RBoder[i].y)>=WIDTH/5){
			RValid[1]=i+1;
			break;
		}
	}
	//�߽�����
	for(i=LValid[0]-1;i>=LValid[1]-5;i--){
		if(abs(2*LBoder[i].y-LBoder[i+1].y-LBoder[i-1].y)>=6){
			if(abs((double)(LBoder[i+4].y-LBoder[i-1].y)/5-(double)(LBoder[i-1].y-LBoder[i-3].y)/2)<=1)
				LBoder[i].y=(LBoder[i+4].y-LBoder[i-1].y)/4+LBoder[i-1].y;
		}
	}
	for(i=RValid[0]-1;i>=RValid[1]-5;i--){
		if(abs(2*RBoder[i].y-RBoder[i+1].y-RBoder[i-1].y)>=6){
			if(abs((double)(RBoder[i+4].y-RBoder[i-1].y)/5-(double)(RBoder[i-1].y-RBoder[i-3].y)/2)<=1)
				RBoder[i].y=(RBoder[i+4].y-RBoder[i-1].y)/4+RBoder[i-1].y;
		}
	}
	//
	//  ������
	CentreValid[0]=(RValid[0]>LValid[0])?LValid[0]:RValid[0];
	i=CentreValid[0];
    if(LBoder[i].y!=1 && RBoder[i].y!= WIDTH - 1)
	{
		Centre[i].y = (LBoder[i].y+RBoder[i].y)/2;
	}
    else if(LBoder[i].y!= 1){
		Centre[i].y = LBoder[i].y + wide[i]; 
	} 
    else if(RBoder[i].y= WIDTH -1 ){
		Centre[i].y = RBoder[i].y - wide[i]; 
    }else {
		Centre[i].y = midgraph;   
    }
	CentreValid[1] = (LValid[1]>RValid[1])?LValid[1]:RValid[1];
    for (i = CentreValid[0]-1;i>=CentreValid[1];i--){
		if(LBoder[i].y!=1 && RBoder[i].y!= WIDTH - 1){
			Centre[i].y  = (LBoder[i].y+RBoder[i].y)/2;
		}
        else if(LBoder[i].y!= 1){
			Centre[i].y= LBoder[i].y + wide[i];
			if(abs(Centre[i].y-Centre[i+1].y)>=2)Centre[i].y = Centre[i+1].y + (LBoder[i].y - LBoder[i+1].y);
        } else if(RBoder[i].y!= WIDTH -1 ){
            Centre[i].y = RBoder[i].y - wide[i];
			if(abs(Centre[i].y-Centre[i+1].y)>=2)Centre[i].y = Centre[i+1].y + (RBoder[i].y - RBoder[i+1].y);
        }else {
            if(i != CentreValid[0]-1) Centre[i].y = 2* Centre[i+1].y- Centre[i+2].y;
            else  Centre[i].y =  Centre[i+1].y;
		}
    }
	//����ֻ�ҵ�һ���߽�����
	/*if(CentreValid[1]>=10){
	if(-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-WIDTH]
	+Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y]>=COLOR_DIF
		&&abs(Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-WIDTH]
	-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-WIDTH+1])<COLOR_DIF
		&&abs(Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-WIDTH]
	-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-WIDTH-1])<COLOR_DIF
		||
		-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-2*WIDTH]
	+Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-WIDTH]>=COLOR_DIF
		&&abs(Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-2*WIDTH]
	-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-2*WIDTH+1])<COLOR_DIF
		&&abs(Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-WIDTH*2]
	-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-2*WIDTH-1])<COLOR_DIF
		||
		-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-2*WIDTH]
	+Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y]>=COLOR_DIF
		&&abs(Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-2*WIDTH]
	-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-2*WIDTH+1])<COLOR_DIF
		&&abs(Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-WIDTH*2]
	-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-2*WIDTH-1])<COLOR_DIF
		||
		-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-3*WIDTH]
	+Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y]>=COLOR_DIF
		&&abs(Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-3*WIDTH]
	-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-3*WIDTH+1])<COLOR_DIF
		&&abs(Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-WIDTH*3]
	+-Graph_process[Centre[CentreValid[1]].x*WIDTH+Centre[CentreValid[1]].y-3*WIDTH-1])<COLOR_DIF){
		if(LValid[0]<RValid[0]){
			OneBoder=1;
					LValid[1]=HEIGHT-1;
		LBoder[LValid[1]].y=1;
		LBoder[LValid[1]].x=HEIGHT-1;
		CentreValid[0]=HEIGHT-1;
		CentreValid[1]=HEIGHT-1;
		}
		else{ 
			OneBoder=-1;
				RValid[1]=HEIGHT-1;
		RBoder[RValid[1]].y=WIDTH-1;
		RBoder[RValid[1]].x=HEIGHT-1;
		CentreValid[0]=HEIGHT-1;
		CentreValid[1]=HEIGHT-1;
		}
	}
	}*/
	if(LValid[0]-LValid[1]<=5||LValid[0]<=HEIGHT/2&&LValid[0]-LValid[1]<=10){
		LValid[1]=HEIGHT-1;
		LBoder[LValid[1]].y=1;
		LBoder[LValid[1]].x=HEIGHT-1;
		CentreValid[0]=HEIGHT-1;
		CentreValid[1]=HEIGHT-1;
		OneBoder=1;
	}
	if(RValid[0]-RValid[1]<=5||RValid[0]<=HEIGHT/2&&RValid[0]-RValid[1]<=10){
		RValid[1]=HEIGHT-1;
		RBoder[RValid[1]].y=WIDTH-1;
		RBoder[RValid[1]].x=HEIGHT-1;
		CentreValid[0]=HEIGHT-1;
		CentreValid[1]=HEIGHT-1;
		OneBoder=-1;
	}
//	if(OneBoder==1){
		if(LValid[1]>RValid[1]){
			for(i=CentreValid[1];i>=RValid[1] ;i--){
				Centre[i].x = (LBoder[LValid[1]].x + RBoder[i].x)/2;
				Centre[i].y = (LBoder[LValid[1]].y + RBoder[i].y)/2;	
			}
		}
//	}
//	if(OneBoder==-1){
		if(LValid[1]<RValid[1]){
			for(i=CentreValid[1];i>=LValid[1] ;i--){
				Centre[i].x = (RBoder[RValid[1]].x + LBoder[i].x)/2;
				Centre[i].y = (RBoder[RValid[1]].y + LBoder[i].y)/2;
			}
		}
//	}
	CentreValid[1] = (LValid[1]<RValid[1])?LValid[1]:RValid[1];
	//�ж�������Ч��Χ
	for(i = CentreValid[0]-2;i >= CentreValid[1]+3;i--){
		if(abs(Centre[i+1].y -2* Centre[i-1].y+ Centre[i-3].y)>=6
		//	||abs(Centre[i+1].y -Centre[i-1].y)<=1&&abs(Centre[i-3].y -Centre[i-1].y)>=3&&abs(Centre[i+1].y -2* Centre[i-1].y+ Centre[i-3].y)>=3
			||abs(Centre[i-3].y -Centre[i-1].y)<=1&&abs(Centre[i+1].y -Centre[i-1].y)>=3&&abs(Centre[i+1].y -2* Centre[i-1].y+ Centre[i-3].y)>=4){
			CentreValid[0]=i-1;
		//	break;
		}
	}

	//��������
	if(CentreValid[0]<=HEIGHT-1){
		for(i=HEIGHT-1;i>=CentreValid[0];i--){
			Centre[i].y=(CentreValid[0]-i)*(Centre[CentreValid[0]-5].y+Centre[CentreValid[0]-4].y+Centre[CentreValid[0]-3].y
				-Centre[CentreValid[0]].y-Centre[CentreValid[0]-1].y-Centre[CentreValid[0]-2].y)/9+Centre[CentreValid[0]].y;
		}
		CentreValid[0]=HEIGHT-1;
	}
	//����Ҫ�ò�ͬ��ɫ��ʾ������
	for(i=CentreValid[1];i<=CentreValid[0];i++){
		int temp = Centre[i].y+WIDTH * Centre[i].x;
		if (temp < HEIGHT*WIDTH-2)
		{
			dest_color[Centre[i].y+WIDTH * Centre[i].x]=1;
			dest_color[Centre[i].y+WIDTH * Centre[i].x+1]=1;
			dest_color[Centre[i].y+WIDTH * Centre[i].x-1]=1;
		}	
	}
	for(i=LValid[0];i>=LValid[1];i--){
		dest_color[LBoder[i].y+i * WIDTH]=1;
	}
	for(i=HEIGHT-1;i>=0;i--){
		dest_color[LBoder[i].y+i * WIDTH]=1;
	}
	for(i=RValid[0];i>=RValid[1];i--){
		dest_color[RBoder[i].y+i * WIDTH]=1;
	}
	for(i=HEIGHT-1;i>=0;i--){
		dest_color[RBoder[i].y+i * WIDTH]=1;
	}
	//dest_color[10]=1;
	//if(inflectionL)dest_color[WIDTH-1]=1;
	//if(inflectionR)dest_color[0]=1;
	_image_output.image_data=Graph_process;
	_image_output.image_color=dest_color;
}

