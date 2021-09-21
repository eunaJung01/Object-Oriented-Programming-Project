#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class MineSweeper
{
private:
	int m, n; // ���� ũ�� : m x n
	int level; // ���̵� : ��(2), ��(1), ��(0)
	double mine_ratio[3] = { 0.1, 0.2, 0.3 }; // ���� ����
	int mine_num; // ���� �� ����
	int** data; // ���� ��ġ��
	string** map; // ���� ȭ��
	pair<int, int> cur; // Ŀ�� ��ġ
	vector<pair<int, int>> memory; // ���� ��ġ ���� vector
	int start_x = 2, start_y = 3; // Ŀ�� ���� ��ġ (���� ó�� ���� ��)

public:
	MineSweeper();
	~MineSweeper();

	void start(); // ����
	bool restartQ(); // �̾��ϱ� ����
	void restart(); // �̾��ϱ�
	bool again(); // ���� ���� �� ����
	bool menu(); // �޴�
	void setData(); // �ʱ�ȭ
	void setMine(); // ���� �ʱ�ȭ
	int count(int i1, int j1, int i2, int j2); // �ֺ� ���� ���� ����

	void printMap(); // ���� ȭ�� ���
	void play();
	bool ing(int row, int col);
	void eight(int row, int col); // 8���� ���� �ڵ� ��ħ

	void showMine(); // �¸� �� ���� ��� ���̱�
	void replay(); // ���÷���
	void reMap(); // map �ʱ�ȭ
	void deleteData(); // ���� �Ҵ� �迭 ����

	pair<int, int> cursor(int& x, int& y); // ����Ű �̵�
	void gotoxy(int x, int y);

	void quit(int x, int y); // ���α׷� ���� ����
	void save(int x, int y); // ����

	void clear_cin(); // ����ó�� �� �Է� ���� �ʱ�ȭ �� '\n' �����
};
