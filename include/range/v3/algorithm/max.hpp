/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_ALGORITHM_MAX_HPP
#define RANGES_V3_ALGORITHM_MAX_HPP

#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-algorithms
        /// @{
        struct max_fn
        {
        private:
            template<typename T, typename C, typename P>
            constexpr const T& max2_impl(const T &a, const T &b, C&& pred, P&& proj) const
            {
                return !pred(proj(a), proj(b)) ? a : b;
            }

        public:
            template<typename Rng, typename C = ordered_less, typename P = ident,
                typename I = range_iterator_t<Rng>, typename V = iterator_value_t<I>,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                CONCEPT_REQUIRES_(InputRange<Rng>::value && Copyable<V>::value &&
                    IndirectCallableRelation<C, Project<I, P>>::value)>
#else
                CONCEPT_REQUIRES_(InputRange<Rng>() && Copyable<V>() &&
                    IndirectCallableRelation<C, Project<I, P>>())>
#endif
            RANGES_CXX14_CONSTEXPR V operator()(Rng &&rng, C pred_ = C{}, P proj_ = P{}) const
            {
                auto && pred = as_function(pred_);
                auto && proj = as_function(proj_);
                auto begin = ranges::begin(rng);
                auto end = ranges::end(rng);
                RANGES_ASSERT(begin != end);
                V result = *begin;
                while(++begin != end)
                {
                    auto && tmp = *begin;
                    if(pred(proj(result), proj(tmp)))
                        result = (decltype(tmp) &&) tmp;
                }
                return result;
            }

            template<typename T, typename C = ordered_less, typename P = ident,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                CONCEPT_REQUIRES_(
                    IndirectCallableRelation<C, Project<const T *, P>>::value)>
#else
                CONCEPT_REQUIRES_(
                    IndirectCallableRelation<C, Project<const T *, P>>())>
#endif
            constexpr const T& operator()(const T &a, const T &b, C pred = C{}, P proj = P{}) const
            {
                return max2_impl(a, b, as_function(pred), as_function(proj));
            }
        };

        /// \sa `max_fn`
        /// \ingroup group-algorithms
        namespace
        {
            constexpr auto&& max = static_const<with_braced_init_args<max_fn>>::value;
        }

        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
