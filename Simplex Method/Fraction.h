#pragma once

#include "stdafx.h"
using namespace std;

class Fraction
{
private:
	int molecular;		//����
	int denominator;	//��ĸ�����ַ�ĸΪ��
	
	void update();
	void throwError(string str) const;

public:
	Fraction(int a = 0, int b = 1);
	~Fraction();

	Fraction operator+ (const Fraction& b) const;
	Fraction operator- (const Fraction& b) const;
	Fraction operator-() const;
	Fraction operator* (const Fraction& b) const;
	Fraction operator/ (const Fraction& b) const;
	Fraction& operator+= (const Fraction& b);
	Fraction& operator/= (const Fraction& b);

	bool operator< (const Fraction& b) const;
	bool operator> (const Fraction& b) const;
	bool operator== (const Fraction& b) const;
	bool operator<= (const Fraction& b) const;
	bool operator>= (const Fraction& b) const;
	bool operator!= (const Fraction& b) const;

	friend ostream& operator<< (ostream& a, const Fraction& b);
	friend istream& operator>> (istream& a, Fraction& b);
};

