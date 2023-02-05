#ifndef FRESNEL_H
#define FRESNEL_H


class Fresnel
{
public:
   virtual ~Fresnel() {}
   virtual float evaluate(float cosi) const = 0;
};

class FresnelDielectric : public Fresnel
{
public:
   FresnelDielectric(float ei, float et);
   float evaluate(float cosi) const;

private:
   float eta_i, eta_t;
};

class FresnelConductor : public Fresnel
{
public:
   FresnelConductor(float _eta, float _k);
   float evaluate(float cosi) const;

private:
   float eta, k;
};

#endif // FRESNEL_H
