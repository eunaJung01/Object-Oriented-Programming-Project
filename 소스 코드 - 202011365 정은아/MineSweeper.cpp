#include "MineSweeper.h"
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <exception>
#include <limits>
#undef max
using namespace std;

MineSweeper::MineSweeper() {}

MineSweeper::~MineSweeper()
{
	deleteData();
}

void MineSweeper::start()
{
	while (1) {
		ifstream file("save.txt"); // ���� ���� ���� ���� Ȯ��
		if (!file.is_open()) {
			cerr << "���� ���� ����";
			exit(0);
		}
		string temp = "";
		file >> temp;
		file.close();

		if (temp.compare("") != 0) { // ����� ������ ���� ���
			if (restartQ()) {
				restart();
				if (!again())
					break;
			}
		}
		menu();
		setData();
		setMine();
		play();
		replay();
		if (!again())
			break;
	}
}

// �̾��ϱ� ����
bool MineSweeper::restartQ() {
	while (1) {
		try {
			cout << "202011365 ������\n" << endl;
			int ans;
			cout << "�߰��� ������ ������ �ֽ��ϴ�.\n�ٽ� �����Ͻðڽ��ϱ�? (��(1), �ƴϿ�(2)) : ";
			cin >> ans;
			if (ans == 1) {
				cout << "\n\n2�� �Ŀ� ������ �ٽ� �����մϴ�." << endl;
				Sleep(2000);
				system("cls");
				return true;
			}
			else if (ans == 2) {
				cout << "\n����Ǿ� �ִ� �����ʹ� ��������, �� ������ �����մϴ�." << endl;
				ofstream file;
				file.open("save.txt", std::ofstream::out | std::ofstream::trunc); // file ���� �����
				file.close();
				Sleep(2000);
				system("cls");
				return false;
			}
			else {
				throw ans;
			}
		}
		catch (int ansExp) { // �Է� ���� ó��
			cout << "\n�ٽ� �Է����ּ���. (1 �Ǵ� 2 �Է�)" << endl;
			Sleep(2000);
			system("cls");
			clear_cin();
		}
	}
}

// �̾��ϱ�
void MineSweeper::restart()
{
	ifstream file("save.txt");
	file >> m >> n >> level >> mine_num;
	file >> start_x >> start_y;

	setData();
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			file >> data[i][j];
		}
	}
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			file >> map[i][j];
		}
	}

	while (!file.eof()) {
		if (file.eof()) { break; }
		int row, col;
		file >> row >> col;
		pair<int, int> point = make_pair(row, col);
		memory.push_back(point);
	}
	memory.pop_back(); // ������ �ߺ� ���尪 ����
	file.close();

	play();
	replay();

	// �̾��ϱ� �� ���� �ʱ�ȭ
	start_x = 2;
	start_y = 3;
	memory.clear();
	ofstream file2;
	file2.open("save.txt", std::ofstream::out | std::ofstream::trunc); // file ���� �����
	file2.close();
}

// ���� ���� �� ����
bool MineSweeper::again()
{
	while (1) {
		try {
			int re;
			cout << "202011365 ������\n" << endl;
			cout << "������ �ٽ� �����Ͻðڽ��ϱ�? (��(1), �ƴϿ�(2)) : ";
			cin >> re;
			if (re == 1) {
				memory.clear();
				deleteData();
				system("cls");
				return true;
			}
			else if (re == 2) {
				cout << "\n����ã�� ������ �����մϴ�." << endl;
				return false;
			}
			else {
				throw re;
			}
		}
		catch (int reExp) { // �Է� ���� ó��
			cout << "\n�ٽ� �Է����ּ���. (1 �Ǵ� 2 �Է�)" << endl;
			Sleep(2000);
			system("cls");
			clear_cin();
		}
	}
}

bool MineSweeper::menu()
{
	while (1) {
		try {
			string m_st, n_st; // �� ũ�� �Է�
			string level_st; // ���̵� �Է�
			cout << "202011365 ������\n" << endl;

			cout << "���� ũ�⸦ �Է��ϼ��� (m�� n��) : ";
			cin >> m_st >> n_st;
			if (m_st.compare("0") <= 0 || m_st.compare("999999") > 0 || n_st.compare("0") <= 0 || n_st.compare("86") > 0) { // �� : 1~9999, �� : 1~86
				cout << "\n�� ũ�⸦ �ٽ� �Է����ּ���. (�� : 1 ~ 999999, �� : 1~86)" << endl;
				throw m_st;
			}
			m = stoi(m_st);
			n = stoi(n_st);

			cout << "���̵��� �Է��ϼ��� (��, ��, ��) : ";
			cin >> level_st;
			if (level_st.compare("��") != 0 && level_st.compare("��") != 0 && level_st.compare("��") != 0) {
				cout << "\n���̵��� �ٽ� �Է����ּ���. (��, ��, �� �߿��� �Է�)" << endl;
				throw level_st;
			}
			if (level_st.compare("��") == 0) { level = 2; }
			else if (level_st.compare("��") == 0) { level = 1; }
			else if (level_st.compare("��") == 0) { level = 0; }

			cout << "\n" << m << "�� " << n << "�� ũ���� ���̵� \"" << level_st << "\" ������ 3�� �Ŀ� �����մϴ�!" << endl;
			cout << "(���� �ߴ��� esc)" << endl;
			Sleep(3000);
			system("cls");
			break;
		}
		// �Է� ���� ó��
		catch (string exp) {
			Sleep(2000);
			system("cls");
			clear_cin();
		}
	}
}

// �ʱ�ȭ
void MineSweeper::setData()
{
	data = new int* [m + 2];
	for (int i = 0; i < m + 2; i++) {
		data[i] = new int[n + 2];
	}
	map = new string * [m + 2];
	for (int i = 0; i < m + 2; i++) {
		map[i] = new string[n + 2];
	}
	for (int i = 0; i < m + 2; i++) { // data, map �ʱ�ȭ
		for (int j = 0; j < n + 2; j++) {
			data[i][j] = 0;
			if (i == 0 || j == 0 || i == m + 1 || j == n + 1)
				map[i][j] = "��"; // �׵θ�
			else
				map[i][j] = "��";
		}
	}
}

void MineSweeper::setMine()
{
	mine_num = m * n * mine_ratio[level]; // ���� ����
	srand(time(0));
	for (int i = 0; i < mine_num; i++) { // ���� ��ġ ����
		int m_m = rand() % m + 1;
		int m_n = rand() % n + 1;
		if (data[m_m][m_n] != 9) { data[m_m][m_n] = 9; } // ���ڰ� �ִ� �� ǥ�� : 9
		else { i--; }
	}
	for (int i = 1; i <= m; i++) { // �ֺ� ���� ���� ����
		for (int j = 1; j <= n; j++) {
			if (data[i][j] != 9)
				data[i][j] = count(i - 1, j - 1, i + 1, j + 1);
		}
	}
}

// �ֺ� ���� ���� ����
int MineSweeper::count(int i1, int j1, int i2, int j2)
{
	int num = 0;
	for (int i = i1; i <= i2; i++) {
		for (int j = j1; j <= j2; j++) {
			if (data[i][j] == 9) { num++; }
		}
	}
	return num;
}

// ���
void MineSweeper::printMap()
{
	cout << "202011365 ������\n" << endl;

	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			cout << setw(2) << map[i][j];
		}
		cout << endl;
	}
	cout << endl;

	// ���� ��ġ��
	/*
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			cout << setw(2) << data[i][j];
		}
		cout << endl;
	}
	//*/
}

void MineSweeper::play()
{
	int x = start_x, y = start_y; // Ŀ�� ���� ��ǥ
	printMap();
	while (1) {
		cur = cursor(x, y); // x(��), y(��)
		int row = cur.second - 2;
		int col = cur.first / 2;
		pair<int, int> point = make_pair(row, col);
		memory.push_back(point); // ��ġ ����
		if (!ing(row, col)) {
			break;
		}
	}
	Sleep(3000);
	system("cls");
}

bool MineSweeper::ing(int row, int col)
{
	if (row > 0 && col > 0 && row < m + 2 && col < n + 2) {
		if (data[row][col] == 9) {
			map[row][col] = "��";
		}
		else if (data[row][col] == 0 && map[row][col].compare("��") == 0) {
			eight(row, col); // 8���� ���� �ڵ� ��ħ
		}
		else {
			map[row][col] = to_string(data[row][col]);
		}
	}
	system("cls");
	printMap();

	if (data[row][col] == 9) { // �й�
		showMine(); // ���� ��� ���̱�
		cout << "����!" << endl;
		return false;
	}

	int count = 0;
	for (int i = 1; i < m + 2; i++) {
		for (int j = 1; j < n + 2; j++) {
			if (map[i][j].compare("��") == 0) {
				count++;
			}
		}
	}
	if (count == mine_num) { // �¸�
		showMine(); // ���� ��� ���̱�
		cout << "�¸�!" << endl;
		return false;
	}
}

// 8���� ���� �ڵ� ��ħ
void MineSweeper::eight(int row, int col)
{
	for (int i = row - 1; i < row + 2; i++) {
		for (int j = col - 1; j < col + 2; j++) {
			if (i > 0 && j > 0 && i < m + 1 && j < n + 1 && map[i][j].compare("��") == 0) {
				if (data[i][j] == 9) {
					map[i][j] = "��";
				}
				else {
					map[i][j] = to_string(data[i][j]);
				}
			}
		}
	}
}

// �¸� �Ǵ� �й� �� ���� ��� ���̱�
void MineSweeper::showMine()
{
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			if (data[i][j] == 9)
				map[i][j] = "��";
		}
	}
	system("cls");
	printMap();
}

// ���÷���
void MineSweeper::replay()
{
	reMap();
	for (int i = 0; i < memory.size(); i++) {
		system("cls");
		ing(memory[i].first, memory[i].second);
		cout << "- ���÷��� -" << endl;
		Sleep(700);
	}
	Sleep(2300);
	system("cls");
}

// map �ʱ�ȭ
void MineSweeper::reMap()
{
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			if (i == 0 || j == 0 || i == m + 1 || j == n + 1)
				map[i][j] = "��"; // �׵θ�
			else
				map[i][j] = "��";
		}
	}
}

// ���� �Ҵ� �迭 ����
void MineSweeper::deleteData()
{
	if (data != nullptr) {
		for (int i = 0; i < m + 2; i++) {
			delete[] data[i];
		}
		delete[] data;
		data = nullptr;
	}
	if (map != nullptr) {
		for (int i = 0; i < m + 2; i++) {
			delete[] map[i];
		}
		delete[] map;
		map = nullptr;
	}
}

// ����Ű �̵�
pair<int, int> MineSweeper::cursor(int& x, int& y) {
	int ch;
	while (1)
	{
		gotoxy(x, y);
		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			switch (ch)
			{
			case 75:
				if (x != 2) { x -= 2; }
				break;
			case 77:
				if (x != 2 + 2 * n - 2) { x += 2; }
				break;
			case 72:
				if (y != 3) { y--; }
				break;
			case 80:
				if (y != 3 + m - 1) { y++; }
				break;
			}
		}
		else if (ch == 13) { // ����Ű
			pair<int, int> p = make_pair(x, y);
			return p;
		}
		else if (ch == 27) { // esc
			quit(x, y);
		}
	}
}

void MineSweeper::gotoxy(int x, int y) {
	COORD Pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// ���α׷� ���� ����
void MineSweeper::quit(int x, int y)
{
	system("cls");
	cout << "202011365 ������\n" << endl;
	while (1) {
		try {
			int ans;
			cout << "���α׷��� �����Ͻðڽ��ϱ�? (��(1), �ƴϿ�(2)) : ";
			cin >> ans;
			if (ans == 1) {
				cout << "���ݱ����� ������ �����Ͻðڽ��ϱ�? (��(1), �ƴϿ�(2)) : ";
				cin >> ans;
				if (ans == 1) {
					cout << "\n������ �����ϰ� �����մϴ�." << endl;
					save(x, y);
					deleteData();
					exit(0);
				}
				else if (ans == 2) {
					cout << "\n������ �������� �ʰ� �����մϴ�." << endl;
					deleteData();
					exit(0);
				}
				else {
					throw ans;
				}
			}
			else if (ans == 2) {
				cout << "\n3�� �� �������� ���ư��ϴ�." << endl;
				Sleep(3000);
				system("cls");
				printMap();
				break;
			}
			else {
				throw ans;
			}
		}
		catch (int exp) { // �Է� ���� ó��
			cout << "\n�ٽ� �Է����ּ���. (1 �Ǵ� 2 �Է�)" << endl;
			Sleep(3000);
			system("cls");
			clear_cin();
		}
	}
}

// ����
void MineSweeper::save(int x, int y)
{
	ofstream file("save.txt");
	file << m << " " << n << " " << level << " " << mine_num << "\n";
	file << x << " " << y << "\n"; // ������ Ŀ�� ��ġ
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			file << to_string(data[i][j]) << " ";
		}
		file << "\n";
	}
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			file << map[i][j] << " ";
		}
		file << "\n";
	}
	for (int i = 0; i < memory.size(); i++) {
		file << memory[i].first << " " << memory[i].second << "\n";
	}
	file.close();
}

void MineSweeper::clear_cin()
{
	cin.clear(); // �ʱ�ȭ
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ���� ����
}
