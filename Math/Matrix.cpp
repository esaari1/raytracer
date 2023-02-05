#include "Matrix.h"
#include "Math/Maths.h"
#include <stdio.h>

Matrix::Matrix() {
   setIdentity();
}

Matrix::Matrix(const Vector3D& row1, const Vector3D& row2, const Vector3D& row3) {
   set(row1, row2, row3);
}

void Matrix::setIdentity() {
   for(int y = 0; y < 3; y++) {
      for(int x = 0; x < 4; x++) {
         m[x][y] = (x == y) ? 1.0 : 0.0;
      }
   }
}

void Matrix::set(const Vector3D& row1, const Vector3D& row2, const Vector3D& row3) {
   m[0][0] = row1.x;   m[1][0] = row1.y;   m[2][0] = row1.z;   m[3][0] = 0.0;
   m[0][1] = row2.x;   m[1][1] = row2.y;   m[2][1] = row2.z;   m[3][1] = 0.0;
   m[0][2] = row3.x;   m[1][2] = row3.y;   m[2][2] = row3.z;   m[3][2] = 0.0;
}

void Matrix::set(const Matrix& matrix) {
   for(int y = 0; y < 4; y++) {
      for(int x = 0; x < 3; x++) {
         m[x][y] = matrix.m[x][y];
      }
   }
}

void Matrix::setPosition(const Point3D& position) {
   m[3][0] = position.x;
   m[3][1] = position.y;
   m[3][2] = position.z;
}

void Matrix::getRow(Vector3D& v, uint8_t row) const {
   if(row > 2) {
      fprintf(stderr, "Invalid value %d for row to getRow(). Must be between 0 and 2.\n", row);
      exit(1);
   }
   v.x = m[0][row];
   v.y = m[1][row];
   v.z = m[2][row];
}

void Matrix::translate(double x, double y, double z) {
   m[3][0] = m[0][0] * x + m[1][0] * y + m[2][0] * z + m[3][0];
   m[3][1] = m[0][1] * x + m[1][1] * y + m[2][1] * z + m[3][1];
   m[3][2] = m[0][2] * x + m[1][2] * y + m[2][2] * z + m[3][2];
}

void Matrix::invTranslate(double x, double y, double z) {
   m[3][0] = -m[0][0] * x - m[1][0] * y - m[2][0] * z + m[3][0];
   m[3][1] = -m[0][1] * x - m[1][1] * y - m[2][1] * z + m[3][1];
   m[3][2] = -m[0][2] * x - m[1][2] * y - m[2][2] * z + m[3][2];
}

void Matrix::scale(double x, double y, double z) {
   m[0][0] *= x;  m[1][0] *= y;  m[2][0] *= z;
   m[0][1] *= x;  m[1][1] *= y;  m[2][1] *= z;
   m[0][2] *= x;  m[1][2] *= y;  m[2][2] *= z;
}

void Matrix::invScale(double x, double y, double z) {
   double invX = 1.0 / x;
   double invY = 1.0 / y;
   double invZ = 1.0 / z;
   m[0][0] *= invX;  m[1][0] *= invY;  m[2][0] *= invZ;
   m[0][1] *= invX;  m[1][1] *= invY;  m[2][1] *= invZ;
   m[0][2] *= invX;  m[1][2] *= invY;  m[2][2] *= invZ;
}

void Matrix::rotateX(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);

   double m10 = m[1][0] * cosA + m[2][0] * sinA;
   double m20 = -m[1][0] * sinA + m[2][0] * cosA;
   double m11 = m[1][1] * cosA + m[2][1] * sinA;
   double m21 = -m[1][1] * sinA + m[2][1] * cosA;
   double m12 = m[1][2] * cosA + m[2][2] * sinA;
   double m22 = -m[1][2] * sinA + m[2][2] * cosA;

   m[1][0] = m10;
   m[2][0] = m20;
   m[1][1] = m11;
   m[2][1] = m21;
   m[1][2] = m12;
   m[2][2] = m22;
}

void Matrix::invRotateX(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);

   double m10 = m[1][0] * cosA - m[2][0] * sinA;
   double m20 = m[1][0] * sinA + m[2][0] * cosA;
   double m11 = m[1][1] * cosA - m[2][1] * sinA;
   double m21 = m[1][1] * sinA + m[2][1] * cosA;
   double m12 = m[1][2] * cosA - m[2][2] * sinA;
   double m22 = m[1][2] * sinA + m[2][2] * cosA;

   m[1][0] = m10;
   m[2][0] = m20;
   m[1][1] = m11;
   m[2][1] = m21;
   m[1][2] = m12;
   m[2][2] = m22;
}

void Matrix::rotateY(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);

   double m00 = m[0][0] * cosA - m[2][0] * sinA;
   double m20 = m[0][0] * sinA + m[2][0] * cosA;
   double m01 = m[0][1] * cosA - m[2][1] * sinA;
   double m21 = m[0][1] * sinA + m[2][1] * cosA;
   double m02 = m[0][2] * cosA - m[2][2] * sinA;
   double m22 = m[0][2] * sinA + m[2][2] * cosA;

   m[0][0] = m00;
   m[2][0] = m20;
   m[0][1] = m01;
   m[2][1] = m21;
   m[0][2] = m02;
   m[2][2] = m22;
}

void Matrix::invRotateY(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);

   double m00 = m[0][0] * cosA + m[2][0] * sinA;
   double m20 = -m[0][0] * sinA + m[2][0] * cosA;
   double m01 = m[0][1] * cosA + m[2][1] * sinA;
   double m21 = -m[0][1] * sinA + m[2][1] * cosA;
   double m02 = m[0][2] * cosA + m[2][2] * sinA;
   double m22 = -m[0][2] * sinA + m[2][2] * cosA;

   m[0][0] = m00;
   m[2][0] = m20;
   m[0][1] = m01;
   m[2][1] = m21;
   m[0][2] = m02;
   m[2][2] = m22;
}

void Matrix::rotateZ(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);

   double m00 = m[0][0] * cosA + m[1][0] * sinA;
   double m10 = -m[0][0] * sinA + m[1][0] * cosA;
   double m01 = m[0][1] * cosA + m[1][1] * sinA;
   double m11 = -m[0][1] * sinA + m[1][1] * cosA;
   double m02 = m[0][2] * cosA + m[1][2] * sinA;
   double m12 = -m[0][2] * sinA + m[1][2] * cosA;

   m[0][0] = m00;
   m[1][0] = m10;
   m[0][1] = m01;
   m[1][1] = m11;
   m[0][2] = m02;
   m[1][2] = m12;
}

void Matrix::invRotateZ(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);

   double m00 = m[0][0] * cosA - m[1][0] * sinA;
   double m10 = m[0][0] * sinA + m[1][0] * cosA;
   double m01 = m[0][1] * cosA - m[1][1] * sinA;
   double m11 = m[0][1] * sinA + m[1][1] * cosA;
   double m02 = m[0][2] * cosA - m[1][2] * sinA;
   double m12 = m[0][2] * sinA + m[1][2] * cosA;

   m[0][0] = m00;
   m[1][0] = m10;
   m[0][1] = m01;
   m[1][1] = m11;
   m[0][2] = m02;
   m[1][2] = m12;
}

Point3D Matrix::operator*(const Point3D& p) const {
   return Point3D(m[0][0] * p.x + m[1][0] * p.y + m[2][0] * p.z + m[3][0],
                  m[0][1] * p.x + m[1][1] * p.y + m[2][1] * p.z + m[3][1],
                  m[0][2] * p.x + m[1][2] * p.y + m[2][2] * p.z + m[3][2]);
}

Vector3D Matrix::operator*(const Vector3D& p) const {
   return Vector3D(m[0][0] * p.x + m[1][0] * p.y + m[2][0] * p.z,
                   m[0][1] * p.x + m[1][1] * p.y + m[2][1] * p.z,
                   m[0][2] * p.x + m[1][2] * p.y + m[2][2] * p.z);
}

void Matrix::invert() {
   double det = m[0][0] * m[1][1] * m[2][2] +
                m[1][0] * m[2][1] * m[0][2] +
                m[2][0] * m[0][1] * m[1][2] -
                m[2][0] * m[1][1] * m[0][2] -
                m[1][0] * m[0][1] * m[2][2] -
                m[0][0] * m[2][1] * m[1][2];
   if(det == 0.0) {
      exit(1);
   }
   det = 1.0 / det;
   m[0][0] = m[0][0] * det; m[1][0] = m[0][1] * det; m[2][0] = m[0][2] * det;
   m[0][1] = m[1][0] * det; m[1][1] = m[1][1] * det; m[2][1] = m[1][2] * det;
   m[0][2] = m[2][0] * det; m[1][2] = m[2][1] * det; m[2][2] = m[2][2] * det;
}

void Matrix::transpose() {
   double m01 = m[0][1];
   double m02 = m[0][2];
   double m10 = m[1][0];
   double m12 = m[1][2];
   double m20 = m[2][0];
   double m21 = m[2][1];

   m[0][1] = m10;
   m[0][2] = m20;
   m[1][0] = m01;
   m[1][2] = m21;
   m[2][0] = m02;
   m[2][1] = m12;
}
