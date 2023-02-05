#include "MarbleCheckerBoard.h"
#include "Geometry/GeometryManager.h"
#include "Math/Vector3D.h"
#include "Parser/Hash.h"

MarbleCheckerBoard::MarbleCheckerBoard() {
}

void MarbleCheckerBoard::setHash(Hash* hash) {
   build();
}

void MarbleCheckerBoard::build() {
   Vector3D t;
   GeometryObject* square = GeometryManager::createRectangle(t, 1, 1, Vector3D());
   
   Hash* h = new Hash();
   h->addValue("type", new Value("marble"));
   h->addValue("seed", new Value(10));
   h->addValue("image", new Value("resources/images/marble1.jpg"));
//   h->addValue("noise", new Value(30.0));
   
   Hash* mat = new Hash();
   mat->addValue("material", new Value(h));
   
   square->setHash(mat);
   
   objects.push_back(square);
}