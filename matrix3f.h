/*********************************************************
 Version 1.000
 
 Code provided by Michael Hemsley and Anthony Dick
 for the course 
 COMP SCI 3014/7090 Computer Graphics
 School of Computer Science
 University of Adelaide
 
 Permission is granted for anyone to copy, use, modify, or distribute this
 program and accompanying programs and documents for any purpose, provided
 this copyright notice is retained and prominently displayed, along with
 a note saying that the original programs are available from the aforementioned 
 course web page. 
 
 The programs and documents are distributed without any warranty, express or
 implied.  As the programs were written for research purposes only, they have
 not been tested to the degree that would be advisable in any important
 application.  All use of these programs is entirely at the user's own risk.
 *********************************************************/

/**
 *  matrix3f.h
 *  A 3x3 matrix with related math-functions to modify, suitable for 3D graphics.
 */

#ifndef MATRIX3F_H
#define MATRIX3F_H

class Matrix3f;

#include "matrix4f.h"

class Matrix3f {
public:
	float m[9];

	/**
	 * Construct an identity matrix
	 */
	Matrix3f();

	/**
	 * Construct a new matrix as a copy of upper-left 3x3 sub-matrix of argument 4x4 matrix
	 * @param mat, Matrix4f to copy values from
	 */
	Matrix3f(const Matrix4f &mat);

	/**
	 * Construct a new matrix with given values
	 * @param mat, array of floats to set initial value
	 */
	Matrix3f(const float mat[9]);

	/**
	 * Set this matrix to identity, discarding old values
	 */
	void loadIdentity();

	/**
	 * Set this matrix to its transpose, discarding old values
	 */
	void loadTranspose();

	/**
	 * Display some debug information about this matrix to standard out
	 */
	void debugOut();

};

#endif /* MATRIX3F_H */
