#include "Riostream.h"

#include "MyCB.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "TMath.h"


// 下面两个构造函数就是简单地把参数传递给父类RooAbsPdf的构造函数
ClassImp(MyCB)
MyCB::MyCB(const char *name, const char *title,
           RooAbsReal& _x,
           RooAbsReal& _mean,
           RooAbsReal& _sigma,
           RooAbsReal& _al,
           RooAbsReal& _nl,
           RooAbsReal& _ar,
           RooAbsReal& _nr) :
    RooAbsPdf(name, title),
    x("x", "x", this, _x),
    mean("mean", "mean", this, _mean),
    sigma("sigma", "sigma", this, _sigma),
    al("al", "al", this, _al),
    nl("nl", "nl", this, _nl),
    ar("ar", "ar", this, _ar),
    nr("nr", "nr", this, _nr)
{
}

MyCB::MyCB(const MyCB& other, const char* name) :
    RooAbsPdf(other, name),
    x("x", this, other.x),
    mean("mean", this, other.mean),
    sigma("sigma", this, other.sigma),
    al("al", this, other.al),
    nl("nl", this, other.nl),
    ar("ar", this, other.ar),
    nr("nr", this, other.nr)
{
}

Double_t MyCB::evaluate() const
// 这个函数是公式的具体表达形式
// 也就是所谓的水晶球函数
{
    if (x < mean - al * sigma)
    {
        return exp(-al * al * 0.5) \
            * pow(nl / al, nl) \
            * pow(nl / al - al - (x - mean) / sigma, -nl);
    }
    else if(x > mean + ar * sigma)
    {
        return exp(-ar * ar * 0.5) \
            * pow(nr / ar, nr) \
            * pow(nr / ar - ar + (x - mean) / sigma, -nr);
    }
    else
    {
        return exp(-(x - mean) * (x - mean) / (2 * sigma * sigma));
    }
}