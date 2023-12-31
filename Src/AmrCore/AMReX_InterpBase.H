#ifndef AMREX_INTERP_BASE_H_
#define AMREX_INTERP_BASE_H_
#include <AMReX_Config.H>

#include <AMReX_BCRec.H>
#include <AMReX_Box.H>
#include <AMReX_Vector.H>

namespace amrex {

class InterpBase;

class InterpolaterBoxCoarsener final
    : public BoxConverter
{
public:
    InterpolaterBoxCoarsener (InterpBase* mapper_, const IntVect& ratio_);
    virtual ~InterpolaterBoxCoarsener ();
    virtual Box doit (const Box& fine) const;
    virtual BoxConverter* clone () const;
private:
    InterpBase* mapper;
    IntVect ratio;
};

class InterpBase
{
public:
    virtual ~InterpBase () = default;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    * This is a pure virtual function and hence MUST
    * be implemented by derived classes.
    *
    * \param fine
    * \param ratio
    */
    virtual Box CoarseBox (const Box& fine, int ratio) = 0;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    * This is a pure virtual function and hence MUST
    * be implemented by derived classes.
    *
    * \param fine
    * \param ratio
    */
    virtual Box CoarseBox (const Box& fine, const IntVect& ratio) = 0;

    InterpolaterBoxCoarsener BoxCoarsener (const IntVect& ratio);

    static Vector<int> GetBCArray (const Vector<BCRec>& bcr);
};

}

#endif
