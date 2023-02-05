#include "Shape2D.h"
#include "Utility/FalloffFilter.h"

Shape2D::Shape2D() : filter(new LinearFilter()) {}

Shape2D::~Shape2D() {
   if(filter != NULL) {
      delete filter;
   }
}

void Shape2D::setFilter(FalloffFilter* f) {
   delete filter;
   filter = f;
}

void Shape2D::setFilterNoDelete(FalloffFilter* f) {
   filter = f;
}
