#include "ShadeRecord.h"

ShadeRecord::ShadeRecord() :
   normal(),
   tangent(),
   binormal(),
   hitPoint(),
   localHitPoint(),
   material(NULL),
   samplePoint(),
   tu(0),
   tv(0)
{
}

ShadeRecord::ShadeRecord(const ShadeRecord& other) {
   normal = other.normal;
   tangent = other.tangent;
   binormal = other.binormal;
   hitPoint = other.hitPoint;
   localHitPoint = other.localHitPoint;
   material = other.material;
   samplePoint = other.samplePoint;
   tu = other.tu;
   tv = other.tv;
}
