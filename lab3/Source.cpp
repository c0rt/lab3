#include <iostream> 
#include <Windows.h> 

using namespace std;

void Line(HDC* hdc, int x1, int y1, int x2, int y2);
void Grid(HDC* hdc, int xn, int yn, int size);
void printPoint(HDC* hdc, int x, int y, int size);
void LineBrezenhem(HDC* hdc, int x1, int y1, int x2, int y2, int size, int **&L);
void Sorttreug(HDC* hdc, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int size);
void DrawCircle(HDC* hdc, int x1, int y1, int radius, int size, int** &L);
void Triangle(HDC * hdc, int x1, int y1, int x2, int y2, int x3, int y3, int size, int **& L);
int LineFill(int x, int y, int dir, int PrevXl, int PrevXr, int **& L, int BolderColor, int Color);

int main() {

	setlocale(0, "");

	HWND hwnd = GetConsoleWindow();
	HDC hdc = GetDC(hwnd);

	RECT rc;

	HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));


	int xn = 0, yn = 0, size;
	int x1, y1, x2, y2;
	int radius;

	cout << "Колиество столбцов: ";
	cin >> xn;
	cout << "Количество строк: ";
	cin >> yn;
	cout << "Размер клетки: ";
	cin >> size;
	xn++; yn++;

	//Перемещаем 0 в левый нижний угол
	GetClientRect(hwnd, &rc);
	SetViewportOrgEx(hdc, rc.left, rc.bottom - 1, NULL);
	SelectObject(hdc, whitePen);
	Grid(&hdc, xn, yn, size);

	//заполнение треугольника
	int trx1, try1, trx2, try2, trx3, try3;
	cout << "Введите точки треугольника: ";
	cin >> trx1 >> try1 >> trx2 >> try2 >> trx3 >> try3;

	SelectObject(hdc, redBrush);
	Sorttreug(&hdc, trx1, try1, trx2, try2, trx3, try3, size);
	system("pause");
	///Вторая часть задания
	system("cls");
	Grid(&hdc, xn, yn, size);

	//буфер
	xn++; yn++;
	int** L = new int*[xn];
	for (int i = 0; i < xn; i++) {
		L[i] = new int[yn];
	}

	for (int i = 0; i < xn; i++) {
		for (int j = 0; j < yn; j++) {
			L[i][j] = 0;
		}
	}

	SelectObject(hdc, blueBrush);
	//рисование окружности
	cout << "Введите центр окружности: ";
	cin >> x1 >> y1;
	cout << "Введите радиус окружности: ";
	cin >> radius;
	DrawCircle(&hdc, x1, y1, radius, size, L);


	cout << "Введите центр окружности: ";
	cin >> x1 >> y1;
	cout << "Введите радиус окружности: ";
	cin >> radius;
	DrawCircle(&hdc, x1, y1, radius, size, L);

	int x3, y3;
	cout << "Введите координаты треугольника: ";
	cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
	Triangle(&hdc, x1, y1, x2, y2, x3, y3, size, L);

	//заливка
	cout << "Введите точку, с которой начинается заливка: ";
	cin >> x1 >> y1;
	LineFill(x1, y1, 1, x1, y1, L, 1, 2);

	for (int i = 0; i < xn; i++) {
		for (int j = 0; j < yn; j++) {
			if (L[i][j] == 1) {
				SelectObject(hdc, blueBrush);
				printPoint(&hdc, i, j, size);
			}
			else if (L[i][j] == 2) {
				SelectObject(hdc, redBrush);
				printPoint(&hdc, i, j, size);
			}

		}
	}


	system("pause");
	return 0;
}

void Line(HDC* hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(*hdc, x1, y1, NULL);
	LineTo(*hdc, x2, y2);
}

void Grid(HDC* hdc, int xn, int yn, int size) {

	for (int y = 0; y <= yn; y++) {
		Line(hdc, 0, -y*size, xn*size, -y*size);
	}

	for (int x = 0; x <= xn; x++) {
		Line(hdc, x*size, 0, x*size, -yn*size);
	}
}

void printPoint(HDC* hdc, int x, int y, int size) {
	x++; y++;
	Ellipse(*hdc, (x - 1)*size + 1, -y*size + 1, x*size, -(y - 1)*size);
}

void LineBrezenhem(HDC* hdc, int x1, int y1, int x2, int y2, int size, int **&L) {
	int dx, dy, ch = 0, i = 0, e, dx2, dy2;
	x2 -= x1; dx = abs(x2);
	y2 -= y1; dy = abs(y2);
	if (!x2 && !y2)
		return;
	if (x2) x2 = x2 < 0 ? -1 : 1;
	if (y2) y2 = y2 < 0 ? -1 : 1;

	if (dy > dx) {
		swap(dy, dx);
		ch = 1;
	}

	dx2 = dx << 1;
	dy2 = dy << 1;

	e = dy2 - dx;
	for (i = 0; i <= dx; ++i) {

		printPoint(hdc, x1, y1, size);
		L[x1][y1] = 1;
		if (e > 0) {
			if (ch) x1 += x2;
			else y1 += y2;
			e -= dx2;
			i--;
		}
		else {
			if (ch) y1 += y2;
			else x1 += x2;
			e += dy2;
		}
	}
}

void LineOne(HDC* hdc, int x1, int y1, int x2, int y2, int size) {
	for (int i = x1; i <= x2; i++) {
		printPoint(hdc, i, y1, size);
	}
}

void Sorttreug(HDC* hdc, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int size) {
	int X[3] = { Ax,Bx,Cx };
	int Y[3] = { Ay,By,Cy };

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			if (Y[j] < Y[j + 1]) {
				swap(Y[j + 1], Y[j]);
				swap(X[j + 1], X[j]);
			}

	Cy = Y[0]; By = Y[1]; Ay = Y[2];
	Cx = X[0]; Bx = X[1]; Ax = X[2];

	int x1, x2;
	for (int sy = Ay; sy <= Cy; sy++)
	{
		x1 = Ax + (sy - Ay) * (Cx - Ax) / (Cy - Ay);
		if (sy < By)
			x2 = Ax + (sy - Ay) * (Bx - Ax) / (By - Ay);
		else
		{
			if (Cy == By)
				x2 = Bx;
			else
				x2 = Bx + (sy - By) * (Cx - Bx) / (Cy - By);
		}
		if (x1 > x2)
			swap(x1, x2);

		LineOne(hdc, x1, sy, x2, sy, size);
	}



}

void DrawCircle(HDC* hdc, int x1, int y1, int radius, int size, int **&L) {

	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while (y >= 0) {
		printPoint(hdc, x1 + x, y1 + y, size);
		printPoint(hdc, x1 + x, y1 - y, size);
		printPoint(hdc, x1 - x, y1 + y, size);
		printPoint(hdc, x1 - x, y1 - y, size);
		L[x1 + x][y1 + y] = 1;
		L[x1 + x][y1 - y] = 1;
		L[x1 - x][y1 + y] = 1;
		L[x1 - x][y1 - y] = 1;
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}

		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void LineBuffer(int x1, int y1, int x2, int y2, int **&L, int Color) {
	for (int i = x1; i <= x2; i++) {
		L[i][y1] = Color;
	}
}

int LineFill(int x, int y, int dir, int PrevXl, int PrevXr, int **&L, int BolderColor, int Color)
{
	int xl = x, xr = x;
	int c;
	do
		c = L[--xl][y];
	while ((c != BolderColor) && (c != Color));
	do
		c = L[++xr][y];
	while ((c != BolderColor) && (c != Color));
	xl++;
	xr--;
	LineBuffer(xl, y, xr, y, L, Color);
	for (x = xl; x <= xr; x++)
	{
		c = L[x][y + dir];
		if ((c != BolderColor) && (c != Color))
			x = LineFill(x, y + dir, dir, xl,
				xr, L, BolderColor, Color);
	}
	for (x = xl; x<PrevXl; x++)
	{
		c = L[x][y - dir];
		if ((c != BolderColor) && (c != Color))
			x = LineFill(x, y - dir, -
				dir, xl, xr, L, BolderColor, Color);
	}
	for (x = PrevXr; x<xr; x++)
	{
		c = L[x][y - dir];

		if ((c != BolderColor) && (c != Color))
			x = LineFill(x, y - dir, -dir, xl,
				xr, L, BolderColor, Color);
	}
	return xr;
}

void Triangle(HDC* hdc, int x1, int y1, int x2, int y2, int x3, int y3, int size, int **&L) {
	LineBrezenhem(hdc, x1, y1, x2, y2, size, L);
	LineBrezenhem(hdc, x2, y2, x3, y3, size, L);
	LineBrezenhem(hdc, x3, y3, x1, y1, size, L);
}
