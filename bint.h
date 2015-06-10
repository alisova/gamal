#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

class bint
{
	static const int OSN = 1000000000;                          // основание системы

	vector<int> elements;                                       // внутреннее хранилище числа

	bool sign;                                                  // знак числа

	bool odd() const;                                           // проверяет, является ли текущее число нечетным
	bool even() const;                                          // проверяет, является ли текущее число четным

	void removezeros();                                         // удаляет ведущие нули
	void shiftright();                                          // сдвигает все разряды на 1 вправо (домножает на OSN)

	bint neg();                                                 // возвращает переданное число с другим знаком
public:
	class divzero : public exception { };                       // исключение при делении на ноль

	string __str__();
	string __repr__();
	string toString();

	bint();                                                     // создает длинное целое число со значением 0
	bint(string str);                                           // создает длинное целое число из строки
	bint(int i);                                                // преобразует int к bint
	operator string() const;                                    // преобразует число к строке

	string read(char* c);                                       // считывает строку с чиcлом из файла
	void write(string str, char* c);                            // записывает строку с числом в файл
	bint readbin(char* c);                                      // считывает чиcло из бинарного файла
	void writebin(const bint& b, char* c);                      // записывает число в бинарный файл

	friend ostream& operator <<(ostream& os, const bint& b);    // печатает число в поток вывода
	friend istream& operator >>(istream& is, bint& b);          // считывает число из потока ввода

	bool operator ==(const bint& right);                        // сравнивает два числа на равенство
	bool operator !=(const bint& right);                        // сравнивает два числа на неравенство
	bool operator <(const bint& right);                         // проверяет, является ли левое число меньше правого
	bool operator >(const bint& right);                         // проверяет, является ли левое число больше правого
	bool operator <=(const bint& right);                        // проверяет, является ли левое число <= правого
	bool operator >=(const bint& right);                        // проверяет, является ли левый число >= правого

	bint operator +(const bint& right) const;                   // складывает два числа
	bint operator -(const bint& right) const;                   // вычитает два числа
	bint operator *(const bint& right) const;                   // перемножает два числа
	bint operator /(const bint& right) const;                   // делит два числа
	bint operator %(const bint& right) const;                   // возвращает остаток от деления двух чисел
	bint operator ^(const bint& right) const;                   // возводит число в указанную степень
	bint powmod(bint& a, bint& k, bint& n);			    // возводит число в указанную степень по указанному модулю

	bint operator +=(const bint& right);                        // прибавляет к текущему числу новое
	bint operator -=(const bint& right);                        // вычитает из текущего числа новое
	bint operator *=(const bint& right);                        // умножает текущее число на указанное
	bint operator /=(const bint& right);                        // делит текущее число на указанное
	bint operator %=(const bint& right);                        // присваивает текущему числу остаток от деления на другое число
	bint operator ^=(const bint& right);                        // возводит текущее число в указанную степень

	bint operator -() const;                                    // возвращает копию переданного числа с другим знаком
	bint operator ++();                                         // префиксный инкремент
	bint operator ++(int);                                      // постфиксный инкремент
	bint operator --();                                         // префиксный декремент
	bint operator --(int);                                      // постфиксный декремент
};
