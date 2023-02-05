#include "Ray.h"
#include "Maths.h"

Ray::Ray() : origin(Point3D(0, 0, 0)), direction(Vector3D(0, 0, -1)), depth(0), tHit(HUGE_VALUE) {
}

Ray::Ray(const Point3D& o, const Vector3D& d, int _depth) : origin(o), direction(d), depth(_depth), tHit(HUGE_VALUE) {
}

Ray::Ray(const Ray& ray) : origin(ray.origin), direction(ray.direction), depth(ray.depth), tHit(HUGE_VALUE) {
}

Matrix Ray::calculateNormalMatrix() const {
   Point3D n = origin - direction;
   Vector3D nVec(n.x, n.y, n.z);
   Vector3D uVec = Vector3D(0, 1, 0).cross(nVec);
   nVec.normalize();
   uVec.normalize();
   Vector3D vVec = nVec.cross(uVec);

   Matrix matrix(uVec, vVec, nVec);
   matrix.invert();
   return matrix;
}
