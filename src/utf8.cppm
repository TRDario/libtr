export module tr:utf8;

import std;
import :integer;

export namespace tr {
    // Forward iterator for basic unicode handling.
    class Utf8ConstIt {
    public:
        using value_type = Ui32;
        using difference_type = std::ptrdiff_t;

        constexpr Utf8ConstIt() noexcept = default;
        constexpr Utf8ConstIt(const char* ptr) noexcept : _impl { ptr } {}

        constexpr friend auto operator<=>(const Utf8ConstIt&, const Utf8ConstIt&) = default;
        constexpr friend bool operator==(const Utf8ConstIt&, const Utf8ConstIt&) = default;

        constexpr Ui32 operator*() const noexcept {
            if (Ui8(*_impl) < 0x80) {
                return *_impl;
            }
            else if (Ui8(*_impl) < 0xE0) {
                return ((_impl[0] & 0x1F) << 6) + (_impl[1] & 0x3F);
            }
            else if (Ui8(*_impl) < 0xF0) {
                return ((_impl[0] & 0xF) << 12) + ((_impl[1] & 0x3F) << 6) + (_impl[2] & 0x3F);
            }
            else {
                return ((_impl[0] & 0x7) << 18) + ((_impl[1] & 0x3F) << 12) + ((_impl[2] & 0x3F) << 6) + + (_impl[3] & 0x3F);
            }
        }

        constexpr Utf8ConstIt& operator++() noexcept {
            if (Ui8(*_impl) < 0x80) {
                ++_impl;
            }
            else if (Ui8(*_impl) < 0xE0) {
                _impl += 2;
            }
            else if (Ui8(*_impl) < 0xF0) {
                _impl += 3;
            }
            else {
                _impl += 4;
            }
            return *this;
        }
        Utf8ConstIt operator++(int) noexcept {
            auto prev { *this };
            ++(*this);
            return prev;
        }
    private:
        const char* _impl;
    };
    constexpr Utf8ConstIt utf8Begin(std::string_view str) noexcept {
        return str.data();
    }
    constexpr Utf8ConstIt utf8End(std::string_view str) noexcept {
        return str.data() + str.size();
    }
    constexpr std::ranges::subrange<Utf8ConstIt> utf8Range(std::string_view str) noexcept {
        return { utf8Begin(str), utf8End(str) };
    }
    constexpr Size utf8Length(std::string_view str) noexcept {
        return std::distance(utf8Begin(str), utf8End(str));
    }
}