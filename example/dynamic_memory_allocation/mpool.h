#pragma once
#include <cstddef>
#include <exception>

#include "nstd_exception.h"

// @@@ sample begin 0:0

class MPoolBadAlloc : public std::bad_alloc {  // Nstd::Exceptionの基底クラス
};
// @@@ sample end
// @@@ sample begin 1:0

class MPool {
public:
    explicit MPool(size_t max_size) : max_size_{max_size} {}

    void* Alloc(size_t size)
    {
        if (size > max_size_) {
            throw MAKE_EXCEPTION(MPoolBadAlloc, "MPF : memory size too big");
        }

        void* mem = alloc(size);

        if (mem == nullptr) {
            throw MAKE_EXCEPTION(MPoolBadAlloc, "MPF : out of memory");
        }

        return mem;
    }

    void* AllocNoExcept(size_t size) noexcept { return alloc(size); }

    void   Free(void* area) noexcept { free(area); }
    size_t GetSize() const noexcept { return get_size(); }  // メモリ最小単位
    size_t GetCount() const noexcept { return get_count(); }  // メモリ最小単位が何個取れるか
    size_t GetCountMin() const noexcept { return get_count_min(); }  // GetCount()の最小値
    bool   IsValid(void const* area) const noexcept { return is_valid(area); }

protected:
    ~MPool() = default;

private:
    size_t const max_size_;

    virtual void*  alloc(size_t size) noexcept               = 0;
    virtual void   free(void* area) noexcept                 = 0;
    virtual size_t get_size() const noexcept                 = 0;
    virtual size_t get_count() const noexcept                = 0;
    virtual size_t get_count_min() const noexcept            = 0;
    virtual bool   is_valid(void const* area) const noexcept = 0;
};
// @@@ sample end
