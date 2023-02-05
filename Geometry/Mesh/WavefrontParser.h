#ifndef WAVEFRONTPARSER_H
#define WAVEFRONTPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "Storage/Storage.h"

class Hash;
class Mesh;
class Material;

using namespace std;

class WavefrontParser
{
public:
   WavefrontParser();
   virtual ~WavefrontParser();

   void setHash(Hash* h);
   bool load(const string& filename);
   GeometryObject* getMesh() { return storage; }

private:
   void getNextLine(ifstream& in, string& line);
   void handleFace(string line, Mesh* mesh, int vertexOffset, int textureOffset, string matName);
   vector<string> faceSplit(string str) const;
   vector<int> vertexSplit(string str) const;
   void loadMaterials(string fname);

   double scale;
   string textureDir;
   Storage* storage;
   map<string, Material*> materials;
   vector<string> includes, excludes;
   bool useMaterials;
};

#endif // WAVEFRONTPARSER_H
