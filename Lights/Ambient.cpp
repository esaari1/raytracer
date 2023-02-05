#include "Ambient.h"
#include "Parser/Hash.h"

Ambient::Ambient() : Light(), ls(0.0), color(1, 1, 1) {
   samples = new float[2];
   samples[0] = samples[1] = 0.5;
}

Ambient::~Ambient() {
   delete[] samples;
}

void Ambient::setHash(Hash* hash) {
   ls = hash->getDouble("radiance");
   color.set(hash->getValue("color")->getArray());
}

Color Ambient::L(const ShadeRecord& sr) {
   return color * ls;
}

Color Ambient::Sample_L(ShadeRecord& sr, float u1, float u2, Vector3D& lightDir, float& pdf) const {
   pdf = 1.0;
   return color * ls;
}

float* Ambient::getSamples() {
   return samples;
}
