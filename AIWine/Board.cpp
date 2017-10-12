#include "Board.h"
#include "ChessShape.h"
#include<assert.h>
#include<iostream>
void Board::initBoard(int size)
{
	memset(board, 0, sizeof(board));

	boardSize = size;
	for (int i = 0; i < 1024; i++)
	{
		if (pointX(i) >= 4 && pointX(i) < boardSize + 4 && pointY(i) >= 4 && pointY(i) < boardSize + 4)
		{
			board[i].piece = EMPTY;
		}
		else
		{
			board[i].piece = OUTSIDE;
		}
	}
	for (int i = 0; i < 1024; i++)
	{
		if (inBoard(i))
		{
			for (int k = 0; k < 4; k++)
			{
				int ii = i - MOV[k];
				for (UCHAR p = 8; p != 0; p >>= 1)
				{
					if (board[ii].piece == OUTSIDE) board[i].pattern[k][0] |= p;
					if (board[ii].piece == OUTSIDE) board[i].pattern[k][1] |= p;
					ii -= MOV[k];
				}
				ii = i + MOV[k];
				for (UCHAR p = 16; p != 0; p <<= 1)
				{
					if (board[ii].piece == OUTSIDE) board[i].pattern[k][0] |= p;
					if (board[ii].piece == OUTSIDE) board[i].pattern[k][1] |= p;
					ii += MOV[k];
				}
			}
		}
	}
	for (int i = 0; i < 1024; i++)
	{
		if (inBoard(i))
		{
			board[i].update1(0);
			board[i].update1(1);
			board[i].update1(2);
			board[i].update1(3);
			board[i].update4();
		}
	}
	chessCount = 0;
	who = BLACK;
	opp = WHITE;
	upperLeft = makePoint(4, 4);
	lowerRight = makePoint(boardSize + 3, boardSize + 3);
	memset(nShape, 0, sizeof(nShape));
}
//����
void Board::move(Point p)
{
	nShape[0][board[p].shape4[0]]--;
	nShape[1][board[p].shape4[1]]--;

	board[p].piece = who;
	remChess[chessCount] = &board[p];
	remPoint[chessCount] = p;
	remULCand[chessCount] = upperLeft;
	remLRCand[chessCount] = lowerRight;
	chessCount++;

	int x1 = pointX(p) - 2, y1 = pointY(p) - 2;
	int x2 = pointX(p) + 2, y2 = pointY(p) + 2;

	if (x1 < pointX(upperLeft)) x1 = max(x1, 4);
	if (y1 < pointY(upperLeft)) y1 = max(y1, 4);
	if (x2 > pointX(lowerRight)) x2 = min(x2, boardSize + 3);
	if (y2 > pointY(lowerRight)) y2 = min(y2, boardSize + 3);
	upperLeft = makePoint(x1, y1);
	lowerRight = makePoint(x2, y2);

	//����λ�����Լ�������Ϣ
	for (int k = 0; k < 4; k++)
	{
		Point move_p = p;
		//move_p��p���ұߣ���λ����
		for (UCHAR m = 16; m != 0; m <<= 1)
		{
			move_p -= MOV[k];
			board[move_p].pattern[k][who] |= m;
			if (board[move_p].piece == EMPTY)
			{
				board[move_p].update1(k);
				nShape[0][board[move_p].shape4[0]]--; nShape[1][board[move_p].shape4[1]]--;
				board[move_p].update4();
				nShape[0][board[move_p].shape4[0]]++; nShape[1][board[move_p].shape4[1]]++;
			}
		}
		move_p = p;
		//move_p��p����ߣ���λ����
		for (UCHAR m = 8; m != 0; m >>= 1)
		{
			move_p += MOV[k];
			board[move_p].pattern[k][who] |= m;
			if (board[move_p].piece == EMPTY)
			{
				board[move_p].update1(k);
				nShape[0][board[move_p].shape4[0]]--; nShape[1][board[move_p].shape4[1]]--;
				board[move_p].update4();
				nShape[0][board[move_p].shape4[0]]++; nShape[1][board[move_p].shape4[1]]++;
			}
		}
	}
	//����8�������������ڵ�������
	for (int i = 0; i < 16; i++)
	{
		board[p + RANGE[i]].neighbor++;
	}
	who = oppent(who);
	opp = oppent(opp);

}
void Board::undo()
{
	chessCount--;
	Point p = remPoint[chessCount];
	upperLeft = remULCand[chessCount];
	lowerRight = remLRCand[chessCount];

	Chess* chess = remChess[chessCount];
	chess->update1(0);
	chess->update1(1);
	chess->update1(2);
	chess->update1(3);
	chess->update4();

	nShape[0][chess->shape4[0]]++;
	nShape[1][chess->shape4[1]]++;

	chess->piece = EMPTY;

	who = oppent(who);
	opp = oppent(opp);

	//����λ�����Լ�������Ϣ
	for (int k = 0; k < 4; k++)
	{
		Point move_p = p;
		//move_p��p���ұߣ���λ����
		for (UCHAR m = 16; m != 0 && move_p - MOV[k]>0; m <<= 1)
		{
			move_p -= MOV[k];
			board[move_p].pattern[k][who] ^= m;
			if (board[move_p].piece == EMPTY)
			{
				board[move_p].update1(k);
				nShape[0][board[move_p].shape4[0]]--; nShape[1][board[move_p].shape4[1]]--;
				board[move_p].update4();
				nShape[0][board[move_p].shape4[0]]++; nShape[1][board[move_p].shape4[1]]++;
			}
		}
		move_p = p;
		//move_p��p����ߣ���λ����
		for (UCHAR m = 8; m != 0; m >>= 1)
		{
			move_p += MOV[k];
			board[move_p].pattern[k][who] ^= m;
			if (board[move_p].piece == EMPTY)
			{
				board[move_p].update1(k);
				nShape[0][board[move_p].shape4[0]]--; nShape[1][board[move_p].shape4[1]]--;
				board[move_p].update4();
				nShape[0][board[move_p].shape4[0]]++; nShape[1][board[move_p].shape4[1]]++;
			}
		}
	}
	//����8�������������ڵ�������
	for (int i = 0; i < 16; i++)
	{
		board[p + RANGE[i]].neighbor++;
	}
}
//�������з�֧
void Board::generateCand(Cand cand[], int& nCand)
{
	if (nShape[who][FIVE] > 0) 
	{ 
		for (int i = upperLeft; i <= lowerRight; i++)
		{
			if (board[i].isCand() && board[i].shape4[who] == FIVE)
			{
				nCand = 1;
				cand[0].point = i;
				return;
			}
		}
	}
	if (nShape[opp][FIVE] > 0)
	{
		for (int i = upperLeft; i <= lowerRight; i++)
		{
			if (board[i].isCand() && board[i].shape4[opp] == FIVE)
			{
				nCand = 1;
				cand[0].point = i;
				return;
			}
		}
	}
	if (nShape[who][FLEX4] > 0)
	{
		for (int i = upperLeft; i <= lowerRight; i++)
		{
			if (board[i].isCand() && board[i].shape4[who] == FLEX4)
			{
				nCand = 1;
				cand[0].point = i;
				return;
			}
		}
	}
	if (nShape[opp][FLEX4] > 0)
	{
		nCand = 0;
		for (int i = upperLeft; i <= lowerRight; i++)
		{
			if (board[i].isCand())
			{
				if (board[i].shape4[who] >= E || board[i].shape4[opp] >= E)
				{
					cand[nCand].value = board[i].prior();
					cand[nCand].point = i;
					if (cand[nCand].value > 0) nCand++;
				}
			}
		}
		return;
	}

	nCand = 0;
	for (int i = upperLeft; i <= lowerRight; i++)
	{
		if (board[i].isCand())
		{
			cand[nCand].value = board[i].prior();
			cand[nCand].point = i;
			if (cand[nCand].value > 0) nCand++;
		}
	}
}
//��������
int Board::evaluate()
{
	int p;
	int eval[2] = { 0 };
	Chess *c;
	for (int i = 0; i < chessCount; i++)
	{
		c = remChess[i];
		p = c->piece;
		if (p != 0 && p != 1)
		{
			p = 0;
		}
		for (int k = 0; k < 4; k++)
		{
			eval[p] += ChessShape::shapePrior[c->pattern[k][p]][c->pattern[k][1 - p]];
		}
	}
	if (who >= 2 || opp >= 2)
	{
		eval[0] = eval[1] = 0;
	}
	return eval[who] - eval[opp];
}
int Board::quickWinSearch()
{
	int q;
	if (nShape[who][A] >= 1) return 1;   
	if (nShape[opp][A] >= 2) return -2;  
	if (nShape[opp][A] == 1)             
	{
		for (int m = 0; m < 1024; m++)
		{
			if (board[m].isCand())
			{
				if (board[m].shape4[opp] == A)
				{

					move(m);
					q = -quickWinSearch();
					undo();
					if (q < 0) q--; else if (q > 0) q++;
					return q;
				}
			}
		}
			
	}
	if (nShape[who][B] >= 1) return 3;   
	if (nShape[who][C] >= 1)             // XOOO_ * _OO
	{
		if (nShape[opp][B] == 0 && nShape[opp][C] == 0 && nShape[opp][D] == 0 && nShape[opp][E] == 0) return 5;
		for (int m = 0; m < 1024; m++)
		{
			if (board[m].isCand())
			{
				if (board[m].shape4[who] == C)
				{
					move(m);
					q = -quickWinSearch();
					undo();
					if (q > 0) return q + 1;
				}
			}
		}
	}
	if (nShape[who][F] >= 1)
	{
		if (nShape[opp][B] == 0 && nShape[opp][C] == 0 && nShape[opp][D] == 0 && nShape[opp][E] == 0) return 5;
	}
	return 0;
}