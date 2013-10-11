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
 *  matrix4f.h
 *  A 4x4 matrix with related math-functions to modify, suitable for 3D graphics.
 */

#ifndef MATRIX4F_H
#define MATRIX4F_H

class Matrix4f;

#include "matrix3f.h"

class Matrix4f {
public:
	float m[16];
	
	/**
	 * Construct an identity matrix
	 */
	Matrix4f();
	
	/**
	 * Construct a new matrix with given values
	 * @param mat, array of floats to set initial value
	 */
	Matrix4f(const float mat[16]);
	
	/**
	 * Copy values from argument matrix to this matrix, discarding old values
	 * @param mat, Matrix4f to copy values from
	 */
	void copy(const Matrix4f &mat);
	
	/**
	 * Loads this matrix with given values, discarding old values
	 * @param mat, array of floats to set this matrix to
	 */
	void load(const float mat[16]);
	
	/**
	 * Set this matrix to identity, discarding old values
	 */
	void loadIdentity();

	/**
	 * Translate this matrix by the given coordinates
	 * @param x, the x-coordinate to translate
	 * @param y, the y-coord
	 * @param z, the z-coord
	 */
	void translate(float x, float y, float z);

	/**
	 * Translate this matrix by the given coordinates
	 * @param x, the x-coordinate to translate
	 * @param y, the y-coord
	 */
	void translate(float x, float y);

	/**
	 * Rotate this matrix by angle around the axis defined by argument x, y and z-vector and store
	 * the result in this matrix, replacing old values
	 * @param angle, angle in degrees to rotate this matrix about
	 * @param x, the x part of the axis angle vector
	 * @param y, the y part
	 * @param z, the z part
	 */
	void rotate(float angle, float x, float y, float z);

	/**
	 * Scale this matrix by amounts given in argument directions x, y and z
	 * @param x, the amount in x direction to scale this matrix
	 * @param y, the amount in the y direction
	 * @param z, the amount in z
	 */
	void scale(float x, float y, float z);

	/**
	 * Set this matrix to a lookAt matrix, discarding old values
	 * The matrix sets a representation of a camera at eye, looking at a reference point center with
	 * an orientation defined by having some vector pointing 'up'
	 * @param eyeX, x-coordinate of the camera
	 * @param eyeY, y-coord
	 * @param eyeZ, z-coord
	 * @param centerX, x-coordinate of the reference point
	 * @param centerY, y-coord
	 * @param centerZ, z-coord
	 * @param upX, x part of vector defining the camera Up direction, generally leave as zero
	 * @param upY, y part, generally set to 1, pointing directly up y-direction
	 * @param upZ, z part, generally leave as zero
	 */
	void lookAt(float eyeX, float eyeY, float eyeZ,
				float centerX, float centerY, float centerZ,
				float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f);

	/**
	 * Set this matrix to a perspective projection matrix, discarding old values
	 * Below are hints for reasonable values, but are mostly not absolute
	 * @param fovy, how many degrees are visible, values 40(thin) to 90(wide) are reasonable
	 * @param aspect, ratio view angle of x compared to y. Value 1, a square view, 2, twice as wide as tall
	 * @param zNear, near clip plane (cutting away things close to and behind the view), positive and != 0
	 * @param zFar, far clip plane (distance after which things will not be drawn), positive and greater than zNear
	 */
	void loadPerspective(double fovy, double aspect, double zNear, double zFar);

	/**
	 * Set this matrix to an orthographic projection matrix, discarding old values
	 * @param left, set left vertical clipping plane
	 * @param right, right clipping plane
	 * @param bottom, set bottom horizontal clipping plane
	 * @param top, top clipping plane
	 * @param nearVal, near clip plane, cutting away things close the the view
	 * @param farVal, far clip plane, cutting away things far from the view
	 */
	void loadOrtho(double left, double right, double bottom, double top, double nearVal, double farVal);

	/**
	 * Set this matrix to a 2D orthographic projection matrix, discarding old values
	 * @param left, set left vertical clipping plane
	 * @param right, right clipping plane
	 * @param bottom, set bottom horizontal clipping plane
	 * @param top, top clipping plane
	 */
	void loadOrtho2D(double left, double right, double bottom, double top);

	/**
	 * Multiply this matrix by argument 4-float vector array on the right hand side and store result
	 * in argument vector array.
	 * @param vec, right-hand-side vector array of 4 floats to multiply with this matrix
	 */
	void transformVec4f(float *rhs);

	/**
	 * Computes the normal matrix of this matrix and returns a Matrix3f with the result
	 * @return Matrix3f containing the normal matrix computed
	 */
	Matrix3f getNormalMat();

	/**
	 * Set this matrix to its transpose, discarding old values
	 */
	void loadTranspose();

	/**
	 * Set the value of the translation for this matrix, discarding old values
	 * @param x, the x-coordinate of the translation
	 * @param y, the y-coord
	 * @param z, the z-coord
	 */
	void setTranslate(float x, float y, float z);
	
	/**
	 * Multiply this matrix by the argument Matrix4f on the right hand side and store result in
	 * this matrix, replacing old values
	 * @param rhs, right-hand-side matrix to multiply with this matrix
	 */
	void multMat(const Matrix4f &rhs);
	
	/**
	 * Multiply this matrix by the argument Matrix4f on the right hand side and store result in
	 * argument array of floats
	 * @param rhs, right-hand-side matrix to multiply with this matrix
	 * @param res, reference to array of 16 floats to store multiplication result
	 */
	void multMat(const Matrix4f &rhs, float *res);

	/**
	 * Multiply this matrix by the argument array of 16 floats on the right hand side and store
	 * result in this matrix, replacing old values
	 * @param rhs, right-hand-side reference of 16 floats to multiply with this matrix
	 */
	void multMat(const float *rhs);
	
	/**
	 * Display some debug information about this matrix to standard out
	 */
	void debugOut();

};

#endif
