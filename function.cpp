#include "function.h"
#include "UAV2022.h"

struct MyPoint11 {
	int x;
	int y;
};

struct struct_area {
	int height;
	int width;
	int number;
	double contrast;

	// 	
	int centeri;
	int upi;
	int upj;
	int downi;
	int downj;
	int lefti;
	int leftj;
	int righti;
	int rightj;
	int length;

	int leftdi;
	int leftdj;
	int rightdi;
	int rightdj;

};

void FindSingleTarget(unsigned char * img0, int height, int width, vector<int>& Single_RowPos, vector<int>& Single_ColPos)
{
	const int w = width;
	const int h = height;
	float sumrows[480] = { 0 };
	float sumrows1[480] = { 0 };
	float sumrows2[480] = { 0 };
	float sumcols[640] = { 0 };
	float sumcols1[640] = { 0 };
	float sumcols2[640] = { 0 };

	int i = 0, j = 0;
	int pointx = 0, pointy = 0;

	ofstream outfile;
	outfile.open(".\\tmp\\单目标水平投影.txt");

	/**************水平投影*****************/

	for (i = 0; i < height; i++)//水平投影
	{
		for (j = 0; j < width; j++)
		{
			sumrows[i] += (float)img0[i*width + j] / width;
		}

		outfile << sumrows[i] << endl;
		if (i == 0)
			continue;
		if (sumrows[i] != 0)
			Single_RowPos.push_back(i);
	}



	/****************竖直投影****************/
	ofstream outfile1;
	outfile1.open(".\\tmp\\单目标竖直投影.txt");
	for (i = 0; i < width; i++)//竖直投影
	{
		for (j = 0; j < height; j++)
		{
			sumcols[i] += (float)img0[j*width + i] / height;
		}
		outfile1 << sumcols[i] << endl;
		if (sumcols[i] != 0)
			Single_ColPos.push_back(i);
	}
}

//阈值分割
int Otsu(Mat& src)
{
	int height = src.rows;
	int width = src.cols;
	long size = height * width;

	//histogram
	float histogram[256] = { 0 };
	for (int m = 0; m < height; m++)
	{
		//unsigned char* p = (unsigned char*)src->imageData + src->widthStep * m;
		for (int n = 0; n < width; n++)
		{
			int p = src.at<uchar>(m, n);
			histogram[p]++;
		}
	}

	int threshold;
	long sum0 = 0, sum1 = 0; //存储前景的灰度总和和背景灰度总和
	long cnt0 = 0, cnt1 = 0; //前景的总个数和背景的总个数
	double w0 = 0, w1 = 0; //前景和背景所占整幅图像的比例
	double u0 = 0, u1 = 0;  //前景和背景的平均灰度
	double variance = 0; //最大类间方差
	int i, j;
	double u = 0;
	double maxVariance = 0;
	for (i = 1; i < 256; i++) //一次遍历每个像素
	{
		sum0 = 0;
		sum1 = 0;
		cnt0 = 0;
		cnt1 = 0;
		w0 = 0;
		w1 = 0;
		for (j = 0; j < i; j++)
		{
			cnt0 += histogram[j];
			sum0 += j * histogram[j];
		}

		u0 = (double)sum0 / cnt0;
		w0 = (double)cnt0 / size;

		for (j = i; j <= 255; j++)
		{
			cnt1 += histogram[j];
			sum1 += j * histogram[j];
		}

		u1 = (double)sum1 / cnt1;
		w1 = 1 - w0; // (double)cnt1 / size;

		u = u0 * w0 + u1 * w1; //图像的平均灰度
							   //printf("u = %f\n", u);
							   //ariance =  w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		variance = w0 * w1 *  (u0 - u1) * (u0 - u1);
		if (variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}

	//printf("threshold = %d\n", threshold);
	return threshold;
}

void MedFilterImage(int ergodic, int *m, int *n)
{
	for (int i = 20; i < 360 / ergodic + 20; i++)
	{
		n[i] = (m[i - 1] + m[i] + m[i + 1] + m[i + 2] + m[i - 2] + m[i + 3] + m[i - 3] + m[i + 4] + m[i - 4] + m[i + 5] + m[i - 5]) / 11;
		// n[i]=(m[i-1]+m[i]+m[i+1]+m[i+2]+m[i-2]+m[i+3]+m[i-3])/7;
		//n[i]=(m[i-1]+m[i]+m[i+1])/3;
		//n[i] = (m[i - 1] + m[i] + m[i + 1] + m[i + 2] + m[i - 2] + m[i + 3] + m[i - 3] + m[i + 4] + m[i - 4] ) / 9;
	}
	for (int i = 0; i < 20; i++)
	{
		n[(360 / ergodic + 20) + i] = n[20 + i];
		n[0 + i] = n[ergodic + i];
	}
}


void cir(double *TH, int *n, int ergodic)
{
	double AAAA = 0;
	double BBBB = 0;
	double RRRR = 0;
	double x[7] = { 0 };
	double y[7] = { 0 };
	double *Px = x;
	double *Py = y;
	//int RR[360]={0};
	ofstream outfile;
	outfile.open(".\\tmp\\曲率.txt");
	for (int ii = 20; ii < ((360 / ergodic) + 20); ii++)
	{
		x[0] = ii - 6;
		x[1] = ii - 3;
		x[2] = ii;
		x[3] = ii + 3;
		x[4] = ii + 6;

		y[0] = n[ii - 6];
		y[1] = n[ii - 3];
		y[2] = n[ii];
		y[3] = n[ii + 3];
		y[4] = n[ii + 6];
		CirR(Px, Py, 5, AAAA, BBBB, RRRR);

		if (RRRR == 0)
		{
			TH[ii - 20] = 0;
			outfile << 0 << endl;
			//cout << "第" << ii - 20 << "度：" << 0 << endl;
		}
		else
		{
			TH[ii - 20] = 1 / RRRR;
			outfile << 1 / RRRR << endl;
			//cout << "第" << ii - 20 << "度：" << 1 / RRRR << endl;
		}
		//
		//  cout<<"第"<<ii-20<<"度："<<BBBB<<endl;
	}
	outfile.close();
}


void CirR(double * Px, double * Py, int changdu, double & AAAA, double & BBBB, double & RRRR)
{
	double X1, X2, X3, Y1, Y2, Y3;
	double a, b，r;//圆心坐标半径
	double **shuzu = new double*[3];
	for (int i = 0; i < 3; ++i)
	{
		shuzu[i] = new double[changdu];
	}

	int NUM = 0;
	for (NUM = 0; NUM < changdu; NUM++)
	{
		shuzu[0][NUM] = NUM;
		shuzu[1][NUM] = Px[NUM];
		shuzu[2][NUM] = Py[NUM];

	}
	int Max_number = 2; int number;
	double count[3] = { 0 };
	double zuiyoujie[4] = { 0 };
	zuiyoujie[3] = 100;
	int k1 = 0;
	int k2 = 0;
	int k3 = 0;
	double x = 0;
	double y = 0;
	double point, distance;
	double aa, bb, rr;
	double distancesum;
	for (k1 = 0; k1 < changdu - 2; k1++)
	{
		for (k2 = k1 + 1; k2 < changdu - 1; k2++)
		{
			for (k3 = k2 + 1; k3 < changdu; k3++)
			{
				distancesum = 0;
				X1 = shuzu[1][k1];
				Y1 = shuzu[2][k1];

				X2 = shuzu[1][k2];
				Y2 = shuzu[2][k2];

				X3 = shuzu[1][k3];
				Y3 = shuzu[2][k3];
				if (((X2 - X1)*(Y3 - Y1) - (X3 - X1)*(Y2 - Y1)) != 0)
				{
					double  a = ((X2*X2 + Y2 * Y2 - (X1*X1 + Y1 * Y1)) * 2 * (Y3 - Y1) - (X3*X3 + Y3 * Y3 - (X1*X1 + Y1 * Y1)) * 2 * (Y2 - Y1)) / (4 * ((X2 - X1)*(Y3 - Y1) - (X3 - X1)*(Y2 - Y1)));
					double  b = (2 * (X2 - X1)*(X3*X3 + Y3 * Y3 - (X1*X1 + Y1 * Y1)) - 2 * (X3 - X1)*(X2*X2 + Y2 * Y2 - (X1*X1 + Y1 * Y1))) / (4 * ((X2 - X1)*(Y3 - Y1) - (X3 - X1)*(Y2 - Y1)));
					double  r = sqrt((double)(X1 - a)*(X1 - a) + (double)(Y1 - b)*(Y1 - b));
					//   double  r = (double)((X1-a)*(X1-a)+(Y1-b)*(Y1-b));
					count[0] = a;
					count[1] = b;
					count[2] = r;
					//  cout<<mmm<<":"<<r<<endl;
					//    mmm++;
				}
				else
				{
					count[2] = 0;
					// cout<<mmm<<":"<<count[2]<<endl;
					//  mmm++;
					continue;
				}
				//////////////
				for (int i = 0; i < changdu; i++)
				{
					aa = count[0];
					bb = count[1];
					rr = count[2];
					x = shuzu[1][i];
					y = shuzu[2][i];
					point = (double)sqrt((x - aa)*(x - aa) + (y - bb)*(y - bb));
					distance = abs(point - rr);
					distancesum = distancesum + distance;
				}
				if (distancesum < zuiyoujie[3])
				{
					zuiyoujie[0] = rr;
					zuiyoujie[1] = aa;
					zuiyoujie[2] = bb;
					zuiyoujie[3] = distancesum;
				}
			}
		}
	}

	AAAA = zuiyoujie[1];
	BBBB = zuiyoujie[2];
	RRRR = zuiyoujie[0];
	// RRRR=sqrt(RRRR);
	for (int i = 0; i < 3; ++i)
		delete[] shuzu[i];
	delete[] shuzu;
}


vector<int> Max_Cur(int n, double *TH)
{
	vector<int> cur;
	int count = n;
	double Temp_TH[401];
	for (int i = 0; i < 401; i++)
	{
		Temp_TH[i] = TH[i];
	}

	while (count)
	{
		double maxcur = Temp_TH[0];
		int index = 0;
		for (int i = 0; i < 360; i++)
		{
			if (maxcur < Temp_TH[i])
			{
				maxcur = Temp_TH[i];
				index = i;
			}
		}
		cur.push_back(index);
		if (index >= 5)
		{
			Temp_TH[index - 5] = Temp_TH[index - 4] = Temp_TH[index - 3] = Temp_TH[index - 2] = Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else if (index == 4)
		{
			Temp_TH[index - 4] = Temp_TH[index - 3] = Temp_TH[index - 2] = Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else if (index == 3)
		{
			Temp_TH[index - 3] = Temp_TH[index - 2] = Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else if (index == 2)
		{
			Temp_TH[index - 2] = Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else if (index == 1)
		{
			Temp_TH[index - 1] = Temp_TH[index] = 0;
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = 0;
		}
		else
		{
			Temp_TH[index + 5] = Temp_TH[index + 4] = Temp_TH[index + 3] = Temp_TH[index + 2] = Temp_TH[index + 1] = Temp_TH[index] = 0;
		}
		count--;
	}

	sort(cur.begin(), cur.end(), less<int>());
	
	return cur;
	//return vector<int>();
}


vector<row_roi> FindTarget(vector<int>& cur, int *n)
{
	vector<row_roi> target_point;
	int sum = 0;
	int avg = 0;
	for (int i = 0; i < cur.size(); i++)
	{
		sum += n[cur[i] + 20];
	}

	avg = sum / cur.size();

	for (int i = 0; i < cur.size(); i++)
	{

		if (n[cur[i] + 20] > avg)
		{
			row_roi temp;
			temp.angle = cur[i];
			temp.roi = n[cur[i] + 20];
			target_point.push_back(temp);
		}
	}

	return target_point;
	//return vector<row_roi>();
}


void Diff2frame(Mat &mat1, Mat &mat2, Mat &result)
{
	int width = mat1.cols;
	int height = mat1.rows;
	Mat greyFrame1, greyFrame2, tempresult;

	//转为单通道
	cvtColor(mat1, greyFrame1, CV_BGR2GRAY);
	cvtColor(mat2, greyFrame2, CV_BGR2GRAY);
	tempresult.create(greyFrame1.size(), greyFrame1.type());

	absdiff(greyFrame1, greyFrame2, result);	// per-element absolute difference

	imwrite(".\\tmp\\帧间差分原始结果.jpg", result);

	int thre = 20;
	int greyMin, greyMax;
	greyMax = greyMin = 0;

	//求取两帧相减后图像的灰度最大值和最小值
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (result.at<uchar>(i, j) > greyMax)
				greyMax = result.at<uchar>(i, j);

			if (result.at<uchar>(i, j) < greyMin)
				greyMin = result.at<uchar>(i, j);
		}


	double greySum = 0, greyAverage = 0;	// greysum: 图像每个像素点的总灰度值
	//归一化 (归一化的作用是什么？）
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			double m = (result.at<uchar>(i, j) - greyMin) * 255 / (greyMax - greyMin);	// .at方法用于获取图像矩阵某点的值
			result.at<uchar>(i, j) = m;
			greySum += m;
		}

	imwrite(".\\tmp\\归一化结果.jpg", result);

	//平均灰度
	greyAverage = greySum / width / height;

	//根据平均灰度阈值转二值图//信息增强且去除微弱变换的杂质点
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			//边界直接忽略  全部赋值为0
			if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
				result.at<uchar>(i, j) = 0;

			if (result.at<uchar>(i, j) >= 20 * greyAverage)     //15
				result.at<uchar>(i, j) = 255;
			else
				result.at<uchar>(i, j) = 0;
		}

	imwrite(".\\tmp\\归一化二值结果.jpg", result);


	ConnectDenoize2(result, height, width, 5);

	imwrite(".\\tmp\\帧间差分结果.jpg", result);


}

// 改进的分配一个动态二维数组的函数，它保持一二维数据的兼容性
// 返回动态二维数组指针
void** fspace_2d(int nRow, int nCol,	// 数组大小，行列
	short typeSize		// 类型大小（字节数目）
)
{
	void **ppPtr = (void **)calloc(nRow, sizeof(void *));
	//void **ppPtr = (void **)(new byte[nRow*sizeof(void *)]);	//modify
	void *pTmp = (void *)calloc(nRow*nCol, typeSize);	// 所有的数据分成一个连续区域，
														//void *pTmp	 = (void *)(new byte[nRow*nCol*typeSize]);	//modify

	if (ppPtr == NULL && pTmp == NULL)
	{
		printf("not enough space:%d\n", nRow);
		return NULL;
	}
	char* pT = (char*)pTmp;
	for (int i = 0; i < nRow; i++)
	{
		ppPtr[i] = (void*)(pT + i * nCol*typeSize);// 指向每一行的开始地址(字节地址)。
	}

	return(ppPtr);
}

// 改进的释放一个动态二维数组，内部提供检查
void ffree_2d(void **ppArr,		// 动态二维数组指针
	int nRow			// 行数
)
{
	if (ppArr != NULL && ppArr[0] != NULL)
	{
		if (sizeof(ppArr[0]) <= 0)
		{
			printf("\nWarning : 堆栈出错,没释放内存!\n");
			return;
		}
		free(ppArr[0]);
		free(ppArr);
		//byte* ptemp = (byte *)ppArr[0];	//modify
		//delete[] ptemp;	//modify
		/*
		if(ppArr != NULL && sizeof(ppArr) >= (unsigned int)nRow)
		{
		free(ppArr);
		//ptemp = (byte*)ppArr;	//modify
		//delete[] ptemp;	//modify
		ppArr = NULL;
		}
		else
		{
		printf("\nWarning : 堆栈出错,释放部分内存!\n");
		}
		*/
	}
}

// 连通域去噪 [12/28/2010 wit]
bool ConnectDenoize2(Mat &src, int row, int col, int LIMIT)
{
	//Mat转 double **InImage
	double **InImage = (double **)fspace_2d(row, col, sizeof(double));
	double **OutImage = (double **)fspace_2d(row, col, sizeof(double));
	for (int m = 0; m < row; m++)
		for (int n = 0; n < col; n++)
			InImage[m][n] = src.at<uchar>(m, n);

	int i, j, k;
	int nDx[] = { -1,-1,-1,0,1,1,1,0 };
	int nDy[] = { -1,0,1,1,1,0,-1,-1 };
	int nStart = 0, nEnd = 0, nCurrX, nCurrY, xx, yy, m = 1;

	MyPoint11 *pnGrowQue = new MyPoint11[row*col];
	struct_area *parea_inf = new struct_area[row*col]; // 连通域信息
	int* pUnRegion = new int[row*col];  // 标记
	int* pUnRegion1 = new int[row*col];
	double **ExpImage = (double **)fspace_2d(row, col, sizeof(double));

	for (i = 0; i < row; i++)
	{

		for (j = 0; j < col; j++)
		{
			parea_inf[i*col + j].number = 0;
			pUnRegion[i*col + j] = 0;
			pUnRegion1[i*col + j] = 0;
			pnGrowQue[i*col + j].x = 0;
			pnGrowQue[i*col + j].y = 0;
			ExpImage[i][j] = InImage[i][j];
		}
	}

	// 找增长图
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			nStart = 0; nEnd = 0;
			// 如果满足条件，开始找该点连通域
			if (InImage[i][j] > 0 && pUnRegion[i*col + j] == 0)
			{
				pnGrowQue[nEnd].x = j;
				pnGrowQue[nEnd].y = i;
				pUnRegion[i*col + j] = m;

				// 找当前点连通域
				while (nStart <= nEnd)
				{
					nCurrX = pnGrowQue[nStart].x;
					nCurrY = pnGrowQue[nStart].y;


					// 找八邻域
					for (k = 0; k < 8; k++)
					{
						xx = nCurrX + nDx[k];
						yy = nCurrY + nDy[k];
						if (xx<0 || yy<0 || xx>col - 1 || yy>row - 1)continue;
						if (pUnRegion[yy*col + xx] == 0 && ExpImage[yy][xx] > 0)
						{
							nEnd++;
							pnGrowQue[nEnd].x = xx;
							pnGrowQue[nEnd].y = yy;
							pUnRegion[yy*col + xx] = m;
							parea_inf[pUnRegion[yy*col + xx]].number++;
						}

					}
					nStart++;
				}

				m++;
			}
		}
	}


	// 去噪
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			OutImage[i][j] = InImage[i][j];
		}
	}



	int k1;
	for (k = 0; k < row*col; k++)
	{
		i = k / col;
		j = k % col;
		k1 = pUnRegion[i*col + j];

		if (k1 == 0)	continue;

		// 如果连通域内在inimage1图上有白的则保留
		if (parea_inf[k1].number < LIMIT)    //按像素个数
		{
			OutImage[i][j] = 0;
			parea_inf[k1].number = 0;
		}


	}

	for (int m = 0; m < row; m++)
		for (int n = 0; n < col; n++)
			src.at<uchar>(m, n) = OutImage[m][n];

	delete[] pnGrowQue;
	delete[] parea_inf;
	delete[] pUnRegion;
	delete[] pUnRegion1;
	ffree_2d((void **)ExpImage, row);

	return true;
}




//求取运动区域的灰度平均值
int PloarGreyaverage(Mat &src)
{
	int thre;
	int width = src.cols;
	int height = src.rows;

	int sum = 0;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			sum += src.at<uchar>(i, j);
		}

	thre = floor(sum / width / height);

	return thre;
}