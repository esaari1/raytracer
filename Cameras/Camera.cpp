#include "Camera.h"
#include "Parser/Parser.h"
#include "Film.h"
#include "Tracer/Tracer.h"
#include "Parser/Hash.h"
#include "Math/Maths.h"
#include <math.h>
#include <queue>
#include <fstream>
#include <pthread.h>
#include "Math/Matrix.h"
#include "Utility/SDL_Utility.h"

using namespace std;

queue<SamplerBounds> bounds;
pthread_mutex_t rectLock;

void* renderThread(void* arg) {
   Camera* c = (Camera*) arg;
   SamplerBounds b;

   while(true) {
      pthread_mutex_lock(&rectLock);
      if(bounds.empty()) {
         pthread_mutex_unlock(&rectLock);
         pthread_exit(NULL);
      }
      b = bounds.front();
      bounds.pop();
      pthread_mutex_unlock(&rectLock);

      c->renderBounds(b);
   }
   pthread_exit(NULL);
   return NULL;
}

Camera::Camera(string fname, int w, int h) : eye(), u(), v(), w(), 
   tracer(new Tracer()), surface(NULL), film(new Film(w, h)), width(w), height(h)
{
   pthread_mutex_init(&rectLock, NULL);
   threadCount = 1;
   
   std::ifstream fin(fname.c_str(), std::ios::in);
   Tokenizer tok(&fin);
   Parser parser(&tok);

   tok.nextToken();
   setHash(parser.readValue()->getHash());
}

Camera::~Camera() {
   delete tracer;
   delete film;
   pthread_mutex_destroy(&rectLock);
}

void Camera::setThreadParameters(int tc, int w, int h) {
   threadCount = tc;
   boxw = w;
   boxh = h;
}

void Camera::setHash(Hash* h) {
   eye.set(h->getValue("eye")->getArray());

   float angle = h->getDouble("angle") / 2.0;
   viewPlaneDistance = width * 0.5 / tan(angle * DEG_TO_RAD);

   samplerHash = h->getValue("sampler")->getHash();

   if(h->contains("bgTexture")) {
      Texture* tex = Texture::createTexture(h->getValue("bgTexture")->getHash());
      tracer->setBackgroundTexture(tex);
   }
   else if(h->contains("bgColor")) {
      tracer->setBackgroundColor(h->getValue("bgColor")->getArray());
   }
   
   if(h->contains("focusPlaneDistance")) {
      f = h->getDouble("focusPlaneDistance");
   }
   else {
      f = viewPlaneDistance;
   }
   fDivV = f / viewPlaneDistance;
   
   if(h->contains("lensRadius")) {
      lensRadius = h->getDouble("lensRadius");
   }
   else {
      lensRadius = 0.f;
   }

   computeUVW(h);
}

void Camera::render() {
   film->reset();
   for(int h = 0; h < height; h += boxh) {
      for(int w = 0; w < width; w += boxw) {
         bounds.push(SamplerBounds(w, w + boxw, h, h + boxh ));
      }
   }

   pthread_attr_t attr;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   
   Uint32 start = SDL_GetTicks();
   
   pthread_t tid[threadCount];
   for(int i = 0; i < threadCount; i++) {
      pthread_create(&tid[i], &attr, renderThread, (void *) this);
   }
   
   for(int i = 0; i < threadCount; i++) {
      pthread_join(tid[i], NULL);
   }
   
   pthread_attr_destroy(&attr);
   
   Uint32 end = SDL_GetTicks();
   printf("Render time = %f seconds\n", (end - start) / 1000.0);
   
   Matrix m(u, v, w);
   film->generateImage(surface, eye, viewPlaneDistance, m);
}

void Camera::renderBounds(const SamplerBounds& bounds) {
   Ray ray;
   Point2D lp;
   int nSamples;
   
   Sampler* sampler = Sampler::createSampler(bounds, samplerHash);
   Sample* samples = new Sample[sampler->getNumSamples()];
   
   while((nSamples = sampler->getSamples(samples)) > 0) {
      for(int i = 0; i < nSamples; i++) {
         double x = (samples[i].imageX - 0.5 * width) * fDivV;
         double y = (samples[i].imageY - 0.5 * height) * fDivV;
         
         Sampler::mapToDisk(samples[i].lensX, samples[i].lensY, &lp.x, &lp.y);
         lp *= lensRadius;
         ray.origin = eye + u * lp.x + v * lp.y;
         
         ray.direction = u * (x - lp.x) + v * (y - lp.y) - w * f;
         ray.direction.normalize();
         
         film->addSample(samples[i].imageX, samples[i].imageY, tracer->traceRay(ray));
      }
   }
}

void Camera::computeUVW(Hash* h) {
   if(h->contains("lookat")) {
      Point3D lookat(h->getValue("lookat")->getArray());
      Vector3D up(0, 1, 0);
      if(h->contains("up")) {
         up.set(h->getValue("up")->getArray());
      }

      if(eye.x == lookat.x && eye.z == lookat.z && eye.y > lookat.y) {
         u.set(1, 0, 0);
         v.set(0, 0, 1);
         w.set(0, 1, 0);
      }
      else {
         up.normalize();
         w = eye - lookat;
         w.normalize();
         u = up.cross(w);
         u.normalize();
         v = w.cross(u);
      }
   }
   else if(h->contains("rotate")) {
      Array* r = h->getValue("rotate")->getArray();
      setRotation(r->at(0)->getDouble(), r->at(1)->getDouble(), r->at(2)->getDouble());
   }
   else {
      fprintf(stderr, "Must specify either lookat or rotate in camera configuration.\n");
      exit(1);
   }
}

void Camera::translate(double x, double y, double z) {
   eye.x += x;
   eye.y += y;
   eye.z += z;
}

void Camera::setRotation(double x, double y, double z) {
   Matrix m;

   m.rotateX(-x);
   m.rotateY(-y);
   m.rotateZ(-z);

   m.getRow(u, 0);
   m.getRow(v, 1);
   m.getRow(w, 2);

   u.normalize();
   v.normalize();
   w.normalize();
}
