#ifndef _GRID_H_
#define _GRID_H_

#include "Math/Ray.h"
#include "Storage.h"

class GridVoxel {

public:
   GridVoxel();
   ~GridVoxel();

   void add(int idx);
   vector<int> objs;
};

class Grid : public Storage {

public:
   Grid(int max = 64);
   virtual ~Grid();

   virtual void setup();

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;

   void setMaxCells(int cells) { maxCells = cells; }

private:
   void cleanup();
   double calculateNext(double rd, double min, double i, double dt, int n, int& step, int& stop) const;

   GridVoxel** voxels;
   int nx, ny, nz;
   int maxCells;
   int numCells;
};

#endif
