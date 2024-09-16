#pragma once

#include <functional>
#include <memory>
#include <type_traits>

namespace cpputil::inline v0::fn {

    template <bool const_invoke, bool nothrow_invoke, class R, class... Args>
    class Basic_function_ref {
        template <class T>
        using Ptr = std::conditional_t<const_invoke, T const, T>*;

        union Ref {
            Ptr<void> m_ptr;
            R (*m_fp)(Args...) noexcept(nothrow_invoke);
        };

        using Thunk = R(Ref, Args...) noexcept(nothrow_invoke);

        Ref    m_ref;
        Thunk* m_thunk;
    public:
        constexpr Basic_function_ref(R (&f)(Args...) noexcept(nothrow_invoke))
            : m_ref { .m_fp = f }
            , m_thunk { [](Ref const ref, Args... args) noexcept(nothrow_invoke) {
                return ref.m_fp(std::forward<Args>(args)...);
            } }
        {}

        template <std::invocable<Args...> Object>
        constexpr Basic_function_ref(Object& object) noexcept
            : m_ref { .m_ptr = std::addressof(object) }
            , m_thunk { [](Ref const ref, Args... args) noexcept(nothrow_invoke) {
                return std::invoke(
                    *static_cast<Ptr<Object>>(ref.m_ptr), std::forward<Args>(args)...);
            } }
        {}

        constexpr auto operator()(Args... args) noexcept(nothrow_invoke) -> R
            requires(!const_invoke)
        {
            return m_thunk(m_ref, std::forward<Args>(args)...);
        }

        constexpr auto operator()(Args... args) const noexcept(nothrow_invoke) -> R
            requires const_invoke
        {
            return m_thunk(m_ref, std::forward<Args>(args)...);
        }
    };

    template <class>
    struct Make_function_ref {
        static_assert(false, "Function_ref instantiated with a non-function type argument");
    };

    template <class R, class... Args>
    struct Make_function_ref<R(Args...)>
        : std::type_identity<Basic_function_ref<false, false, R, Args...>> {};

    template <class R, class... Args>
    struct Make_function_ref<R(Args...) noexcept>
        : std::type_identity<Basic_function_ref<false, true, R, Args...>> {};

    template <class R, class... Args>
    struct Make_function_ref<R(Args...) const>
        : std::type_identity<Basic_function_ref<true, false, R, Args...>> {};

    template <class R, class... Args>
    struct Make_function_ref<R(Args...) const noexcept>
        : std::type_identity<Basic_function_ref<true, true, R, Args...>> {};

    template <class F>
    using Function_ref = Make_function_ref<F>::type;

} // namespace cpputil::inline v0::fn
