#ifndef KDTREE_H
#define KDTREE_H

#include "Storage.h"

typedef unsigned int uint32_t;

struct KdNode {
   void initLeaf(uint32_t* _idxs, size_t np);
   void initInterior(uint32_t axis, uint32_t ac, float s);

   float getSplit() const { return split; }
   uint32_t nPrimitives() const { return nPrims >> 2; }
   uint32_t getAxis() const { return flags & 3; }
   bool isLeaf() const { return (flags & 3) == 3; }
   uint32_t getAboveChild() const { return aboveChild >> 2; }

   union {
      float split;      // interior
      uint32_t* idxs;   // leaf
   };

private:
   union {
      uint32_t flags;
      uint32_t nPrims;
      uint32_t aboveChild;
   };
};

struct BoundEdge {
   void set(float t, bool s, int n) { tsplit = t; type = s ? START : END; pnum = n; }
   bool operator< (const BoundEdge& e) const;

   float tsplit;
   enum { START, END } type;
   int pnum;
};

class KdTree : public Storage {

public:
   KdTree();
   virtual ~KdTree();

   virtual void setup();

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray) const;

private:
   void buildTree(int nodeNum, unsigned depth, uint32_t* idxs, size_t nPrimitives, const BBox& bounds, uint32_t* lidxs, uint32_t* ridxs);
   void findSplit(uint32_t* idxs, size_t np, const BBox& bounds, int& axis, double& split, int& offset);

   KdNode *nodes;
   BoundEdge* edges;
   int nAllocedNodes, nextFreeNode;
   unsigned maxDepth;
   unsigned travCost;
   unsigned isectCost;
};

#endif // KDTREE_H
