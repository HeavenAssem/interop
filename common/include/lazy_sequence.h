#pragma once
//
// Created by islam on 08.04.18.
//

#include "exceptions.h"

#include <functional>
#include <optional>

namespace interop {
namespace details {

template <typename T>
struct lazy_sequence_value_t {
    using type = std::optional<T>;
};

template <typename T>
struct lazy_sequence_value_t<T &> {
    using type = std::optional<std::reference_wrapper<T>>;
};

template <typename T>
struct value_in_lazy_sequence_t {
    template <typename U>
    struct remove_wrapper {
        using type = U;
    };

    template <typename U>
    struct remove_wrapper<std::reference_wrapper<U>> {
        using type = U &;
    };

    using type = typename remove_wrapper<typename T::value_type>::type;
};

} // namespace details

template <typename T>
using lazy_sequence_value_t = typename details::lazy_sequence_value_t<T>::type;

template <typename T>
using value_in_lazy_sequence_t = typename details::value_in_lazy_sequence_t<T>::type;

struct lazy_sequence_started_t: public error_t {
    lazy_sequence_started_t()
      : error_t("lazy sequence already started. (begin should be called only once)")
    {}
};

template <class T>
class lazy_sequence_t {
  public:
    using value_t     = lazy_sequence_value_t<T>;
    using generator_t = std::function<value_t()>;
    using predicate_t = std::function<bool(const T &)>;

  private:
    generator_t generator;
    std::vector<predicate_t> filters;
    bool started;

    value_t next()
    {
        while (value_t generated = generator()) {
            if (std::all_of(filters.begin(), filters.end(),
                            [&generated](const predicate_t & satisfies) {
                                return satisfies(static_cast<const T &>(*generated));
                            })) {
                return generated;
            }
        }

        return std::nullopt;
    }

  public:
    class iterator_t {
        friend lazy_sequence_t;

        lazy_sequence_t & sequence;
        value_t value;

        iterator_t(lazy_sequence_t & sequence, value_t value)
          : sequence(sequence)
          , value(std::move(value))
        {}

      public:
        iterator_t & operator++()
        {
            if (auto next = sequence.next()) {
                value.emplace(std::move(*next));
            } else {
                value.reset();
            }
            return *this;
        }

        bool operator==(const iterator_t & other) const noexcept
        {
            return &sequence == &other.sequence && (value.has_value() == other.value.has_value());
        }

        bool operator!=(const iterator_t & other) const noexcept { return !((*this) == other); }

        T & operator*() noexcept { return *value; }
    };

    template <typename Generator>
    lazy_sequence_t(Generator && generator)
      : generator(std::move(generator))
      , started(false)
    {}

    lazy_sequence_t(const lazy_sequence_t &) = default;
    lazy_sequence_t(lazy_sequence_t &&)      = default;

    template <typename Predicate>
    lazy_sequence_t filter(Predicate && predicate)
    {
        filters.push_back(predicate);
        return std::move(*this);
    }

    template <typename Thunk>
    auto transformed(Thunk && thunk) const
    {
        using transformed_value_t = std::result_of_t<Thunk(T &&)>;
        using transformed_t       = value_in_lazy_sequence_t<transformed_value_t>;

        return lazy_sequence_t<transformed_t>{
            [parent = *this, thunk = std::forward<Thunk>(thunk)]() mutable -> transformed_value_t {
                while (auto parent_value = parent.next()) {
                    if (auto transformed_value = thunk(std::move(*parent_value))) {
                        return transformed_value;
                    }
                }
                return std::nullopt;
            }};
    }

    iterator_t begin()
    {
        if (started) {
            throw lazy_sequence_started_t();
        }
        started = true;
        return {*this, next()};
    }

    iterator_t end() { return {*this, std::nullopt}; }
};
} // namespace interop
