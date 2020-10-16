#pragma once

namespace Simthetiq
{
namespace Core
{
    #ifndef UNUSED
    #define UNUSED(x) (void) x
    #endif

    // remove warning if function not used
    #if defined(__clang__)
    #pragma clang diagnostic ignored "-Wunused-function"

    #elif defined( _MSC_VER )
    #pragma warning(disable : 4505)

    #elif defined(__GNUC__)
    #pragma GCC diagnostic ignored "-Wunused-function"
    
    #endif
}
}