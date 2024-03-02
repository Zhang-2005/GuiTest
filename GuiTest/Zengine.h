#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <atlstr.h>
#include <commdlg.h>
#include <windows.h>
#include <string>
#include <cwchar>
#include <iostream>
#include <vector>
#include <ctime>
#include <Windows.h>
#include <cmath>
#include <random>
#include <queue>
using namespace std;

sf::Font font;
struct MousePos {
	int mouseX, mouseY;
	int mouseL, mouseM, mouseR;
	int clickL;
	int dx, dy;
	int Space, s;
	void Get(sf::RenderWindow* window) {
		dx = sf::Mouse::getPosition(*window).x - mouseX;
		dy = sf::Mouse::getPosition(*window).y - mouseY;
		mouseX = sf::Mouse::getPosition(*window).x;
		mouseY = sf::Mouse::getPosition(*window).y;
		mouseM = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
		mouseR = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == 1) {
			if (mouseL == 0)  mouseL = clickL = 1;
			else clickL = 0;
		}
		else {
			mouseL = clickL = 0;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (s == 0)  s = Space = 1;
			else Space = 0;
		}
		else {
			s = Space = 0;
		}
	}
}mousePos;
struct Z_Point {
	double X, Y;
};
struct Z_Line {
	Z_Point u;
	Z_Point v;
};
struct Z_View {
	double vp1, vp2, vp3, vp4;
	double cX = 0, cY = 0;
	double Z = 1;
	struct V6 {
		int L, R, U, D, Lenth, Width;
	};

	V6 Get(sf::RenderWindow* window) {
		int winX = window->getSize().x;
		int winY = window->getSize().y;
		return V6{ -(int)(winX * vp3 / 2),(int)(winX * vp3 / 2) ,-(int)(winY * vp3 / 2),(int)(winY * vp3 / 2),(int)(winX * vp3),(int)(winY * vp4) };
	}
};
static Z_Point CTW(sf::RenderWindow* window, Z_Point a, Z_View V) {
	double wX, wY;
	int winX = window->getSize().x;
	int winY = window->getSize().y;
	double ttt = 1;
	wX = winX * V.vp1 + winX * V.vp3 / 2 - (V.cX - a.X) / V.Z;
	wY = winY * V.vp2 + winY * V.vp4 / 2 - (V.cY - a.Y) / V.Z;
	return Z_Point{ wX,wY };
}

struct Z_Shape {
	Z_Line OutLines[5000];
	int OutLineCnt;
	Z_Line Lines[5000];
	int LineCnt = 0;
	bool cmp(Z_Line a, Z_Line b) {
		double A = min(a.u.X, a.v.X);
		double B = min(b.u.X, b.v.X);
		if (A == B) {
			A = max(a.u.X, a.v.X);
			B = max(b.u.X, b.v.X);
		}
		return A < B;
	}
	void Divide(Z_Line L[5000], int n) {
		for (int i = 0; i < n; i++) {
			OutLines[i] = L[i];
		}
		OutLineCnt = n;
		//sort(OutLines, OutLines + n, cmp);
		double minY = min(OutLines[0].u.Y, OutLines[0].v.Y), maxY = max(OutLines[0].u.Y, OutLines[0].v.Y);
		for (int i = 1; i < OutLineCnt; i++) {
			minY = min(min(OutLines[i].u.Y, OutLines[i].v.Y), minY);
			maxY = max(max(OutLines[i].u.Y, OutLines[i].v.Y), maxY);
		}
		for (int i = minY; i <= maxY; i++) {
			double arr[50] = {};
			int cnt = 0;
			for (int j = 0; j < OutLineCnt; j++) {
				if (abs(i - OutLines[j].u.Y) <= 0.1 || abs(i - OutLines[j].v.Y) <= 0.1) {
					for (int k = LineCnt; k >= 1; k--) {
						if (Lines[k].u.Y == i - 1) {
							LineCnt++;
							Lines[LineCnt] = Z_Line{ {Lines[k].u.X,(double)i},{Lines[k].v.X,(double)i} };

						}
					}
					break;
				}
				if ((OutLines[j].u.Y - i) * (OutLines[j].v.Y - i) < 0) {
					double ax, ay, bx, by, xx;
					ax = OutLines[j].u.X;
					ay = OutLines[j].u.Y;
					bx = OutLines[j].v.X;
					by = OutLines[j].v.Y;
					xx = (i - ay) * (bx - ax) / (by - ay) + ax;
					cnt++;
					arr[cnt] = xx;
				}
			}
			sort(arr + 1, arr + 1 + cnt);
			for (int j = 1; j < cnt; j += 2) {
				LineCnt++;
				Lines[LineCnt] = Z_Line{ {arr[j],(double)i},{arr[j + 1],(double)i} };
			}
		}
	}
	void Render(sf::RenderWindow* window, Z_View views, int r1 = 229, int g1 = 219, int b1 = 200, int a1 = 255, int showBorder = 0) {
		int winX = window->getSize().x;
		int winY = window->getSize().y;
		sf::View view1;
		view1.setSize(views.vp3 * winX, views.vp4 * winY);
		view1.setViewport(sf::FloatRect(views.vp1, views.vp2, views.vp3, views.vp4));
		view1.setCenter(views.cX, views.cY);
		window->setView(view1);
		for (int i = 0; i <= LineCnt; i++) {
			int wY = CTW(window, { 0,Lines[i].u.Y }, views).Y;
			if (wY <= views.vp2 * winY || wY >= (views.vp2 + views.vp4) * winY) {
				continue;
			}
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(Lines[i].u.X,Lines[i].u.Y)),
				sf::Vertex(sf::Vector2f(Lines[i].v.X,Lines[i].v.Y))
			};
			line[0].color = sf::Color(r1, g1, b1, a1);
			line[1].color = sf::Color(r1, g1, b1, a1);
			window->draw(line, 2, sf::Lines);
		}
		for (int i = 0; i <= OutLineCnt; i++) {
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(OutLines[i].u.X, OutLines[i].u.Y)),
				sf::Vertex(sf::Vector2f(OutLines[i].v.X, OutLines[i].v.Y))
			};
			line[0].color = sf::Color(0, 0, 0);
			line[1].color = sf::Color(0, 0, 0);
			window->draw(line, 2, sf::Lines);
		}
	}

};
struct Z_Rect {
	Z_View views;
	int X, Y, sizeX, sizeY;
	int r1 = 255, g1 = 255, b1 = 255, a1 = 255;
	int outThick = 2;
	int r2 = 0, g2 = 0, b2 = 0, a2 = 200;
	void DrawRect(sf::RenderWindow* window) {
		int winX = window->getSize().x;
		int winY = window->getSize().y;
		sf::View view1;
		view1.setSize(views.vp3 * winX, views.vp4 * winY);
		view1.setViewport(sf::FloatRect(views.vp1, views.vp2, views.vp3, views.vp4));
		view1.setCenter(views.cX, views.cY);
		window->setView(view1);
		sf::RectangleShape rect(sf::Vector2f(sizeX, sizeY));
		rect.setOrigin(sizeX / 2, sizeY / 2);
		rect.setPosition(X, Y);
		rect.setFillColor(sf::Color(r1, g1, b1, a1));
		rect.setOutlineThickness(outThick);
		rect.setOutlineColor(sf::Color(r2, g2, b2, a2));
		window->draw(rect);
	}
};
struct Z_Button {
	Z_View views;
	int X, Y, sizeX, sizeY;
	int r1 = 255, g1 = 255, b1 = 255, a1 = 255;
	int outThick = 2;
	int r2 = 0, g2 = 0, b2 = 0, a2 = 200;
	int r3 = 200, g3 = 200, b3 = 200, a3 = 250;
	//wchar_t T[100];
	int tsize, rt = 0, gt = 0, bt = 0, at = 255;
	int DrawButton(sf::RenderWindow* window, wchar_t T[100], int opt = 1) {
		int winX = window->getSize().x;
		int winY = window->getSize().y;
		sf::View view1;
		view1.setSize(views.vp3 * winX, views.vp4 * winY);
		view1.setViewport(sf::FloatRect(views.vp1, views.vp2, views.vp3, views.vp4));
		view1.setCenter(views.cX, views.cY);
		window->setView(view1);
		sf::RectangleShape rect(sf::Vector2f(sizeX, sizeY));
		rect.setOrigin(sizeX / 2, sizeY / 2);
		rect.setPosition(X, Y);
		rect.setFillColor(sf::Color(r1, g1, b1, a1));
		rect.setOutlineThickness(outThick);
		rect.setOutlineColor(sf::Color(r2, g2, b2, a2));
		int wX, wY;
		wX = winX * views.vp1 + winX * views.vp3 / 2 - (views.cX - X) / views.Z;
		wY = winY * views.vp2 + winY * views.vp4 / 2 - (views.cY - Y) / views.Z;
		if (opt == 1) {
			if (abs(mousePos.mouseX - wX) <= sizeX / 2 / views.Z && abs(mousePos.mouseY - wY) <= sizeY / 2 / views.Z) {
				rect.setFillColor(sf::Color(r3, g3, b3, a3));
				if (mousePos.clickL) {
					mousePos.clickL = 0;
					return 1;
				}
			}
			else {
				if (mousePos.clickL) {
					return -1;
				}
			}
		}
		window->draw(rect);
		sf::Text tmp;
		tsize = min((int)(sizeY * 0.7), wcslen(T) > 0 ? (int)(sizeX / wcslen(T)) : 99999);
		tmp.setFont(font);
		tmp.setString(T);
		tmp.setCharacterSize(tsize);
		tmp.setFillColor(sf::Color(rt, gt, bt, at));
		tmp.setOutlineThickness(1);
		tmp.setOutlineColor(sf::Color::White);
		tmp.setOrigin(wcslen(T) * tsize / 2, tsize * 0.65);
		tmp.setPosition(X, Y);
		window->draw(tmp);
		return 0;
	}
};
struct Particles {
	int X, Y;
	int R;
	int r1, g1, b1, a1;
	void RenderParticles(sf::RenderWindow* window, Z_View views) {
		int winX = window->getSize().x;
		int winY = window->getSize().y;
		sf::View view1;
		view1.setSize(views.vp3 * winX, views.vp4 * winY);
		view1.setViewport(sf::FloatRect(views.vp1, views.vp2, views.vp3, views.vp4));
		view1.setCenter(views.cX, views.cY);
		window->setView(view1);
		srand(time(NULL));
		for (int i = 1; i <= 10; i++) {
			int dx = rand() % R;
			int dy = rand() % R;
			sf::Vertex point(sf::Vector2f(X + dx, Y + dy), sf::Color::Black);
			window->draw(&point, 1, sf::Points);

		}
	}

};
struct Pile {
	int n;
	int stat;
};
static double Getdis(Z_Point t1, Z_Point t2) {
	return sqrt((t1.X - t2.X) * (t1.X - t2.X) + (t1.Y - t2.Y) * (t1.Y - t2.Y));
}
static void AddWchar(wchar_t a[], wchar_t b[]) {
	int flag = 0;
	for (int i = 0; ; i++) {
		if (a[i] == '\0') {
			for (int j = 0; j == 0 || b[j - 1] != '\0'; j++, i++) {
				a[i] = b[j];
			}
			break;
		}
	}
}
static void IntToString(int x, char ch[]) {
	char tmp[100];
	if (x == 0) {
		ch[0] = '0';
		ch[1] = '\0';
		return;
	}
	int cnt = 0;
	int fu = 0;
	if (x < 0) {
		x = -x;
		fu = 1;

	}

	while (x) {
		tmp[cnt++] = (x % 10) + '0';
		x /= 10;
	}
	if (fu)tmp[cnt++] = '-';
	for (int i = 0; i < cnt; i++) ch[i] = tmp[cnt - 1 - i];

	ch[cnt] = '\0';
}
static void RenderTxt(sf::RenderWindow* window, Z_View views, wchar_t T[], int size, int x, int y, int r1 = 0, int g1 = 0, int b1 = 0, int a1 = 255) {
	int winX = window->getSize().x;
	int winY = window->getSize().y;
	sf::View view1;
	view1.setSize(views.vp3 * winX, views.vp4 * winY);
	view1.setViewport(sf::FloatRect(views.vp1, views.vp2, views.vp3, views.vp4));
	view1.setCenter(views.cX, views.cY);
	window->setView(view1);
	sf::Text tmp;
	tmp.setFont(font);
	tmp.setString(T);
	tmp.setCharacterSize(size);
	tmp.setFillColor(sf::Color(r1, g1, b1, a1));
	tmp.setOutlineThickness(1);
	tmp.setOutlineColor(sf::Color::White);
	window->draw(tmp);
}
static void RenderNum(sf::RenderWindow* window, Z_View	views, int t, int size, int x, int y, int rt = 0, int gt = 0, int bt = 0, int at = 255) {
	int winX = window->getSize().x;
	int winY = window->getSize().y;
	sf::View view1;
	view1.setSize(views.vp3 * winX, views.vp4 * winY);
	view1.setViewport(sf::FloatRect(views.vp1, views.vp2, views.vp3, views.vp4));
	view1.setCenter(views.cX, views.cY);
	window->setView(view1);
	char T[100];
	IntToString(t, T);
	sf::Text tmp;
	tmp.setFont(font);
	tmp.setString(T);
	tmp.setCharacterSize(size);
	tmp.setFillColor(sf::Color(rt, gt, bt, at));
	tmp.setOutlineThickness(1);
	tmp.setOutlineColor(sf::Color::White);
	tmp.setOrigin(strlen(T) * size / 2, size * 0.65);
	tmp.setPosition(x, y);
	window->draw(tmp);

}
static void GetEvent(sf::RenderWindow* window) {
	mousePos.Get(window);
	window->display();
	window->clear(sf::Color::White);
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed)window->close();
		//if (event.type == Event::MouseWheelMoved && showMapView) {
		//	if (event.mouseWheel.delta > 0)delay = 15;
		//	if (event.mouseWheel.delta < 0)delay = -15;
		//}
	}
}
static void RnederBG(sf::RenderWindow* window, Z_View views, int r1, int g1, int b1, int a1) {
	int winX = window->getSize().x;
	int winY = window->getSize().y;
	sf::View view1;
	view1.setSize(views.vp3 * winX, views.vp4 * winY);
	view1.setViewport(sf::FloatRect(views.vp1, views.vp2, views.vp3, views.vp4));
	view1.setCenter(views.cX, views.cY);
	window->setView(view1);
	sf::RectangleShape rect(sf::Vector2f(winX * views.vp3, winY * views.vp4));
	rect.setOrigin(winX * views.vp3 / 2, winY * views.vp4 / 2);
	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color(r1, g1, b1, a1));
	window->draw(rect);
}
static void RenderLines(sf::RenderWindow* window, Z_View views, Z_Point Points[], int n, int r1, int g1, int b1, int a1, int w) {
	int winX = window->getSize().x;
	int winY = window->getSize().y;
	sf::View view1;
	view1.setSize(views.vp3 * winX, views.vp4 * winY);
	view1.setViewport(sf::FloatRect(views.vp1, views.vp2, views.vp3, views.vp4));
	view1.setCenter(views.cX, views.cY);
	window->setView(view1);
	for (int i = 1; i < n; i++) {
		if (w) {
			double dis = Getdis(Points[i], Points[i - 1]);
			sf::RectangleShape rect(sf::Vector2f(dis, w));
			rect.setFillColor(sf::Color(r1, g1, b1, a1));
			rect.setOrigin(dis / 2, w / 2);
			rect.setRotation(atan((Points[i].Y - Points[i - 1].Y) / (Points[i].X - Points[i - 1].X)) * 180 / 3.1415926);
			rect.setPosition((Points[i].X + Points[i - 1].X) / 2, (Points[i].Y + Points[i - 1].Y) / 2);
			window->draw(rect);
			sf::CircleShape shape1(w / 2);
			shape1.setOrigin(w / 2, w / 2);
			shape1.setFillColor(sf::Color(r1, g1, b1, a1 / 2));
			shape1.setPosition(Points[i].X, Points[i].Y);
			window->draw(shape1);
			continue;
		}
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(Points[i].X,Points[i].Y)),
			sf::Vertex(sf::Vector2f(Points[i - 1].X, Points[i - 1].Y))
		};
		line[0].color = sf::Color(r1, g1, b1, a1);
		line[1].color = sf::Color(r1, g1, b1, a1);
		window->draw(line, 2, sf::Lines);
		sf::CircleShape shape1(1);
		shape1.setOrigin(1, 1);
		shape1.setFillColor(sf::Color(r1, g1, b1, a1 / 3));
		shape1.setPosition(Points[i].X, Points[i].Y);
		window->draw(shape1);
	}
}

string Get_Path() {
	OPENFILENAME ofn;
	char szFile[300];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.lpstrFile[0] = '\0';
	LPTSTR lpstrCustomFilter;
	DWORD nMaxCustFilter;
	ofn.nFilterIndex = 1;
	LPTSTR lpstrFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"PMM\0*.PMM\0";
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	string path_image = "";
	if (GetOpenFileName(&ofn)) {
		int len = WideCharToMultiByte(CP_ACP, 0, ofn.lpstrFile, -1, NULL, 0, NULL, NULL);
		if (len <= 0) {
			return "";
		}
		else {
			char* dest = new char[len];
			WideCharToMultiByte(CP_ACP, 0, ofn.lpstrFile, -1, dest, len, NULL, NULL);
			dest[len - 1] = 0;
			string str(dest);
			delete[] dest;
			return str;
		}
	}
	else {
		return "";
	}
}
