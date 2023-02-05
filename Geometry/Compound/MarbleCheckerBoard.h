#ifndef raytracer_MarbleCheckerBoard_h
#define raytracer_MarbleCheckerBoard_h

#include "Compound.h"

class MarbleCheckerBoard : public Compound {

public:
   MarbleCheckerBoard();
   
   virtual void setHash(Hash* hash);

private:
   void build();
};

#endif
