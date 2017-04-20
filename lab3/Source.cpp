#include <iostream> 
#include <Windows.h> 

using namespace std;

void Line(HDC* hdc, int x1, int y1, int x2, int y2);
void Grid(HDC* hdc, int xn, int yn, int size);
void printPoint(HDC* hdc, int x, int y, int size);
void LineCda(HDC* hdc, int x1, int y1, int x2, int y2, int size);
int sign(double a);
void LineBrezenhem(HDC* hdc, int x1, int y1, int x2, int y2, int size);
void DrawCircle(HDC* hdc, int x1, int y1, int radius, int size);
void Sorttreug(HDC* hdc, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int size);

int main() {

	setlocale(0, "");

	HWND hwnd = GetConsoleWindow();
	HDC hdc = GetDC(hwnd);

	RECT rc;

	HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));



	int xn = 0, yn = 0, size;
	int x1, y1, x2, y2;
	int radius;

	cout << "Введите количество столбцов сетки: ";
	cin >> xn;
	cout << "Введите количество строк сетки: ";
	cin >> yn;
	cout << "Введите размер клетки (Длину ребра квадрата): ";
	cin >> size;
	/*cout << "Введите координаты начала отрезка для алгоритма ЦДА: ";
	cin >> x1 >> y1;
	cout << "Введите координаты конца отрезка для алгоритма ЦДА: ";
	cin >> x2 >> y2;*/


	GetClientRect(hwnd, &rc);
	SetViewportOrgEx(hdc, rc.left, rc.bottom - 1, NULL);
	SelectObject(hdc, whitePen);
	Grid(&hdc, xn, yn, size);

	Sorttreug(&hdc, 0, 0, 0, 20, 10, 15, size);

	/*LineCda(&hdc, x1, y1, x2, y2, size);

	cout << "Введите координаты начала отрезка для алгоритма Брезенхема: ";
	cin >> x1 >> y1;
	cout << "Введите координаты конца отрезка для алгоритма Брезенхема: ";
	cin >> x2 >> y2;

	LineBrezenhem(&hdc, x1, y1, x2, y2, size);

	cout << "Введите координаты центра окружности для алгоритма Брезенхема: ";
	cin >> x1 >> y1;
	cout << "Введите радиус окружности: ";
	cin >> radius;

	DrawCircle(&hdc, x1, y1, radius, size);*/

	return 0;
}

//рисование линии 
void Line(HDC* hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(*hdc, x1, y1, NULL);
	LineTo(*hdc, x2, y2);
}

//рисование сетки
void Grid(HDC* hdc, int xn, int yn, int size) {

	for (int y = 0; y <= yn; y++) {
		Line(hdc, 0, -y*size, xn*size, -y*size);
	}

	for (int x = 0; x <= xn; x++) {
		Line(hdc, x*size, 0, x*size, -yn*size);
	}
}

//рисование точки 
void printPoint(HDC* hdc, int x, int y, int size) {
	x++; y++;
	Ellipse(*hdc, (x - 1)*size + 1, -y*size + 1, x*size, -(y - 1)*size);
}

//рисование отрезка по алгоритму цифрового дифференциального анализатора (ЦДА) 
void LineCda(HDC* hdc, int x1, int y1, int x2, int y2, int size) {

	HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	SelectObject(*hdc, yellowBrush);

	double Dx = 0, Dy = 0;
	double x, y, lenght = 0;

	lenght = fabs(x1 - x2) > fabs(y1 - y1) ? fabs(x1 - x2) : fabs(y1 - y1);

	Dx = (x2 - x1) / lenght;
	Dy = (y2 - y1) / lenght; // или Dх или Dу равно 1 

	x = x1 + 0.5*sign(Dx); // начальные точки 
	y = y1 + 0.5*sign(Dy);

	for (int i = 1; i <= lenght; i++) {
		printPoint(hdc, int(x), int(y), size);
		x += Dx;
		y += Dy;
	}
}

//функция для определения знака числа 
int sign(double a) {
	if (a > 0)
		return 1;
	else if (a < 0)
		return -1;
	else if (a == 0)
		return 0;
}

//рисование отрезка по алгоритму Брезенхема 
void LineBrezenhem(HDC* hdc, int x1, int y1, int x2, int y2, int size) {
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(*hdc, redBrush);

	int dx, dy, ch = 0, i = 0, e, dx2, dy2;
	x2 -= x1; dx = abs(x2);
	y2 -= y1; dy = abs(y2);
	if (!x2 && !y2)
		return; // Если начало совпадает с концом отрезка 
	if (x2) x2 = x2 < 0 ? -1 : 1;
	if (y2) y2 = y2 < 0 ? -1 : 1;

	if (dy > dx) {
		swap(dy, dx);
		ch = 1;
	} // меняем местами x и y 

	dx2 = dx << 1;
	dy2 = dy << 1; // dx2 = 2*dx; dy2 = 2*dy; 

	e = dy2 - dx; // Начальное значение ошибки;
	for (i = 0; i < dx; ++i) {

		printPoint(hdc, x1, y1, size);
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

//рисование окружности по алгоритму Брезенхема
void DrawCircle(HDC* hdc, int x1, int y1, int radius, int size) {
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while (y >= 0) {
		printPoint(hdc, x1 + x, y1 + y, size);
		printPoint(hdc, x1 + x, y1 - y, size);
		printPoint(hdc, x1 - x, y1 + y, size);
		printPoint(hdc, x1 - x, y1 - y, size);
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
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


void Sorttreug(HDC* hdc, int Ax, int Ay, int Bx, int By, int Cx, int Cy, int size) {
	int X[3] = { Ay,By,Cy };
	int Y[3] = { Ax,Bx,Cx };

	for (int i = 0; i<2; i++)
		for (int j = 0; j<2; j++)
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

		LineBrezenhem(hdc, x1, sy + 1, x2, sy + 1, size);
	}
}