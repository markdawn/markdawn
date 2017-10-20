#pragma once

#define INTERFACE_NUM          8//�������

//#define WM_START_MSG				 WM_USER + 100
#define WM_GUIDISPLAY_MSG   WM_USER + 101
//#define  WM_PROCESS_MSG       WM_USER + 102
//#define  WM_DATABASE_MSG     WM_USER + 103
//#define  WM_CALGRID_MSG     WM_USER + 104
//#define WM_UPDATE_MSG          WM_USER + 105  //update the new message to main thread
#define  WM_STARTWORK_MSG  WM_USER + 106 

#define WM_MAINDIS_MSG               WM_USER + 200
#define  WM_ISOTHERM_MSG     WM_USER + 201
#define  WM_AREAARVGT_MSG  WM_USER + 202
#define  WM_TTREND_MSG          WM_USER + 203
#define  WM_AREATINFO_MSG    WM_USER + 204
#define  WM_POSITION_MSG       WM_USER + 205
#define  WM_TIME_MSG                WM_USER + 206
#define  WM_LEAK_MSG               WM_USER + 207


#define BACKGROUND_COLOR     RGB(149,149,149)
#define BLACK_COLOR					RGB(0,0,0)
#define RED_COLOR							RGB(255,0,0)
#define WHITE_COLOR                    RGB(242,242,242)
#define BLUE_COLOR                      RGB(0,0,255)
#define  LIGHT_BLUE_COLOR         RGB(8,140,156)
#define  GREEN_COLOR                 RGB(8,165,8)

#define  GREEN_COLOR_1                 RGB(88,165,88)
#define  GREEN_COLOR_2                 RGB(208,165,18)
#define  PURPLE_COLOR                 RGB(189,16,206)
#define  GRAY_COLOR                 RGB(173,156,173)
#define  ORANGE_COLOR                 RGB(230,165,8)
#define  NAVY_COLOR                 RGB(13,9,187)
#define  GRIDLINE_COLOR                 RGB(7,95,12)
#define  REDWINE_COLOR                 RGB(141,18,36)
#define SYSTEM_COLOR    BLACK_COLOR //RGB(53,54,57)

#define START_COLOR RGB(253,17,11)
//#define  END_COLOR   RGB(252,234,12)
#define  END_COLOR   RGB(252,234,200)

//Socket data
#define CMD_LENTH            4  //�����---�ֽ�
#define PACKAGE_LENTH 1028 //���ݰ�����---�ֽ�
#define PACKAGE_NUM     10 //���ݰ�����
#define DATA_LENTH         (PACKAGE_LENTH-CMD_LENTH)//�ź����ݳ���----�ֽ�
#define  TOTLA_LENTH     DATA_LENTH*PACKAGE_NUM //�������ź����ݳ���---�ֽ�

//ȫ�ֻ���
#define PERMODULE_DATA_LENTH     (TOTLA_LENTH/2)//ÿ�����ݿ����ݸ���---�����ֽ���ϳ�һ����Ч����
#define PINGPONG  2//0-Ping��������1-Pong������

//module
//#define AREA_BLOCK_NUM 16  //ƽ���¶��������,����16������
#define ARM_NUM  5//8//ARMģ�����
//#define PATH_NUM 24 //�ܹ�24��·��
#define SOUNDDEV_NUM  5//5//��Ҫ�����豸�ĸ���
#define AREA_BLOCK_NUM 12 
//#define ARM_NUM 4//4//ARMģ�����  ����һ����ɨ������ 4+1=5
#define PATH_NUM 16 //�ܹ�5��·��
//#define SOUNDDEV_NUM  4//��Ҫ�����豸�ĸ���
#define MAX_DELAY  10000//�ȴ�����������ʱ
#define SOUND_TIMES 5  //ÿ��ģ��ķ�������
#define  SIDETEMP   1000 //�߽��¶�

//�¶ȳ���������
#define GRID_ROW_NUM   31 //��������31��
#define GRID_COLUMN_NUM 51 //51��

//�ع��㷨
#define N_ARRAY 5120
#define N_DATALENTH TOTLA_LENTH

// ������ʾ�ֱ���
#define RESOLUTION_X  1920.0f //����ϵͳ��ʾ����ʵķֱ���
#define RESOLUTION_Y  1080.0f 
#define XCOFF  (GetSystemMetrics(SM_CXSCREEN)/RESOLUTION_X) //��ͬ�ֱ���ʱ��ϵ��
#define YCOFF  (GetSystemMetrics(SM_CYSCREEN)/RESOLUTION_Y)

#define SAMPLE_DATA_NUM       4096
#define PI 3.1415926535897932

#define ERROR_READINI -1

typedef enum _ERROR_MYSQL{
	NO_MYSQL_ERROR = 0,
	ERROR_MYSQL_INIT = -1,
	ERROR_MYSQL_CONNECT = -2,
	ERROR_MYSQL_READ = -3,
	ERROR_MYSQL_WRITE = -4,
	ERROR_MYSQL_DELETE = -5,
}ERROR_MYSQL;

typedef struct  _StartEndTime
{
	time_t startTIme;
	time_t endTime;
}StartEndTime;

//�¶ȳ�����������Ϣ
typedef struct _GridBaiscInfo{
	UINT row; //��
	UINT col;//��
	float width;//��
	float hight;//��
	float maxValue;//���ֵ
	float minValue;//��Сֵ
}GridBaiscInfo;

//��¼һ����ֵ���ϵ�����
typedef struct _ContourPoint{
	float x;
	float y;
}ContourPoint;


typedef struct _PathSendReceiveModule{
	UINT iSendModule;
	UINT iReveiveModule;
}PathSendReceiveModule;

//����ƽ���¶ȷ�ֵ����
typedef struct _AreaTParaSet{
	int iLowTLimit;
	int iHighTLimit;
}AreaTParaSet;

//·����Ӧ������ARMģ��
//const PathSendReceiveModule gPathOrder[PATH_NUM]={{1,3},{1,4},{1,5},{1,6},{1,7},{1,8}, //ARM1����
//{2,3},{2,4},{2,5},{2,6},{2,7},{2,8}, //ARM2����
//{3,5},{3,6},{3,7},{3,8}, //ARM3����
//{4,5},{4,6},{4,7},{4,8}, //ARM4����
//{5,7},{5,8}, //ARM5����
//{6,7},{6,8}}; //ARM6����
const PathSendReceiveModule gPathOrder[PATH_NUM] =
{
	{ 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 4 }, //ARM1����
	{ 2, 1 }, { 2, 2 }, { 2, 3 }, { 2, 4 }, //ARM2����
	{ 3, 1 }, { 3, 2 }, { 3, 3 }, { 3, 4 }, //ARM3����
	{ 4, 1 }, { 4, 2 }, { 4, 3 }, { 4, 4 }  //ARM4����
};

//·����ӦARMģ��洢������--�ɶ�ʱ��ʹ�õ�����
const UINT PathUseArmDataOrder[28] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 6 };


///////////////////////////ͨ��TCP/IP���յ�������/////////////////////////////////////////////
#define  VDD   3000.0f   //mv
#define   vCoff  (VDD/4096.0f)    //����ת����ʽ��v=data*(VDD/4096)
#if 1

#define  _DEBUG_PROJECT
#define DEBUG_ISOTHERM

#endif

const POINT LEDALARM_ORPOINT = { 900, 20 };//������ƣ�ͣ�����ģ�����
const POINT LEDFAULT_ORPOINT = { 1000, 20 };//����

///////////////////////////////////////////////////////��������������С/////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4244)
const RECT  Logo_Rect = { 0, 0, 714 * XCOFF, 184 * YCOFF };//Logo��С
const RECT  BoilerPos_Rect = { 0, 270 * YCOFF, 350 * XCOFF, 820 * YCOFF };//��¯������㲼������ͼʾ��С
const RECT  ArmModule_Rect = { 630 * XCOFF, 5 * YCOFF, 1150 * XCOFF, 585 * YCOFF };//ARMģ������ͼʾ��С
const RECT  ArmModule1_Rect = { 630 * XCOFF, 100 * YCOFF, 1150 * XCOFF, 300 * YCOFF };//ARMģ������ͼʾ��С
const RECT  ArmModule2_Rect = { 630 * XCOFF, 380 * YCOFF, 1150 * XCOFF, 580 * YCOFF };//ARMģ������ͼʾ��С
const RECT  Isotherm_Rect = { 1182 * XCOFF, 34 * YCOFF, 1880 * XCOFF, 490 * YCOFF };//�¶ȳ�����ͼʾ��С
const RECT  ValvePipe_Rect = { 50 * XCOFF, 200 * YCOFF, 1150 * XCOFF, 900 * YCOFF };//��ŷ��ܵ�����ͼʾ��С
const RECT  AreaT_Rect = { 1182 * XCOFF, 520 * YCOFF, 1880 * XCOFF, 920 * YCOFF };//ƽ���¶�����ͼʾ��С



const POINT  Led_Point = { 310 * XCOFF, 533 * YCOFF };
const POINT  MainLed_Point = { 560 * XCOFF, 500 * YCOFF };
#define W_ARMS   (30*XCOFF)//����ARMģ����ͼƬ֮��ľ���left
#define H_ARMS   (280*YCOFF)//����ARMģ����ͼƬ֮��ľ���top

/////////////////////////////////////////////////////�ӽ�����ʾ�����С/////////////////////////////////////////////////////////////////////////////////////////////////
const RECT  PointPos_Rect = { 10 * XCOFF, 10 * YCOFF, 1910 * XCOFF, 920 * YCOFF };//���λ�ý���
const RECT  AverageT_Rect = { 10 * XCOFF, 10 * YCOFF, 1900 * XCOFF, 930 * YCOFF };//����ƽ���¶Ƚ���
const RECT  TrendT_Rect = { 150 * XCOFF, 10 * YCOFF, 1900 * XCOFF, 930 * YCOFF };//����ƽ���¶�ʵʱ���ƽ���
const RECT  QueryT_Rect = { 100 * XCOFF, 100 * YCOFF, 1800 * XCOFF, 860 * YCOFF };//�¶���Ϣ��ѯ����
const RECT  Signal_Rect = { 150 * XCOFF, 20 * YCOFF, 1800 * XCOFF, 460 * YCOFF };//ʱ�����
const RECT  Signal_Rect2 = { 150 * XCOFF, 470 * YCOFF, 1800 * XCOFF, 930 * YCOFF };//ʱ�����
const RECT  IsothermT_Rect = { 80 * XCOFF, 10 * YCOFF, 1880 * XCOFF, 950 * YCOFF };//�¶ȳ�����