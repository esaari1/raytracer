#ifndef raytracer_Gaussian_h
#define raytracer_Gaussian_h

#include <SDL/SDL.h>

double* CreateGaussianFilter(float piSigma, float piAlpha, int& lSize);
SDL_Surface* Convolute(SDL_Surface* source, double* filter, int fSize);

#endif
