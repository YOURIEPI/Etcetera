// com2us2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Node {
	int g;
	int h;
	pair<int, int> pos;
	Node* parent;

	Node(int H, int G, pair<int, int> Pos, Node* Parent = NULL) : g(G), h(H), pos(Pos), parent(Parent) {}
	bool operator <(const Node& node) const {
		return g + h < node.g + node.h;
	}
};

void ShortestPathUseAStar(vector<vector<int>>& map);
bool BorderCheck(vector<vector<int>>& map, bool** inCloseList, int y, int x);
int GetHeuristic(pair<int, int> p1, pair<int, int> p2);
int GetGeometry(pair<int, int> p1, pair<int, int> p2);

void ShortestPathUseAStar(vector<vector<int>>& map) {

	int row = map.size();
	int col = map[0].size();
	int offsetY[4] = { 1, -1, 0, 0 };
	int offsetX[4] = { 0, 0, 1, -1 };

	pair<int, int> startPos, endPos;

	for (int i = 0; i<row; i++)
	{
		for (int j = 0; j<col; j++)
		{
			if (map[i][j] == 2)
			{
				startPos = { i, j };
			}
			else if (map[i][j] == 3)
			{
				endPos = { i, j };
			}
		}
	}

	priority_queue<Node, vector<Node>, less<Node>> openList;

	bool** inCloseList = new bool*[row];
	for (int i = 0; i < row; i++) {
		inCloseList[i] = new bool[col];
		inCloseList[i] = { 0 };
	}

	pair<int, int>** pathRecord = new pair<int, int>*[row];
	for (int i = 0; i < row;i++) {
		pathRecord[i] = new pair<int, int>[col];
	}
	

	Node startNode(GetHeuristic(startPos, endPos), GetGeometry(startPos, endPos), startPos);
	openList.push(startNode);
	

	while (!openList.empty())
	{
		Node cur = openList.top();
		openList.pop();
		inCloseList[cur.pos.first][cur.pos.second] = true;

		if (cur.pos == endPos) {
			break;
		}

		for (int j = 0; j < 4; j++)
		{
			int y = cur.pos.first + offsetY[j];
			int x = cur.pos.second + offsetX[j];
			pair<int, int> pos = { y, x };

			if (BorderCheck(map, inCloseList, y, x)) {
				Node n(GetHeuristic(pos, endPos), GetGeometry(pos, startPos), pos, &cur);
				openList.push(n);
			}
		}
	}

	for (auto i = pathRecord[endPos.first][endPos.second]; i != startPos; i = pathRecord[i.first][i.second])
	{
		map[i.first][i.second] = 4;
	}
	
	delete[] pathRecord;
	delete[] inCloseList;
}

bool BorderCheck(vector<vector<int>>& map, bool** inCloseList, int y, int x)
{
	if (y >= map.size() || y < 0) return false;
	if (x >= map[0].size() || x < 0) return false;
	if (map[y][x] == 1) return false;
	if (inCloseList[y][x]) return false;
	
	return true;
}

int GetHeuristic(pair<int, int> p1, pair<int, int> p2) {
	return (int)(10 * (sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2))));
}

int GetGeometry(pair<int, int> p1, pair<int, int> p2) {
	return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

int main()
{
	vector<vector<int>> map = 
	{
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 2,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1 },
		{ 1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1 },
		{ 1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1 },
		{ 1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1 },
		{ 1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1 },
		{ 1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1 },
		{ 1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1 },
		{ 1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1 },
		{ 1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1 },
		{ 1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1 },
		{ 1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,0,1 },
		{ 1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1 },
		{ 1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1 },
		{ 1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1 },
		{ 1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1 },
		{ 1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,1 },
		{ 1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,3 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
	};

	ShortestPathUseAStar(map);

	for (int i = 0; i<map.size(); i++)
	{
		for (int j = 0; j<map[i].size(); j++)
		{
			if (map[i][j] == 1) cout << "#";
			else if (map[i][j] == 4) cout << "*";
			else cout << " ";
		}
		cout << endl;
	}

    return 0;
}

