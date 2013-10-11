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

#include "matrix4f.h"

/**
 *
 *	0,  4,  8, 12,
 *  1,  5,  9, 13,
 *  2,  6, 10, 14,
 *  3,  7, 11, 15
 *
 * m11 m12 m13 m14
 * m21 m22 m23 m24
 * m31 m32 m33 m34
 * m41 m42 m43 m44
 *
 */

// This matrix access
#define m11 m[0]
#define m12 m[4]
#define m13 m[8]
#define m14 m[12]
#define m21 m[1]
#define m22 m[5]
#define m23 m[9]
#define m24 m[13]
#define m31 m[2]
#define m32 m[6]
#define m33 m[10]
#define m34 m[14]
#define m41 m[3]
#define m42 m[7]
#define m43 m[11]
#define m44 m[15]

// This matrix translation access
#define tx m[12]
#define ty m[13]
#define tz m[14]
#define tw m[15]

// Temporary matrix access
#define a11 a[0]
#define a12 a[4]
#define a13 a[8]
#define a14 a[12]
#define a21 a[1]
#define a22 a[5]
#define a23 a[9]
#define a24 a[13]
#define a31 a[2]
#define a32 a[6]
#define a33 a[10]
#define a34 a[14]
#define a41 a[3]
#define a42 a[7]
#define a43 a[11]
#define a44 a[15]

// 3x3 matrix access
#define t11 a[0]
#define t12 a[3]
#define t13 a[6]
#define t21 a[1]
#define t22 a[4]
#define t23 a[7]
#define t31 a[2]
#define t32 a[5]
#define t33 a[8]

#define EPS 0.00001f	// Test against zero for floating point precision

const double PI = 3.141592;
const double PIover = PI/180.0;


inline static void cross3f(	float *r1, float *r2, float *r3,
						float a1, float a2, float a3,
						float b1, float b2, float b3 )
{
	*r1 = a2*b3 - a3*b2;
	*r2 = a3*b1 - a1*b3;
	*r3 = a1*b2 - a2*b1;
}

Matrix4f::Matrix4f() {
	loadIdentity();
}

Matrix4f::Matrix4f(const float mat[16]) {
	for (int i=0; i<16; i++)
		m[i] = mat[i];
}

void Matrix4f::copy(const Matrix4f &mat) {
	for (int i=0; i<16; i++)
		m[i] = mat.m[i];
}

void Matrix4f::load(const float mat[16]) {
	for (int i=0; i<16; i++) {
		m[i] = mat[i];
	}
}

void Matrix4f::loadTranspose() {
	float tmp;
	tmp = m12;
	m12 = m21;
	m21 = tmp;
	
	tmp = m13;
	m13 = m31;
	m31 = tmp;
	
	tmp = m14;
	m14 = m41;
	m41 = tmp;
	
	tmp = m23;
	m23 = m32;
	m32 = tmp;
	
	tmp = m24;
	m24 = m42;
	m42 = tmp;
	
	tmp = m34;
	m34 = m43;
	m43 = tmp;
}

void Matrix4f::loadIdentity() {
	m12 = 0.0f;
	m13 = 0.0f;
	m14 = 0.0f;
	
	m21 = 0.0f;
	m23 = 0.0f;
	m24 = 0.0f;
	
	m31 = 0.0f;
	m32 = 0.0f;
	m34 = 0.0f;
	
	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	
	m11 = 1.0f;
	m22 = 1.0f;
	m33 = 1.0f;
	m44 = 1.0f;			
}

void Matrix4f::setTranslate(float x, float y, float z) {
	tx = x;
	ty = y;
	tz = z;
	tw = 1.0f;
}

void Matrix4f::translate(float x, float y, float z) {
	float nx = m11*x + m12*y + m13*z + m14;
	float ny = m21*x + m22*y + m23*z + m24;
	float nz = m31*x + m32*y + m33*z + m34;
	float nw = m41*x + m42*y + m43*z + m44;
	
	tx = nx;
	ty = ny;
	tz = nz;
	tw = nw;
}

void Matrix4f::translate(float x, float y) {
	float nx = m11*x + m12*y + m13 + m14;
	float ny = m21*x + m22*y + m23 + m24;
	float nz = m31*x + m32*y + m33 + m34;
	float nw = m41*x + m42*y + m43 + m44;
	
	tx = nx;
	ty = ny;
	tz = nz;
	tw = nw;
}

void Matrix4f::rotate(float angle, float x, float y, float z) {

	angle = angle*PIover;
	double c = cos(angle);
	double s = sin(angle);
	
	double cinv = 1.0-c;
	
	double xycinv = x*y*cinv;
	double xzcinv = x*z*cinv;
	double yzcinv = y*z*cinv;
	
	double ys = y*s;
	double xs = x*s;
	double zs = z*s;
	
	float a[16];
	
	a11 = x*x*cinv	+ c;
	a12 = xycinv	- zs;
	a13 = xzcinv	+ ys;
	a14 = 0.0f;
	
	a21 = xycinv	+ zs;
	a22 = y*y*cinv	+ c;
	a23 = yzcinv	- xs;
	a24 = 0.0f;
	
	a31 = xzcinv	- ys;
	a32 = yzcinv	+ xs;
	a33 = z*z*cinv	+ c;
	a34 = 0.0f;
	
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
	
	multMat(a);
}

void Matrix4f::multMat(const Matrix4f &rhs) {
	float res[16];		// Temp storage
	const float *a = rhs.m;

	res[0] = 	m11*a11 + m12*a21 + m13*a31 + m14*a41;
	res[4] = 	m11*a12 + m12*a22 + m13*a32 + m14*a42;
	res[8] = 	m11*a13 + m12*a23 + m13*a33 + m14*a43;
	res[12] = 	m11*a14 + m12*a24 + m13*a34 + m14*a44;

	res[1] = 	m21*a11 + m22*a21 + m23*a31 + m24*a41;
	res[5] = 	m21*a12 + m22*a22 + m23*a32 + m24*a42;
	res[9] = 	m21*a13 + m22*a23 + m23*a33 + m24*a43;
	res[13] = 	m21*a14 + m22*a24 + m23*a34 + m24*a44;

	res[2] = 	m31*a11 + m32*a21 + m33*a31 + m34*a41;
	res[6] = 	m31*a12 + m32*a22 + m33*a32 + m34*a42;
	res[10] = 	m31*a13 + m32*a23 + m33*a33 + m34*a43;
	res[14] = 	m31*a14 + m32*a24 + m33*a34 + m34*a44;

	res[3] = 	m41*a11 + m42*a21 + m43*a31 + m44*a41;
	res[7] = 	m41*a12 + m42*a22 + m43*a32 + m44*a42;
	res[11] = 	m41*a13 + m42*a23 + m43*a33 + m44*a43;
	res[15] = 	m41*a14 + m42*a24 + m43*a34 + m44*a44;

	m11 = res[0];
	m12 = res[4];
	m13 = res[8];
	m14 = res[12];
	
	m21 = res[1];
	m22 = res[5];
	m23 = res[9];
	m24 = res[13];
	
	m31 = res[2];
	m32 = res[6];
	m33 = res[10];
	m34 = res[14];
	
	m41 = res[3];
	m42 = res[7];
	m43 = res[11];
	m44 = res[15];
}

void Matrix4f::multMat(const Matrix4f &rhs, float *res) {
	const float *a = rhs.m;

	res[0] = 	m11*a11 + m12*a21 + m13*a31 + m14*a41;
	res[4] = 	m11*a12 + m12*a22 + m13*a32 + m14*a42;
	res[8] = 	m11*a13 + m12*a23 + m13*a33 + m14*a43;
	res[12] = 	m11*a14 + m12*a24 + m13*a34 + m14*a44;

	res[1] = 	m21*a11 + m22*a21 + m23*a31 + m24*a41;
	res[5] = 	m21*a12 + m22*a22 + m23*a32 + m24*a42;
	res[9] = 	m21*a13 + m22*a23 + m23*a33 + m24*a43;
	res[13] = 	m21*a14 + m22*a24 + m23*a34 + m24*a44;

	res[2] = 	m31*a11 + m32*a21 + m33*a31 + m34*a41;
	res[6] = 	m31*a12 + m32*a22 + m33*a32 + m34*a42;
	res[10] = 	m31*a13 + m32*a23 + m33*a33 + m34*a43;
	res[14] = 	m31*a14 + m32*a24 + m33*a34 + m34*a44;

	res[3] = 	m41*a11 + m42*a21 + m43*a31 + m44*a41;
	res[7] = 	m41*a12 + m42*a22 + m43*a32 + m44*a42;
	res[11] = 	m41*a13 + m42*a23 + m43*a33 + m44*a43;
	res[15] = 	m41*a14 + m42*a24 + m43*a34 + m44*a44;
}

void Matrix4f::multMat(const float *a) {
	float res[16];		// Temp storage

	res[0] = 	m11*a11 + m12*a21 + m13*a31 + m14*a41;
	res[4] = 	m11*a12 + m12*a22 + m13*a32 + m14*a42;
	res[8] = 	m11*a13 + m12*a23 + m13*a33 + m14*a43;
	res[12] = 	m11*a14 + m12*a24 + m13*a34 + m14*a44;

	res[1] = 	m21*a11 + m22*a21 + m23*a31 + m24*a41;
	res[5] = 	m21*a12 + m22*a22 + m23*a32 + m24*a42;
	res[9] = 	m21*a13 + m22*a23 + m23*a33 + m24*a43;
	res[13] = 	m21*a14 + m22*a24 + m23*a34 + m24*a44;

	res[2] = 	m31*a11 + m32*a21 + m33*a31 + m34*a41;
	res[6] = 	m31*a12 + m32*a22 + m33*a32 + m34*a42;
	res[10] = 	m31*a13 + m32*a23 + m33*a33 + m34*a43;
	res[14] = 	m31*a14 + m32*a24 + m33*a34 + m34*a44;

	res[3] = 	m41*a11 + m42*a21 + m43*a31 + m44*a41;
	res[7] = 	m41*a12 + m42*a22 + m43*a32 + m44*a42;
	res[11] = 	m41*a13 + m42*a23 + m43*a33 + m44*a43;
	res[15] = 	m41*a14 + m42*a24 + m43*a34 + m44*a44;

	m11 = res[0];
	m12 = res[4];
	m13 = res[8];
	m14 = res[12];
	
	m21 = res[1];
	m22 = res[5];
	m23 = res[9];
	m24 = res[13];
	
	m31 = res[2];
	m32 = res[6];
	m33 = res[10];
	m34 = res[14];
	
	m41 = res[3];
	m42 = res[7];
	m43 = res[11];
	m44 = res[15];
}

void Matrix4f::transformVec4f(float *rhs) {
	float t0 = m11*rhs[0] + m12*rhs[1] + m13*rhs[2] + m14*rhs[3];
	float t1 = m21*rhs[0] + m22*rhs[1] + m23*rhs[2] + m24*rhs[3];
	float t2 = m31*rhs[0] + m32*rhs[1] + m33*rhs[2] + m34*rhs[3];
	float t3 = m41*rhs[0] + m42*rhs[1] + m43*rhs[2] + m44*rhs[3];

	rhs[0] = t0;
	rhs[1] = t1;
	rhs[2] = t2;
	rhs[3] = t3;
}

void Matrix4f::scale(float x, float y, float z) {
	float a[16];
	a11 = x;
	a12 = 0.0f;
	a13 = 0.0f;
	a14 = 0.0f;

	a21 = 0.0f;
	a22 = y;
	a23 = 0.0f;
	a24 = 0.0f;

	a31 = 0.0f;
	a32 = 0.0f;
	a33 = z;
	a34 = 0.0f;

	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;

	this->multMat(a);
}

Matrix3f Matrix4f::getNormalMat() {

	// The normal matrix is the inverse-transpose of the upper left 3x3 sub-matrix of a 4x4 transform
	// matrix. If scaling has been restricted to only uniform transforms (where all axis are scaled
	// equally), then the normal matrix is just equal the upper 3x3 sub-matrix.

	Matrix3f norm;

	// Determinate
	float det = (	  m11 * ( m22 * m33 - m32 * m23 )
					- m12 * ( m21 * m33 - m23 * m31 )
					+ m13 * ( m21 * m32 - m22 * m31 )	);

	// Check determinate against zero
	if (det == 0 || (det < EPS && det > -EPS)) {
		std::cout << "Determinate of 3x3 matrix was zero. Quitting\n";	// Die horribly
		exit(1);
	}

	// Inverse transposed
	float *a = norm.m;
	t11 =  ( m22 * m33 - m32 * m23 )/det;
	t21 = -( m12 * m33 - m13 * m32 )/det;
	t31 =  ( m12 * m23 - m13 * m22 )/det;
	t12 = -( m21 * m33 - m23 * m31 )/det;
	t22 =  ( m11 * m33 - m13 * m31 )/det;
	t32 = -( m11 * m23 - m21 * m13 )/det;
	t13 =  ( m21 * m32 - m31 * m22 )/det;
	t23 = -( m11 * m32 - m31 * m12 )/det;
	t33 =  ( m11 * m22 - m21 * m12 )/det;

	return norm;
}

void Matrix4f::lookAt(	float eyeX, float eyeY, float eyeZ,
						float centerX, float centerY, float centerZ,
						float upX, float upY, float upZ)
{
	// F
	float fx = centerX-eyeX;
	float fy = centerY-eyeY;
	float fz = centerZ-eyeZ;
	
	double fabs = sqrt(fx*fx + fy*fy + fz*fz);
	// f
	fx /= fabs;
	fy /= fabs;
	fz /= fabs;
	
	// UP'
	double uabs = sqrt(fx*fx + fy*fy + fz*fz);
	float upx = upX/uabs;
	float upy = upY/uabs;
	float upz = upZ/uabs;
	
	// s = f x UP'
	float sx;
	float sy;
	float sz;
	cross3f(&sx, &sy, &sz, fx, fy, fz, upx, upy, upz);
	// Normalise side vector
	double sabs = sqrt(sx*sx + sy*sy + sz*sz);
	sx /= sabs;
	sy /= sabs;
	sz /= sabs;
	
	// u = s x f
	float ux;
	float uy;
	float uz;
	cross3f(&ux, &uy, &uz, sx, sy, sz, fx, fy, fz);
	
	float a[16];
	
	a11 = sx;
	a12 = sy;
	a13 = sz;
	a14 = 0.0f;
	
	a21 = ux;
	a22 = uy;
	a23 = uz;
	a24 = 0.0f;
	
	a31 = -fx;
	a32 = -fy;
	a33 = -fz;
	a34 = 0.0f;
	
	a41 = 0.0f;
	a42 = 0.0f;
	a43 = 0.0f;
	a44 = 1.0f;
	
	multMat(a);
	translate(-eyeX, -eyeY, -eyeZ);
}

void Matrix4f::loadPerspective(double fovy, double aspect, double zNear, double zFar) {

	double fovyRad = fovy * PIover;

	double f = 1.0/tan(fovyRad/2.0);
	double zD = zNear-zFar;
	
	m11 = f/aspect;
	m22 = f;
	m33 = (zFar+zNear) / zD;
	m34 = (2.0*zFar*zNear) / zD;
	m43 = -1.0f;
	
	m12 = 0.0f;
	m13 = 0.0f;
	m14 = 0.0f;
	m21 = 0.0f;
	m23 = 0.0f;
	m24 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
	m41 = 0.0f;
	m42 = 0.0f;
	m44 = 0.0f;

}

void Matrix4f::loadOrtho(double left, double right, double bottom, double top, double nearVal, double farVal) {
	double rl = right-left;
	double tb = top-bottom;
	double fn = farVal-nearVal;
	
	m11 = 2.0/rl;
	m22 = 2.0/tb;
	m33 = -2.0/fn;
	m44 = 1.0f;
	
	m14 = -(right+left)/rl;
	m24 = -(top+bottom)/tb;
	m34 = -(farVal+nearVal)/fn;
	
	m12 = 0.0f;
	m13 = 0.0f;
	m21 = 0.0f;
	m23 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
}

void Matrix4f::loadOrtho2D(double left, double right, double bottom, double top) {
	double rl = right-left;
	double tb = top-bottom;
	
	m11 = 2.0/rl;
	m22 = 2.0/tb;
	m33 = -1.0f;
	m44 = 1.0f;
	
	m14 = -(right+left)/rl;
	m24 = -(top+bottom)/tb;
	m34 = 0.0f;
	
	m12 = 0.0f;
	m13 = 0.0f;
	m21 = 0.0f;
	m23 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
}

void Matrix4f::debugOut() {
	static const int w = 6;
	std::cout 	<< std::left << std::setprecision(3) << std::fixed
				<< "[ " << std::setw(w) << m11 << " " << std::setw(w) << m12 << " " << std::setw(w) << m13 << " " << std::setw(w) << m14 << " ]\n"
				<< "[ " << std::setw(w) << m21 << " " << std::setw(w) << m22 << " " << std::setw(w) << m23 << " " << std::setw(w) << m24 << " ]\n"
				<< "[ " << std::setw(w) << m31 << " " << std::setw(w) << m32 << " " << std::setw(w) << m33 << " " << std::setw(w) << m34 << " ]\n"
				<< "[ " << std::setw(w) << m41 << " " << std::setw(w) << m42 << " " << std::setw(w) << m34 << " " << std::setw(w) << m44 << " ]\n";
}
