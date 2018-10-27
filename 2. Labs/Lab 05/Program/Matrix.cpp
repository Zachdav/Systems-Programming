//Zachary Davis

//Necessary includes to run the code including the Matrix headerfile
#include <iostream>
#include <initializer_list>
#include "Matrix.h"

//shorthands for the following.
using std::cout;
using std::ostream;
using std::endl;

//short hand for the the following.
typedef unsigned int uint;
//typedef intitializer_list<initializer_list<double>> i_list;

//Matrix constructor that sets all values to zero.
Matrix::Matrix(uint rows, uint cols){
	
	this->rows = rows;
	this->cols = cols;
	total = (rows*cols);
	matrixPointer = new double [total];
	for(uint i=0; i<total; i++){    //loop through every indice setting the value pointed to to xero
		matrixPointer[i] = 0;
	}
}

//Copy constructor that creates a matrix identical to the passed in matrix.
Matrix::Matrix(const Matrix & m){
	
	this->rows = m.numRows();
	this->cols = m.numCols();
	total = (m.numRows()*m.numCols());
	matrixPointer = new double [total];

	for(uint i=0; i<total; i++){                   //looping through the matrix pointer setting each corrosponding
		matrixPointer[i] = m.matrixPointer[i]; //value from the old matrix to the same spot in the new matrix.
	}
}

//Destructor that deletes the allocated memory from the object.
Matrix::~Matrix(){

	delete[] matrixPointer;
}

//Add function that adds s to every value in the matrix.
Matrix Matrix::add(double s) const{

	//create a temp matrix 
	Matrix tempMatrix(numRows(), numCols());

	//loop through matrix adding s to every value.
	for(uint i=0; i<this->total; i++){
		tempMatrix.matrixPointer[i] = matrixPointer[i] + s;
	}
	
	return tempMatrix;
}

//Add one matrix to another
Matrix Matrix::add(const Matrix & m) const{

	Matrix tempMatrix(numRows(), numCols());

	//loop through both matrices adding corrosponding values and setting them in the corrosponding spot in the temp.
	for(uint i=0; i<this->total; i++){
		tempMatrix.matrixPointer[i] = matrixPointer[i] + m.matrixPointer[i];
	}

	return tempMatrix;
}

//Subtracting s from every value in the matrix.
Matrix Matrix::subtract(double s) const{

	Matrix tempMatrix(numRows(), numCols());

	//looping through the matrix subtract s from every value and storing the answer in temp.
	for(uint i=0; i<this->total; i++){
		tempMatrix.matrixPointer[i] = matrixPointer[i] - s;
	}

	return tempMatrix;
}

//subtracting two matrices from each other.
Matrix Matrix::subtract(const Matrix & m) const{

	Matrix tempMatrix(numRows(), numCols());

	//looping through both matrices subtracting corrosponding values and placing them in the same spot in temp
	for(uint i=0; i<this->total; i++){
		tempMatrix.matrixPointer[i] = matrixPointer[i] - m.matrixPointer[i];
	}

	return tempMatrix;
}

//multiplying every value in the matrix by s.
Matrix Matrix::multiply(double s) const{

	Matrix tempMatrix(numRows(), numCols());

	//loops through the matrix subtract every value with s and storing that in temp.
	for(uint i=0; i<this->total; i++){
		tempMatrix.matrixPointer[i] = matrixPointer[i] * s;
	}

	return tempMatrix;
}

//multiplies two matrices together
Matrix Matrix::multiply(const Matrix & m) const{

	Matrix tempMatrix(numRows(), m.numCols());

	//complicated for loop that follows the rules of basic matrix mulitplication and stores the results in temp.
	for(uint i=0; i<numRows(); i++){
		int currentRow = (i*m.numRows());
		for(uint j=0; j<m.numCols(); j++){
			int tempSum = 0;
			for(uint k=0; k<m.numRows(); k++){
				tempSum += (matrixPointer[k]*m.matrixPointer[k*m.numCols()]);
			}
			tempMatrix.matrixPointer[currentRow + j] = tempSum;
		}
	}

	return tempMatrix;
}

//divides all matrix values by s
Matrix Matrix::divide(double s) const{
	
	Matrix tempMatrix(numRows(), numCols());

	//divides every value by s and stores it in the appropriate spot in temp.
	for(uint i=0; i<this->total; i++){
		tempMatrix.matrixPointer[i] = matrixPointer[i] / s;
	}

	return tempMatrix;
}


//flips the matrix to the left and rotates it 90 degrees to the left.
Matrix Matrix::t() const{

	Matrix tempMatrix(numCols(), numRows());

	//nested for loop that switches to locations of the values appropriately
	for(uint i=0; i<numRows(); i++){
		for(uint j=0; j<numCols(); j++){
			tempMatrix.matrixPointer[i+(j*numRows())] = matrixPointer[j+(i*numCols())];
		}
	}

	return tempMatrix;
}

//returns the number of rows in the matrix
const uint Matrix::numRows() const{
	return this->rows;
}

//returns the number of cols in the matrix
const uint Matrix::numCols() const{
	return this->cols;
}

//gets/sets the value at that coordinate
double & Matrix::at(uint rows, uint cols){

	int location = (((rows-1)*numCols())+(cols-1));
	return matrixPointer[location];
}

//same as above method but const qualified.
const double & Matrix::at(uint rows, uint cols) const{

	int location = (((rows-1)*numCols())+(cols-1));
        return matrixPointer[location];
}

//overriding the + to call the add function
Matrix Matrix::operator+(double s) const{
	return add(s);
}

//overriding the + to call the add function
Matrix Matrix::operator+(const Matrix & m) const{
        return add(m);
}

//overriding the - to call the subtract function
Matrix Matrix::operator-(double s) const{
        return subtract(s);
}

//overriding the - to call the subtract function
Matrix Matrix::operator-(const Matrix & m) const{
        return subtract(m);
}

//overriding * to call the multiply function
Matrix Matrix::operator*(double s) const{
        return multiply(s);
}

//overriding * to call the multiply function
Matrix Matrix::operator*(const Matrix & m) const{
        return multiply(m);
}

//overriding divide to call the divide function
Matrix Matrix::operator/(double s) const{
        return divide(s);
}

//overiding = to copy matrices.
Matrix Matrix::operator=(const Matrix & m) const{
        return Matrix(m);
}

//like a toString method in java to print the matrix
ostream & operator<<(ostream & os, const Matrix & m){
	
	std::string matrix=("5");
	
	//looping through the matrix adding everything the one string.
	for(uint i=1; i<=m.numRows(); i++){
		matrix += ("[ ");
		for(uint j=1; j<=m.numCols(); j++){
			matrix += (m.at(i, j));
			if(j != m.numCols()){
				matrix += (", ");
			}
		}
		matrix += (" ]\n");
	}

	return os << matrix;
}
