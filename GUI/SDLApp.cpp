#include "SDLApp.h"
#include "Math/Ray.h"
#include "Cameras/Camera.h"
#include "Parser/Parser.h"
#include "Lights/LightManager.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/Mesh/MeshManager.h"
#include "Overlays/OverlayManager.h"
#include "Utility/Animation.h"
#include <math.h>
#include <fstream>
#include <SDL/SDL_image.h>
#include "Utility/SDL_Utility.h"

SDLApp::SDLApp(int argc, char** argv) :stopApp(false), surface(NULL), camera(NULL), animation(NULL) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

   IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

   loadConfiguration(argc, argv);
}

SDLApp::~SDLApp() {
   delete camera;
   SDL_FreeSurface(surface);

   if(animation != NULL) {
      delete animation;
   }

   IMG_Quit();
   SDL_Quit();
}

void SDLApp::loadConfiguration(int argc, char** argv) {
   const char* config_file = "config/config.txt";
   if (argc > 1) {
      config_file = argv[1];
   }
   std::ifstream fin(config_file, std::ios::in);

   Tokenizer tok(&fin);
   Parser parser(&tok);

   tok.nextToken();
   Hash *h = parser.readValue()->getHash();
   int width = h->getInteger("width");
   int height = h->getInteger("height");

   int threadCount = h->getInteger("threads");
   int boxw = h->getInteger("boxWidth");
   int boxh = h->getInteger("boxHeight");

   surface = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
   if (surface == NULL) {
      fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
      SDL_Quit();
      exit(2);
   }

   if(h->contains("mesh")) {
      MeshManager::instance().loadMeshes(h->getString("mesh"));
   }

   if(h->contains("storage")) {
      GeometryManager::instance().setStorageConfig(h->getValue("storage")->getHash());
   }

   LightManager::instance().loadLights(h->getString("lights"));
   GeometryManager::instance().loadObjects(h->getString("objects"));

   if(h->contains("overlays")) {
      OverlayManager::instance().loadOverlays(h->getString("overlays"));
   }

   camera = new Camera(h->getString("camera"), width, height);
   camera->setSurface(surface);
   camera->setThreadParameters(threadCount, boxw, boxh);

   if(h->contains("animation")) {
      animation = new Animation(camera, surface);
      animation->setup(h->getString("animation"));
   }
}

void SDLApp::run() {
   SDL_Event event;

   if(animation != NULL) {
      animation->play();
   }
   else {
      camera->render();
   }

   while(!stopApp) {
      SDL_WaitEvent(&event);
      switch (event.type) {
         case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE) {
               stopApp = true;
            }
            else if(event.key.keysym.sym == 's') {
               saveImage(surface, "image.png");
            }
            break;

         case SDL_QUIT:
            stopApp = true;
            break;
      }
   }
}
