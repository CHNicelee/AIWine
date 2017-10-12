#pragma once
#include "AITypes.h"
#include "Board.h"


class AIWine
{
public:
	AIWine();
	~AIWine();
	static const int MaxSize = 28;
	static const int MaxDepth = 8;
	static const int MaxCand = 256;
	void reStart();
	void turnUndo();
	bool setSize(int size);
	bool isValidPos(int x, int y);
	void turnMove(int x, int y);
	void turnBest(int &x, int &y);
	Cand rootSearch(int depth, int alpha, int beta);
	int search(int depth, int alpha, int beta);

	int nSearched;							//ÿ��˼��ʱ,�����ľ�����
	Cand rootCand[MaxCand];					//���ڵ��֧
	int nRootCand;							//���ڵ��֧��
	int timeout_turn;
	int timeout_match;
	int time_left;

	void delLoseCand(Cand cand[],int &nCand);
	void sortCand(Cand cand[], int nCand);
	int pointX(int index) { return index >> 5; }
	int pointY(int index) { return index & 31; }
	int makePoint(int x, int y) { return (x << 5) + y; }
	
private:
	Board *board;
};
