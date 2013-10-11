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


#include <iostream>
#include <ostream>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "matrix4f.h"
#include "matrix3f.h"

/**
 *
 *	0,  3,  6,
 *  1,  4,  7,
 *  2,  5,  8
 *
 * m11 m12 m13
 * m21 m22 m23
 * m31 m32 m33
 *
 */

// This matrix access
#define m11 m[0]
#define m12 m[3]
#define m13 m[6]
#define m21 m[1]
#define m22 m[4]
#define m23 m[7]
#define m31 m[2]
#define m32 m[5]
#define m33 m[8]

// Temporary matrix access
#define a11 a[0]
#define a12 a[3]
#define a13 a[6]
#define a21 a[1]
#define a22 a[4]
#define a23 a[7]
#define a31 a[2]
#define a32 a[5]
#define a33 a[8]

// Matrix4f access
#define f11 m[0]
#define f12 m[4]
#define f13 m[8]
#define f14 m[12]
#define f21 m[1]
#define f22 m[5]
#define f23 m[9]
#define f24 m[13]
#define f31 m[2]
#define f32 m[6]
#define f33 m[10]
#define f34 m[14]
#define f41 m[3]
#define f42 m[7]
#define f43 m[11]
#define f44 m[15]

#define DET_MIN 0.00001f

Matrix3f::Matrix3f() {

	loadIdentity();
}

Matrix3f::Matrix3f(const Matrix4f &mat) {
	m11 = mat.f11;
	m12 = mat.f12;
	m13 = mat.f13;

	m21 = mat.f21;
	m22 = mat.f22;
	m23 = mat.f23;

	m31 = mat.f31;
	m32 = mat.f32;
	m33 = mat.f33;
}

Matrix3f::Matrix3f(const float mat[9]) {
	for (int i=0; i<9; i++)
		m[i] = mat[i];
}

void Matrix3f::loadIdentity() {
	m11 = 1.0f;
	m12 = 0.0f;
	m13 = 0.0f;

	m21 = 0.0f;
	m22 = 1.0f;
	m23 = 0.0f;

	m31 = 0.0f;
	m32 = 0.0f;
	m33 = 1.0f;
}

void Matrix3f::loadTranspose() {
	float tmp;
	tmp = m12;
	m12 = m21;
	m21 = tmp;

	tmp = m13;
	m13 = m31;
	m31 = tmp;

	tmp = m23;
	m23 = m32;
	m32 = tmp;
}

void Matrix3f::debugOut() {
	static const int w = 6;
	std::cout 	<< std::left << std::setprecision(3) << std::fixed
				<< "[ " << std::setw(w) << m11 << " " << std::setw(w) << m12 << " " << std::setw(w) << m13 << " " <<  " ]\n"
				<< "[ " << std::setw(w) << m21 << " " << std::setw(w) << m22 << " " << std::setw(w) << m23 << " " <<  " ]\n"
				<< "[ " << std::setw(w) << m31 << " " << std::setw(w) << m32 << " " << std::setw(w) << m33 << " " <<  " ]\n";
}

