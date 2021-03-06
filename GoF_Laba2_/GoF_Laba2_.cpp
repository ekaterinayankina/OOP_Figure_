// GoF_Laba2_.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "afxwin.h"
#include "iostream"
using namespace std;

HWND hwnd = 0;
HDC  hdc = 0;
void InitGraphic() {
	system("mode con cols=168 lines=55");	system("pause >> void");
	hwnd = FindWindow(_T("ConsoleWindowClass"), _T("C:\\Windows\\system32\\cmd.exe")); hdc = GetWindowDC(hwnd);
}
void CloseGraphic() { ReleaseDC(hwnd, hdc); CloseHandle(hwnd); }

//-------------------------------------------------------------------------- IFigure
class IFigure { //интерфейсный АБСТРАКТНЫЙ класс, т.к. имеет пустые вирт-е ф-и
	friend class RecordPlayer;// чтоб получить доступ к полям fMove,fClr из RecordPlayer (чтоб знать какие фигуры мигать, двигать и тп)
protected:
	int fMove; //0 - фигура двигается; 1 - фигура мигает на месте; 2 - фигура стоит на месте; 
	int fClr; //0 - фигура цвет не меняет; 1 - фигура меняет цвет
public:
	IFigure() : fMove(0), fClr(0) {/*cout<<"\n IFigure()";*/ }
	virtual void show() = 0;		//Чистый спецификатор. Чистая виртуальная функция (в производных классах надо переопределить)
	virtual void hide() = 0;		//Чистая виртуальная функция (в производных классах надо переопределить)
	virtual void move(int x, int y) = 0;//Чистая виртуальная функция (в производных классах надо переопределить)
	virtual void ChangeColor() {};
};

//-------------------------------------------------------------------------Square
class Square :virtual public IFigure {	//Унаследовал интерфейсную (публичную) часть класса IFigure
	POINT pt[5];						//Координаты вершин квадрата на экране
	COLORREF color;
public:
	//Конструктор с одним параметром, который может также играть роль конструктора преобразования типа 
	Square(POINT* p) : color(RGB(255, 0, 0)) { for (int i = 0; i <5; i++) pt[i] = p[i]; }
	void SetColor(COLORREF cl) { color = cl; }
	void show() {
		CPen pen(PS_SOLID, 2, color);
		SelectObject(hdc, pen);
		Polyline(hdc, pt, 5);
	}
	void hide() {
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		SelectObject(hdc, pen);
		Polyline(hdc, pt, 5);
	}
	void move(int x, int y) { for (int i = 0; i<5; i++) { pt[i].x += x; pt[i].y += y; } }
	virtual ~Square() {/*cout<<"\t ~Square()";*/ }
	Square(POINT* p, int fMove, int fClr) : color(RGB(255, 0, 0)) {
		for (int i = 0; i <5; i++) pt[i] = p[i];
		IFigure::fMove = fMove;
		IFigure::fClr = fClr;
	}

	void ChangeColor() {
		SetColor(RGB((BYTE)(rand() % 255), (BYTE)(rand() % 255), (BYTE)(rand() % 255)));
	}

};
//---------------------------------------------------------------------------ClsEllipse
class ClsEllipse : virtual  public IFigure {
	CPoint pt1, pt2;			//Координаты эллипса на экране    
	COLORREF color;
public:
	ClsEllipse() :pt1(100, 100), pt2(200, 200) { color = (RGB(255, 255, 0)); fMove = 0; fClr = 0; }
	//конструктор с координатами цвет
	ClsEllipse(CPoint p1, CPoint p2, COLORREF col) { pt1 = p1; pt2 = p2; color = col; }
	ClsEllipse(CPoint p1, CPoint p2, COLORREF col, int fMove, int fClr) { pt1 = p1; pt2 = p2; color = col; IFigure::fMove = fMove;
	IFigure::fClr = fClr; }
	//элипс меняет цвет
	virtual void SetColor(COLORREF cl) { color = cl; }
	virtual void show() {
		CPen pen(PS_SOLID, 2, color);
		SelectObject(hdc, pen);
		Arc(hdc, pt1.x, pt1.y, pt2.x, pt2.y, 100, 200, 0, 100);
	}
	virtual void hide() {
		CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
		SelectObject(hdc, pen);
		Arc(hdc, pt1.x, pt1.y, pt2.x, pt2.y, 100, 200, 0, 100);
	}
	virtual void move(int x, int y) { pt1.x += x, pt1.y += y, pt2.x += x, pt2.y += y; }
	virtual ~ClsEllipse() {/*cout<<"\t ~ClsEllipse()";*/ }

	ClsEllipse(int fMove, int fClr) :pt1(100, 100), pt2(200, 200) {
		IFigure::fMove = fMove;
		IFigure::fClr = fClr;
	}
	void ChangeColor() {
		color = RGB((BYTE)(rand() % 255), (BYTE)(rand() % 255), (BYTE)(rand() % 255));
	}
};
//-------------------------------------------------------------------------Treang
class Triangle: virtual public IFigure {
	POINT pt[4];						//Координаты вершин на экране
	COLORREF color;
public:
	Triangle (POINT* p) : color(RGB(136, 6, 206)) { for (int i = 0; i <5; i++) pt[i] = p[i]; }
	void SetColor(COLORREF cl) { color = cl; }
	void show() {
		CPen pen(PS_SOLID, 2, color);
		SelectObject(hdc, pen);
		Polyline(hdc, pt, 4);
	}
	void hide() {
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		SelectObject(hdc, pen);
		Polyline(hdc, pt, 4);
	}
	void move(int x, int y) { for (int i = 0; i<4; i++) { pt[i].x += x; pt[i].y += y; } }
	virtual ~Triangle() { }
	Triangle(POINT* p, int fMove, int fClr) : color(RGB(136, 6, 206)) {
		for (int i = 0; i <4; i++) pt[i] = p[i];
		IFigure::fMove = fMove;
		IFigure::fClr = fClr;
	}
	void ChangeColor() {
		SetColor(RGB((BYTE)(rand() % 255), (BYTE)(rand() % 255), (BYTE)(rand() % 255)));
	}

};
//-------------------------------------------------------------------------Rectan
class Rectan : virtual public IFigure {
	Square* pSq;
public:
	Rectan & operator=(Rectan &r) { return r; }; //operator=
	virtual void show() { pSq->show(); }//Делегирование
	virtual void move(int x, int y) { pSq->move(x, y); }//Делегирование
	virtual void hide() { pSq->hide(); }//Делегирование
	void SetColor(COLORREF cl) { pSq->SetColor(cl); }
	Rectan(Square& p) { pSq = new Square(p); }
	virtual ~Rectan() { delete pSq; }
	//Для задания "текст в прямоугольнике"
	Rectan(POINT *p) { pSq = new Square(p); }
	Rectan(POINT *p, int fMove, int fClr) { pSq = new Square(p, fMove, fClr); IFigure::fMove = fMove; IFigure::fClr = fClr;
	}
	void ChangeColor() {
		if (IFigure::fClr == 1) {
			pSq->SetColor(RGB((BYTE)(rand() % 255), (BYTE)(rand() % 255), (BYTE)(rand() % 255)));
		}
	}

};
//-------------------------------------------------------------------------DrowTxt
class DrowTxt: virtual public IFigure{	
	// Класс RecordPlayer работает с объектами типа IFigure => Чтобы можно было вызвать из RecordPlayer должен наследоваться от IFigure
	// Класс IFigure - абстрактный. Соответственоо надо переопределить функции hide() и move(), чтоб DrowTxt не был абстрактным и можно было создавать его объекты
	//Для реализации move() необходимо ввести координаты
	POINT pText;
	CString str;
	COLORREF color;
public:
	DrowTxt(CString s) :str(s) {
		pText.x = 400;
		pText.y = 200;
		color = RGB(255, 0, 0);
	};
	void show() {
		CDC* pCDC = CDC::FromHandle(hdc);
		pCDC->SetTextColor(color);
		pCDC->SetBkColor(RGB(0, 0, 0));
		//pCDC->TextOutW(300, 100, str); pCDC->TextOutW(0, 0, " ");			//Было
		pCDC->TextOutW(pText.x, pText.y, str); pCDC->TextOutW(0, 0, " ");	// Стало после введения координат
	}
	void move(int x, int y) {	//virtual можно не писать. Итак будет виртуальной
		CDC* pCDC = CDC::FromHandle(hdc);
		pCDC->SetTextColor(color);
		pCDC->SetBkColor(RGB(0, 0, 0));
		pText.x += x;
		pText.y += y;
		pCDC->TextOutW(pText.x, pText.y, str); pCDC->TextOutW(0, 0, " ");
	}
	void hide() {
		CDC* pCDC = CDC::FromHandle(hdc);
		pCDC->SetTextColor(color);
		pCDC->SetBkColor(RGB(0, 0, 0));
		pCDC->TextOutW(pText.x, pText.y, str); pCDC->TextOutW(0, 0, " ");
	}
	//Конструктор для задания "текст в прямоугольнике". Содержит координаты создания текста
	DrowTxt(CString s, int x, int y) :str(s) {
		pText.x = x;
		pText.y = y;
		color = RGB(255, 0, 0);
	};
	DrowTxt(CString s, int x, int y, int fMove, int fClr) :str(s) {
		pText.x = x;
		pText.y = y;
		IFigure::fMove = fMove;
		IFigure::fClr = fClr;
	};
	void ChangeColor() {
		color = RGB((BYTE)(rand() % 255), (BYTE)(rand() % 255), (BYTE)(rand() % 255));
	}

};
//---------------------------------------------------------------TextInRectangle
class TextInRectangle: public Rectan, public DrowTxt {
public:
	TextInRectangle(POINT *p, CString s) : Rectan(p), DrowTxt(s, p[0].x + 10, p[0].y + 10) {  }
	void show() {
		Rectan::show();
		DrowTxt::show();
	}
	void hide() {
		Rectan::hide();
		DrowTxt::hide();
	}
	void move(int x, int y) {
		Rectan::move(x, y);
		DrowTxt::move(x, y);
	}
	TextInRectangle(POINT *p, CString s, int fMove, int fClr) : Rectan(p, fMove, fClr), DrowTxt(s, p[0].x + 15, p[0].y + 15, fMove, fClr) {
		IFigure::fMove = fMove;
		IFigure::fClr = fClr;
	}
	void ChangeColor() {
		Rectan::ChangeColor();
		DrowTxt::ChangeColor();
	}
};

//-------------------------------------------------------------------------Heir
class Heir : public  Square, public ClsEllipse { //Виртуальный базовый класс
public:                                        //(Множественное наследование)
	Heir(POINT *p) :Square(p), ClsEllipse() {/*cout<<"\t Heir()";*/ }	//Список инициализации
	void show() { Square::show(); ClsEllipse::show(); }
	void move(int x, int y) { Square::move(x, y); ClsEllipse::move(x, y); }
	void hide() { Square::hide(); ClsEllipse::hide(); }
	virtual void SetColor(COLORREF cl) { ClsEllipse::SetColor(cl); }
	virtual ~Heir() {/*cout<<"\n ~Heir()";*/ }
	void ChangeColor() {}
};
//------------------------------------------------------------------------RecordPlayer
class RecordPlayer { //Чтобы воспользоваться классом, объекты должны поддерживать интерфейс IFigure
	IFigure**pFig;//Массив указателей IFigure*
	int n; //Текущее количество указателей в массиве
	int N; //Размерность массива
public:
	void Insert(IFigure* pF) { if (n<N) pFig[n++] = pF; }
	RecordPlayer(int Nfig) : N(Nfig), n(0) { pFig = new IFigure*[N]; }
	virtual void show() { for (int i = 0; i < n; i++) pFig[i]->show(); }//Полиморфизм
	virtual void hide() { for (int i = 0; i < n; i++) pFig[i]->hide(); }//Полиморфизм
	virtual void move(int x, int y) {
		for (int i = 0; i < n; i++) {
			
			if ((*(pFig[i])).fMove == 0)	pFig[i]->move(x, y); //двигаться
			else if ((*(pFig[i])).fMove == 1) {					//мигать
				show();
				Sleep(50);
				hide();
				Sleep(50);
			}
			//менять цвет
			if ((*(pFig[i])).fClr == 1) pFig[i]->ChangeColor();
		}//Полиморфизм
	}
	void PlayMyObject(int x, int y) { for (int i = 0; i <150; i++) { show(); Sleep(24); hide(); move(x, y); } show(); }
	virtual ~RecordPlayer() { delete[]pFig; }
};


void main() {

	InitGraphic();
	int zadanie = 1;
	switch (zadanie) {
	case 1:
	{
		POINT pt1[5];
		pt1[0].x = 40; pt1[0].y = 40;
		pt1[1].x = 40; pt1[1].y = 140;
		pt1[2].x = 140; pt1[2].y = 140;
		pt1[3].x = 140; pt1[3].y = 40;
		pt1[4].x = 40; pt1[4].y = 40;

		DrowTxt dtxt("Привет");
		dtxt.show();
		getchar();

		Heir hr(pt1);
		for (int i = 0; i < 100; i++) { hr.show();  Sleep(14);  hr.hide();  hr.move(0, 3); }
		getchar();

		//0 - фигура двигается; 1 - фигура мигает на месте; 2 - фигура стоит на месте; 
		//0 - фигура цвет не меняет; 1 - фигура меняет цвет
		ClsEllipse elp(0, 1);
		Square sq1(pt1, 1, 0), sq2(pt1, 2, 1), sq3(pt1, 2, 1);

		sq1.SetColor(RGB(255, 255, 0));  sq2.SetColor(RGB(0, 255, 0));
		sq3.SetColor(RGB(0, 0, 255));     hr.SetColor(RGB(0, 255, 255));
		sq2.move(20, 20); 	sq3.move(40, 30); 	hr.move(0, -150);

		Rectan rec(pt1, 2, 1);
		RecordPlayer RPlayer(6);	//сколько фигур будет проигрываться

		RPlayer.Insert(&elp);
		RPlayer.Insert(&sq1);
		RPlayer.Insert(&sq2);
		RPlayer.Insert(&rec);
		RPlayer.Insert(&hr);
		
		POINT pt2[5];
		pt2[0].x = 40; pt2[0].y = 40;
		pt2[1].x = 40; pt2[1].y = 240;
		pt2[2].x = 240; pt2[2].y = 240;
		pt2[3].x = 240; pt2[3].y = 40;
		pt2[4].x = 40; pt2[4].y = 40;
		TextInRectangle tir(pt2, "SomeTextInTheRectangle", 2, 1);
		RPlayer.Insert(&tir);

		RPlayer.PlayMyObject(3, 0);
		getchar();
	}
	case 2:
	{
		
		POINT tria1[4];
		tria1[0].x = 100; tria1[0].y = 500;
		tria1[1].x = 250; tria1[1].y = 300;
		tria1[2].x = 400; tria1[2].y = 500;
		tria1[3].x = 100; tria1[3].y = 500;
		Triangle triangle1(tria1,2,0);
		POINT tria2[4];
		tria2[0].x = 250; tria2[0].y = 500;
		tria2[1].x = 400; tria2[1].y = 350;
		tria2[2].x = 550; tria2[2].y = 500;
		tria2[3].x = 250; tria2[3].y = 500;
		Triangle triangle2(tria2, 2, 0);
		ClsEllipse elipse(CPoint(0,150), CPoint(100, 250),RGB(255,255,0));
		//ClsEllipse elipse(4, 5,RGB(255, 255, 0));
		RecordPlayer Picture(3);
		Picture.Insert(&triangle1);
		Picture.Insert(&triangle2);
		Picture.Insert(&elipse);
		Picture.PlayMyObject(2,-1);
		//0 - фигура двигается; 1 - фигура мигает на месте; 2 - фигура стоит на месте; 
		//0 - фигура цвет не меняет; 1 - фигура меняет цвет
		//облака
		ClsEllipse el1(CPoint(400, 200), CPoint(450, 250), RGB(65,105,255));
		ClsEllipse el2(CPoint(450, 210), CPoint(500, 260), RGB(65, 105, 255));
		ClsEllipse el3(CPoint(440, 180), CPoint(490, 230), RGB(65, 105, 255));
		ClsEllipse el4(CPoint(480, 180), CPoint(530, 230), RGB(65, 105, 255));
		ClsEllipse el5(CPoint(490, 200), CPoint(550, 260), RGB(65, 105, 255));
		ClsEllipse el6(CPoint(520, 190), CPoint(580, 250), RGB(65, 105, 255));
		RecordPlayer Picture2(6);
		Picture2.Insert(&el1);
		Picture2.Insert(&el2);
		Picture2.Insert(&el3);
		Picture2.Insert(&el4);
		Picture2.Insert(&el5);
		Picture2.Insert(&el6);
		Picture2.PlayMyObject(-2, 0);
		getchar();
	}
	}
		CloseGraphic();
}
