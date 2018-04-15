#pragma once
//
// Created by islam on 08.04.18.
//

#include <functional>


class lazy_sequence_end_t: public std::exception
{
    const char * what() const noexcept override
    {
        return "lazy sequence end";
    }
};

class lazy_sequence_started_t: public std::exception
{
    const char * what() const noexcept override
    {
        return "lazy sequence already started. (begin should be called only once)";
    }
};

template <class T>
class lazy_sequence_t {
    std::function<T()> generator;
    bool               started;

public:
    class iterator_t {
        friend lazy_sequence_t;

        lazy_sequence_t &  sequence;
        T                  value;
        bool               is_end;

        iterator_t(lazy_sequence_t & sequence)
            : sequence(sequence)
        {
            ++(*this);
        }

        iterator_t(lazy_sequence_t & sequence, bool is_end)
            : sequence(sequence)
            , is_end(is_end)
        {}

    public:
        iterator_t & operator ++() {
            try {
                value = sequence.generator();
            } catch (const lazy_sequence_end_t &) {
                is_end = true;
            }
            return *this;
        }

        bool operator==(const iterator_t & other) const noexcept {
            if (this == &other) {
                return true;
            }

            return is_end && other.is_end;
        }

        bool operator!=(const iterator_t & other) const noexcept {
            return !((*this) == other);
        }

        T & operator * () noexcept {
            return value;
        }
    };

    lazy_sequence_t(std::function<T()> && generator)
        : generator(std::move(generator))
        , started(false)
    {}

    lazy_sequence_t(const lazy_sequence_t &) = default;
    lazy_sequence_t(lazy_sequence_t &&) = default;

    iterator_t begin() {
        if (started) {
            throw lazy_sequence_started_t();
        }
        started = true;
        return { *this };
    }

    iterator_t end() {
        return { *this, true };
    }
};

