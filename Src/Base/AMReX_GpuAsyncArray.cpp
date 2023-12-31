#include <AMReX_GpuAsyncArray.H>

#ifdef AMREX_USE_GPU

#if !defined(AMREX_USE_DPCPP)
extern "C" {
#if defined(AMREX_USE_HIP)
    void amrex_asyncarray_delete ( hipStream_t /*stream*/,  hipError_t /*error*/, void* p)
#elif defined(AMREX_USE_CUDA)
    void CUDART_CB amrex_asyncarray_delete (void* p)
#endif
    {
        void** pp = (void**)p;
        void* dp = pp[0];
        void* hp = pp[1];
        std::free(p);
        amrex::The_Arena()->free(dp);
        amrex::The_Pinned_Arena()->free(hp);
    }
}
#endif

#endif
