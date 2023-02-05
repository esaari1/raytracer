#ifndef _SHADE_RECORD_H_
#define _SHADE_RECORD_H_

#include "Math/Vector3D.h"
#include "Math/Point3D.h"

class Material;

class ShadeRecord {

public:
   ShadeRecord();
   ShadeRecord(const ShadeRecord& other);

   Vector3D normal;
   Vector3D tangent;
   Vector3D binormal;
   Point3D hitPoint;
   Point3D localHitPoint;
   Material* material;

   Point3D samplePoint;

   double tu, tv;
};

#endif
