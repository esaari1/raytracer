#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Point3D.h"
#include "Vector3D.h"
#include <SDL/SDL.h>

class Matrix {

public:
   double m[4][3];

   Matrix();
   Matrix(const Vector3D& row1, const Vector3D& row2, const Vector3D& row3);

   void setIdentity();
   void set(const Vector3D& row1, const Vector3D& row2, const Vector3D& row3);
   void set(const Matrix& matrix);
   void setPosition(const Point3D& position);
   
   void getRow(Vector3D& v, uint8_t row) const;

   void translate(double x, double y, double z);
   void invTranslate(double x, double y, double z);

   void scale(double x, double y, double z);
   void invScale(double x, double y, double z);

   void rotateX(double angle);
   void invRotateX(double angle);

   void rotateY(double angle);
   void invRotateY(double angle);

   void rotateZ(double angle);
   void invRotateZ(double angle);

   void invert();
   void transpose();

   Point3D operator*(const Point3D& p) const;
   Vector3D operator*(const Vector3D& p) const;
};

#endif
