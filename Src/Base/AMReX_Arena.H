#ifndef AMREX_ARENA_H_
#define AMREX_ARENA_H_
#include <AMReX_Config.H>

#include <AMReX_BLassert.H>
#include <AMReX_INT.H>
#include <cstddef>
#include <cstdlib>
#include <limits>

namespace amrex {

/**
 * \brief Given a minimum required size of size bytes, this returns
 * the next largest arena size that will align to align_requirement bytes
 */
inline std::size_t aligned_size (std::size_t align_requirement, std::size_t size) noexcept
{
    return ((size + (align_requirement-1)) / align_requirement) * align_requirement;
}

inline bool is_aligned (const void* p, std::size_t alignment) noexcept
{
    return (reinterpret_cast<std::size_t>(p) % alignment) == 0;
}

class Arena;

Arena* The_Arena ();
Arena* The_Async_Arena ();
Arena* The_Device_Arena ();
Arena* The_Managed_Arena ();
Arena* The_Pinned_Arena ();
Arena* The_Cpu_Arena ();

struct ArenaInfo
{
    Long release_threshold = std::numeric_limits<Long>::max();
    bool use_cpu_memory = false;
    bool device_use_managed_memory = true;
    bool device_set_readonly = false;
    bool device_set_preferred = false;
    bool device_use_hostalloc = false;
    ArenaInfo& SetReleaseThreshold (Long rt) noexcept {
        release_threshold = rt;
        return *this;
    }
    ArenaInfo& SetDeviceMemory () noexcept {
        device_use_managed_memory = false;
        device_use_hostalloc = false;
        return *this;
    }
    ArenaInfo& SetReadOnly () noexcept {
        BL_ASSERT(device_use_managed_memory);
        device_set_readonly = true;
        return *this;
    }
    ArenaInfo& SetPreferred () noexcept {
        BL_ASSERT(device_use_managed_memory);
        device_set_preferred = true;
        return *this;
    }
    ArenaInfo& SetHostAlloc () noexcept {
        device_use_hostalloc = true;
        device_use_managed_memory = false;
        return *this;
    }
    ArenaInfo& SetCpuMemory () noexcept {
        use_cpu_memory = true;
        device_use_managed_memory = false;
        device_set_readonly = false;
        device_set_preferred = false;
        device_use_hostalloc = false;
        return *this;
    }
};

/**
* \brief
* A virtual base class for objects that manage their own dynamic
* memory allocation.
*/

class Arena
{
public:

    virtual ~Arena ();
    /**
    * Allocate a dynamic memory arena
    * \param sz size of the memory request
    * \return a pointer to the allocated memory
    */
    virtual void* alloc (std::size_t sz) = 0;
    /**
    * \brief A pure virtual function for deleting the arena pointed to by pt
    */
    virtual void free (void* pt) = 0;

    /**
    * \brief Free unused memory back to the system.  Return value is the
    * amount memory freed.
    */
    virtual std::size_t freeUnused () { return 0; }

    // isDeviceAccessible and isHostAccessible can both be true.
    virtual bool isDeviceAccessible () const;
    virtual bool isHostAccessible () const;

    // Note that isManaged, isDevice and isPinned are mutually exclusive.
    // For memory allocated by cudaMalloc* etc., one of them returns true.
    // Otherwise, neither is true.
    virtual bool isManaged () const;
    virtual bool isDevice () const;
    virtual bool isPinned () const;

    /**
     * \brief Does the device have enough free memory for allocating this
     * much memory?  For CPU builds, this always return true.  This is not a
     * const funciton because it may attempt to release memory back to the
     * system.
     */
    virtual bool hasFreeDeviceMemory (std::size_t sz);

    /**
    * \brief Given a minimum required arena size of sz bytes, this returns
    * the next largest arena size that will align to align_size bytes
    */
    static std::size_t align (std::size_t sz);

    static void Initialize ();
    static void PrintUsage ();
    static void PrintUsageToFiles (std::string const& filename, std::string const& message);
    static void Finalize ();

#if 0
    union Word
    {
        void*  p;
        long long ll;
        long double ld;
        void (*f) ();
    };
    static const std::size_t align_size = sizeof(Word);
#endif

    static const std::size_t align_size = 16;

    /**
     *  \brief Return the ArenaInfo object for querying
     */
    const ArenaInfo& arenaInfo () const { return arena_info; }

protected:

    ArenaInfo arena_info;

    virtual std::size_t freeUnused_protected () { return 0; }
    void* allocate_system (std::size_t nbytes);
    void deallocate_system (void* p, std::size_t nbytes);
};

}

#endif /*BL_ARENA_H*/
