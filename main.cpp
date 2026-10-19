#include <iostream>
#include <graphics.h>
#include <string.h>
#include <fstream>

//Upper
bool running = true;

//Board
int initboard[8][8]{
	{0, 0, 1, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0, 2},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 2, 0, 0, 2, 0, 0}
};
int boardstatus[8][8]{
	{0, 0, 1, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0, 2},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 2, 0, 0, 2, 0, 0}
};
int botboard[8][8];

//Control
int Stage = 1;
int code = 0;
int current_piece = 1;
int botcommand[6];

const TCHAR* title = _T("亚马逊棋");
const TCHAR* text1 = _T("双人对战");
const TCHAR* text2 = _T("存档");
const TCHAR* text3 = _T("退出游戏");
const TCHAR* text4 = _T("保存并退出");
const TCHAR* text5 = _T("直接退出");
const TCHAR* text6 = _T("人机对战");

ExMessage msg;

//Storage

struct info {
	int pe; // 1p, 2e;
	int storage[8][8];
	int status; // 1r, 2b;
	int ended; // 0 ended, 1;
};
info ar[8]{};

//Stream

void read() {
	std::ifstream infile("save.dat");
	for (int i = 0; i < 8; i++) {
		infile >> ar[i].pe;
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				infile >> ar[i].storage[j][k];
			}
		}
		infile >> ar[i].status;
		infile >> ar[i].ended;
	}
	infile.close();
}

void write() {
	std::ofstream outfile("save.dat");
	for (int i = 0; i < 8; i++) {
		outfile << ar[i].pe << " ";
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				outfile << ar[i].storage[j][k] << " ";
			}
		}
		outfile << ar[i].status << " ";
		outfile << ar[i].ended << " ";
	}
	outfile.close();
}

//Menu
bool clicked(int mx, int my, int x1, int y1, int x2, int y2) {
	if (mx >= x1 && mx <= x2 && my >= y1 && my <= y2) {
		return true;
	}
	return false;
}

bool button(int x1, int y1, int x2, int y2, const TCHAR* text, int size) {
	line(x1, y1, x2, y1);
	line(x1, y1, x1, y2);
	line(x2, y1, x2, y2);
	line(x1, y2, x2, y2);
	settextstyle(size, 0, L"等线");
	int hSpace = (x2 - x1 - textwidth(text)) / 2;
	int vSpace = (y2 - y1 - textheight(text)) / 2;
	outtextxy(x1 + hSpace, y1 + vSpace, text);

	if (msg.message == WM_LBUTTONDOWN && clicked(msg.x, msg.y, x1, y1, x2, y2)) {
		return true;
	}
	return false;
}

void menu() {
	settextcolor(WHITE);
	while (true) {
		peekmessage(&msg, EX_MOUSE);
		BeginBatchDraw();
		cleardevice();
		settextstyle(160, 0, L"宋体");
		outtextxy((1100 - textwidth(title)) / 2, 150, title);
		if (button(355, 380, 745, 440, text6, 37)) {
			bool flag = true;
			for (int i = 0; i < 8; i++) {
				if (ar[i].ended == 0) {
					ar[i].ended = 1;
					ar[i].pe = 2;
					ar[i].status = 1;
					memcpy(ar[i].storage, initboard, sizeof(initboard));
					code = i;
					flag = false;
					break;
				}
			}
			if (flag) {
				MessageBox(GetHWnd(), _T("无法开启新游戏"), _T("存档已满，请清理存档"), MB_OK);
				Stage = 4;
				return;
			}
			Stage = 3;
			return;
		}
		if (button(355, 460, 745, 520, text1, 37)) {
			bool flag = true;
			for (int i = 0; i < 8; i++) {
				if (ar[i].ended == 0) {
					ar[i].ended = 1;
					ar[i].pe = 1;
					ar[i].status = 1;
					memcpy(ar[i].storage, initboard, sizeof(initboard));
					code = i;
					flag = false;
					break;
				}
			}
			if (flag) {
				MessageBox(GetHWnd(), _T("无法开启新游戏"), _T("存档已满，请清理存档"), MB_OK);
				Stage = 4;
				return;
			}
			Stage = 2;
			return;
		}
		if (button(355, 540, 745, 600, text2, 37)) {
			Stage = 4;
			return;
		}
		if (button(355, 620, 745, 680, text3, 37)) {
			Stage = 0;
			return;
		}
		EndBatchDraw();
	}
}

//Sub

bool subbutton(int x1, int y1, int x2, int y2, const TCHAR* text, int size, int c) {
	if (button(x1, y1, x2, y2, text, size)) {
		if (ar[c].ended == 0) {
			memcpy(ar[c].storage, initboard, sizeof(initboard));
			code = c;
		}
		else {
			code = c;
		}
		return true;
	}
	return false;
}

void submenu() {
	std::wstring list[8];
	for (int i = 0; i < 8; i++) {
		if (ar[i].ended == 0) {
			list[i] = L"空";
		}
		else {
			if (ar[i].pe == 1) {
				list[i] = L"PvP";
			}
			else {
				list[i] = L"PvE";
			}
		}
	}
	const TCHAR* text0 = list[0].c_str();
	const TCHAR* text1 = list[1].c_str();
	const TCHAR* text2 = list[2].c_str();
	const TCHAR* text3 = list[3].c_str();
	const TCHAR* text4 = list[4].c_str();
	const TCHAR* text5 = list[5].c_str();
	const TCHAR* text6 = list[6].c_str();
	const TCHAR* text7 = list[7].c_str();
	const TCHAR* text8 = _T("返回主菜单");
	const TCHAR* text9 = _T("X");
	const TCHAR* text10 = _T("双人对战");
	const TCHAR* text11 = _T("人机对战");
	settextcolor(WHITE);
	msg.message = 0;
	while (true) {
		peekmessage(&msg, EX_MOUSE);
		BeginBatchDraw();
		cleardevice();
		if (subbutton(59, 50, 267, 300, text0, 60, 0)) {
			EndBatchDraw();
			msg.message = 0;
			if (ar[0].ended == 0) {
				BeginBatchDraw();
				cleardevice();
				EndBatchDraw();
				while (true) {
					peekmessage(&msg, EX_MOUSE);
					if (button(300, 100, 800, 200, text10, 60)) {
						Stage = 2;
						ar[0].ended = 1;
						ar[0].pe = 1;
						ar[0].status = 1;
						break;
					}
					if (button(300, 260, 800, 360, text11, 60)) {
						Stage = 3;
						ar[0].ended = 1;
						ar[0].pe = 2;
						ar[0].status = 1;
						break;
					}
				}
			}
			else {
				if (ar[0].pe == 1) {
					Stage = 2;
				}
				else {
					Stage = 3;
				}
			}
			return;
		}
		if (subbutton(317, 50, 525, 300, text1, 60, 1)) {
			EndBatchDraw();
			msg.message = 0;
			if (ar[1].ended == 0) {
				BeginBatchDraw();
				cleardevice();
				EndBatchDraw();
				while (true) {
					peekmessage(&msg, EX_MOUSE);
					if (button(300, 100, 800, 200, text10, 60)) {
						Stage = 2;
						ar[1].ended = 1;
						ar[1].pe = 1;
						ar[1].status = 1;
						break;
					}
					if (button(300, 260, 800, 360, text11, 60)) {
						Stage = 3;
						ar[1].ended = 1;
						ar[1].pe = 2;
						ar[1].status = 1;
						break;
					}
				}
			}
			else {
				if (ar[1].pe == 1) {
					Stage = 2;
				}
				else {
					Stage = 3;
				}
			}
			return;
		}
		if (subbutton(575, 50, 783, 300, text2, 60, 2)) {
			EndBatchDraw();
			msg.message = 0;
			if (ar[2].ended == 0) {
				BeginBatchDraw();
				cleardevice();
				EndBatchDraw();
				while (true) {
					peekmessage(&msg, EX_MOUSE);
					if (button(300, 100, 800, 200, text10, 60)) {
						Stage = 2;
						ar[2].ended = 1;
						ar[2].pe = 1;
						ar[2].status = 1;
						break;
					}
					if (button(300, 260, 800, 360, text11, 60)) {
						Stage = 3;
						ar[2].ended = 1;
						ar[2].pe = 2;
						ar[2].status = 1;
						break;
					}
				}
			}
			else {
				if (ar[2].pe == 1) {
					Stage = 2;
				}
				else {
					Stage = 3;
				}
			}
			return;
		}
		if (subbutton(833, 50, 1041, 300, text3, 60, 3)) {
			EndBatchDraw();
			msg.message = 0;
			if (ar[3].ended == 0) {
				BeginBatchDraw();
				cleardevice();
				EndBatchDraw();
				while (true) {
					peekmessage(&msg, EX_MOUSE);
					if (button(300, 100, 800, 200, text10, 60)) {
						Stage = 2;
						ar[3].ended = 1;
						ar[3].pe = 1;
						ar[3].status = 1;
						break;
					}
					if (button(300, 260, 800, 360, text11, 60)) {
						Stage = 3;
						ar[3].ended = 1;
						ar[3].pe = 2;
						ar[3].status = 1;
						break;
					}
				}
			}
			else {
				if (ar[3].pe == 1) {
					Stage = 2;
				}
				else {
					Stage = 3;
				}
			}
			return;
		}
		if (subbutton(59, 350, 267, 600, text4, 60, 4)) {
			EndBatchDraw();
			msg.message = 0;
			if (ar[4].ended == 0) {
				BeginBatchDraw();
				cleardevice();
				EndBatchDraw();
				while (true) {
					peekmessage(&msg, EX_MOUSE);
					if (button(300, 100, 800, 200, text10, 60)) {
						Stage = 2;
						ar[4].ended = 1;
						ar[4].pe = 1;
						ar[4].status = 1;
						break;
					}
					if (button(300, 260, 800, 360, text11, 60)) {
						Stage = 3;
						ar[4].ended = 1;
						ar[4].pe = 2;
						ar[4].status = 1;
						break;
					}
				}
			}
			else {
				if (ar[4].pe == 1) {
					Stage = 2;
				}
				else {
					Stage = 3;
				}
			}
			return;
		}
		if (subbutton(317, 350, 525, 600, text5, 60, 5)) {
			EndBatchDraw();
			msg.message = 0;
			if (ar[5].ended == 0) {
				BeginBatchDraw();
				cleardevice();
				EndBatchDraw();
				while (true) {
					peekmessage(&msg, EX_MOUSE);
					if (button(300, 100, 800, 200, text10, 60)) {
						Stage = 2;
						ar[5].ended = 1;
						ar[5].pe = 1;
						ar[5].status = 1;
						break;
					}
					if (button(300, 260, 800, 360, text11, 60)) {
						Stage = 3;
						ar[5].ended = 1;
						ar[5].pe = 2;
						ar[5].status = 1;
						break;
					}
				}
			}
			else {
				if (ar[5].pe == 1) {
					Stage = 2;
				}
				else {
					Stage = 3;
				}
			}
			return;
		}
		if (subbutton(575, 350, 783, 600, text6, 60, 6)) {
			EndBatchDraw();
			msg.message = 0;
			if (ar[6].ended == 0) {
				BeginBatchDraw();
				cleardevice();
				EndBatchDraw();
				while (true) {
					peekmessage(&msg, EX_MOUSE);
					if (button(300, 100, 800, 200, text10, 60)) {
						Stage = 2;
						ar[6].ended = 1;
						ar[6].pe = 1;
						ar[6].status = 1;
						break;
					}
					if (button(300, 260, 800, 360, text11, 60)) {
						Stage = 3;
						ar[6].ended = 1;
						ar[6].pe = 2;
						ar[6].status = 1;
						break;
					}
				}
			}
			else {
				if (ar[6].pe == 1) {
					Stage = 2;
				}
				else {
					Stage = 3;
				}
			}
			return;
		}
		if (subbutton(833, 350, 1041, 600, text7, 60, 7)) {
			EndBatchDraw();
			msg.message = 0;
			if (ar[7].ended == 0) {
				BeginBatchDraw();
				cleardevice();
				EndBatchDraw();
				while (true) {
					peekmessage(&msg, EX_MOUSE);
					if (button(300, 100, 800, 200, text10, 60)) {
						Stage = 2;
						ar[7].ended = 1;
						ar[7].pe = 1;
						ar[7].status = 1;
						break;
					}
					if (button(300, 260, 800, 360, text11, 60)) {
						Stage = 3;
						ar[7].ended = 1;
						ar[7].pe = 2;
						ar[7].status = 1;
						break;
					}
				}
			}
			else {
				if (ar[7].pe == 1) {
					Stage = 2;
				}
				else {
					Stage = 3;
				}
			}
			return;
		}
		if (button(350, 675, 750, 775, text8, 50)) {
			Stage = 1;
			break;
		}
		if (button(270, 40, 285, 55, text9, 13)) {
			ar[0].ended = 0;
			break;
		}
		if (button(528, 40, 543, 55, text9, 13)) {
			ar[1].ended = 0;
			break;
		}
		if (button(786, 40, 801, 55, text9, 13)) {
			ar[2].ended = 0;
			break;
		}
		if (button(1044, 40, 1059, 55, text9, 13)) {
			ar[3].ended = 0;
			break;
		}
		if (button(270, 340, 285, 355, text9, 13)) {
			ar[4].ended = 0;
			break;
		}
		if (button(528, 340, 543, 355, text9, 13)) {
			ar[5].ended = 0;
			break;
		}
		if (button(786, 340, 801, 355, text9, 13)) {
			ar[6].ended = 0;
			break;
		}
		if (button(1044, 340, 1059, 355, text9, 13)) {
			ar[7].ended = 0;
			break;
		}
		EndBatchDraw();
	}
	return;
}

//PvP

//Functions
//Judgement
bool Checkdead(int x, int y) {
	if (x == 0) {
		if (y == 7) {
			if (boardstatus[x + 1][y] == 0 || boardstatus[x][y - 1] == 0 || boardstatus[x + 1][y - 1] == 0) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (y == 0) {
			if (boardstatus[x + 1][y] == 0 || boardstatus[x][y + 1] == 0 || boardstatus[x + 1][y + 1] == 0) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (boardstatus[x][y - 1] == 0 || boardstatus[x][y + 1] == 0 || boardstatus[x + 1][y] == 0 || boardstatus[x + 1][y + 1] == 0 || boardstatus[x + 1][y - 1] == 0) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	else if (x == 7) {
		if (y == 7) {
			if (boardstatus[x - 1][y] == 0 || boardstatus[x][y - 1] == 0 || boardstatus[x - 1][y - 1] == 0) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (y == 0) {
			if (boardstatus[x - 1][y] == 0 || boardstatus[x][y + 1] == 0 || boardstatus[x - 1][y + 1] == 0) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (boardstatus[x][y + 1] == 0 || boardstatus[x][y - 1] == 0 || boardstatus[x - 1][y] == 0 || boardstatus[x - 1][y - 1] == 0 || boardstatus[x - 1][y + 1] == 0) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	else if (y == 0 && x != 0 && x != 7) {
		if (boardstatus[x - 1][y] == 0 || boardstatus[x + 1][y] == 0 || boardstatus[x][y + 1] == 0 || boardstatus[x - 1][y + 1] == 0 || boardstatus[x + 1][y + 1] == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (y == 7 && x != 0 && x != 7) {
		if (boardstatus[x + 1][y] == 0 || boardstatus[x - 1][y] == 0 || boardstatus[x][y - 1] == 0 || boardstatus[x - 1][y - 1] == 0 || boardstatus[x + 1][y - 1] == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (boardstatus[x + 1][y] == 0 || boardstatus[x - 1][y] == 0 || boardstatus[x][y + 1] == 0 || boardstatus[x][y - 1] == 0 || boardstatus[x + 1][y + 1] == 0 || boardstatus[x + 1][y - 1] == 0 || boardstatus[x - 1][y + 1] == 0 || boardstatus[x - 1][y - 1] == 0) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool CheckWin(int x) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (boardstatus[i][j] == x) {
				if (Checkdead(i, j) == true) {
					return false;
				}
			}
		}
	}
	return true;
}

//Visual
void DrawBoard() {
	line(300, 100, 1100, 100);
	line(300, 200, 1100, 200);
	line(300, 300, 1100, 300);
	line(300, 400, 1100, 400);
	line(300, 500, 1100, 500);
	line(300, 600, 1100, 600);
	line(300, 700, 1100, 700);
	line(300, 0, 300, 800);
	line(400, 0, 400, 800);
	line(500, 0, 500, 800);
	line(600, 0, 600, 800);
	line(700, 0, 700, 800);
	line(800, 0, 800, 800);
	line(900, 0, 900, 800);
	line(1000, 0, 1000, 800);
	line(40, 70, 260, 70);
	line(40, 70, 40, 130);
	line(40, 130, 260, 130);
	line(260, 70, 260, 130);
	line(40, 150, 260, 150);
	line(40, 150, 40, 210);
	line(40, 210, 260, 210);
	line(260, 150, 260, 210);
	settextcolor(WHITE);
	settextstyle(28, 0, L"等线");
	int hSpace = (220 - textwidth(text4)) / 2;
	int vSpace = (60 - textheight(text4)) / 2;
	outtextxy(40 + hSpace, 70 + vSpace, text4);
	hSpace = (220 - textwidth(text5)) / 2;
	vSpace = (60 - textheight(text5)) / 2;
	outtextxy(40 + hSpace, 150 + vSpace, text5);
}

void DrawPiece() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (boardstatus[i][j] == 1) {
				setfillcolor(RED);
				fillcircle(100 * j + 350, 100 * i + 50, 25);
			}
			else if (boardstatus[i][j] == 2) {
				setfillcolor(BLUE);
				fillcircle(100 * j + 350, 100 * i + 50, 25);
			}
			else if (boardstatus[i][j] == 3) {
				setfillcolor(GREEN);
				fillcircle(100 * j + 350, 100 * i + 50, 25);
			}
		}
	}
}

void DrawTipText(int current_piece) {
	static TCHAR str[64];
	if (current_piece == 1) {
		_stprintf_s(str, _T("当前棋子类型: 红"));
		settextstyle(30, 0, L"宋体");
		settextcolor(RGB(255, 175, 25));
		outtextxy(0, 0, str);
	}
	else {
		_stprintf_s(str, _T("当前棋子类型: 蓝"));
		settextstyle(30, 0, L"宋体");
		settextcolor(RGB(255, 175, 25));
		outtextxy(0, 0, str);
	}
}

//Operation
bool savenquit(int x1, int y1, int x2, int y2) {
	if (msg.message == WM_LBUTTONDOWN && clicked(msg.x, msg.y, x1, y1, x2, y2)) {
		ar[code].status = current_piece;
		Stage = 1;
		running = false;
		return true;
	}
	return false;
}
bool quit(int x1, int y1, int x2, int y2) {
	if (msg.message == WM_LBUTTONDOWN && clicked(msg.x, msg.y, x1, y1, x2, y2)) {
		ar[code].ended = 0;
		running = false;
		return true;
	}
	return false;
}

void selecting(int a, int* pt1, int* pt2) {
	msg.message = 0;
	while (true) {
		peekmessage(&msg);
		if (msg.message == WM_LBUTTONDOWN) {
			int x = msg.x - 300;
			int y = msg.y;
			int idx_x = x / 100;
			int idx_y = y / 100;
			if (boardstatus[idx_y][idx_x] == a && Checkdead(idx_y, idx_x)) {
				*pt1 = idx_x;
				*pt2 = idx_y;
				circle(idx_x * 100 + 350, idx_y * 100 + 50, 50);
				return;
			}
			if (savenquit(40, 70, 260, 130)) {
				return;
			}
			if (quit(40, 150, 260, 210)) {
				return;
			}
		}
	}
}

bool judgepath(int x, int y, int xd, int yd) {
	if (boardstatus[xd][yd] == 0) {
		if (x == xd) {
			if (yd > y) {
				for (int i = y + 1; i <= yd; i++) {
					if (boardstatus[x][i] != 0) {
						return false;
					}
				}
				return true;
			}
			else if (yd < y) {
				for (int i = yd + 1; i < y; i++) {
					if (boardstatus[x][i] != 0) {
						return false;
					}
				}
				return true;
			}
			else {
				return false;
			}
		}
		else if (y == yd) {
			if (xd > x) {
				for (int i = x + 1; i <= xd; i++) {
					if (boardstatus[i][y] != 0) {
						return false;
					}
				}
				return true;
			}
			else if (xd < x) {
				for (int i = xd + 1; i < x; i++) {
					if (boardstatus[i][y] != 0) {
						return false;
					}
				}
				return true;
			}
			else {
				return false;
			}
		}
		else if (abs(x - xd) == abs(y - yd)) {
			if (xd > x && yd > y) {
				for (int i = 1; i <= xd - x; i++) {
					if (boardstatus[x + i][y + i] != 0) {
						return false;
					}
				}
				return true;
			}
			else if (xd > x && yd < y) {
				for (int i = 1; i <= xd - x; i++) {
					if (boardstatus[x + i][y - i] != 0) {
						return false;
					}
				}
				return true;
			}
			else if (xd < x && yd > y) {
				for (int i = 1; i <= x - xd; i++) {
					if (boardstatus[x - i][y + i] != 0) {
						return false;
					}
				}
				return true;
			}
			else if (xd < x && yd < y) {
				for (int i = 1; i <= x - xd; i++) {
					if (boardstatus[x - i][y - i] != 0) {
						return false;
					}
				}
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void shooting(int xs, int ys, int* a, int* b) {
	msg.message = 0;
	while (true) {
		peekmessage(&msg);
		if (msg.message == WM_LBUTTONDOWN) {
			int x = msg.x - 300;
			int y = msg.y;
			int idx_x = x / 100;
			int idx_y = y / 100;
			if (judgepath(ys, xs, idx_y, idx_x)) {
				*a = idx_x;
				*b = idx_y;
				return;
			}
			if (savenquit(40, 70, 260, 130)) {
				return;
			}
			if (quit(40, 150, 260, 210)) {
				return;
			}
		}
	}
}

void turn(int a) {
	int x = 0, y = 0;
	int xd = 0, yd = 0;
	int xa = 0, ya = 0;

	//Phase 1: Selecting
	selecting(a, &x, &y);
	if (running == false) {
		return;
	}

	//Phase 2: Riding
	msg.message = 0;
	while (true) {
		peekmessage(&msg);
		if (msg.message == WM_LBUTTONDOWN) {
			int idx_x = (msg.x - 300) / 100;
			int idx_y = msg.y / 100;
			if (savenquit(40, 70, 260, 130)) {
				return;
			}
			if (quit(40, 150, 260, 210)) {
				return;
			}
			if (judgepath(y, x, idx_y, idx_x)) {
				xd = idx_x;
				yd = idx_y;
				setfillcolor(RGB(100, 100, 100));
				fillcircle(100 * idx_x + 350, 100 * idx_y + 50, 25);
				break;
			}
			else if (boardstatus[idx_y][idx_x] == a && Checkdead(idx_y, idx_x)) {
				x = idx_x;
				y = idx_y;
				BeginBatchDraw();
				cleardevice();
				DrawBoard();
				DrawPiece();
				DrawTipText(a);
				EndBatchDraw();
				circle(idx_x * 100 + 350, idx_y * 100 + 50, 50);
			}
		}
	}
	boardstatus[y][x] = 0;
	boardstatus[yd][xd] = a;

	//Phase 3; Shooting
	shooting(xd, yd, &xa, &ya);
	boardstatus[ya][xa] = 3;

	return;
}

void pvp(int code) {

	current_piece = ar[code].status;

	memcpy(boardstatus, ar[code].storage, sizeof(ar[code].storage));


	running = true;

	//Graphics

	while (running) {

		memcpy(ar[code].storage, boardstatus, sizeof(boardstatus));
		//Operation Status
		//Player's Turn
		BeginBatchDraw();
		cleardevice();
		DrawBoard();
		DrawPiece();
		DrawTipText(current_piece);
		EndBatchDraw();

		turn(current_piece);

		//Judgement status
		if (CheckWin(1) || CheckWin(2)) {
			BeginBatchDraw();
			cleardevice();
			DrawBoard();
			DrawPiece();
			EndBatchDraw();
			if (CheckWin(1)) {
				MessageBox(GetHWnd(), _T("蓝方玩家获胜"), _T("返回到主菜单"), MB_OK);
			}
			else {
				MessageBox(GetHWnd(), _T("红方玩家获胜"), _T("返回到主菜单"), MB_OK);
			}
			ar[code].ended = 0;
			running = false;
		}

		if (current_piece == 1) {
			current_piece = 2;
		}
		else {
			current_piece = 1;
		}
	}
	Stage = 1;
	return;
}

//PvE
int stcheck(int a, int b) {
	int count = 0;
	int i = 1;
	while (b + i < 8 && botboard[a][b + i] == 0) {
		count++;
		i++;
	}
	i = -1;
	while (b + i >= 0 && botboard[a][b + i] == 0) {
		count++;
		i--;
	}
	i = 1;
	while (a + i < 8 && botboard[a + i][b] == 0) {
		count++;
		i++;
	}
	i = -1;
	while (a + i >= 0 && botboard[a + i][b] == 0) {
		count++;
		i--;
	}
	i = 1;
	while (a + i < 8 && b + i < 8 && botboard[a + i][b + i] == 0) {
		count++;
		i++;
	}
	i = -1;
	while (a + i >= 0 && b + i >= 0 && botboard[a + i][b + i] == 0) {
		count++;
		i--;
	}
	i = 1;
	while (a - i >= 0 && b + i < 8 && botboard[a - i][b + i] == 0) {
		count++;
		i++;
	}
	i = 1;
	while (a + i < 8 && b - i >= 0 && botboard[a + i][b - i] == 0) {
		count++;
		i++;
	}
	return count;
}

int check(int a) {
	int total = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (botboard[i][j] == a) {
				int k = 1;
				while (j + k < 8 && botboard[i][j + k] == 0) {
					botboard[i][j + k - 1] = 0;
					botboard[i][j + k] = a;
					total += stcheck(i, j + k);
					k++;
				}
				botboard[i][j + k - 1] = 0;
				botboard[i][j] = a;

				k = -1;
				while (j + k >= 0 && botboard[i][j + k] == 0) {
					botboard[i][j + k + 1] = 0;
					botboard[i][j + k] = a;
					total += stcheck(i, j + k);
					k--;
				}
				botboard[i][j + k + 1] = 0;
				botboard[i][j] = a;

				k = 1;
				while (i + k < 8 && botboard[i + k][j] == 0) {
					botboard[i + k - 1][j] = 0;
					botboard[i + k][j] = a;
					total += stcheck(i + k, j);
					k++;
				}
				botboard[i + k - 1][j] = 0;
				botboard[i][j] = a;

				k = -1;
				while (i + k >= 0 && botboard[i + k][j] == 0) {
					botboard[i + k + 1][j] = 0;
					botboard[i + k][j] = a;
					total += stcheck(i + k, j);
					k--;
				}
				botboard[i + k + 1][j] = 0;
				botboard[i][j] = a;

				k = 1;
				while (i + k < 8 && j + k < 8 && botboard[i + k][j + k] == 0) {
					botboard[i + k - 1][j + k - 1] = 0;
					botboard[i + k][j + k] = a;
					total += stcheck(i + k, j + k);
					k++;
				}
				botboard[i + k - 1][j + k - 1] = 0;
				botboard[i][j] = a;

				k = -1;
				while (i + k >= 0 && j + k >= 0 && botboard[i + k][j + k] == 0) {
					botboard[i + k + 1][j + k + 1] = 0;
					botboard[i + k][j + k] = a;
					total += stcheck(i + k, j + k);
					k--;
				}
				botboard[i + k + 1][j + k + 1] = 0;
				botboard[i][j] = a;

				k = 1;
				while (i - k >= 0 && j + k < 8 && botboard[i - k][j + k] == 0) {
					botboard[i - (k - 1)][j + (k - 1)] = 0;
					botboard[i - k][j + k] = a;
					total += stcheck(i - k, j + k);
					k++;
				}
				botboard[i - (k - 1)][j + (k - 1)] = 0;
				botboard[i][j] = a;

				k = 1;
				while (i + k < 8 && j - k >= 0 && botboard[i + k][j - k] == 0) {
					botboard[i + (k - 1)][j - (k - 1)] = 0;
					botboard[i + k][j - k] = a;
					total += stcheck(i + k, j - k);
					k++;
				}
				botboard[i + (k - 1)][j - (k - 1)] = 0;
				botboard[i][j] = a;
			}
		}
	}
	return total;
}

void botshoot(int a, int b, int x, int y, int *mx) {

	int i = 1, p = 0, q = 0;
	while (b + i < 8 && botboard[a][b + i] == 0) {
		if (botboard[a][b + i - 1] == 3) {
			botboard[a][b + i - 1] = 0;
		}
		botboard[a][b + i] = 3;
		p = check(2);
		q = check(1);
		if (p - q > *mx) {
			*mx = p - q;
			botcommand[0] = x;
			botcommand[1] = y;
			botcommand[2] = a;
			botcommand[3] = b;
			botcommand[4] = a;
			botcommand[5] = b + i;
		}
		i++;
	}
	botboard[a][b + i - 1] = 0;

	i = -1;
	while (b + i >= 0 && botboard[a][b + i] == 0) {
		if (b + i + 1 < 8 && botboard[a][b + i + 1] == 3) {
			botboard[a][b + i + 1] = 0;
		}
		botboard[a][b + i] = 3;
		p = check(2);
		q = check(1);
		if (p - q > *mx) {
			*mx = p - q;
			botcommand[0] = x;
			botcommand[1] = y;
			botcommand[2] = a;
			botcommand[3] = b;
			botcommand[4] = a;
			botcommand[5] = b + i;
		}
		i--;
	}
	if (b + i + 1 >= 0) {
		botboard[a][b + i + 1] = 0;
	}

	i = 1;
	while (a + i < 8 && botboard[a + i][b] == 0) {
		if (a + i - 1 >= 0 && botboard[a + i - 1][b] == 3) {
			botboard[a + i - 1][b] = 0;
		}
		botboard[a + i][b] = 3;
		p = check(2);
		q = check(1);
		if (p - q > *mx) {
			*mx = p - q;
			botcommand[0] = x;
			botcommand[1] = y;
			botcommand[2] = a;
			botcommand[3] = b;
			botcommand[4] = a + i;
			botcommand[5] = b;
		}
		i++;
	}
	if (a + i - 1 >= 0) {
		botboard[a + i - 1][b] = 0;
	}

	i = -1;
	while (a + i >= 0 && botboard[a + i][b] == 0) {
		if (a + i + 1 < 8 && botboard[a + i + 1][b] == 3) {
			botboard[a + i + 1][b] = 0;
		}
		botboard[a + i][b] = 3;
		p = check(2);
		q = check(1);
		if (p - q > *mx) {
			*mx = p - q;
			botcommand[0] = x;
			botcommand[1] = y;
			botcommand[2] = a;
			botcommand[3] = b;
			botcommand[4] = a + i;
			botcommand[5] = b;
		}
		i--;
	}
	if (a + i + 1 >= 0) {
		botboard[a + i + 1][b] = 0;
	}

	i = 1;
	while (a + i < 8 && b + i < 8 && botboard[a + i][b + i] == 0) {
		if (a + i - 1 >= 0 && b + i - 1 >= 0 && botboard[a + i - 1][b + i - 1] == 3) {
			botboard[a + i - 1][b + i - 1] = 0;
		}
		botboard[a + i][b + i] = 3;
		p = check(2);
		q = check(1);
		if (p - q > *mx) {
			*mx = p - q;
			botcommand[0] = x;
			botcommand[1] = y;
			botcommand[2] = a;
			botcommand[3] = b;
			botcommand[4] = a + i;
			botcommand[5] = b + i;
		}
		i++;
	}
	if (a + i - 1 >= 0 && b + i - 1 >= 0) {
		botboard[a + i - 1][b + i - 1] = 0;
	}

	i = -1;
	while (a + i >= 0 && b + i >= 0 && botboard[a + i][b + i] == 0) {
		if (a + i + 1 < 8 && b + i + 1 < 8 && botboard[a + i + 1][b + i + 1] == 3) {
			botboard[a + i + 1][b + i + 1] = 0;
		}
		botboard[a + i][b + i] = 3;
		p = check(2);
		q = check(1);
		if (p - q > *mx) {
			*mx = p - q;
			botcommand[0] = x;
			botcommand[1] = y;
			botcommand[2] = a;
			botcommand[3] = b;
			botcommand[4] = a + i;
			botcommand[5] = b + i;
		}
		i--;
	}
	if (a + i + 1 >= 0 && b + i + 1 >= 0) {
		botboard[a + i + 1][b + i + 1] = 0;
	}

	i = 1;
	while (a - i >= 0 && b + i < 8 && botboard[a - i][b + i] == 0) {
		if (a - i + 1 < 8 && b + i - 1 >= 0 && botboard[a - i + 1][b + i - 1] == 3) {
			botboard[a - i + 1][b + i - 1] = 0;
		}
		botboard[a - i][b + i] = 3;
		p = check(2);
		q = check(1);
		if (p - q > *mx) {
			*mx = p - q;
			botcommand[0] = x;
			botcommand[1] = y;
			botcommand[2] = a;
			botcommand[3] = b;
			botcommand[4] = a - i;
			botcommand[5] = b + i;
		}
		i++;
	}
	if (a - i + 1 >= 0 && b + i - 1 >= 0) {
		botboard[a - i + 1][b + i - 1] = 0;
	}

	i = 1;
	while (a + i < 8 && b - i >= 0 && botboard[a + i][b - i] == 0) {
		if (a + i - 1 >= 0 && b - i + 1 < 8 && botboard[a + i - 1][b - i + 1] == 3) {
			botboard[a + i - 1][b - i + 1] = 0;
		}
		botboard[a + i][b - i] = 3;
		p = check(2);
		q = check(1);
		if (p - q > *mx) {
			*mx = p - q;
			botcommand[0] = x;
			botcommand[1] = y;
			botcommand[2] = a;
			botcommand[3] = b;
			botcommand[4] = a + i;
			botcommand[5] = b - i;
		}
		i++;
	}
	if (a + i - 1 >= 0 && b - i + 1 >= 0) {
		botboard[a + i - 1][b - i + 1] = 0;
	}
}

void bot() {
	int mx = -10000;
	memcpy(botboard, boardstatus, sizeof(boardstatus));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (botboard[i][j] == 2) {
				int k = 1;
				while (j + k < 8 && botboard[i][j + k] == 0) {
					botboard[i][j + k - 1] = 0;
					botboard[i][j + k] = 2;
					botshoot(i, j + k, i, j, &mx);
					k++;
				}
				botboard[i][j + k - 1] = 0;
				botboard[i][j] = 2;

				k = -1;
				while (j + k >= 0 && botboard[i][j + k] == 0) {
					botboard[i][j + k + 1] = 0;
					botboard[i][j + k] = 2;
					botshoot(i, j + k, i, j, &mx);
					k--;
				}
				botboard[i][j + k + 1] = 0;
				botboard[i][j] = 2;

				k = 1;
				while (i + k < 8 && botboard[i + k][j] == 0) {
					botboard[i + k - 1][j] = 0;
					botboard[i + k][j] = 2;
					botshoot(i + k, j, i, j, &mx);
					k++;
				}
				botboard[i + k - 1][j] = 0;
				botboard[i][j] = 2;

				k = -1;
				while (i + k >= 0 && botboard[i + k][j] == 0) {
					botboard[i + k + 1][j] = 0;
					botboard[i + k][j] = 2;
					botshoot(i + k, j, i, j, &mx);
					k--;
				}
				botboard[i + k + 1][j] = 0;
				botboard[i][j] = 2;

				k = 1;
				while (i + k < 8 && j + k < 8 && botboard[i + k][j + k] == 0) {
					botboard[i + k - 1][j + k - 1] = 0;
					botboard[i + k][j + k] = 2;
					botshoot(i + k, j + k, i, j, &mx);
					k++;
				}
				botboard[i + k - 1][j + k - 1] = 0;
				botboard[i][j] = 2;

				k = -1;
				while (i + k >= 0 && j + k >= 0 && botboard[i + k][j + k] == 0) {
					botboard[i + k + 1][j + k + 1] = 0;
					botboard[i + k][j + k] = 2;
					botshoot(i + k, j + k, i, j, &mx);
					k--;
				}
				botboard[i + k + 1][j + k + 1] = 0;
				botboard[i][j] = 2;

				k = 1;
				while (i - k >= 0 && j + k < 8 && botboard[i - k][j + k] == 0) {
					botboard[i - (k - 1)][j + (k - 1)] = 0;
					botboard[i - k][j + k] = 2;
					botshoot(i - k, j + k, i, j, &mx);
					k++;
				}
				botboard[i - (k - 1)][j + (k - 1)] = 0;
				botboard[i][j] = 2;

				k = 1;
				while (i + k < 8 && j - k >= 0 && botboard[i + k][j - k] == 0) {
					botboard[i + (k - 1)][j - (k - 1)] = 0;
					botboard[i + k][j - k] = 2;
					botshoot(i + k, j - k, i, j, &mx);
					k++;
				}
				botboard[i + (k - 1)][j - (k - 1)] = 0;
				botboard[i][j] = 2;
			}
		}
	}
}

void pve() {
	current_piece = ar[code].status;

	memcpy(boardstatus, ar[code].storage, sizeof(ar[code].storage));


	running = true;

	//Graphics

	while (running) {

		memcpy(ar[code].storage, boardstatus, sizeof(boardstatus));
		//Operation Status
		//Player's Turn
		BeginBatchDraw();
		cleardevice();
		DrawBoard();
		DrawPiece();
		DrawTipText(current_piece);
		EndBatchDraw();

		if (current_piece == 1) {
			turn(1);
		}

		else {
			bot();
			boardstatus[botcommand[0]][botcommand[1]] = 0;
			boardstatus[botcommand[2]][botcommand[3]] = 2;
			boardstatus[botcommand[4]][botcommand[5]] = 3;
			BeginBatchDraw();
			cleardevice();
			DrawBoard();
			DrawPiece();
			EndBatchDraw();
		}

		//Judgement status
		if (CheckWin(1) || CheckWin(2)) {
			BeginBatchDraw();
			cleardevice();
			DrawBoard();
			DrawPiece();
			EndBatchDraw();
			if (CheckWin(1)) {
				MessageBox(GetHWnd(), _T("人机获胜"), _T("返回到主菜单"), MB_OK);
			}
			else {
				MessageBox(GetHWnd(), _T("玩家获胜"), _T("返回到主菜单"), MB_OK);
			}
			ar[code].ended = 0;
			running = false;
		}

		if (current_piece == 1) {
			current_piece = 2;
		}
		else {
			current_piece = 1;
		}
	}
	Stage = 1;
	return;
}

//Main
int main() {
	initgraph(1100, 800);
	read();
	while (true) {
		if (Stage == 1) {
			menu();
		}
		else if (Stage == 4) {
			submenu();
		}
		else if (Stage == 2) {
			pvp(code);
		}
		else if (Stage == 3) {
			pve();
		}
		else {
			break;
		}
	}
	write();
	return 0;
}
