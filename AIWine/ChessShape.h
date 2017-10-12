#pragma once
#include "AITypes.h"
#include<iostream>
using namespace std;

class  ChessShape
{
public:
	static void initShape();
	static void testShape();
	static int testShapeScore(string shapeStr);
	static short calPrior(UCHAR pattern[4][2]);
	static char shapeTable[256][256];	//���ͱ�
	static int shapePrior[256][256];	//���͵ȼ���
	static char fourShapeTable[10][10][10][10];//������α�
	
private:
	//��ȡ�������
	static int getFourShape(int s1, int s2, int s3, int s4);
	//�ж�����
	static int checkFive(UCHAR p1, UCHAR p2);
	//�ж���������
	static int checkShape(UCHAR p1, UCHAR p2, int nextShape);
	static int linePiece(UCHAR p1, UCHAR p2, UCHAR mask) { return ((p1&mask ? 1 : 0) << 1) + (p2&mask ? 1 : 0); }
	static void putStone(UCHAR &p1, UCHAR &p2, UCHAR mask) { p1 |= mask; p2 &= ~mask; }
	static string getShapeName(string shapeStr);
	static int getShapePrior(UCHAR p1, UCHAR p2);
	static UCHAR bitCount(UCHAR p);
};


	
