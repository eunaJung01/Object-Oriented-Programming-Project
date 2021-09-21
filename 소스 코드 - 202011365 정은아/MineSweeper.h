#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class MineSweeper
{
private:
	int m, n; // 맵의 크기 : m x n
	int level; // 난이도 : 상(2), 중(1), 하(0)
	double mine_ratio[3] = { 0.1, 0.2, 0.3 }; // 지뢰 비율
	int mine_num; // 지뢰 총 개수
	int** data; // 지뢰 위치도
	string** map; // 게임 화면
	pair<int, int> cur; // 커서 위치
	vector<pair<int, int>> memory; // 선택 위치 저장 vector
	int start_x = 2, start_y = 3; // 커서 시작 위치 (게임 처음 시작 시)

public:
	MineSweeper();
	~MineSweeper();

	void start(); // 시작
	bool restartQ(); // 이어하기 질문
	void restart(); // 이어하기
	bool again(); // 게임 종료 후 질문
	bool menu(); // 메뉴
	void setData(); // 초기화
	void setMine(); // 지뢰 초기화
	int count(int i1, int j1, int i2, int j2); // 주변 지뢰 개수 세기

	void printMap(); // 게임 화면 출력
	void play();
	bool ing(int row, int col);
	void eight(int row, int col); // 8방향 연속 자동 펼침

	void showMine(); // 승리 시 지뢰 모두 보이기
	void replay(); // 리플레이
	void reMap(); // map 초기화
	void deleteData(); // 동적 할당 배열 삭제

	pair<int, int> cursor(int& x, int& y); // 방향키 이동
	void gotoxy(int x, int y);

	void quit(int x, int y); // 프로그램 종료 여부
	void save(int x, int y); // 저장

	void clear_cin(); // 예외처리 시 입력 버퍼 초기화 및 '\n' 지우기
};
