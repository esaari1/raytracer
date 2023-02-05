#include "GeometryObject.h"
#include <stdexcept>

const double GeometryObject::epsilon = 1.0 * pow(10, -6);

GeometryObject::GeometryObject() : doDelete(true), ignoreShadow(false), bbox() {
}

GeometryObject::~GeometryObject() {
}

Point3D GeometryObject::sample(const Point3D& hitPoint, float u1, float u2, Vector3D& normal) {
   throw runtime_error("Method sample not implemented");
}

double GeometryObject::pdf(const ShadeRecord& sr) const {
   throw runtime_error("Method pdf not implemented");
}
