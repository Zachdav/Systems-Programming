//Zachary Davis
//headerfile

#ifndef MATRIX_H
#define MATRIX_H

//necessary includes
#include<initializer_list>
#include<iostream>

//useful macros
//typedef initializer_list<initializer_list<double>> i_list;
typedef unsigned int uint;

//prototype for the matrix class
class Matrix {

	//class members for the objects
	double * matrixPointer;
	uint rows;
	uint cols;
	uint total;

	public:
   
		//function prototypes explained in Matrix.cpp
		Matrix(uint rows, uint cols);
		//Matrix(const i_ist & list);
    		Matrix(const Matrix & m);
    		~Matrix();
    
		Matrix add(double s) const;
    		Matrix add(const Matrix & m) const;
    	
		Matrix subtract(double s) const;
    		Matrix subtract(const Matrix & m) const;
    
		Matrix multiply(double s) const;
    		Matrix multiply(const Matrix & m) const;
    
		Matrix divide(double s) const;
    		Matrix t() const;
    
		const uint numRows() const;
    		const uint numCols() const;
    
		double & at(uint row, uint col);
    		const double & at (uint row, uint col) const;

		Matrix operator+(double s) const;
		Matrix operator+(const Matrix & m) const;

		Matrix operator-(double s) const;
                Matrix operator-(const Matrix & m) const;

		Matrix operator*(double s) const;
                Matrix operator*(const Matrix & m) const;

		Matrix operator/(double s) const;
                
		Matrix operator=(const Matrix & m) const;
}; //Martix Header
#endif
