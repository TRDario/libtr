module;
#include <cassert>

export module tr:integer;

import std;
import :concepts;

export namespace tr {
    using Ui8  = std::uint8_t;
    using Ui16 = std::uint16_t;
    using Ui32 = std::uint32_t;
    using Ui64 = std::uint64_t;
    using Size = std::size_t;
    using Si8  = std::int8_t;
    using Si16 = std::int16_t;
    using Si32 = std::int32_t;
    using Si64 = std::int64_t;
    using PtrDiff = std::ptrdiff_t;
    using Byte = std::byte;

    template <std::integral T>
    consteval T checkedIntegerLiteral(Ui64 i) noexcept
    {
        assert(i < std::numeric_limits<T>::max());
        return T(i);
    }
    consteval Ui8  operator ""_u8 (unsigned long long i) noexcept { return checkedIntegerLiteral<Ui8>(i); }
    consteval Ui16 operator ""_u16(unsigned long long i) noexcept { return checkedIntegerLiteral<Ui16>(i); }
    consteval Ui32 operator ""_u32(unsigned long long i) noexcept { return checkedIntegerLiteral<Ui32>(i); }
    consteval Ui64 operator ""_u64(unsigned long long i) noexcept { return i; }
    consteval Si8  operator ""_i8 (unsigned long long i) noexcept { return checkedIntegerLiteral<Si8>(i); }
    consteval Si16 operator ""_i16(unsigned long long i) noexcept { return checkedIntegerLiteral<Si16>(i); }
    consteval Si32 operator ""_i32(unsigned long long i) noexcept { return checkedIntegerLiteral<Si32>(i); }
    consteval Si64 operator ""_i64(unsigned long long i) noexcept { return checkedIntegerLiteral<Si64>(i); }
}