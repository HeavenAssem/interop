#pragma once
//
// Created by islam on 08.04.18.
//

#include <functional>
#include <optional>

namespace interop {
class lazy_sequence_end_t: public std::exception {
    const char * what() const noexcept override { return "lazy sequence end"; }
};

class lazy_sequence_started_t: public std::exception {
    const char * what() const noexcept override
    {
        return "lazy sequence already started. (begin should be called only once)";
    }
};

template <class T>
class lazy_sequence_t {
  public:
    using value_t = std::optional<T>;

  private:
    std::function<value_t()> generator;
    bool started;

  public:
    class iterator_t {
        friend lazy_sequence_t;

        lazy_sequence_t & sequence;
        value_t value;

        iterator_t(lazy_sequence_t & sequence, value_t && value)
          : sequence(sequence)
          , value(value)
        {}

      public:
        iterator_t & operator++()
        {
            if (auto next = sequence.generator()) {
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

    template <typename Fn>
    lazy_sequence_t(Fn && generator)
      : generator(std::move(generator))
      , started(false)
    {}

    lazy_sequence_t(const lazy_sequence_t &) = default;
    lazy_sequence_t(lazy_sequence_t &&)      = default;

    iterator_t begin()
    {
        if (started) {
            throw lazy_sequence_started_t();
        }
        started = true;
        return {*this, generator()};
    }

    iterator_t end() { return {*this, std::nullopt}; }
};
} // namespace interop
