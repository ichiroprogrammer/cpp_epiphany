#include <unwind.h>

#include <cstring>

#include "gtest_wrapper.h"

#include "mpool_fixed.h"

#ifndef __CYGWIN__  // この実装は、cygwinでは動作しない

// @@@ sample begin 0:0

// https://github.com/hjl-tools/gcc/blob/master/libstdc%2B%2B-v3/libsupc%2B%2B/unwind-cxx.h
// の抜粋
namespace __cxxabiv1 {
struct __cxa_exception {
    // @@@ ignore begin
#if defined(__LP64__) || defined(_WIN64) || defined(_LIBCXXABI_ARM_EHABI)
    // Now _Unwind_Exception is marked with __attribute__((aligned)),
    // which implies __cxa_exception is also aligned. Insert padding
    // in the beginning of the struct, rather than before unwindHeader.
    void* reserve;

    // This is a new field to support C++ 0x exception_ptr.
    // For binary compatibility it is at the start of this
    // struct which is prepended to the object thrown in
    // __cxa_allocate_exception.
    size_t referenceCount;
#endif

    //  Manage the exception object itself.
    std::type_info* exceptionType;
    void (*exceptionDestructor)(void*);
    std::unexpected_handler unexpectedHandler;
    std::terminate_handler  terminateHandler;

    __cxa_exception* nextException;

    int handlerCount;

#if defined(_LIBCXXABI_ARM_EHABI)
    __cxa_exception* nextPropagatingException;
    int              propagationCount;
#else
    int                  handlerSwitchValue;
    unsigned char const* actionRecord;
    unsigned char const* languageSpecificData;
    void*                catchTemp;
    void*                adjustedPtr;
#endif

#if !defined(__LP64__) && !defined(_WIN64) && !defined(_LIBCXXABI_ARM_EHABI)
    // This is a new field to support C++ 0x exception_ptr.
    // For binary compatibility it is placed where the compiler
    // previously adding padded to 64-bit align unwindHeader.
    size_t referenceCount;
#endif
    _Unwind_Exception unwindHeader;
    // @@@ ignore end
};
}  // namespace __cxxabiv1

namespace {

constexpr size_t             offset{sizeof(__cxxabiv1::__cxa_exception)};
MPoolFixed<offset + 128, 50> mpf_exception;
}  // namespace

extern "C" {

void* __cxa_allocate_exception(size_t thrown_size)
{
    auto alloc_size = thrown_size + offset;  // メモリの実際の必要量はthrown_size+offset
    auto mem        = mpf_exception.AllocNoExcept(alloc_size);

    assert(mem != nullptr);

    memset(mem, 0, alloc_size);
    auto* ret = static_cast<uint8_t*>(mem);

    ret += offset;

    return ret;
}

void __cxa_free_exception(void* thrown_exception)
{
    auto* ret = static_cast<uint8_t*>(thrown_exception);

    ret -= offset;
    mpf_exception.Free(ret);
}
// @@@ sample end
}

namespace {
TEST(NewOverload, exception_allocator)
{
    // @@@ sample begin 1:0

    auto count             = mpf_exception.GetCount();
    auto exception_occured = false;

    try {
        throw std::exception{};
    }
    catch (std::exception const& e) {
        ASSERT_EQ(count - 1, mpf_exception.GetCount());  // 1個消費
        exception_occured = true;
    }

    ASSERT_TRUE(exception_occured);
    ASSERT_EQ(count, mpf_exception.GetCount());  // 1個解放
    // @@@ sample end
}
}  // namespace
#endif
