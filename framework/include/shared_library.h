#pragma once
//
// Created by islam on 06.05.17.
//

#include "definitions.h"

#include <type_traits.hpp>

#include <string>

namespace interop {
/**
 * @class shared_library_t
 * @brief encapsulates shared library, manages its lifetime
 * @attention non default - constructable; non copyable; movable
 */
class shared_library_t {
    library_handle handle;
    std::string library_name;

  public:
    explicit shared_library_t(const std::string_view & path, const std::string_view & name = "");

    shared_library_t(const shared_library_t &) = delete;

    shared_library_t(shared_library_t &&) noexcept;

    ~shared_library_t();

    library_symbol_handle symbol(const std::string_view & name) const;

    template <typename T>
    T symbol_as(const std::string_view & name) const
    {
        if constexpr (is_function_pointer<T>) {
            return reinterpret_cast<T>(symbol(name));
        } else {
            return *reinterpret_cast<typename std::remove_reference<T>::type *>(symbol(name));
        }
    }

    const std::string & name() const;

    void unload();

    operator bool() const noexcept;
    bool operator!() const noexcept;

  private:
    void reset() noexcept;
};
} // namespace interop
