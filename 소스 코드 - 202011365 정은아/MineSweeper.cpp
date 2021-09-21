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
		ifstream file("save.txt"); // 저장 게임 존재 유무 확인
		if (!file.is_open()) {
			cerr << "파일 오픈 실패";
			exit(0);
		}
		string temp = "";
		file >> temp;
		file.close();

		if (temp.compare("") != 0) { // 저장된 게임이 있을 경우
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

// 이어하기 질문
bool MineSweeper::restartQ() {
	while (1) {
		try {
			cout << "202011365 정은아\n" << endl;
			int ans;
			cout << "중간에 종료한 게임이 있습니다.\n다시 시작하시겠습니까? (예(1), 아니오(2)) : ";
			cin >> ans;
			if (ans == 1) {
				cout << "\n\n2초 후에 게임을 다시 시작합니다." << endl;
				Sleep(2000);
				system("cls");
				return true;
			}
			else if (ans == 2) {
				cout << "\n저장되어 있던 데이터는 지워지고, 새 게임을 생성합니다." << endl;
				ofstream file;
				file.open("save.txt", std::ofstream::out | std::ofstream::trunc); // file 내용 지우기
				file.close();
				Sleep(2000);
				system("cls");
				return false;
			}
			else {
				throw ans;
			}
		}
		catch (int ansExp) { // 입력 예외 처리
			cout << "\n다시 입력해주세요. (1 또는 2 입력)" << endl;
			Sleep(2000);
			system("cls");
			clear_cin();
		}
	}
}

// 이어하기
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
	memory.pop_back(); // 마지막 중복 저장값 삭제
	file.close();

	play();
	replay();

	// 이어하기 후 값들 초기화
	start_x = 2;
	start_y = 3;
	memory.clear();
	ofstream file2;
	file2.open("save.txt", std::ofstream::out | std::ofstream::trunc); // file 내용 지우기
	file2.close();
}

// 게임 종료 후 질문
bool MineSweeper::again()
{
	while (1) {
		try {
			int re;
			cout << "202011365 정은아\n" << endl;
			cout << "게임을 다시 시작하시겠습니까? (예(1), 아니오(2)) : ";
			cin >> re;
			if (re == 1) {
				memory.clear();
				deleteData();
				system("cls");
				return true;
			}
			else if (re == 2) {
				cout << "\n지뢰찾기 게임을 종료합니다." << endl;
				return false;
			}
			else {
				throw re;
			}
		}
		catch (int reExp) { // 입력 예외 처리
			cout << "\n다시 입력해주세요. (1 또는 2 입력)" << endl;
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
			string m_st, n_st; // 맵 크기 입력
			string level_st; // 난이도 입력
			cout << "202011365 정은아\n" << endl;

			cout << "맵의 크기를 입력하세요 (m행 n열) : ";
			cin >> m_st >> n_st;
			if (m_st.compare("0") <= 0 || m_st.compare("999999") > 0 || n_st.compare("0") <= 0 || n_st.compare("86") > 0) { // 행 : 1~9999, 열 : 1~86
				cout << "\n맵 크기를 다시 입력해주세요. (행 : 1 ~ 999999, 열 : 1~86)" << endl;
				throw m_st;
			}
			m = stoi(m_st);
			n = stoi(n_st);

			cout << "난이도를 입력하세요 (상, 중, 하) : ";
			cin >> level_st;
			if (level_st.compare("상") != 0 && level_st.compare("중") != 0 && level_st.compare("하") != 0) {
				cout << "\n난이도를 다시 입력해주세요. (상, 중, 하 중에서 입력)" << endl;
				throw level_st;
			}
			if (level_st.compare("상") == 0) { level = 2; }
			else if (level_st.compare("중") == 0) { level = 1; }
			else if (level_st.compare("하") == 0) { level = 0; }

			cout << "\n" << m << "행 " << n << "열 크기의 난이도 \"" << level_st << "\" 게임을 3초 후에 시작합니다!" << endl;
			cout << "(게임 중단은 esc)" << endl;
			Sleep(3000);
			system("cls");
			break;
		}
		// 입력 예외 처리
		catch (string exp) {
			Sleep(2000);
			system("cls");
			clear_cin();
		}
	}
}

// 초기화
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
	for (int i = 0; i < m + 2; i++) { // data, map 초기화
		for (int j = 0; j < n + 2; j++) {
			data[i][j] = 0;
			if (i == 0 || j == 0 || i == m + 1 || j == n + 1)
				map[i][j] = "◆"; // 테두리
			else
				map[i][j] = "▦";
		}
	}
}

void MineSweeper::setMine()
{
	mine_num = m * n * mine_ratio[level]; // 지뢰 개수
	srand(time(0));
	for (int i = 0; i < mine_num; i++) { // 지뢰 위치 지정
		int m_m = rand() % m + 1;
		int m_n = rand() % n + 1;
		if (data[m_m][m_n] != 9) { data[m_m][m_n] = 9; } // 지뢰가 있는 곳 표시 : 9
		else { i--; }
	}
	for (int i = 1; i <= m; i++) { // 주변 지뢰 개수 세기
		for (int j = 1; j <= n; j++) {
			if (data[i][j] != 9)
				data[i][j] = count(i - 1, j - 1, i + 1, j + 1);
		}
	}
}

// 주변 지뢰 개수 세기
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

// 출력
void MineSweeper::printMap()
{
	cout << "202011365 정은아\n" << endl;

	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			cout << setw(2) << map[i][j];
		}
		cout << endl;
	}
	cout << endl;

	// 지뢰 위치도
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
	int x = start_x, y = start_y; // 커서 시작 좌표
	printMap();
	while (1) {
		cur = cursor(x, y); // x(열), y(행)
		int row = cur.second - 2;
		int col = cur.first / 2;
		pair<int, int> point = make_pair(row, col);
		memory.push_back(point); // 위치 저장
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
			map[row][col] = "▶";
		}
		else if (data[row][col] == 0 && map[row][col].compare("▦") == 0) {
			eight(row, col); // 8방향 연속 자동 펼침
		}
		else {
			map[row][col] = to_string(data[row][col]);
		}
	}
	system("cls");
	printMap();

	if (data[row][col] == 9) { // 패배
		showMine(); // 지뢰 모두 보이기
		cout << "지뢰!" << endl;
		return false;
	}

	int count = 0;
	for (int i = 1; i < m + 2; i++) {
		for (int j = 1; j < n + 2; j++) {
			if (map[i][j].compare("▦") == 0) {
				count++;
			}
		}
	}
	if (count == mine_num) { // 승리
		showMine(); // 지뢰 모두 보이기
		cout << "승리!" << endl;
		return false;
	}
}

// 8방향 연속 자동 펼침
void MineSweeper::eight(int row, int col)
{
	for (int i = row - 1; i < row + 2; i++) {
		for (int j = col - 1; j < col + 2; j++) {
			if (i > 0 && j > 0 && i < m + 1 && j < n + 1 && map[i][j].compare("▦") == 0) {
				if (data[i][j] == 9) {
					map[i][j] = "▶";
				}
				else {
					map[i][j] = to_string(data[i][j]);
				}
			}
		}
	}
}

// 승리 또는 패배 시 지뢰 모두 보이기
void MineSweeper::showMine()
{
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			if (data[i][j] == 9)
				map[i][j] = "▶";
		}
	}
	system("cls");
	printMap();
}

// 리플레이
void MineSweeper::replay()
{
	reMap();
	for (int i = 0; i < memory.size(); i++) {
		system("cls");
		ing(memory[i].first, memory[i].second);
		cout << "- 리플레이 -" << endl;
		Sleep(700);
	}
	Sleep(2300);
	system("cls");
}

// map 초기화
void MineSweeper::reMap()
{
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < n + 2; j++) {
			if (i == 0 || j == 0 || i == m + 1 || j == n + 1)
				map[i][j] = "◆"; // 테두리
			else
				map[i][j] = "▦";
		}
	}
}

// 동적 할당 배열 삭제
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

// 방향키 이동
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
		else if (ch == 13) { // 엔터키
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

// 프로그램 종료 여부
void MineSweeper::quit(int x, int y)
{
	system("cls");
	cout << "202011365 정은아\n" << endl;
	while (1) {
		try {
			int ans;
			cout << "프로그램을 종료하시겠습니까? (예(1), 아니오(2)) : ";
			cin >> ans;
			if (ans == 1) {
				cout << "지금까지의 게임을 저장하시겠습니까? (예(1), 아니오(2)) : ";
				cin >> ans;
				if (ans == 1) {
					cout << "\n게임을 저장하고 종료합니다." << endl;
					save(x, y);
					deleteData();
					exit(0);
				}
				else if (ans == 2) {
					cout << "\n게임을 저장하지 않고 종료합니다." << endl;
					deleteData();
					exit(0);
				}
				else {
					throw ans;
				}
			}
			else if (ans == 2) {
				cout << "\n3초 후 게임으로 돌아갑니다." << endl;
				Sleep(3000);
				system("cls");
				printMap();
				break;
			}
			else {
				throw ans;
			}
		}
		catch (int exp) { // 입력 예외 처리
			cout << "\n다시 입력해주세요. (1 또는 2 입력)" << endl;
			Sleep(3000);
			system("cls");
			clear_cin();
		}
	}
}

// 저장
void MineSweeper::save(int x, int y)
{
	ofstream file("save.txt");
	file << m << " " << n << " " << level << " " << mine_num << "\n";
	file << x << " " << y << "\n"; // 마지막 커서 위치
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
	cin.clear(); // 초기화
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 버퍼 비우기
}
