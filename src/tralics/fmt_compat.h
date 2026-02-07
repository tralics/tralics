#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <sstream>

#if FMT_VERSION < 80000
namespace fmt {
    template <typename T>
    struct streamed_view {
        const T &value;
    };

    template <typename T>
    auto streamed(const T &value) -> streamed_view<T> {
        return streamed_view<T>{value};
    }

    template <typename T, typename Char>
    struct formatter<streamed_view<T>, Char> : formatter<basic_string_view<Char>> {
        template <typename FormatContext>
        auto format(const streamed_view<T> &view, FormatContext &ctx) -> decltype(ctx.out()) {
            std::basic_ostringstream<Char> os;
            os << view.value;
            auto s = os.str();
            auto &base = static_cast<formatter<basic_string_view<Char>> &>(*this);
            return base.format(basic_string_view<Char>(s), ctx);
        }
    };
} // namespace fmt
#endif
