#pragma once
#include "Chess.h"
#include "AITypes.h"
class Board
{
public:
	Chess board[1024];						//��������,��¼ÿ��λ�õ������Ϣ
	Point remPoint[1024];					//��¼ÿ�����λ��
	Chess* remChess[1024];					//��¼ÿ�����chessָ��
	Point remULCand[1024];					//��¼���Ͻ�
	Point remLRCand[1024];					//��¼���½�
	int chessCount;							//������
	int who, opp;							//��ǰ���ӷ����Լ���һ��
	int nShape[2][10];						//˫����һ���ܳɵ�����ͳ��
	int boardSize;							//���̳ߴ�
	Point upperLeft;						//���Ͻ�
	Point lowerRight;						//���½�

	void initBoard(int size);
	void move(Point p);
	void undo();
	void generateCand(Cand cand[], int &nCand);
	int evaluate();
	int quickWinSearch();

	//��������
	int pointPiece(int x, int y) { return board[makePoint(x, y)].piece; }
	int pointX(int index) { return index >> 5; }
	int pointY(int index) { return index & 31; }
	int makePoint(int x, int y) { return (x << 5) + y; }
	int max(int a, int b) { return a > b ? a : b; }
	int min(int a, int b) { return a < b ? a : b; }
	bool inBoard(int index) { return board[index].piece != OUTSIDE; }
	int oppent(int piece) { return piece == BLACK ? WHITE : BLACK; }
private:
	const int MAX_BOARD_SIZE = 30;
};
