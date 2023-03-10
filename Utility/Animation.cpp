#include "Animation.h"
#include "Parser/Parser.h"
#include <iostream>
#include <fstream>
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Geometry/Instance.h"
#include "Math/Maths.h"
#include "Cameras/Camera.h"
#include "Utility/SDL_Utility.h"
#include "Storage/Storage.h"
#include <limits>

typedef vector<FrameObject*>::const_iterator FrameObjectIter;

FrameObject::FrameObject(Instance* i) : instance(i), position(), rotation() {
}

void FrameObject::setup() {
   instance->reset();
   instance->rotate(rotation.x, rotation.y, rotation.z);
   instance->translate(position.x, position.y, position.z);
   instance->computeBBox();
}

Animation::Animation(Camera* c, SDL_Surface* s) : camera(c), surface(s), objFrames(), cameraFrames(), outputDir(""), startFrame(0), endFrame(numeric_limits<int>::max()) {
}

Animation::~Animation() {
}

void Animation::setup(const string& fname) {
   ifstream fp(fname.c_str());
   Tokenizer tok(&fp);
   Parser parser(&tok);

   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }

      string name = tok.getStringValue();
      Hash* hash = parser.readValue()->getHash();

      if(name == "config") {
         outputDir = hash->getString("outputDir");
         startFrame = hash->getInteger("startFrame", 0);
         endFrame = hash->getInteger("endFrame", numeric_limits<int>::max());
      }
      else if(name == "animation") {
         loadAnimation(hash);
      }
   }
}

void Animation::loadAnimation(Hash* hash) {
   string objName = hash->getString("name");
   if(objName == "camera") {
      loadCameraFrames(hash->getValue("frames")->getArray());
   }
   else {
      GeometryObject* obj = GeometryManager::instance().getStorage()->getObject(objName);
      GeometryManager::instance().getStorage()->removeObject(obj);

      Instance* instance = new Instance(obj);
      GeometryManager::instance().getStorage()->addObject(instance);
      loadAnimationFrames(hash->getValue("frames")->getArray(), instance);
   }

   if(endFrame == numeric_limits<int>::max()) {
      endFrame = max(cameraFrames.size(), objFrames.size());
   }
}

void Animation::loadAnimationFrames(Array* frames, Instance* instance) {
   if(objFrames.empty()) {
      objFrames.reserve(frames->size());
      for(unsigned i = 0; i < frames->size(); i++) {
         objFrames.push_back(new Frame());
      }
   }

   int i = 0;
   for(ValueIter it = frames->begin(); it != frames->end(); ++it) {
      Hash* frame = (*it)->getHash();
      FrameObject* obj = new FrameObject(instance);
      obj->position.set(frame->getValue("position")->getArray());
      obj->rotation.set(frame->getValue("rotation")->getArray());
      objFrames[i++]->objects.push_back(obj);
   }
}

void Animation::loadCameraFrames(Array* frames) {
   for(ValueIter it = frames->begin(); it != frames->end(); ++it) {
      Hash* frame = (*it)->getHash();
      CameraFrame* cf = new CameraFrame();
      cf->position.set(frame->getValue("position")->getArray());
      cf->rotation.set(frame->getValue("rotation")->getArray());
      cameraFrames.push_back(cf);
   }
}

void Animation::play() {
   char fname[512];

   for(size_t i = startFrame; i < endFrame; i++) {
      if(cameraFrames.size() > i) {
         camera->setRotation(cameraFrames[i]->rotation.x, cameraFrames[i]->rotation.y, cameraFrames[i]->rotation.z);
         camera->setPosition(cameraFrames[i]->position);
      }

      if(objFrames.size() > i) {
         for(FrameObjectIter it = objFrames[i]->objects.begin(); it != objFrames[i]->objects.end(); ++it) {
            (*it)->setup();
         }
      }

      GeometryManager::instance().getStorage()->setup();
      printf("Frame %lu\t", i);
      camera->render();
      sprintf(fname, "%s/image%05lu.png", outputDir.c_str(), i);
      saveImage(surface, fname);
   }

   printf("Animation done\n");
}
