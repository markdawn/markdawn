#include "StdAfx.h"
#include "ContourTrace.h"
#include "GetIsothremData.h"
#pragma warning(disable:4018)
CContourTrace::CContourTrace(void)
	: xSide(NULL)
	, ySide(NULL)
	, m_currentCurveLine(NULL)
{
	m_pCurveList = NULL;
	m_ppGridData= NULL;
	m_valueTracing = 0.0f;
	

}


CContourTrace::~CContourTrace(void)
{
	FreeMemory(xSide);
	FreeMemory(ySide);

}


void CContourTrace::SetCurveList(CCurveList* pCurveList)
{
	if (pCurveList == NULL)
	{
		return;
	}
	m_pCurveList = pCurveList;
}


// ׷�ٵ�ֵ��
BOOL CContourTrace::ExecuteTracing(float contourValue)
{
	//1.��������ֵ��
	//2.׷�ٵ���ֵ��
	//3.��ֵ������
	ASSERT( m_gridDataInfo.col != 0 ); //�ڵ��øú���֮ǰ�������SetGridDataInfo()������������������Ϣ

	if( contourValue < m_gridDataInfo.minValue && contourValue > m_gridDataInfo.maxValue )
	{
		return FALSE;
	}

	m_valueTracing = contourValue;


	//1.ΪxSide��ySide�����ڴ�ռ�
	AllocateMemory();

	//2.ɨ�������ݺ�ߣ��ڲ��ֵ��
	//�ú���֮�У��ڼ����ֵ��ʱ������׷��ֵ��������ϵ�������ͬʱ��
	//�ڼ���ǰ���������������(����һСƫ��ֵ)

	// ׷�ٵ�ֵ��
	InterpolateTracingValue(); 

	//3.��׷�ٿ�����
	TracingNonClosedContour();

	//4.��׷�ٱ�����
	TracingClosedContour();

	return TRUE;
}


// ���������Ϣ
void CContourTrace::GetGridBasicInfo(GridBaiscInfo dataInfo)
{
	m_gridDataInfo = dataInfo;
}


// �õ�դ������
void CContourTrace::GetGridData(float** ppGridData)
{
	ASSERT( ppGridData != NULL );
	m_ppGridData = ppGridData;
}

//�����ڴ�
void CContourTrace::AllocateMemory(void)
{
	UINT cols = m_gridDataInfo.col;
	UINT rows = m_gridDataInfo.row;

	if (xSide == NULL)
	{
		//�����д���rows*(cols-1)����ߣ�������ҪΪxSide����rows*(cols-1)�ռ����
		ASSERT (AllocMemory(xSide,rows,cols-1)== TRUE);
	}

	if (ySide == NULL)
	{
		//�����д���(rows-1)*cols���ݱߣ�������ҪΪySide����(rows-1)*cols�ռ����
		ASSERT (AllocMemory(ySide,rows-1,cols)==TRUE);
	}
}


// ׷�ٵ�ֵ��
void CContourTrace::InterpolateTracingValue(void)
{
	ASSERT(m_ppGridData!=NULL);
	ASSERT(xSide!=NULL);
	ASSERT(ySide!=NULL);

	UINT i = 0;
	UINT j = 0;

	UINT rows = m_gridDataInfo.row;
	UINT cols = m_gridDataInfo.col;
	float **ppData = m_ppGridData;
	float currentValue = m_valueTracing;

	float H1 = 0.0f, H2 = 0.0f;//��¼һ���������������ֵ
	float shift = 0.001f; //����ֵ
	float flag = 0.0f;
  /*******************-2��ʶ��ֵ��û���ڱ���******************************************/

	//ɨ�貢�������ϵĵ�ֵ�㣺row*��cols-1��
	for (i = 0; i < rows;i++)
	{
		for (j = 0; j<cols-1;j++)
		{
			H1 = ppData[i][j];
			H2 = ppData[i][j+1];

			if (H1 == H2)
			{
				xSide[i][j] = -2;
			}
			else
			{
				flag = (currentValue-H1)*(currentValue - H2);//�ж��Ƿ��ڱ���
				if (flag < 0) //�ڱ���
				{
					xSide[i][j] = (currentValue- H1)/(H2-H1);
					ASSERT(xSide[i][j]>=0&&xSide[i][j]<=1.0f);
				}
				else if (flag > 0) //���ڱ���
				{
					xSide[i][j] = -2;
				}
				else if(flag == 0) //��ֵ�����������ȣ���Ҫ����
				{
					if (H1== currentValue)
					{
						H1 += shift;
					}

					if (H2==currentValue)
					{
						H1 += shift;
					}

					xSide[i][j] = (currentValue- H1)/(H2-H1);
				}
			}

		}
	}

	//ɨ�貢�����ݱ��ϵĵ�ֵ�㣺��row-1��*cols
	for (i = 0;i<rows-1;i++)
	{
		for (j = 0;j<cols;j++)
		{
			H1 = ppData[i][j];
			H2 = ppData[i+1][j];
			if (H1==H2)
			{
				ySide[i][j] = -2;
			}
			else
			{
				flag = (currentValue-H1)*(currentValue-H2);
				if (flag > 0)
				{
					ySide[i][j] = -2;
				}
				else if (flag < 0)
				{
					ySide[i][j] = (currentValue - H1)/(H2-H1);
					ASSERT(ySide[i][j]>=0&&ySide[i][j]<=1.0f);
				}
				else if (flag == 0)
				{
					if (currentValue== H1)
					{
						H1 += shift;
					}
					else
					{
						H2 += shift;
					}
					ySide[i][j] = (currentValue - H1)/(H2-H1);

				}
			}

		}
	}
}


// ׷����������,���ĸ��߽翪ʼ׷�ٿ�����
void CContourTrace::TracingNonClosedContour(void)
{
	int i = 0,j = 0;
	int cols = m_gridDataInfo.col;
	int rows = m_gridDataInfo.row;	

	//1.�����ױ߿��ϵĵ�ֵ��
	for (j = 0; j <cols-1;j++)
	{
		if (IsContourPoint(xSide[0][j]))//���������е�ֵ��
		{
			/*���������ϵ�ǰ����������ǰһ���i,j*/
			PreviousPoint.i = -1; // ����ǰһ����-1��,������һ�㵽��ǰ���ǰ����������������ϵ�
			PreviousPoint.j = j;
			PreviousPoint.bHorV = TRUE;

			CurrentPoint.i = 0; //�ױߵ��к�Ϊ0������������ͷ��iΪ0
			CurrentPoint.j = j;
			CurrentPoint.bHorV = TRUE; //�ױ��Ǻ��

			TracingOneNonClosedContour();	
		}
	}

	//2.������߿��ϵĵ�ֵ��
	for (i=0;i<rows-1;i++)
	{
		if (IsContourPoint(ySide[i][0]))
		{
			//�������������е㣬
			PreviousPoint.i = i;  
			PreviousPoint.j = -1; //����һ��ʹ�䰴�������ҵķ�������
			PreviousPoint.bHorV = FALSE;//��߿����ݱ�

			CurrentPoint.i = i;
			CurrentPoint.j = 0;
			CurrentPoint.bHorV = FALSE;
			TracingOneNonClosedContour();	

		}
	}
	//3.�����ϱ߿��ϵĵ�ֵ��
	for (j = 0; j <cols-1;j++)
	{
		if (IsContourPoint(xSide[rows-1][j]))//����ϱ߿����е�ֵ��
		{
			/*���������µ�ǰ����������ǰһ���i,j*/
			PreviousPoint.i = rows-1;
			PreviousPoint.j = j;
			PreviousPoint.bHorV = TRUE;

			CurrentPoint.i = rows-1; //
			CurrentPoint.j = j;
			CurrentPoint.bHorV = TRUE;

			TracingOneNonClosedContour();	
		}
	}

	//4.�����ұ߿��ϵĵ�ֵ��
	for (i=0;i<rows-1;i++)
	{
		if (IsContourPoint(ySide[i][cols-1]))
		{
			//��������������һ�㣬
			PreviousPoint.i = i;  
			PreviousPoint.j = cols-1; 
			PreviousPoint.bHorV = FALSE;

			CurrentPoint.i = i;
			CurrentPoint.j = cols-1;
			CurrentPoint.bHorV = FALSE;
			TracingOneNonClosedContour();	

		}
	}

}


// ׷�ٱ�����
void CContourTrace::TracingClosedContour(void)
{
	int cols = m_gridDataInfo.col;
	int rows = m_gridDataInfo.row;

	//�������еĳ��˱߿�����ݱ�,����������ÿһ���ϵ��ݱߣ�����һ�У����µ�������
	for(int j=1; j<cols-1; j++) //j��1��ʼ
		for(int i=0; i<rows-1; i++) //i��0��ʼ
		{
			if( IsContourPoint(ySide[i][j])) 
			{
				TracingOneClosedContour(i,j);				
			}

		}
}


// �����ڴ�
BOOL CContourTrace::AllocMemory(float** &pData, UINT row, UINT col)
{
	float **pp;
	pp=NULL;
	pp=new float*[row];
	if (!pp)
	{
		//TRACE("Alloc memory Open error!!\n");
		return FALSE;
	}

	pp[0]=new float [row*col];//allocate a big block
	if (!pp[0])
	{
		//TRACE("memeroy Open error!!\n");
		return FALSE;
	}
	for(UINT i=1;i<row;i++)
	{
		pp[i]=pp[0]+(long)col*i;
	}
	pData = pp;
	return TRUE;
}


// �����ֵ������
void CContourTrace::CalcAndSaveOnePointCoord(int i, int j, BOOL bHorizon, float& x, float& y)
{
	 float deltX = m_gridDataInfo.width/ ( m_gridDataInfo.col - 1 );
	 float deltY = m_gridDataInfo.hight / ( m_gridDataInfo.row - 1 );

	 //////////////////////��ʱ///////////////
	 float nx = 0.0f; //���x��С����
	 float ny = 0.0f;//�ݱ�y��С����

	 if (bHorizon)//���
	 {
		 x = nx + (j+xSide[i][j])*deltX;
		 y = ny + i*deltY;
	 }
	 else
	 {
		 x = nx + j * deltX;
		 y = ny + (i+ySide[i][j])*deltY;
	 }

	 ContourPoint point;
	 point.x = x;
	 point.y = y;
	 m_currentCurveLine->Add(point);

}


// ׷��һ��������
void CContourTrace::TracingOneNonClosedContour(void)
{
	CCurve* pCtrLine = new CCurve;
	m_pCurveList->AddTail(pCtrLine);
	m_currentCurveLine = pCtrLine;
	//�����ߵĵ�һ���ڵ�Ĵ洢�����ߴ�����¶�ֵ
	ContourPoint point0;
	point0.x = m_valueTracing;
	point0.y = m_valueTracing;
	m_currentCurveLine->Add(point0);


	//��¼����ͷ���ڱߵ�i��j�����ݱ߱�ʶ
	int startPt_i = CurrentPoint.i; 
	int startPt_j = CurrentPoint.j;
	BOOL startPt_bHoriz = CurrentPoint.bHorV;

	//��֤��ͷ�ڱ߽���
	ASSERT( startPt_i == 0                     ||  /*��ͷ�ڵױ߽���*/
		startPt_i == m_gridDataInfo.row-1 ||  /*��ͷ���ϱ߽���*/
		startPt_j == 0                     ||  /*��ͷ����߽���*/
		startPt_j == m_gridDataInfo.col-1     /*��ͷ���ұ߽���*/ 
		);
	//�������ͷ�����겢����
	CalcAndSaveOnePointCoord(startPt_i,startPt_j,startPt_bHoriz,CurrentPoint.x,CurrentPoint.y); 

	if( startPt_bHoriz )
	{
		xSide[startPt_i][startPt_j] = -2.0f;  //��ʶ�˵���׷�ٹ�
	}
	else
	{
		ySide[startPt_i][startPt_j] = -2.0f;//��ʶ�˵���׷�ٹ�
	}

	BOOL bFinished = FALSE;
	UINT rows = m_gridDataInfo.row;
	UINT cols = m_gridDataInfo.col;

	while(!bFinished) //�ж��Ƿ�׷����ϣ������ߵ���һ��һ���ڱ߽���
	{
		//׷�ٳ�NextPoint
		TracingNextPoint(); 

		PreviousPoint = CurrentPoint;
		CurrentPoint = NextPoint;

		bFinished = (CurrentPoint.i == 0 && CurrentPoint.bHorV == TRUE )  ||  /*�����ױ߽�,(ע:���ܽ�����CurrentPoint.i == 0�ж�)*/
			(CurrentPoint.i == rows-1)                            ||  /*�����ϱ߽�*/
			(CurrentPoint.j == 0 && CurrentPoint.bHorV == FALSE ) ||  /*������߽�,(ע:���ܽ�����CurrentPoint.j == 0�ж�)*/
			(CurrentPoint.j == cols-1) ;                              /*�����ұ߽�*/
	}
}


// �ж��Ƿ��ǵ�ֵ��
BOOL CContourTrace::IsContourPoint(float r)
{
	BOOL bFlag = FALSE;
	if ( r ==-2)
	{
		bFlag = FALSE;
	}
	else
	{
		if (r>1 || r<0)
		{
			bFlag = FALSE;
		}
		else
			bFlag = TRUE;
	}
	return bFlag;
}


// ׷����һ����
void CContourTrace::TracingNextPoint(void)
{
/*
	    ǰ��������������ж���
		if( ��ǰ��.�к� > ǰһ��.�к� )
		{
			��---->��
		}
		else if( ��ǰ��.�к� > ǰһ��.�к� )
		{
			��---->��
		}
		else if( ��ǰ���ں���� )
		{
			��---->��
		}
		else
		{
			��---->��
		}
		*/
	if(CurrentPoint.i > PreviousPoint.i )//��---->��
	{
		FromBottom2TopTracing();
	}
	else if (CurrentPoint.j > PreviousPoint.j )//��---->��
	{
		FromLeft2RightTracing();
	}
	else if ( CurrentPoint.bHorV == TRUE)//��---->��
	{
		ASSERT( CurrentPoint.i <= PreviousPoint.i &&CurrentPoint.j <= PreviousPoint.j );
		FromTop2BottomTracing();
	}
	else
	{
		ASSERT(CurrentPoint.bHorV==FALSE);//���ݱ���
		ASSERT( CurrentPoint.i <= PreviousPoint.i &&CurrentPoint.j <= PreviousPoint.j );

		FromRight2LeftTracing();
	}
}


// ���µ���׷��
void CContourTrace::FromBottom2TopTracing(void)
{
	ASSERT( CurrentPoint.i > PreviousPoint.i );   //��ǰ����к� > ǰһ����к�
	ASSERT( CurrentPoint.bHorV == TRUE );


	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;

	if(i>29)
	{
		i=29;
	}
	if( ySide[i][j] < ySide[i][j+1] )
	{
		if( ySide[i][j] > 0 )
			HandlingAfterNextPointFounded(i,j,FALSE);
		else
			HandlingAfterNextPointFounded(i,j+1,FALSE);
	}
	else if( ySide[i][j] == ySide[i][j+1] )
	{
		if( ySide[i][j] < 0 /*namely, ySide[i][j] == ySide[i][j+1] == -2.0*/)
		{
			HandlingAfterNextPointFounded(i+1,j,TRUE);
		}
		else
		{
			float xSideIJ = (m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i][j+1] - m_ppGridData[i][j] );
			if( xSideIJ <=0.5f )
			{
				HandlingAfterNextPointFounded(i,j,FALSE);
			}
			else
			{
				HandlingAfterNextPointFounded(i,j+1,FALSE);
			}
		}
	}
	else if( ySide[i][j] > ySide[i][j+1] )
	{
		if( ySide[i][j+1] > 0 )
			HandlingAfterNextPointFounded(i,j+1,FALSE);
		else
			HandlingAfterNextPointFounded(i,j,FALSE);		
	}
}


// ������
void CContourTrace::FromLeft2RightTracing(void)
{
	ASSERT( CurrentPoint.j > PreviousPoint.j );
	ASSERT( CurrentPoint.bHorV == FALSE );

	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;	

	if( xSide[i][j] < xSide[i+1][j] )
	{
		if( xSide[i][j] > 0 )
			HandlingAfterNextPointFounded(i,j,TRUE);
		else
			HandlingAfterNextPointFounded(i+1,j,TRUE);
	}
	else if( xSide[i][j] == xSide[i+1][j] )
	{
		if( xSide[i][j] < 0 /*namely, xSide[i][j] == xSide[i+1][j] == -2.0*/)
		{
			HandlingAfterNextPointFounded(i,j+1,FALSE);
		}
		else
		{		
			float ySideIJ = ( m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i+1][j] - m_ppGridData[i][j] );
			if( ySideIJ <= 0.5f )
				HandlingAfterNextPointFounded(i,j,TRUE);
			else
				HandlingAfterNextPointFounded(i+1,j,TRUE);
		}
	}
	else if( xSide[i][j] > xSide[i+1][j] )
	{
		if( xSide[i+1][j] > 0 )
			HandlingAfterNextPointFounded(i+1,j,TRUE);
		else
			HandlingAfterNextPointFounded(i,j,TRUE);

	}
}


// ���ϵ���׷��
void CContourTrace::FromTop2BottomTracing(void)
{
	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;



	//�Ƚϣ�ySide[i-1][j]      ySide[i-1][j+1]
	//                 
	//               xSide[i-1][j] 
	if (i == 0)
	{
		i = 1;
	}
	if( ySide[i-1][j] < ySide[i-1][j+1] )
	{
		if( ySide[i-1][j] > 0 )
			HandlingAfterNextPointFounded(i-1,j,FALSE);
		else
			HandlingAfterNextPointFounded(i-1,j+1,FALSE);
	}
	else if( ySide[i-1][j] == ySide[i-1][j+1] )
	{
		if( ySide[i-1][j] < 0 /*��ySide[i-1][j] == ySide[i-1][j+1] ==-2*/) 
		{
			HandlingAfterNextPointFounded(i-1,j,TRUE);//��һ���ڶ���ĺ����
		}
		else
		{

			//ע��:������Ҫ���¼���xSide[i][j]����Ϊ����һ���Ѿ�����Ϊ-2.0
			float xSideIJ = ( m_valueTracing - m_ppGridData[i][j] ) / ( m_ppGridData[i][j+1] - m_ppGridData[i][j] );
			if(  xSideIJ <= 0.5f )
			{
				HandlingAfterNextPointFounded(i-1,j,FALSE);
			}
			else
			{
				HandlingAfterNextPointFounded(i-1,j+1,FALSE);
			}
		}
	}
	else if( ySide[i-1][j] > ySide[i-1][j+1] )
	{
		if( ySide[i-1][j+1] > 0 )
			HandlingAfterNextPointFounded(i-1,j+1,FALSE);
		else
			HandlingAfterNextPointFounded(i-1,j,FALSE);
	}
}


// ���ҵ���
void CContourTrace::FromRight2LeftTracing(void)
{
	ASSERT( CurrentPoint.bHorV == FALSE );
	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;	
	if (j == 0)
	{
		j = 1;
	}

	if( xSide[i][j-1] < xSide[i+1][j-1] )
	{
		if( xSide[i][j-1] > 0 )
			HandlingAfterNextPointFounded(i,j-1,TRUE);
		else
			HandlingAfterNextPointFounded(i+1,j-1,TRUE);
	}
	else if( xSide[i][j-1] == xSide[i+1][j-1] )
	{
		if( xSide[i][j-1] < 0 /*namely, xSide[i][j-1] == xSide[i+1][j-1] == -2.0f*/ )
		{
			HandlingAfterNextPointFounded(i,j-1,FALSE);
		}
		else
		{
			float ySideIJ = ( m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i+1][j] - m_ppGridData[i][j] );
			if( ySideIJ <= 0.5f )
				HandlingAfterNextPointFounded(i,j-1,TRUE);
			else
				HandlingAfterNextPointFounded(i+1,j-1,TRUE);
		}
	}
	else if( xSide[i][j-1] > xSide[i+1][j-1] )
	{
		if( xSide[i+1][j-1] > 0 )
			HandlingAfterNextPointFounded(i+1,j-1,TRUE);
		else
			HandlingAfterNextPointFounded(i,j-1,TRUE);

	}
}


// //����һ����ֵ���ҵ�������Ӧ�Ĵ���,��������
void CContourTrace::HandlingAfterNextPointFounded(int i, int j, bool bHorizon)
{
	//����һ����ֵ���ҵ�������Ӧ�Ĵ���,���£�
	//1.��¼�õ�ֵ���i,j
	//2.���㲢����õ�ֵ�������
	//3.��־�õ�ֵ�����ڱߵ��Ѿ�������

	//��֤i��[0,rows-1], j��[0,cols-1]
	if(i<0||i>m_gridDataInfo.row-1||j<0||j>m_gridDataInfo.col-1)
	{
		Sleep(100);
	}
	ASSERT( i>=0 && i<=m_gridDataInfo.row-1 && j>=0 && j<=m_gridDataInfo.col-1 );

	//1.
	NextPoint.i = i;
	NextPoint.j = j;
	NextPoint.bHorV = bHorizon;

	//2.
	CalcAndSaveOnePointCoord(i,j,bHorizon,NextPoint.x,NextPoint.y);

	//3.
	if( NextPoint.bHorV )
	{
		xSide[i][j] = -2.0f; //�Ѿ�׷�ٹ�
	}
	else
	{	
		ySide[i][j] = -2.0f; //�Ѿ�׷�ٹ�
	}
}


// ׷��һ����ֵ��
void CContourTrace::TracingOneClosedContour(int startPt_i, int startPt_j)
{
	CCurve* pCtrLine = new CCurve;

	m_pCurveList->AddTail(pCtrLine);
	m_currentCurveLine = pCtrLine;

	//�����ߵĵ�һ���ڵ�Ĵ洢�����ߴ�����¶�ֵ
	ContourPoint point0;
	point0.x = m_valueTracing;
	point0.y = m_valueTracing;
	m_currentCurveLine->Add(point0);

	//����ǰһ��ֵ���i,j�����������������׷�ٵ�����
	PreviousPoint.i = startPt_i;
	PreviousPoint.j = 0;
	PreviousPoint.bHorV = FALSE;

	CurrentPoint.i = startPt_i;
	CurrentPoint.j = startPt_j;
	CurrentPoint.bHorV = FALSE;   //��FALSE����Ϊ�����ݱ���

	CalcAndSaveOnePointCoord(startPt_i,startPt_j,FALSE/*��ͷ���ݱ���*/,CurrentPoint.x,CurrentPoint.y);

	BOOL bColsed = FALSE;

	while(!bColsed) 
	{
		TracingNextPoint();

		PreviousPoint = CurrentPoint;
		CurrentPoint = NextPoint;

		//�õ�ֵ�����ڱߵı�ʶi,j���ж������Ƿ��գ�����x��y�������Ƚϣ���Ϊ�����������������
		//	bColsed = (CurrentPoint.x == startPt_x) && (CurrentPoint.y == startPt_y);
		bColsed = ( CurrentPoint.i == startPt_i ) && 
			( CurrentPoint.j == startPt_j ) && 
			( CurrentPoint.bHorV == FALSE );
	}

	ySide[startPt_i][startPt_j] = -2.0f; //�Ѿ�׷�ٹ�
}

// �ͷ��ڴ�
void CContourTrace::FreeMemory(float**& pData)
{
	if(pData)
	{//�ͷ�˳��::һάָ��-->��άָ��

		delete []pData[0];
		pData[0]=NULL;

		delete []pData;
		pData=NULL;
	}
}
