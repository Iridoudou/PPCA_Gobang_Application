#ifndef GAMEMODEL_H
#define GAMEMODEL_H
//#include "aicontroller.h"
#include <utility>
#include <cstring>
#include <vector>
#include <queue>
#include <functional>
#include <random>
#include <time.h>

const int CHESS_RADIUS=16;
const int MARGIN=30;
const int BLOCK_SIZE=40;
const int MARKK_SIZE=6;
const int POS_OFFSET=BLOCK_SIZE*0.3;
const int BOARD_GRAD_SIZE=17;
const int BUTTON_WIDTH=100;

class Gobang
{
public:
const int DEPTH_LIMIT = 3;
const int SEARCH_DEPTH = 6;
const int NEIGHBOUR_SEARCH_WIDTH = 2;
const int NUM_OF_INSPIRED_SEARCH = 10;

const int AI_WIN_5 = 100000;
const int AI_ALIVE_4 = 10000;
const int AI_ALIVE_3 = 1000;
const int AI_ALIVE_2 = 100;
const int AI_ALIVE_1 = 10;
const int AI_DIE_4 = 1000;
const int AI_DIE_3 = 100;
const int AI_DIE_2 = 10;

const int HUMAN_WIN_5 = 100000;
const int HUMAN_ALIVE_4 = 10000;
const int HUMAN_ALIVE_3 = 1000;
const int HUMAN_ALIVE_2 = 100;
const int HUMAN_ALIVE_1 = 10;
const int HUMAN_DIE_4 = 1000;
const int HUMAN_DIE_3 = 100;
const int HUMAN_DIE_2 = 10;

const int NEGIFF = -2147483648;
const int IFF = 2147483647;

int ai_side; //0: black, 1: white
std::string ai_name = "your_ai_name_here";

int turn;
bool flipFlag=false;
int board[15][15];
int pieceNum=0;
//内嵌结构体
struct direction  //检查方向
{
    int dx;
    int dy;
    void operator=(direction &other)
    {
        dx = other.dx;
        dy = other.dy;
    }
};
struct situation //棋子类型统计
{
    int win_5 = 0,
        alive_4 = 0, d_alive_4 = 0, die_4 = 0,
        alive_3 = 0, d_alive_3 = 0, die_3 = 0,
        alive_2 = 0, d_alive_2 = 0, die_2 = 0,
        alive_1 = 0, d_alive_1 = 0, die_1 = 0;
    situation operator+=(situation &other)
    {
        win_5 += other.win_5;
        alive_4 += other.alive_4;
        d_alive_4 += other.d_alive_4;
        die_4 += other.die_4;
        alive_3 += other.alive_3;
        d_alive_3 += other.d_alive_3;
        die_3 += other.die_3;
        alive_2 += other.alive_2;
        d_alive_2 += other.d_alive_2;
        die_2 += other.die_2;
        alive_1 += other.alive_1;
        d_alive_1 += other.d_alive_1;
        die_1 += other.die_1;
        return *this;
    }
};
struct value
{
    int aiValue;
    int humanValue;
};
struct waitPoint //待选棋子
{
    value val;
    std::pair<int, int> cur;
    waitPoint(value v, std::pair<int, int> c) : val(v), cur(c) {}
};
struct cmp //比较
{
    bool operator()(waitPoint &cur1, waitPoint &cur2)
    {
        return cur1.val.aiValue < cur2.val.aiValue;
    }
};
struct _cmp //比较
{
    bool operator()(waitPoint &cur1, waitPoint &cur2)
    {
        return cur1.val.aiValue > cur2.val.aiValue;
    }
};
//两个基础函数
Gobang(){}
void init()
{
    turn = 0;
    pieceNum=0;
    memset(board, -1, sizeof(board));
}
std::pair<int, int> action(std::pair<int, int> loc)
{
    if (loc.first == -1 && pieceNum == 0) //先手
    {
        place(1, 1, ai_side);
        ++turn;
        return std::make_pair(1, 1);
    }
    else if (loc.first == -1) //换手
    {
        ++turn;
        flip();
        goto find;
    }
    //place(loc.first, loc.second, 1 - ai_side);
    //++turn;
find:
    if(pieceNum==1)
    {
        if (isEmpty(1, 1))
    {
        ++turn;
        place(1, 1, ai_side);
        return std::make_pair(1, 1);
    }
    else
    {
        ++turn;
        place(1, 2, ai_side);
        return std::make_pair(1, 2);
    }
    }
    std::priority_queue<waitPoint, std::vector<waitPoint>, cmp> tmp = decisiveAction();
    if (!tmp.empty())
    {
        place(tmp.top().cur.first, tmp.top().cur.second, ai_side);
        ++turn;
        return tmp.top().cur;
    }
    if (turn != 3)
    {
        std::pair<int, int> bestMove;
        minimax(bestMove, SEARCH_DEPTH, NEGIFF, IFF, ai_side);
        place(bestMove.first, bestMove.second, ai_side);
        ++turn;
        return bestMove;
    }
    else
    {
        flip();
        ++turn;
        flipFlag=true;
        return std::make_pair(-1, -1);
        std::pair<int, int> bestMove;
        minimax(bestMove, SEARCH_DEPTH, NEGIFF, IFF, ai_side);
        place(bestMove.first, bestMove.second, ai_side);
        ++turn;
        return bestMove;
    }
}
//函数定义
bool isEmpty(int x, int y)
{
    return board[x][y] == -1;
}
void place(int x, int y, int side)
{
    board[x][y] = side;
    ++pieceNum;
}
void unplace(int x, int y)
{
    board[x][y] = -1;
    --pieceNum;
}
bool isFull()
{
    return pieceNum == 225;
}
bool isInBoard(int x, int y)
{
    if (x < 0 || y < 0 || x >= 15 || y >= 15)
        return false;
    return true;
}
direction dir(int i)
{
    direction dir1 = {1, 0};
    direction dir2 = {0, 1};
    direction dir3 = {1, 1};
    direction dir4 = {1, -1};
    switch (i)
    {
    case 1:
        return dir1;
        break;
    case 2:
        return dir2;
        break;
    case 3:
        return dir3;
        break;
    case 4:
        return dir4;
        break;
    }
}
std::pair<int, int> nextPoint(std::pair<int, int> cur, int directionNo, int len)
{
    direction d = dir(directionNo);
    int x = cur.first + d.dx * len;
    int y = cur.second + d.dy * len;
    return std::make_pair(x, y);
}
void getLinkPiece(std::pair<int, int> cur, int directionNo, int side, int &len, int pace, std::pair<int, int> &linkPoint)
{
    direction d = dir(directionNo);
    std::pair<int, int> tmp = nextPoint(cur, directionNo, pace);
    linkPoint = cur;
    while (isInBoard(tmp.first, tmp.second) && board[tmp.first][tmp.second] == side /*&& !marked[directionNo][tmp.first][tmp.second]*/)
    {
        linkPoint = tmp;
        //marked[directionNo][tmp.first][tmp.second] = true;
        tmp = nextPoint(tmp, directionNo, pace);
        ++len;
    }
}
void getBoundary(int *left, int *right, std::pair<int, int> l, std::pair<int, int> r, int directionNo, int side)
{
    direction d = dir(directionNo);
    int opposite = 1 - ai_side;
    for (register int i = 1; i <= 4; ++i)
    {
        std::pair<int, int> tmp = nextPoint(l, directionNo, -i);
        if (isInBoard(tmp.first, tmp.second))
            left[i] = board[tmp.first][tmp.second];
        else
            left[i] = opposite;
        tmp = nextPoint(r, directionNo, i);
        if (isInBoard(tmp.first, tmp.second))
            right[i] = board[tmp.first][tmp.second];
        else
            right[i] = opposite;
    }
}
bool judge(std::pair<int, int> cur, int side)
{
    int i = cur.first;
    int j = cur.second;
    if (!isEmpty(i, j) && board[i][j] == side)
    {
        for (register int k = 1; k <= 4; ++k)
        {
            direction d = dir(k);
            int len = 1;
            std::pair<int, int> tmpLeft, tmpRight;
            getLinkPiece(cur, k, side, len, 1, tmpRight);
            getLinkPiece(cur, k, side, len, -1, tmpLeft);
            if (len >= 5)
                return true;
        }
    }
    return false;
}
bool judge_alive4(std::pair<int, int> cur, int side)
{
    situation tmp, res;
    for (register int i = 1; i <= 4; ++i)
    {
        direction d = dir(i);
        int len = 1;
        std::pair<int, int> l, r, tmpPoint;
        int left[5], right[5];
        getLinkPiece(cur, i, side, len, -1, l);
        getLinkPiece(cur, i, side, len, 1, r);
        getBoundary(left, right, l, r, i, side);
        tmp = situationAnalysis(len, side, left, right);
        res += tmp;
    }
    if (res.alive_4 >= 1)
        return true;
    return false;
}
bool judge_double3(std::pair<int, int> cur, int side)
{
    situation tmp, res;
    for (register int i = 1; i <= 4; ++i)
    {
        direction d = dir(i);
        int len = 1;
        std::pair<int, int> l, r, tmpPoint;
        int left[5], right[5];
        getLinkPiece(cur, i, side, len, -1, l);
        getLinkPiece(cur, i, side, len, 1, r);
        getBoundary(left, right, l, r, i, side);
        tmp = situationAnalysis(len, side, left, right);
        res += tmp;
    }
    if (res.alive_3 >= 2 || (res.alive_3 >= 1 && res.die_4 >= 1))
        return true;
    return false;
}
situation situationAnalysis(int len, int side, int *left, int *right)
{
    situation ans;
    if (len == 5)
        ++ans.win_5;
    else if (len == 4)
    {
        if (left[1] == -1 && right[1] == -1)
            ++ans.alive_4;
        else if (left[1] == -1 || right[1] == -1)
            ++ans.d_alive_4;
        else
            ++ans.die_4;
    }
    else if (len == 3)
    {
        if ((left[1] == -1 && left[2] == side) || (right[1] == -1 && right[2] == side))
            ++ans.d_alive_4;
        else if (left[1] == -1 && right[1] == -1 && (left[2] == -1 || right[2] == -1))
            ++ans.alive_3;
        else if ((left[1] == -1 && left[2] == -1) || (right[1] == -1 && right[2] == -1))
            ++ans.d_alive_3;
        else
            ++ans.die_3;
    }
    else if (len == 2)
    {
        if (left[1] == -1 && left[2] == side && left[3] == side && right[1] == -1 && right[2] == side && right[3] == side)
            ++ans.d_alive_4;
        else if (left[1] == -1 && right[1] == -1 && ((left[2] == side && left[3] == -1) || (right[2] == side && right[3] == -1)))
            ++ans.d_alive_3;
        else if ((left[1] == -1 && left[2] == side && left[3] == -1) || (right[1] == -1 && right[2] == side && right[3] == -1))
            ++ans.die_3;
        else if (left[1] == -1 && right[1] == -1 && (left[2] == side || right[2] == side))
            ++ans.die_3;
        else if ((left[1] == -1 && left[2] == -1 && left[3] == side) || (right[2] = -1 && right[3] == side && right[1] == -1))
            ++ans.die_3;
        else if ((left[1] == -1 && right[1] == -1 && right[2] == -1 && right[3] == -1) || (left[1] == -1 && left[2] == -1 && right[1] == -1 && right[2] == -1) || right[1] == -1 && left[1] == -1 && left[2] == -1 && left[3] == -1)
            ++ans.alive_2;
        else if ((left[1] == -1 && left[2] == -1 && left[3] == -1) || (right[1] == -1 && right[2] == -1 && right[3] == -1))
            ++ans.die_2;
    }
    else if (len == 1)
    {
        if ((left[1] == -1 && left[2] == side && left[3] == side && left[4] == side) || (right[1] == -1 && right[2] == -1 && right[3] == -1 && right[4] == -1))
            ++ans.d_alive_4;
        else if ((left[1] == -1 && right[1] == -1) && ((left[2] == side && left[3] == side && left[4] == -1) || (right[2] == side && right[3] == side && right[4] == -1)))
            ++ans.d_alive_3;
        else if ((left[1] == -1 && right[1] == -1) && ((left[2] == side && left[3] == side) || (right[2] == side && right[3] == side)))
            ++ans.die_3;
        else if ((left[1] == -1 && left[2] == side && left[3] == side && left[4] == -1) || (right[1] == -1 && right[2] == side && right[3] == side && right[4] == -1))
            ++ans.die_3;
        else if ((left[1] == -1 && left[2] == -1 && left[3] == side && left[4] == side) || (right[1] == -1 && right[2] == -1 && right[3] == side && right[4] == side))
            ++ans.die_3;
        else if ((left[1] == -1 && left[2] == side && left[3] == -1 && left[4] == side) || (right[1] == -1 && right[2] == side && right[3] == -1 && right[4] == side))
            ++ans.die_3;
        else if ((left[1] == -1 && right[1] == -1 && right[3] == -1 && right[2] == side) && (left[2] == -1 || right[4] == -1))
            ++ans.d_alive_2;
        else if ((right[1] == -1 && left[1] == -1 && left[3] == -1 && left[2] == side) && (right[2] == -1 || left[4] == -1))
            ++ans.d_alive_2;
        else if ((left[1] == -1 && right[1] == -1 && right[2] == -1 && right[4] == -1 && right[3] == side) || (right[1] == -1 && left[1] == -1 && left[2] == -1 && left[4] == -1 && left[3] == side))
            ++ans.d_alive_2;
        else if ((left[1] == -1 && left[2] == side && left[3] == -1 && left[4] == -1) || (right[1] == -1 && right[2] == side && right[3] == -1 && right[4] == -1))
            ++ans.die_2;
        else if ((left[1] == -1 && right[1] == -1 && right[2] == -1 && left[2] == -1) || (left[1] == -1 && right[1] == -1 && left[2] == -1 && right[2] == side))
            ++ans.die_2;
        else if ((left[1] == -1 && left[2] == -1 && left[3] == side && left[4] == -1) || (right[1] == -1 && right[2] == -1 && right[3] == side && right[4] == -1))
            ++ans.die_2;
        else if ((left[1] == -1 && left[2] == -1 && left[3] == side && right[1] == -1) || (right[1] == -1 && right[2] == -1 && right[3] == side && left[1] == -1))
            ++ans.die_2;
        else if ((left[1] == -1 && left[2] == -1 && left[3] == -1 && left[4] == side) || (right[1] == -1 && right[2] == -1 && right[3] == -1 && right[4] == side))
            ++ans.die_2;
        else if (left[1] == -1 && right[1] == -1)
            ++ans.alive_1;
        else if (left[1] == -1 || right[1] == -1)
            ++ans.d_alive_1;
        else
            ++ans.die_1;
    }
    return ans;
}
int singleEvaluation(std::pair<int, int> cur, int side)
{
    int ans = 0;
    situation res, tmp;
    for (register int i = 1; i <= 4; ++i)
    {
        direction d = dir(i);
        int len = 1;
        std::pair<int, int> l, r, tmpPoint;
        int left[5], right[5];
        getLinkPiece(cur, i, side, len, -1, l);
        getLinkPiece(cur, i, side, len, 1, r);
        getBoundary(left, right, l, r, i, side);
        tmp = situationAnalysis(len, side, left, right);
        res += tmp;
    }
    if (res.win_5 >= 1)
        ans += 20000000;
    if (res.alive_4 >= 1 || res.d_alive_4 >= 2 || res.alive_3 >= 2 || (res.d_alive_4 >= 1 && res.alive_3 >= 1)) //绝杀
        ans += 10000000;
    ans += (res.d_alive_4 * 10000 +
            res.die_4 * 5000 +
            res.alive_3 * 10000 +
            res.d_alive_4 * 1000 +
            res.die_3 * 500 +
            res.alive_2 * 1000 +
            res.d_alive_2 * 100 +
            res.die_2 * 50 +
            res.alive_1 * 100 +
            res.d_alive_1 * 10 +
            res.die_1 * 5);
    return ans;
}
value wholeEvaluation()
{
    value val;
    //situation aiSituation, humanSituation;

    /*for (register int i = 0; i < 15; ++i)
        for (register int j = 0; j < 15; ++j)
        {
            if (!isEmpty(i, j))
            {
                if (board[i][j] == ai_side)
                {
                    val.aiValue += singleEvaluation({i, j}, ai_side);
                }
                else
                    val.humanValue += singleEvaluation({i, j}, 1 - ai_side);
            }
        }
    val.aiValue -= val.humanValue;
    return val;*/

    std::pair<int, int> left, right, tmpLeft, tmpRight;
    int len, leftLen, rightLen;
    situation curSituation_ai, curSituation_human;
    //横着数
    for (register int i = 0; i < 15; ++i)
    {
        left = std::make_pair(i, 0);
    continueCount1:
        while (board[left.first][left.second] == -1)
        {
            left.second++;
            if (!isInBoard(left.first, left.second))
                goto nextLoop1;
        }
        right = left;
        len = 1;
        while (isInBoard(right.first, right.second + 1) && board[left.first][left.second] == board[right.first][right.second + 1])
        {
            ++right.second;
            ++len;
        }

        tmpLeft.first = left.first;
        tmpLeft.second = left.second;
        tmpRight.first = right.first;
        tmpRight.second = right.second;
        leftLen = rightLen = 0;
        while (isInBoard(tmpLeft.first, tmpLeft.second - 1) && board[tmpLeft.first][tmpLeft.second - 1] == -1)
        {
            --tmpLeft.second;
            ++leftLen;
        }
        while (isInBoard(tmpRight.first, tmpRight.second + 1) && board[tmpRight.first][tmpRight.second + 1] == -1)
        {
            ++tmpRight.second;
            ++rightLen;
        }

        if (board[left.first][left.second] == ai_side)
        {
            if (len == 5)
                ++curSituation_ai.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.alive_4;
                        break;
                    case 3:
                        ++curSituation_ai.alive_3;
                        break;
                    case 2:
                        ++curSituation_ai.alive_2;
                        break;
                    case 1:
                        ++curSituation_ai.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first, tmpLeft.second - 2) && board[tmpLeft.first][tmpLeft.second - 2] == ai_side) || (isInBoard(tmpRight.first, tmpRight.second + 2) && board[tmpRight.first][tmpRight.second + 2] == ai_side))
                            ++curSituation_ai.die_4;
                        else
                            ++curSituation_ai.die_3;
                        break;
                    case 2:
                        ++curSituation_ai.die_2;
                        break;
                    }
                }
            }
        }
        else
        {
            if (len == 5)
                ++curSituation_human.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.alive_4;
                        break;
                    case 3:
                        ++curSituation_human.alive_3;
                        break;
                    case 2:
                        ++curSituation_human.alive_2;
                        break;
                    case 1:
                        ++curSituation_human.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first, tmpLeft.second - 2) && board[tmpLeft.first][tmpLeft.second - 2] == 1 - ai_side) || (isInBoard(tmpRight.first, tmpRight.second + 2) && board[tmpRight.first][tmpRight.second + 2] == 1 - ai_side))
                            ++curSituation_human.die_4;
                        else
                            ++curSituation_human.die_3;
                        break;
                    case 2:
                        ++curSituation_human.die_2;
                        break;
                    }
                }
            }
        }
        if (isInBoard(right.first, right.second + 1))
        {
            left = std::make_pair(right.first, right.second + 1);
            goto continueCount1;
        }
    nextLoop1:;
    }
    //竖着数
    for (register int i = 0; i < 15; ++i)
    {
        left = std::make_pair(0, i);
    continueCount2:
        while (board[left.first][left.second] == -1)
        {
            left.first++;
            if (!isInBoard(left.first, left.second))
                goto nextLoop2;
        }
        right = left;
        len = 1;
        while (isInBoard(right.first + 1, right.second) && board[left.first][left.second] == board[right.first + 1][right.second])
        {
            ++right.first;
            ++len;
        }

        tmpLeft.first = left.first;
        tmpLeft.second = left.second;
        tmpRight.first = right.first;
        tmpRight.second = right.second;
        leftLen = rightLen = 0;
        while (isInBoard(tmpLeft.first - 1, tmpLeft.second) && board[tmpLeft.first - 1][tmpLeft.second] == -1)
        {
            --tmpLeft.first;
            ++leftLen;
        }
        while (isInBoard(tmpRight.first + 1, tmpRight.second) && board[tmpRight.first + 1][tmpRight.second] == -1)
        {
            ++tmpRight.first;
            ++rightLen;
        }

        if (board[left.first][left.second] == ai_side)
        {
            if (len == 5)
                ++curSituation_ai.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.alive_4;
                        break;
                    case 3:
                        ++curSituation_ai.alive_3;
                        break;
                    case 2:
                        ++curSituation_ai.alive_2;
                        break;
                    case 1:
                        ++curSituation_ai.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first - 2, tmpLeft.second) && board[tmpLeft.first - 2][tmpLeft.second] == ai_side) || (isInBoard(tmpRight.first + 2, tmpRight.second) && board[tmpRight.first + 2][tmpRight.second] == ai_side))
                            ++curSituation_ai.die_4;
                        else
                            ++curSituation_ai.die_3;
                        break;
                    case 2:
                        ++curSituation_ai.die_2;
                        break;
                    }
                }
            }
        }
        else
        {
            if (len == 5)
                ++curSituation_human.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.alive_4;
                        break;
                    case 3:
                        ++curSituation_human.alive_3;
                        break;
                    case 2:
                        ++curSituation_human.alive_2;
                        break;
                    case 1:
                        ++curSituation_human.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first - 2, tmpLeft.second) && board[tmpLeft.first - 2][tmpLeft.second] == 1 - ai_side) || (isInBoard(tmpRight.first + 2, tmpRight.second) && board[tmpRight.first + 2][tmpRight.second] == 1 - ai_side))
                            ++curSituation_human.die_4;
                        else
                            ++curSituation_human.die_3;
                        break;
                    case 2:
                        ++curSituation_human.die_2;
                        break;
                    }
                }
            }
        }
        if (isInBoard(right.first + 1, right.second))
        {
            left = std::make_pair(right.first + 1, right.second);
            goto continueCount2;
        }
    nextLoop2:;
    }
    //斜下数
    for (register int i = 0; i < 15; ++i)
    {
        left = std::make_pair(0, i);
    continueCount3:
        while (board[left.first][left.second] == -1)
        {
            left.first++;
            left.second++;
            if (!isInBoard(left.first, left.second))
                goto nextLoop3;
        }
        right = left;
        len = 1;
        while (isInBoard(right.first + 1, right.second + 1) && board[left.first][left.second] == board[right.first + 1][right.second + 1])
        {
            ++right.first;
            ++right.second;
            ++len;
        }

        tmpLeft.first = left.first;
        tmpLeft.second = left.second;
        tmpRight.first = right.first;
        tmpRight.second = right.second;
        leftLen = rightLen = 0;
        while (isInBoard(tmpLeft.first - 1, tmpLeft.second - 1) && board[tmpLeft.first - 1][tmpLeft.second - 1] == -1)
        {
            --tmpLeft.first;
            --tmpLeft.second;
            ++leftLen;
        }
        while (isInBoard(tmpRight.first + 1, tmpRight.second + 1) && board[tmpRight.first + 1][tmpRight.second + 1] == -1)
        {
            ++tmpRight.first;
            ++tmpRight.second;
            ++rightLen;
        }

        if (board[left.first][left.second] == ai_side)
        {
            if (len == 5)
                ++curSituation_ai.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.alive_4;
                        break;
                    case 3:
                        ++curSituation_ai.alive_3;
                        break;
                    case 2:
                        ++curSituation_ai.alive_2;
                        break;
                    case 1:
                        ++curSituation_ai.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first - 2, tmpLeft.second - 2) && board[tmpLeft.first - 2][tmpLeft.second - 2] == ai_side) || (isInBoard(tmpRight.first + 2, tmpRight.second + 2) && board[tmpRight.first + 2][tmpRight.second + 2] == ai_side))
                            ++curSituation_ai.die_4;
                        else
                            ++curSituation_ai.die_3;
                        break;
                    case 2:
                        ++curSituation_ai.die_2;
                        break;
                    }
                }
            }
        }
        else
        {
            if (len == 5)
                ++curSituation_human.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.alive_4;
                        break;
                    case 3:
                        ++curSituation_human.alive_3;
                        break;
                    case 2:
                        ++curSituation_human.alive_2;
                        break;
                    case 1:
                        ++curSituation_human.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first - 2, tmpLeft.second - 2) && board[tmpLeft.first - 2][tmpLeft.second - 2] == 1 - ai_side) || (isInBoard(tmpRight.first + 2, tmpRight.second + 2) && board[tmpRight.first + 2][tmpRight.second + 2] == 1 - ai_side))
                            ++curSituation_human.die_4;
                        else
                            ++curSituation_human.die_3;
                        break;
                    case 2:
                        ++curSituation_human.die_2;
                        break;
                    }
                }
            }
        }
        if (isInBoard(right.first + 1, right.second + 1))
        {
            left = std::make_pair(right.first + 1, right.second + 1);
            goto continueCount3;
        }
    nextLoop3:;
    }
    for (register int i = 0; i < 14; ++i)
    {
        left = std::make_pair(i, 0);
    continueCount4:
        while (board[left.first][left.second] == -1)
        {
            left.first++;
            left.second++;
            if (!isInBoard(left.first, left.second))
                goto nextLoop4;
        }
        right = left;
        len = 1;
        while (isInBoard(right.first + 1, right.second + 1) && board[left.first][left.second] == board[right.first + 1][right.second + 1])
        {
            ++right.first;
            ++right.second;
            ++len;
        }

        tmpLeft.first = left.first;
        tmpLeft.second = left.second;
        tmpRight.first = right.first;
        tmpRight.second = right.second;
        leftLen = rightLen = 0;
        while (isInBoard(tmpLeft.first - 1, tmpLeft.second - 1) && board[tmpLeft.first - 1][tmpLeft.second - 1] == -1)
        {
            --tmpLeft.first;
            --tmpLeft.second;
            ++leftLen;
        }
        while (isInBoard(tmpRight.first + 1, tmpRight.second + 1) && board[tmpRight.first + 1][tmpRight.second + 1] == -1)
        {
            ++tmpRight.first;
            ++tmpRight.second;
            ++rightLen;
        }

        if (board[left.first][left.second] == ai_side)
        {
            if (len == 5)
                ++curSituation_ai.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.alive_4;
                        break;
                    case 3:
                        ++curSituation_ai.alive_3;
                        break;
                    case 2:
                        ++curSituation_ai.alive_2;
                        break;
                    case 1:
                        ++curSituation_ai.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first - 2, tmpLeft.second - 2) && board[tmpLeft.first - 2][tmpLeft.second - 2] == ai_side) || (isInBoard(tmpRight.first + 2, tmpRight.second + 2) && board[tmpRight.first + 2][tmpRight.second + 2] == ai_side))
                            ++curSituation_ai.die_4;
                        else
                            ++curSituation_ai.die_3;
                        break;
                    case 2:
                        ++curSituation_ai.die_2;
                        break;
                    }
                }
            }
        }
        else
        {
            if (len == 5)
                ++curSituation_human.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.alive_4;
                        break;
                    case 3:
                        ++curSituation_human.alive_3;
                        break;
                    case 2:
                        ++curSituation_human.alive_2;
                        break;
                    case 1:
                        ++curSituation_human.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first - 2, tmpLeft.second - 2) && board[tmpLeft.first - 2][tmpLeft.second - 2] == 1 - ai_side) || (isInBoard(tmpRight.first + 2, tmpRight.second + 2) && board[tmpRight.first + 2][tmpRight.second + 2] == 1 - ai_side))
                            ++curSituation_human.die_4;
                        else
                            ++curSituation_human.die_3;
                        break;
                    case 2:
                        ++curSituation_human.die_2;
                        break;
                    }
                }
            }
        }
        if (isInBoard(right.first + 1, right.second + 1))
        {
            left = std::make_pair(right.first + 1, right.second + 1);
            goto continueCount4;
        }
    nextLoop4:;
    }
    //斜上数
    for (register int i = 0; i < 15; ++i)
    {
        left = std::make_pair(i, 0);
    continueCount5:
        while (board[left.first][left.second] == -1)
        {
            left.first--;
            left.second++;
            if (!isInBoard(left.first, left.second))
                goto nextLoop5;
        }
        right = left;
        len = 1;
        while (isInBoard(right.first - 1, right.second + 1) && board[left.first][left.second] == board[right.first - 1][right.second + 1])
        {
            --right.first;
            ++right.second;
            ++len;
        }

        tmpLeft.first = left.first;
        tmpLeft.second = left.second;
        tmpRight.first = right.first;
        tmpRight.second = right.second;
        leftLen = rightLen = 0;
        while (isInBoard(tmpLeft.first + 1, tmpLeft.second - 1) && board[tmpLeft.first + 1][tmpLeft.second - 1] == -1)
        {
            ++tmpLeft.first;
            --tmpLeft.second;
            ++leftLen;
        }
        while (isInBoard(tmpRight.first - 1, tmpRight.second + 1) && board[tmpRight.first - 1][tmpRight.second + 1] == -1)
        {
            --tmpRight.first;
            ++tmpRight.second;
            ++rightLen;
        }

        if (board[left.first][left.second] == ai_side)
        {
            if (len == 5)
                ++curSituation_ai.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.alive_4;
                        break;
                    case 3:
                        ++curSituation_ai.alive_3;
                        break;
                    case 2:
                        ++curSituation_ai.alive_2;
                        break;
                    case 1:
                        ++curSituation_ai.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first + 2, tmpLeft.second - 2) && board[tmpLeft.first + 2][tmpLeft.second - 2] == ai_side) || (isInBoard(tmpRight.first - 2, tmpRight.second + 2) && board[tmpRight.first - 2][tmpRight.second + 2] == ai_side))
                            ++curSituation_ai.die_4;
                        else
                            ++curSituation_ai.die_3;
                        break;
                    case 2:
                        ++curSituation_ai.die_2;
                        break;
                    }
                }
            }
        }
        else
        {
            if (len == 5)
                ++curSituation_human.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.alive_4;
                        break;
                    case 3:
                        ++curSituation_human.alive_3;
                        break;
                    case 2:
                        ++curSituation_human.alive_2;
                        break;
                    case 1:
                        ++curSituation_human.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first + 2, tmpLeft.second - 2) && board[tmpLeft.first + 2][tmpLeft.second - 2] == 1 - ai_side) || (isInBoard(tmpRight.first - 2, tmpRight.second + 2) && board[tmpRight.first - 2][tmpRight.second + 2] == 1 - ai_side))
                            ++curSituation_human.die_4;
                        else
                            ++curSituation_human.die_3;
                        break;
                    case 2:
                        ++curSituation_human.die_2;
                        break;
                    }
                }
            }
        }
        if (isInBoard(right.first - 1, right.second + 1))
        {
            left = std::make_pair(right.first - 1, right.second + 1);
            goto continueCount5;
        }
    nextLoop5:;
    }

    for (register int i = 1; i < 15; ++i)
    {
        left = std::make_pair(14, i);
    continueCount6:
        while (board[left.first][left.second] == -1)
        {
            left.first--;
            left.second++;
            if (!isInBoard(left.first, left.second))
                goto nextLoop6;
        }
        right = left;
        len = 1;
        while (isInBoard(right.first - 1, right.second + 1) && board[left.first][left.second] == board[right.first - 1][right.second + 1])
        {
            --right.first;
            ++right.second;
            ++len;
        }

        tmpLeft.first = left.first;
        tmpLeft.second = left.second;
        tmpRight.first = right.first;
        tmpRight.second = right.second;
        leftLen = rightLen = 0;
        while (isInBoard(tmpLeft.first + 1, tmpLeft.second - 1) && board[tmpLeft.first + 1][tmpLeft.second - 1] == -1)
        {
            ++tmpLeft.first;
            --tmpLeft.second;
            ++leftLen;
        }
        while (isInBoard(tmpRight.first - 1, tmpRight.second + 1) && board[tmpRight.first - 1][tmpRight.second + 1] == -1)
        {
            --tmpRight.first;
            ++tmpRight.second;
            ++rightLen;
        }

        if (board[left.first][left.second] == ai_side)
        {
            if (len == 5)
                ++curSituation_ai.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.alive_4;
                        break;
                    case 3:
                        ++curSituation_ai.alive_3;
                        break;
                    case 2:
                        ++curSituation_ai.alive_2;
                        break;
                    case 1:
                        ++curSituation_ai.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_ai.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first + 2, tmpLeft.second - 2) && board[tmpLeft.first + 2][tmpLeft.second - 2] == ai_side) || (isInBoard(tmpRight.first - 2, tmpRight.second + 2) && board[tmpRight.first - 2][tmpRight.second + 2] == ai_side))
                            ++curSituation_ai.die_4;
                        else
                            ++curSituation_ai.die_3;
                        break;
                    case 2:
                        ++curSituation_ai.die_2;
                        break;
                    }
                }
            }
        }
        else
        {
            if (len == 5)
                ++curSituation_human.win_5;
            else
            {
                if (leftLen >= 1 && rightLen >= 1 && leftLen + rightLen >= 5 - len)
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.alive_4;
                        break;
                    case 3:
                        ++curSituation_human.alive_3;
                        break;
                    case 2:
                        ++curSituation_human.alive_2;
                        break;
                    case 1:
                        ++curSituation_human.alive_1;
                        break;
                    }
                else if (leftLen + rightLen >= 5 - len)
                {
                    switch (len)
                    {
                    case 4:
                        ++curSituation_human.die_4;
                        break;
                    case 3:
                        if ((isInBoard(tmpLeft.first + 2, tmpLeft.second - 2) && board[tmpLeft.first + 2][tmpLeft.second - 2] == 1 - ai_side) || (isInBoard(tmpRight.first - 2, tmpRight.second + 2) && board[tmpRight.first - 2][tmpRight.second + 2] == 1 - ai_side))
                            ++curSituation_human.die_4;
                        else
                            ++curSituation_human.die_3;
                        break;
                    case 2:
                        ++curSituation_human.die_2;
                        break;
                    }
                }
            }
        }
        if (isInBoard(right.first - 1, right.second + 1))
        {
            left = std::make_pair(right.first - 1, right.second + 1);
            goto continueCount6;
        }
    nextLoop6:;
    }
    val.aiValue += (curSituation_ai.win_5 * AI_WIN_5 + curSituation_ai.alive_4 * AI_ALIVE_4 + curSituation_ai.alive_3 * AI_ALIVE_3 + curSituation_ai.alive_2 * AI_ALIVE_2 + curSituation_ai.alive_1 * AI_ALIVE_1 + curSituation_ai.die_4 * AI_DIE_4 + curSituation_ai.die_3 * AI_DIE_3 + curSituation_ai.die_2 * AI_DIE_2);
    val.humanValue += (curSituation_human.win_5 * HUMAN_WIN_5 + curSituation_human.alive_4 * HUMAN_ALIVE_4 + curSituation_human.alive_3 * HUMAN_ALIVE_3 + curSituation_human.alive_2 * HUMAN_ALIVE_2 + curSituation_human.alive_1 * HUMAN_ALIVE_1 + curSituation_human.die_4 * HUMAN_DIE_4 + curSituation_human.die_3 * HUMAN_DIE_3 + curSituation_human.die_2 * HUMAN_DIE_2);
    val.aiValue -= val.humanValue;
    return val;
}
bool hasNeighbour(std::pair<int, int> cur, int side)
{
    for (register int i = 1; i <= 4; ++i)
    {
        direction d = dir(i);
        for (register int j = 1; j <= NEIGHBOUR_SEARCH_WIDTH; ++j)
        {
            std::pair<int, int> tmp = nextPoint(cur, i, j);
            if (isInBoard(tmp.first, tmp.second) && board[tmp.first][tmp.second] != -1)
                return true;
            tmp = nextPoint(cur, i, -j);
            if (isInBoard(tmp.first, tmp.second) && board[tmp.first][tmp.second] != -1)
                return true;
        }
    }
    return false;
}
std::vector<std::pair<int, int>> inspiredSearch(int side)
{
    std::vector<std::pair<int, int>> ans;
    if (side == ai_side)
    {
        std::priority_queue<waitPoint, std::vector<waitPoint>, cmp> q;
        for (register int i = 0; i < 15; ++i)
            for (register int j = 0; j < 15; ++j)
            {
                if (isEmpty(i, j) && hasNeighbour(std::make_pair(i, j), side))
                {
                    place(i, j, side);
                    q.push(waitPoint(wholeEvaluation(), {i, j}));
                    unplace(i, j);
                }
            }
        for (register int i = 0; i < NUM_OF_INSPIRED_SEARCH; ++i)
        {
            ans.push_back(q.top().cur);
            q.pop();
        }
    }
    else
    {
        std::priority_queue<waitPoint, std::vector<waitPoint>, _cmp> q;
        for (register int i = 0; i < 15; ++i)
            for (register int j = 0; j < 15; ++j)
            {
                if (isEmpty(i, j) && hasNeighbour(std::make_pair(i, j), 1 - side))
                {
                    place(i, j, 1 - side);
                    q.push(waitPoint(wholeEvaluation(), {i, j}));
                    unplace(i, j);
                }
            }
        for (register int i = 0; i < NUM_OF_INSPIRED_SEARCH; ++i)
        {
            ans.push_back(q.top().cur);
            q.pop();
        }
    }

    return ans;
}
value findAiMove(std::pair<int, int> &bestMove, int depth, int alpha, int beta)
{
    std::pair<int, int> dc;
    value val, responseVal;
    std::priority_queue<waitPoint, std::vector<waitPoint>, cmp> q;
    if (isFull())
        return wholeEvaluation();
    else if (depth == DEPTH_LIMIT)
        return wholeEvaluation();
    else
    {
        std::vector<std::pair<int, int>> tmp = inspiredSearch(ai_side);
        val.aiValue = alpha;
        bestMove = tmp[0];
        for (auto p : tmp)
        {
            if (val.aiValue > beta)
                break;
            if (isEmpty(p.first, p.second))
            {
                place(p.first, p.second, ai_side);
                responseVal = findHumanMove(dc, depth + 1, val.aiValue, beta);
                unplace(p.first, p.second);
                if (depth == 1)
                    q.push(waitPoint(responseVal, p));
                else if (responseVal.aiValue > val.aiValue)
                {
                    val = responseVal;
                    bestMove = {p.first, p.second};
                }
            }
        }
    }
    if (depth == 1)
        intelligentRandom(bestMove, val, q);
    return val;
}
value findHumanMove(std::pair<int, int> &bestMove, int depth, int alpha, int beta)
{
    std::pair<int, int> dc;
    value val, responseVal;
    if (isFull())
        return wholeEvaluation();
    else if (depth == DEPTH_LIMIT)
        return wholeEvaluation();
    else
    {
        std::vector<std::pair<int, int>> tmp = inspiredSearch(ai_side);
        val.aiValue = beta;
        bestMove = tmp[0];
        for (auto p : tmp)
        {
            if (alpha > val.aiValue)
                break;
            if (isEmpty(p.first, p.second))
            {
                place(p.first, p.second, 1 - ai_side);
                responseVal = findHumanMove(dc, depth + 1, alpha, val.aiValue);
                unplace(p.first, p.second);
                if (responseVal.aiValue < val.aiValue)
                {
                    val = responseVal;
                    bestMove = {p.first, p.second};
                }
            }
        }
    }
    return val;
}
void intelligentRandom(std::pair<int, int> &bestMove, value &val, std::priority_queue<waitPoint, std::vector<waitPoint>, cmp> &q)
{
    waitPoint first = q.top();
    q.pop();
    waitPoint second = q.top();
    if (first.val.aiValue <= second.val.aiValue * 50)
    {
        std::default_random_engine e(time(0));
        std::bernoulli_distribution u;
        if (u(e) == 1)
        {
            bestMove = first.cur;
            val = first.val;
        }
        else
        {
            bestMove = second.cur;
            val = second.val;
        }
    }
    else
    {
        bestMove = first.cur;
        val = first.val;
    }
}
value minimax(std::pair<int, int> &bestMove, int depth, int alpha, int beta, int side)
{
    value val, maxVal, minVal;
    std::pair<int, int> dc;
    if (depth == 0)
        return wholeEvaluation();
    if (side == ai_side)
    {
        maxVal.aiValue = NEGIFF;
        std::vector<std::pair<int, int>> tmp = inspiredSearch(ai_side);
        for (auto p : tmp)
        {
            place(p.first, p.second, ai_side);
            val = minimax(dc, depth - 1, alpha, beta, 1 - ai_side);
            unplace(p.first, p.second);
            if (val.aiValue > maxVal.aiValue)
            {
                maxVal = val;
                bestMove = {p.first, p.second};
            }
            alpha = std::max(alpha, val.aiValue);
            if (beta <= alpha)
                break;
        }
        return maxVal;
    }
    else
    {
        minVal.aiValue = IFF;
        std::vector<std::pair<int, int>> tmp = inspiredSearch(1 - ai_side);
        for (auto p : tmp)
        {
            place(p.first, p.second, 1 - ai_side);
            val = minimax(dc, depth - 1, alpha, beta, ai_side);
            unplace(p.first, p.second);
            if (val.aiValue < minVal.aiValue)
            {
                minVal = val;
                bestMove = {p.first, p.second};
            }
            beta = std::min(beta, val.aiValue);
            if (beta <= alpha)
                break;
        }
    }
    return minVal;
}
void flip()
{
    for (register int i = 0; i < 15; ++i)
        for (register int j = 0; j < 15; ++j)
            if (board[i][j] != -1)
                board[i][j] = 1 - board[i][j];
}
std::priority_queue<waitPoint, std::vector<waitPoint>, cmp> decisiveAction()
{
    //return std::make_pair(-1, -1);
    std::priority_queue<waitPoint, std::vector<waitPoint>, cmp> q;
    for (register int i = 0; i < 15; ++i)
        for (register int j = 0; j < 15; ++j)
        {
            if (isEmpty(i, j))
            {
                place(i, j, ai_side);
                if (judge(std::make_pair(i, j), ai_side))
                {
                    q.push(waitPoint(wholeEvaluation(), {i, j}));
                }
                unplace(i, j);
            }
        }
    if (!q.empty())
        return q;
    for (register int i = 0; i < 15; ++i)
        for (register int j = 0; j < 15; ++j)
        {
            if (isEmpty(i, j))
            {
                place(i, j, 1 - ai_side);
                if (judge(std::make_pair(i, j), 1 - ai_side))
                {
                    board[i][j] = ai_side;
                    q.push(waitPoint(wholeEvaluation(), {i, j}));
                }
                unplace(i, j);
            }
        }
    if (!q.empty())
        return q;

    for (register int i = 0; i < 15; ++i)
        for (register int j = 0; j < 15; ++j)
        {
            if (isEmpty(i, j))
            {
                place(i, j, ai_side);
                if (judge_alive4({i, j}, ai_side))
                {
                    q.push(waitPoint(wholeEvaluation(), {i, j}));
                }
                unplace(i, j);
            }
        }
    if (!q.empty())
        return q;
    for (register int i = 0; i < 15; ++i)
        for (register int j = 0; j < 15; ++j)
        {
            if (isEmpty(i, j))
            {
                place(i, j, 1 - ai_side);
                if (judge_alive4({i, j}, 1 - ai_side))
                {
                    board[i][j] = ai_side;
                    q.push(waitPoint(wholeEvaluation(), {i, j}));
                }
                unplace(i, j);
            }
        }
    if (!q.empty())
        return q;

    for (register int i = 0; i < 15; ++i)
        for (register int j = 0; j < 15; ++j)
        {
            if (isEmpty(i, j))
            {
                place(i, j, ai_side);
                if (judge_double3({i, j}, ai_side))
                {
                    q.push(waitPoint(wholeEvaluation(), {i, j}));
                }
                unplace(i, j);
            }
        }
    if (!q.empty())
        return q;

    for (register int i = 0; i < 15; ++i)
        for (register int j = 0; j < 15; ++j)
        {
            if (isEmpty(i, j))
            {
                place(i, j, 1 - ai_side);
                if (judge_double3({i, j}, 1 - ai_side))
                {
                    board[i][j] = ai_side;
                    q.push(waitPoint(wholeEvaluation(), {i, j}));
                }
                unplace(i, j);
            }
        }
    return q;
}

};


#endif // GAMEMODEL_H
