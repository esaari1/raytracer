#include "Material.h"
#include "Textures/ImageTexture.h"
#include "Math/Matrix.h"
#include "Parser/Hash.h"
#include "Matte.h"
#include "Phong.h"
#include "Reflective.h"
#include "Emissive.h"
#include "Glass.h"
#include "Marble.h"
#include "AlphaBlend.h"
#include "Metal.h"
#include "BRDFs/Lambertian.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"

Material::Material() : bsdf(), ambientBRDF(new Lambertian()), normalMap(NULL) {
}

Material::~Material() {
   if(normalMap != NULL) {
      delete normalMap;
   }
}

Material* Material::createMaterial(Hash* hash) {
   string type = hash->getString("type");

   Material* material;
   if(type == "matte") {
      material = new Matte();
   }
   else if(type == "phong") {
      material = new Phong();
   }
   else if(type == "reflective") {
      material = new Reflective();
   }
   else if(type == "emissive") {
      material = new Emissive();
   }
   else if(type == "glass") {
      material = new Glass();
   }
   else if(type == "marble") {
      material = new Marble();
   }
   else if(type == "alphaBlend") {
      material = new AlphaBlend();
   }
   else if(type == "metal") {
      material = new Metal();
   }
   else {
      fprintf(stderr, "Unknown material type %s\n", type.c_str());
      exit(1);
   }

   material->setHash(hash);
   if(hash->contains("normalMap")) {
      material->normalMap = Texture::createTexture(hash->getValue("normalMap")->getHash());
   }

   if(hash->contains("normalMapFile")) {
      ImageTexture* tex = new ImageTexture();
      tex->setTextureFile(hash->getString("normalMapFile"));
      material->normalMap = tex;
   }

   return material;
}

void Material::applyNormalMap(ShadeRecord& sr) {
   if(normalMap == NULL) return;

   Matrix tangentMatrix(sr.tangent, sr.binormal, sr.normal);
   tangentMatrix.invert();

   Color color = normalMap->getColor(sr);
   Vector3D mapNormal(2.0 * color.red - 1.0, 2.0 * color.green - 1.0, 2.0 * color.blue - 1.0);
   mapNormal.normalize();
   sr.normal = tangentMatrix * mapNormal;
   sr.normal.normalize();
}
