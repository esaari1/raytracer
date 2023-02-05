#include "M3DSParser.h"
#include "Mesh.h"
#include "Utility/ChunkParser.h"
#include "Utility/3dschunk.h"
#include "Materials/Matte.h"
#include "Materials/Phong.h"
#include "Materials/Metal.h"
#include "Parser/Hash.h"
#include "Storage/KdTree.h"
#include "Storage/Grid.h"
#include "Storage/ObjectList.h"
#include <sstream>
#include <algorithm>

// Include this line to see output of unprocessed chunks
//#define PRINT_UNPROCESSED

M3DSParser::M3DSParser() : scale(1.0), textureDir(""), storage(NULL), reverse(false), applyNormalMap(false), excludes() {
}

M3DSParser::~M3DSParser() {
   // Do not delete the material objects here. They are in use by the mesh class and need to be
   // deleted by the mesh destructor.
   materials.clear();
}

void M3DSParser::setHash(Hash* h) {
   if(h->contains("scale")) {
      scale = h->getDouble("scale");
   }
   if(h->contains("textureDir")) {
      textureDir = h->getString("textureDir");
   }
   if(h->contains("reverse")) {
      reverse = true;
   }
   if(h->contains("storage")) {
      Hash* s = h->getValue("storage")->getHash();
      string type = s->getString("type");

      if(type == "grid") {
         storage = new Grid();
      }
      else if(type == "kdtree") {
         storage = new KdTree();
      }
      else {
         storage = new ObjectList();
      }
      storage->setHash(s);
   }
   else {
      storage = new KdTree();
   }
   if(h->contains("applyNormalMap")) {
      string s = h->getString("applyNormalMap");
      if(s == "true") {
         applyNormalMap = true;
      }
   }

   if(h->contains("excludes")) {
      stringstream strstr(h->getString("excludes"));
      string s;
      while (getline(strstr, s, ',')) {
         excludes.push_back(s);
      }
   }
}

bool M3DSParser::load(const string& filename) {
   in.open(filename.c_str(), ios::in | ios::binary);
   if (!in.good()) {
      fprintf(stderr, "Read3DSFile: Error opening %s\n", filename.c_str());
      return false;
   }

   uint16 chunkType = readUshortLE(in);
   if (chunkType != M3DCHUNK_MAGIC) {
      fprintf(stderr, "Read3DSFile: Wrong magic number in header\n");
      return false;
   }

   int chunkSize = readIntLE(in);
   if (in.bad()) {
      fprintf(stderr, "Read3DSFile: Error reading 3DS file.\n");
      return false;
   }

   int contentSize = chunkSize - 6;
   processTopLevelChunk(contentSize);

   in.close();

   storage->setup();
   return true;
}

void M3DSParser::processTopLevelChunk(int nBytes) {
   int bytesRead = 0;

   while (bytesRead < nBytes) {
      uint16 chunkType = readUshortLE(in);
      int chunkSize = readIntLE(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MESHDATA) {
         processSceneChunk(contentSize);
      }
      else {
#ifdef PRINT_UNPROCESSED
         printf("processTopLevelChunk %X %d\n", chunkType, chunkSize);
#endif
         skipBytes(contentSize);
      }
   }

   if (bytesRead != nBytes) {
      fprintf(stderr, "In processTopLevelChunk expected %d bytes but read %d\n", nBytes, bytesRead);
   }
}

void M3DSParser::processSceneChunk(int nBytes) {
   int bytesRead = 0;

   while (bytesRead < nBytes) {
      uint16 chunkType = readUshortLE(in);
      int chunkSize = readIntLE(in);
      size_t contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_NAMED_OBJECT) {
         string name = readString(in);
         if(find(excludes.begin(), excludes.end(), name) == excludes.end()) {
            processModelChunk(contentSize - (name.length() + 1), name);
         }
      }
      else if (chunkType == M3DCHUNK_MATERIAL_ENTRY) {
         processMaterialChunk(contentSize);
      }
      else {
#ifdef PRINT_UNPROCESSED
         printf("processSceneChunk %X %d\n", chunkType, chunkSize);
#endif
         skipBytes(contentSize);
      }
   }

   if (bytesRead != nBytes) {
      fprintf(stderr, "In processSceneChunk expected %d bytes but read %d\n", nBytes, bytesRead);
   }
}

void M3DSParser::processModelChunk(size_t nBytes, string name) {
   int bytesRead = 0;

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshortLE(in);
      int chunkSize = readIntLE(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_TRIANGLE_MESH) {
         processTriMeshChunk(contentSize, name);
      }
      else {
#ifdef PRINT_UNPROCESSED
         printf("processModelChunk %X %d\n", chunkType, chunkSize);
#endif
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      fprintf(stderr, "In processModelChunk expected %lu bytes but read %d\n", nBytes, bytesRead);
   }
}

void M3DSParser::processTriMeshChunk(int nBytes, string name) {
   int bytesRead = 0;
   Mesh* mesh = new Mesh();

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshortLE(in);
      int chunkSize = readIntLE(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_POINT_ARRAY) {
         readPointArray(mesh);
      }
      else if (chunkType == M3DCHUNK_FACE_ARRAY) {
         readFaceArray(mesh, contentSize);
      }
      else if (chunkType == M3DCHUNK_MESH_TEXTURE_COORDS) {
         int numPoints = readShortLE(in);
         mesh->textureCoordsReserve(numPoints);

         for(int i = 0; i < numPoints; i++) {
            float u = readFloatLE(in);
            float v = readFloatLE(in);
            mesh->addTextureCoord(u, v);
         }
      }
      else {
#ifdef PRINT_UNPROCESSED
         printf("processTriMeshChunk %X %d\n", chunkType, chunkSize);
#endif
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      fprintf(stderr, "In processTriMeshChunk expected %d bytes but read %d\n", nBytes, bytesRead);
   }
   mesh->calculateNormals();

   storage->addObject(mesh);
}

void M3DSParser::processMaterialChunk(size_t nBytes) {
   int bytesRead = 0;

   Hash* props = new Hash();
   string name;
   short shade = 1;
   Color specColor;
   float highlightPercent = 1;

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshortLE(in);
      int chunkSize = readIntLE(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MATERIAL_NAME) {
         name = readString(in);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_AMBIENT) {
         Color c = processColorChunk(contentSize);
         props->addValue("ambientColor", new Value(c.toArray()));
      }
      else if (chunkType == M3DCHUNK_MATERIAL_DIFFUSE) {
         Color c = processColorChunk(contentSize);
         props->addValue("color", new Value(c.toArray()));
      }
      else if (chunkType == M3DCHUNK_MATERIAL_SPECULAR) {
         specColor = processColorChunk(contentSize);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_SHININESS) {
         props->addValue("exp", new Value(processPercentageChunk(contentSize)));
      }
      else if (chunkType == M3DCHUNK_MATERIAL_SHIN2PCT) {
         highlightPercent = processPercentageChunk(contentSize);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_SHADING) {
         shade = readUshortLE(in);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_TEXMAP) {
         string texture = processTexmapChunk(contentSize);
         props->addValue("textureFile", new Value(texture));

         checkForNormalMap(props, texture);
      }
//      else if (chunkType == M3DCHUNK_MATERIAL_TRANSPARENCY) {
//         float p;
//         processPercentageChunk(contentSize, p);
//      }
      else {
#ifdef PRINT_UNPROCESSED
         printf("processMaterialChunk %X %d\n", chunkType, chunkSize);
#endif
         skipBytes(contentSize);
      }
   }

   specColor *= highlightPercent;
   props->addValue("specColor", new Value(specColor.toArray()));

   Material* material;

   if(shade == 1) {
      material = new Matte();
   }
   else if(shade == 3) {
      material = new Metal();
   }
   else {
      material = new Phong();
   }

   material->setHash(props);
   materials[name] = material;

   if(bytesRead != nBytes) {
      fprintf(stderr, "In processMaterialChunk expected %lu bytes but read %d\n", nBytes, bytesRead);
   }
}

void M3DSParser::checkForNormalMap(Hash* props, string texMap) const {
   if(applyNormalMap) {
      // Generate nornal map file name
      string normalMap = texMap.substr(0, texMap.length() - 4) + "Normal.bmp";

      // Check if normal map file exists
      ifstream inp;
      inp.open(normalMap.c_str(), ifstream::in);
      if(!inp.fail()) {
         inp.close();
         props->addValue("normalMapFile", new Value(normalMap));
      }
      inp.clear(ios::failbit);
   }
}

Color M3DSParser::processColorChunk(int nBytes) {
   int bytesRead = 0;
   Color color;

   while (bytesRead < nBytes) {
      uint16 chunkType = readUshortLE(in);
      int chunkSize = readIntLE(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_COLOR_24) {
         readColor(&color);
      }
      else if (chunkType == (M3DCHUNK_COLOR_FLOAT)) {
         readFloatColor(&color);
      }
      else {
#ifdef PRINT_UNPROCESSED
         printf("processColorChunk %X %d\n", chunkType, chunkSize);
#endif
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      fprintf(stderr, "In processColorChunk expected %d bytes but read %d\n", nBytes, bytesRead);
   }

   return color;
}

float M3DSParser::processPercentageChunk(int nBytes) {
   int bytesRead = 0;
   float percent;
   while (bytesRead < nBytes) {
      uint16 chunkType = readUshortLE(in);
      int chunkSize = readIntLE(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_INT_PERCENTAGE) {
         int p = readShortLE(in);
         percent = p / 100.0;
      }
      else if (chunkType == M3DCHUNK_FLOAT_PERCENTAGE) {
         percent = readFloatLE(in);
      }
      else {
#ifdef PRINT_UNPROCESSED
         printf("processPercentageChunk %X %d\n", chunkType, chunkSize);
#endif
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      fprintf(stderr, "In processPercentageChunk expected %d bytes but read %d\n", nBytes, bytesRead);
   }
   return percent;
}

string M3DSParser::processTexmapChunk(int nBytes) {
   int bytesRead = 0;
   string texName;

   while (bytesRead < nBytes) {
      uint16 chunkType = readUshortLE(in);
      int chunkSize = readIntLE(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MATERIAL_MAPNAME) {
         texName = textureDir + readString(in);
      }
      else {
#ifdef PRINT_UNPROCESSED
         printf("processTexmapChunk %X %d\n", chunkType, chunkSize);
#endif
         skipBytes(contentSize);
      }
   }

   return texName;
}


void M3DSParser::readPointArray(Mesh* mesh) {
   uint16 nPoints = readUshortLE(in);
   mesh->pointsReserve(nPoints);

   for (int i = 0; i < (int) nPoints; i++) {
      float x = readFloatLE(in) * scale;
      float y = readFloatLE(in) * scale;
      float z = readFloatLE(in) * scale;
      mesh->addPoint(new Point3D(x, y, z));
   }
}

void M3DSParser::readFaceArray(Mesh* mesh, int contentSize) {
   uint16 nFaces = readUshortLE(in);

   for (int i = 0; i < (int) nFaces; i++) {
      uint16 v0 = readUshortLE(in);
      uint16 v1 = readUshortLE(in);
      uint16 v2 = readUshortLE(in);
      readUshortLE(in);
      if(reverse) {
         mesh->addFace(v2, v1, v0);
      }
      else {
         mesh->addFace(v0, v1, v2);
      }
   }

   int bytesLeft = contentSize - (8 * nFaces + 2);
   if (bytesLeft > 0) {
      processFaceArrayChunk(bytesLeft, mesh);
   }
}

void M3DSParser::processFaceArrayChunk(int nBytes, Mesh* mesh) {
   int bytesRead = 0;

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshortLE(in);
      int chunkSize = readIntLE(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MESH_MATERIAL_GROUP) {
         // For now, we just assume that there is only one material group
         // per triangle mesh, and that the material applies to all faces in
         // the mesh.
         string materialName = readString(in);
         uint16 nFaces = readUshortLE(in);

         for (uint16 i = 0; i < nFaces; i++) {
            uint16 fidx = readUshortLE(in);
            mesh->setFaceMaterial(fidx, materials[materialName]);
         }
      }
      else if(chunkType == M3DCHUNK_MESH_SMOOTH_GROUP) {
         for(FaceIter it = mesh->facesBegin(), end = mesh->facesEnd(); it != end; it++) {
            unsigned int group = readUIntLE(in);
            unsigned mask = 1;
            for(int i = 0; i < 32; i++) {
               if(mask & group) {
                  if(mesh->smoothingGroups.find(group) == mesh->smoothingGroups.end()) {
                     mesh->smoothingGroups[group] = new SmoothingGroup();
                  }
                 (*it)->smoothGroup = group;
                 mesh->smoothingGroups[group]->addFace(*it);
               }
               mask = mask << 1;
            }
         }
      }
      else {
#ifdef PRINT_UNPROCESSED
         printf("processFaceArrayChunk %X %d\n", chunkType, chunkSize);
#endif
         skipBytes(contentSize);
      }
   }
}

void M3DSParser::readColor(Color* color) {
   unsigned char r = readChar(in);
   unsigned char g = readChar(in);
   unsigned char b = readChar(in);

   color->set((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f);
}

void M3DSParser::readFloatColor(Color* color) {
   color->set(readFloatLE(in) / 255.0f, readFloatLE(in) / 255.0f, readFloatLE(in) / 255.0f);
}

void M3DSParser::skipBytes(size_t count) {
   char c;
   while (count-- > 0) {
      in.get(c);
   }
}
