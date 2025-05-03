#ifndef __MYCB__
#define __MYCB__

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

class MyCB : public RooAbsPdf {
public:
    MyCB() {};
    MyCB(
        const char *name,
        const char *title,
        RooAbsReal& _x,
        RooAbsReal& _mean,
        RooAbsReal& _sigma,
        RooAbsReal& _al,
        RooAbsReal& _nl,
        RooAbsReal& _ar,
        RooAbsReal& _nr
    );
    MyCB(const MyCB& other, const char* name=0);

    // 下面这个clone函数在C++里面很常见把
    virtual TObject* clone(const char* newname) const { return new MyCB(*this, newname); }
    virtual ~MyCB() {}

protected:
    RooRealProxy x; 
    RooRealProxy mean;
    RooRealProxy sigma;
    RooRealProxy al;
    RooRealProxy nl;
    RooRealProxy ar;
    RooRealProxy nr;

    Double_t evaluate() const;

private:
    ClassDef(MyCB, 1) // MyCB
    // 这个ClassDef是ROOT的一个宏，主要用于ROOT的反射机制
};

#endif
