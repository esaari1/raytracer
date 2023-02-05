#ifndef _M3DS_PARSER_H_
#define _M3DS_PARSER_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include "Utility/Color.h"
#include "Storage/Storage.h"

using namespace std;

class Hash;
class Mesh;
class Material;

class M3DSParser {

public:
   M3DSParser();
   ~M3DSParser();

   bool load(const string& filename);
   void setHash(Hash* h);
   GeometryObject* getMesh() { return storage; }

private:
   void processTopLevelChunk(int nBytes);
   void processSceneChunk(int nBytes);
   void processModelChunk(size_t nBytes, string name);
   void processTriMeshChunk(int nBytes, string name);
   void processMaterialChunk(size_t nBytes);
   Color processColorChunk(int nBytes);
   float processPercentageChunk(int nBytes);
   void processFaceArrayChunk(int nBytes, Mesh* mesh);
   string processTexmapChunk(int nBytes);

   void readPointArray(Mesh* mesh);
   void readFaceArray(Mesh* mesh, int contentSize);
   void readColor(Color* color);
   void readFloatColor(Color* color);

   void skipBytes(size_t count);

   void checkForNormalMap(Hash* props, string texMap) const;

   double scale;
   string textureDir;
   Storage* storage;
   bool reverse;
   bool applyNormalMap;
   ifstream in;
   map<string, Material*> materials;
   vector<string> excludes;
};

#endif
