#include "function.h"

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
	outfile.open(".\\tmp\\��Ŀ��ˮƽͶӰ.txt");

	/**************ˮƽͶӰ*****************/

	for (i = 0; i < height; i++)//ˮƽͶӰ
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



	/****************��ֱͶӰ****************/
	ofstream outfile1;
	outfile1.open(".\\tmp\\��Ŀ����ֱͶӰ.txt");
	for (i = 0; i < width; i++)//��ֱͶӰ
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

//��ֵ�ָ�
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
	long sum0 = 0, sum1 = 0; //�洢ǰ���ĻҶ��ܺͺͱ����Ҷ��ܺ�
	long cnt0 = 0, cnt1 = 0; //ǰ�����ܸ����ͱ������ܸ���
	double w0 = 0, w1 = 0; //ǰ���ͱ�����ռ����ͼ��ı���
	double u0 = 0, u1 = 0;  //ǰ���ͱ�����ƽ���Ҷ�
	double variance = 0; //�����䷽��
	int i, j;
	double u = 0;
	double maxVariance = 0;
	for (i = 1; i < 256; i++) //һ�α���ÿ������
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

		u = u0 * w0 + u1 * w1; //ͼ���ƽ���Ҷ�
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

void Diff2frame(Mat &mat1, Mat &mat2, Mat &result)
{
	int width = mat1.cols;
	int height = mat1.rows;
	Mat greyFrame1, greyFrame2, tempresult;

	//תΪ��ͨ��
	cvtColor(mat1, greyFrame1, CV_BGR2GRAY);
	cvtColor(mat2, greyFrame2, CV_BGR2GRAY);
	tempresult.create(greyFrame1.size(), greyFrame1.type());

	absdiff(greyFrame1, greyFrame2, result);	// per-element absolute difference

	imwrite(".\\tmp\\֡����ԭʼ���.jpg", result);

	int thre = 20;
	int greyMin, greyMax;
	greyMax = greyMin = 0;

	//��ȡ��֡�����ͼ��ĻҶ����ֵ����Сֵ
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (result.at<uchar>(i, j) > greyMax)
				greyMax = result.at<uchar>(i, j);

			if (result.at<uchar>(i, j) < greyMin)
				greyMin = result.at<uchar>(i, j);
		}


	double greySum = 0, greyAverage = 0;	// greysum: ͼ��ÿ�����ص���ܻҶ�ֵ
	//��һ�� (��һ����������ʲô����
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			double m = (result.at<uchar>(i, j) - greyMin) * 255 / (greyMax - greyMin);	// .at�������ڻ�ȡͼ�����ĳ���ֵ
			result.at<uchar>(i, j) = m;
			greySum += m;
		}

	imwrite(".\\tmp\\��һ�����.jpg", result);

	//ƽ���Ҷ�
	greyAverage = greySum / width / height;

	//����ƽ���Ҷ���ֵת��ֵͼ//��Ϣ��ǿ��ȥ��΢���任�����ʵ�
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			//�߽�ֱ�Ӻ���  ȫ����ֵΪ0
			if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
				result.at<uchar>(i, j) = 0;

			if (result.at<uchar>(i, j) >= 20 * greyAverage)     //15
				result.at<uchar>(i, j) = 255;
			else
				result.at<uchar>(i, j) = 0;
		}

	imwrite(".\\tmp\\��һ����ֵ���.jpg", result);


	ConnectDenoize2(result, height, width, 5);

	imwrite(".\\tmp\\֡���ֽ��.jpg", result);


}

// �Ľ��ķ���һ����̬��ά����ĺ�����������һ��ά���ݵļ�����
// ���ض�̬��ά����ָ��
void** fspace_2d(int nRow, int nCol,	// �����С������
	short typeSize		// ���ʹ�С���ֽ���Ŀ��
)
{
	void **ppPtr = (void **)calloc(nRow, sizeof(void *));
	//void **ppPtr = (void **)(new byte[nRow*sizeof(void *)]);	//modify
	void *pTmp = (void *)calloc(nRow*nCol, typeSize);	// ���е����ݷֳ�һ����������
														//void *pTmp	 = (void *)(new byte[nRow*nCol*typeSize]);	//modify

	if (ppPtr == NULL && pTmp == NULL)
	{
		printf("not enough space:%d\n", nRow);
		return NULL;
	}
	char* pT = (char*)pTmp;
	for (int i = 0; i < nRow; i++)
	{
		ppPtr[i] = (void*)(pT + i * nCol*typeSize);// ָ��ÿһ�еĿ�ʼ��ַ(�ֽڵ�ַ)��
	}

	return(ppPtr);
}

// �Ľ����ͷ�һ����̬��ά���飬�ڲ��ṩ���
void ffree_2d(void **ppArr,		// ��̬��ά����ָ��
	int nRow			// ����
)
{
	if (ppArr != NULL && ppArr[0] != NULL)
	{
		if (sizeof(ppArr[0]) <= 0)
		{
			printf("\nWarning : ��ջ����,û�ͷ��ڴ�!\n");
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
		printf("\nWarning : ��ջ����,�ͷŲ����ڴ�!\n");
		}
		*/
	}
}

// ��ͨ��ȥ�� [12/28/2010 wit]
bool ConnectDenoize2(Mat &src, int row, int col, int LIMIT)
{
	//Matת double **InImage
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
	struct_area *parea_inf = new struct_area[row*col]; // ��ͨ����Ϣ
	int* pUnRegion = new int[row*col];  // ���
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

	// ������ͼ
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			nStart = 0; nEnd = 0;
			// ���������������ʼ�Ҹõ���ͨ��
			if (InImage[i][j] > 0 && pUnRegion[i*col + j] == 0)
			{
				pnGrowQue[nEnd].x = j;
				pnGrowQue[nEnd].y = i;
				pUnRegion[i*col + j] = m;

				// �ҵ�ǰ����ͨ��
				while (nStart <= nEnd)
				{
					nCurrX = pnGrowQue[nStart].x;
					nCurrY = pnGrowQue[nStart].y;


					// �Ұ�����
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


	// ȥ��
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

		// �����ͨ������inimage1ͼ���а׵�����
		if (parea_inf[k1].number < LIMIT)    //�����ظ���
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




//��ȡ�˶�����ĻҶ�ƽ��ֵ
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