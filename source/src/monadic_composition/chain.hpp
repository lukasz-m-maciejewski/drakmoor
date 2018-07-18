#pragma once

#include <functional>
#include <optional>
#include <type_traits>

namespace mc
{
template <typename T1, typename F, typename T2 = std::invoke_result_t<F, T1>>
auto apply(std::optional<T1> m, F f)
    -> std::enable_if_t<std::is_invocable_r_v<T2, F, T1>, std::optional<T2>>
{
    if (m)
    {
        return f(*m);
    }
    else
    {
        return std::nullopt;
    }
}

template <typename T1, typename F, typename T2 = std::invoke_result_t<F, T1>>
auto operator>=(std::optional<T1> m, F f)
    -> std::enable_if_t<std::is_invocable_r_v<T2, F, T1>, std::optional<T2>>
{
    return mc::apply(m, f);
}


}
