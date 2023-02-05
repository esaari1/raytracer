#ifndef _MESH_H_
#define _MESH_H_

#include "Math/Point3D.h"
#include "Math/Point2D.h"
#include "Math/Vector3D.h"
#include "Geometry/Compound/Compound.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class Mesh;

class Face : public GeometryObject {

public:
   Face(Mesh& mesh, int idx1, int idx2, int idx3);

   virtual void setHash(Hash* hash) {}
   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;
   void setTextureCoords(ShadeRecord& sr, double b1, double b2) const;
   void setTextureIdxs(int idx1, int idx2, int idx3);

   int vertIdxs[3];
   int textureIdxs[3];
   Vector3D p1p2;
   Vector3D p1p3;
   Vector3D normal;
//   Vector3D dpdu;
//   Vector3D dpdv;
   uint32_t smoothGroup;
   Mesh& parent;
};

class SmoothingGroup {
public:
   ~SmoothingGroup();

   void addFace(Face* f);
   void normalize();
   Vector3D interpolateNormal(const Face* face, const double beta, const double gamma);
//   Vector3D interpolateDPDU(const Face* face, const double beta, const double gamma);
//   Vector3D interpolateDPDV(const Face* face, const double beta, const double gamma);

   map<int, Vector3D*> normals;
//   map<int, Vector3D*> dpdu;
//   map<int, Vector3D*> dpdv;
};

typedef vector<Face*>::const_iterator FaceIter;

class Mesh : public Compound {

public:
   Mesh();
   virtual ~Mesh();

   virtual vector<GeometryObject*> getObjects() const { return vector<GeometryObject*>(faces.begin(), faces.end()); }

   void pointsReserve(int size) { points.reserve(size); }
   size_t addPoint(Point3D* p);

   void facesReserve(int size) { faces.reserve(size); }
   Face* addFace(int v1, int v2, int v3);

   void textureCoordsReserve(int size) { textureCoords.reserve(size); }
   void addTextureCoord(float u, float v) { textureCoords.push_back(Point2D(u, v)); }
   vector<Point2D> textureCoords;

   FaceIter facesBegin() const { return faces.begin(); }
   FaceIter facesEnd() const { return faces.end(); }
   void setFaceMaterial(int idx, Material* material) { faces[idx]->setMaterial(material); }
   Point3D* getPointAt(int idx) const { return points[idx]; }

   void calculateNormals();

   virtual void setHash(Hash* hash);
   virtual void setMaterial(Material* m);

   void setSmoothingGroup(int group);
   map<unsigned int, SmoothingGroup*> smoothingGroups;

protected:
   size_t getPointCount() const { return points.size(); }

   vector<Point3D*> points;
   vector<Face*> faces;

private:
//   void computePartialDerivitives(Face* face) const;
//   void getUVs(double uv[3][2], Face* face) const;
};

#endif
