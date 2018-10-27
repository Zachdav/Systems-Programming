#include<iostream>
#include<cstdlib>
#include<initializer_list>
#include"Matrix.h"

int main() {

	//Initializing two matrices to explore the functions of Matrix.cpp
	Matrix a(3, 3);
	Matrix b(3, 3);

	//Using the at function to make a 3x3 matrix of all ones.
	a.at(0, 0) = 1;
	a.at(0, 1) = 1;
	a.at(0, 2) = 1;
	a.at(1, 0) = 1;
	a.at(1, 1) = 1;
	a.at(1, 2) = 1;
	a.at(2, 0) = 1;
	a.at(2, 1) = 1;
	a.at(2, 2) = 1;

	//Using the at function to make 3x3 matrix of all threes
	b.at(0, 0) = 3;
        b.at(0, 1) = 3;
        b.at(0, 2) = 3;
        b.at(1, 0) = 3;
        b.at(1, 1) = 3;
        b.at(1, 2) = 3;
        b.at(2, 0) = 3;
        b.at(2, 1) = 3;
        b.at(2, 2) = 3;

	//testing the overloaded operator= by copying a into c.
	Matrix c(3, 3);
	c = a;

	//Testing the operator+ and the add with a double function.
	Matrix d(3, 3);
	d = (a + 2);

	//Testing the operator+ and the add with another matrix function.
	Matrix e(3, 3);
	e = (a + b);

	//Testing the operator- and the subtract with a double function.
	Matrix f(3, 3);
	f = (a - 2);
	
	//Testing the operator- and the subtract with another matrix function.
	Matrix g(3, 3);
	g = (b - a);
	
	//Testing the operator* and the multiply with a double function.
	Matrix h(3, 3);
	h = (b * 2);
	
	//Testing the operator* and the multiply with a matrix function.
	Matrix i(3, 3);
	i = (a * b);
	
	//Testing the operator/ and the divide with a double function.
	Matrix j(3, 3);
	j = (a / 2);

	//Testing the transpose function on matrix b.
	Matrix k(3, 3);
	k = b.t();
}
