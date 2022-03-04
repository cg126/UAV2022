#pragma once

#include<opencv2\highgui\highgui.hpp>
#include<opencv2\opencv.hpp>
#include<opencv2\core.hpp>//һ�㶨��MAT����������
#include<iostream>
#include<fstream>
#include<algorithm >

using namespace std;//����std�Ŀռ�
using namespace cv;//ʡȥ����ǰ���cv

struct row_roi
{
	int angle;
	int roi;
};

struct Point2D
{
	int x;
	int y;
};

void quickFindTarget(unsigned char *img0, int height, int width, int *centerx, int *centery, int *greyaverage);
void FindSingleTarget(unsigned char *img0, int height, int width, vector<int> &Single_RowPos, vector<int> &Single_ColPos);
void quickFindTarget1(unsigned char *img0, int height, int width, int *centerx, int *centery, int *greyaverage);
void quickFindTarget2(Mat&src, int height, int width, int *centerx, int *centery, int *greyaverage);
void KittlerMinError(const Mat& inimg, Mat& outimg, int width, int height, int *th);
int Otsu(Mat& src);
void MedFilterImage();
void cir();
void CirR(double*Px, double*Py, int changdu, double &AAAA, double &BBBB, double &RRRR);
void mid(int k, int*max);
vector<int> Max_Cur(int n);
vector<row_roi> FindTarget(vector<int> &cur);
static int frameNum = 0;

//2021//07//22
//��֡���
void Diff2frame(Mat &mat1, Mat &mat2, Mat &result);

//������ת��ͼ��ƽ���Ҷ�ֵ
int PloarGreyaverage(Mat &src);

bool ConnectDenoize2(Mat &src, int row, int col, int LIMIT);

void ffree_2d(void **ppArr,		// ��̬��ά����ָ��
	int nRow			// ����
);

void** fspace_2d(int nRow, int nCol,	// �����С������
	short typeSize		// ���ʹ�С���ֽ���Ŀ��
);

