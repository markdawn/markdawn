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


// 追踪等值线
BOOL CContourTrace::ExecuteTracing(float contourValue)
{
	//1.产生等温值点
	//2.追踪等温值点
	//3.等值线搜索
	ASSERT( m_gridDataInfo.col != 0 ); //在调用该函数之前必须调用SetGridDataInfo()函数设置网格数据信息

	if( contourValue < m_gridDataInfo.minValue && contourValue > m_gridDataInfo.maxValue )
	{
		return FALSE;
	}

	m_valueTracing = contourValue;


	//1.为xSide和ySide分配内存空间
	AllocateMemory();

	//2.扫描网格纵横边，内插等值点
	//该函数之中，在计算等值点时，发现追踪值与网格点上的数据相同时，
	//在计算前，会对数据做修正(加上一小偏移值)

	// 追踪等值点
	InterpolateTracingValue(); 

	//3.先追踪开曲线
	TracingNonClosedContour();

	//4.再追踪闭曲线
	TracingClosedContour();

	return TRUE;
}


// 获得网格信息
void CContourTrace::GetGridBasicInfo(GridBaiscInfo dataInfo)
{
	m_gridDataInfo = dataInfo;
}


// 得到栅格数据
void CContourTrace::GetGridData(float** ppGridData)
{
	ASSERT( ppGridData != NULL );
	m_ppGridData = ppGridData;
}

//分配内存
void CContourTrace::AllocateMemory(void)
{
	UINT cols = m_gridDataInfo.col;
	UINT rows = m_gridDataInfo.row;

	if (xSide == NULL)
	{
		//网格中存在rows*(cols-1)条横边，所有需要为xSide分配rows*(cols-1)空间就行
		ASSERT (AllocMemory(xSide,rows,cols-1)== TRUE);
	}

	if (ySide == NULL)
	{
		//网格中存在(rows-1)*cols条纵边，所有需要为ySide分配(rows-1)*cols空间就行
		ASSERT (AllocMemory(ySide,rows-1,cols)==TRUE);
	}
}


// 追踪等值点
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

	float H1 = 0.0f, H2 = 0.0f;//记录一条边上两个点的数值
	float shift = 0.001f; //修正值
	float flag = 0.0f;
  /*******************-2标识等值点没有在边上******************************************/

	//扫描并计算横边上的等值点：row*（cols-1）
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
				flag = (currentValue-H1)*(currentValue - H2);//判断是否在边上
				if (flag < 0) //在边上
				{
					xSide[i][j] = (currentValue- H1)/(H2-H1);
					ASSERT(xSide[i][j]>=0&&xSide[i][j]<=1.0f);
				}
				else if (flag > 0) //不在边上
				{
					xSide[i][j] = -2;
				}
				else if(flag == 0) //等值点与网格点相等，需要修正
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

	//扫描并计算纵边上的等值点：（row-1）*cols
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


// 追踪所有曲线,从四个边界开始追踪开曲线
void CContourTrace::TracingNonClosedContour(void)
{
	int i = 0,j = 0;
	int cols = m_gridDataInfo.col;
	int rows = m_gridDataInfo.row;	

	//1.搜索底边框上的等值线
	for (j = 0; j <cols-1;j++)
	{
		if (IsContourPoint(xSide[0][j]))//如果横边上有等值点
		{
			/*按自下向上的前进方向虚设前一点的i,j*/
			PreviousPoint.i = -1; // 假设前一点在-1行,这样下一点到当前点的前进方向就是自下向上的
			PreviousPoint.j = j;
			PreviousPoint.bHorV = TRUE;

			CurrentPoint.i = 0; //底边的行号为0，所以设置线头的i为0
			CurrentPoint.j = j;
			CurrentPoint.bHorV = TRUE; //底边是横边

			TracingOneNonClosedContour();	
		}
	}

	//2.搜索左边框上的等值线
	for (i=0;i<rows-1;i++)
	{
		if (IsContourPoint(ySide[i][0]))
		{
			//自左向右虚设有点，
			PreviousPoint.i = i;  
			PreviousPoint.j = -1; //虚设一列使其按自左向右的方向搜索
			PreviousPoint.bHorV = FALSE;//左边框是纵边

			CurrentPoint.i = i;
			CurrentPoint.j = 0;
			CurrentPoint.bHorV = FALSE;
			TracingOneNonClosedContour();	

		}
	}
	//3.搜索上边框上的等值线
	for (j = 0; j <cols-1;j++)
	{
		if (IsContourPoint(xSide[rows-1][j]))//如果上边框上有等值点
		{
			/*按自上向下的前进方向虚设前一点的i,j*/
			PreviousPoint.i = rows-1;
			PreviousPoint.j = j;
			PreviousPoint.bHorV = TRUE;

			CurrentPoint.i = rows-1; //
			CurrentPoint.j = j;
			CurrentPoint.bHorV = TRUE;

			TracingOneNonClosedContour();	
		}
	}

	//4.搜索右边框上的等值线
	for (i=0;i<rows-1;i++)
	{
		if (IsContourPoint(ySide[i][cols-1]))
		{
			//自右向左虚设有一点，
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


// 追踪闭曲线
void CContourTrace::TracingClosedContour(void)
{
	int cols = m_gridDataInfo.col;
	int rows = m_gridDataInfo.row;

	//搜索所有的除了边框外的纵边,从左到右搜索每一列上的纵边，对于一列，从下到上搜索
	for(int j=1; j<cols-1; j++) //j从1开始
		for(int i=0; i<rows-1; i++) //i从0开始
		{
			if( IsContourPoint(ySide[i][j])) 
			{
				TracingOneClosedContour(i,j);				
			}

		}
}


// 分配内存
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


// 计算等值点坐标
void CContourTrace::CalcAndSaveOnePointCoord(int i, int j, BOOL bHorizon, float& x, float& y)
{
	 float deltX = m_gridDataInfo.width/ ( m_gridDataInfo.col - 1 );
	 float deltY = m_gridDataInfo.hight / ( m_gridDataInfo.row - 1 );

	 //////////////////////暂时///////////////
	 float nx = 0.0f; //横边x最小坐标
	 float ny = 0.0f;//纵边y最小坐标

	 if (bHorizon)//横边
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


// 追踪一个开曲线
void CContourTrace::TracingOneNonClosedContour(void)
{
	CCurve* pCtrLine = new CCurve;
	m_pCurveList->AddTail(pCtrLine);
	m_currentCurveLine = pCtrLine;
	//将曲线的第一个节点的存储该曲线代表的温度值
	ContourPoint point0;
	point0.x = m_valueTracing;
	point0.y = m_valueTracing;
	m_currentCurveLine->Add(point0);


	//记录下线头所在边的i，j，横纵边标识
	int startPt_i = CurrentPoint.i; 
	int startPt_j = CurrentPoint.j;
	BOOL startPt_bHoriz = CurrentPoint.bHorV;

	//验证线头在边界上
	ASSERT( startPt_i == 0                     ||  /*线头在底边界上*/
		startPt_i == m_gridDataInfo.row-1 ||  /*线头在上边界上*/
		startPt_j == 0                     ||  /*线头在左边界上*/
		startPt_j == m_gridDataInfo.col-1     /*线头在右边界上*/ 
		);
	//计算出线头的坐标并保存
	CalcAndSaveOnePointCoord(startPt_i,startPt_j,startPt_bHoriz,CurrentPoint.x,CurrentPoint.y); 

	if( startPt_bHoriz )
	{
		xSide[startPt_i][startPt_j] = -2.0f;  //标识此点已追踪过
	}
	else
	{
		ySide[startPt_i][startPt_j] = -2.0f;//标识此点已追踪过
	}

	BOOL bFinished = FALSE;
	UINT rows = m_gridDataInfo.row;
	UINT cols = m_gridDataInfo.col;

	while(!bFinished) //判断是否追踪完毕，开曲线的另一端一定在边界上
	{
		//追踪出NextPoint
		TracingNextPoint(); 

		PreviousPoint = CurrentPoint;
		CurrentPoint = NextPoint;

		bFinished = (CurrentPoint.i == 0 && CurrentPoint.bHorV == TRUE )  ||  /*遇到底边界,(注:不能仅仅用CurrentPoint.i == 0判断)*/
			(CurrentPoint.i == rows-1)                            ||  /*遇到上边界*/
			(CurrentPoint.j == 0 && CurrentPoint.bHorV == FALSE ) ||  /*遇到左边界,(注:不能仅仅用CurrentPoint.j == 0判断)*/
			(CurrentPoint.j == cols-1) ;                              /*遇到右边界*/
	}
}


// 判断是否是等值点
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


// 追踪下一个点
void CContourTrace::TracingNextPoint(void)
{
/*
	    前进方向可以如下判定：
		if( 当前点.行号 > 前一点.行号 )
		{
			下---->上
		}
		else if( 当前点.列号 > 前一点.列号 )
		{
			左---->右
		}
		else if( 当前点在横边上 )
		{
			上---->下
		}
		else
		{
			右---->左
		}
		*/
	if(CurrentPoint.i > PreviousPoint.i )//下---->上
	{
		FromBottom2TopTracing();
	}
	else if (CurrentPoint.j > PreviousPoint.j )//左---->右
	{
		FromLeft2RightTracing();
	}
	else if ( CurrentPoint.bHorV == TRUE)//上---->下
	{
		ASSERT( CurrentPoint.i <= PreviousPoint.i &&CurrentPoint.j <= PreviousPoint.j );
		FromTop2BottomTracing();
	}
	else
	{
		ASSERT(CurrentPoint.bHorV==FALSE);//在纵边上
		ASSERT( CurrentPoint.i <= PreviousPoint.i &&CurrentPoint.j <= PreviousPoint.j );

		FromRight2LeftTracing();
	}
}


// 从下到上追踪
void CContourTrace::FromBottom2TopTracing(void)
{
	ASSERT( CurrentPoint.i > PreviousPoint.i );   //当前点的行号 > 前一点的行号
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


// 从左到右
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


// 从上到下追踪
void CContourTrace::FromTop2BottomTracing(void)
{
	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;



	//比较：ySide[i-1][j]      ySide[i-1][j+1]
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
		if( ySide[i-1][j] < 0 /*即ySide[i-1][j] == ySide[i-1][j+1] ==-2*/) 
		{
			HandlingAfterNextPointFounded(i-1,j,TRUE);//下一点在对面的横边上
		}
		else
		{

			//注意:这里需要重新计算xSide[i][j]，因为在上一次已经被置为-2.0
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


// 从右到左
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


// //当下一个等值点找到后做相应的处理,计算坐标
void CContourTrace::HandlingAfterNextPointFounded(int i, int j, bool bHorizon)
{
	//当下一个等值点找到后做相应的处理,如下：
	//1.记录该等值点的i,j
	//2.计算并保存该等值点的坐标
	//3.标志该等值点所在边的已经搜索过

	//验证i∈[0,rows-1], j∈[0,cols-1]
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
		xSide[i][j] = -2.0f; //已经追踪过
	}
	else
	{	
		ySide[i][j] = -2.0f; //已经追踪过
	}
}


// 追踪一条等值线
void CContourTrace::TracingOneClosedContour(int startPt_i, int startPt_j)
{
	CCurve* pCtrLine = new CCurve;

	m_pCurveList->AddTail(pCtrLine);
	m_currentCurveLine = pCtrLine;

	//将曲线的第一个节点的存储该曲线代表的温度值
	ContourPoint point0;
	point0.x = m_valueTracing;
	point0.y = m_valueTracing;
	m_currentCurveLine->Add(point0);

	//虚设前一等值点的i,j，让其满足从左向右追踪的条件
	PreviousPoint.i = startPt_i;
	PreviousPoint.j = 0;
	PreviousPoint.bHorV = FALSE;

	CurrentPoint.i = startPt_i;
	CurrentPoint.j = startPt_j;
	CurrentPoint.bHorV = FALSE;   //是FALSE，因为是在纵边上

	CalcAndSaveOnePointCoord(startPt_i,startPt_j,FALSE/*线头在纵边上*/,CurrentPoint.x,CurrentPoint.y);

	BOOL bColsed = FALSE;

	while(!bColsed) 
	{
		TracingNextPoint();

		PreviousPoint = CurrentPoint;
		CurrentPoint = NextPoint;

		//用等值点所在边的标识i,j来判断曲线是否封闭，不用x、y坐标来比较，因为浮点数计算会出现误差
		//	bColsed = (CurrentPoint.x == startPt_x) && (CurrentPoint.y == startPt_y);
		bColsed = ( CurrentPoint.i == startPt_i ) && 
			( CurrentPoint.j == startPt_j ) && 
			( CurrentPoint.bHorV == FALSE );
	}

	ySide[startPt_i][startPt_j] = -2.0f; //已经追踪过
}

// 释放内存
void CContourTrace::FreeMemory(float**& pData)
{
	if(pData)
	{//释放顺序::一维指针-->二维指针

		delete []pData[0];
		pData[0]=NULL;

		delete []pData;
		pData=NULL;
	}
}
