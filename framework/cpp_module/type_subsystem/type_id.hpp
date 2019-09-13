#pragma once

#include "user_type_registry.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>

#define widthof(TYPE) (sizeof(TYPE) * 8)
#define padding_width(TYPE, OCCUPIED_WIDTH) widthof(TYPE) - ((OCCUPIED_WIDTH) % widthof(TYPE))

namespace interop {

/**
 * @brief universal basic type identifier
 */
union type_id_t {
    enum class data_class_e : uint8_t {
        Undefined,
        Value,
        Reference,
        Size_c,
    };

    enum class type_class_e : uint8_t {
        Builtin,
        User,
        Size_c,
    };

    enum class builtin_type_e : uint8_t {
        None,
        Bool,
        Char,
        Short,
        Float,
        Double,
        Int,
        Long,
        LongLong,
        String,
        Array,
        Map,
        Size_c,
    };

  private:
    /* \/\/\/ data layout calculation \/\/\/ */
    static constexpr const auto data_class_width_c   = 2;
    static constexpr const auto type_class_width_c   = 1;
    static constexpr const auto builtin_type_width_c = 4;
    static constexpr const auto total_width_c        = widthof(uint32_t);
    static constexpr const auto reserved1_width_c =
        padding_width(uint8_t, data_class_width_c + type_class_width_c);
    static constexpr const auto reserved2_width_c = std::max(
        total_width_c - (data_class_width_c + type_class_width_c + reserved1_width_c), 0ul);
    static constexpr const auto reserved_width_c = reserved1_width_c + reserved2_width_c;

    static_assert(uint8_t(data_class_e::Size_c) <= (1 << data_class_width_c));
    static_assert(uint8_t(type_class_e::Size_c) <= (1 << type_class_width_c));
    static_assert(uint8_t(builtin_type_e::Size_c) <= (1 << builtin_type_width_c));
    static_assert(reserved2_width_c % 8 == 0);
    /* /\/\/\ data layout calculation /\/\/\ */

    uint32_t id;
    struct {
        uint8_t data_class: data_class_width_c;
        uint8_t type_class: type_class_width_c;

        uint8_t reserved1: reserved1_width_c;
        std::array<uint8_t, reserved2_width_c / 8> reserved2;
    } info;

    struct builtin_t {
        static constexpr auto reserved_width_c =
            widthof(uint32_t) - (data_class_width_c + type_class_width_c + builtin_type_width_c);

        uint8_t type: builtin_type_width_c;
        uint32_t reserved: reserved_width_c;
    };

    struct user_t {
        static constexpr auto reserved_width_c =
            widthof(uint32_t) -
            (data_class_width_c + type_class_width_c + widthof(user_type_index_t));

        uint16_t type;
        uint16_t reserved: reserved_width_c;
    };

    static_assert(sizeof(id) >= sizeof(info));

    inline uint32_t read_reserved() const
    {
        uint32_t reserved2 = 0;
        for (auto byte : info.reserved2) {
            reserved2 <<= 8;
            reserved2 |= byte;
        }

        return uint32_t{info.reserved1} << reserved2_width_c | reserved2;
    }

    inline builtin_t read_reserved_as_builtin() const
    {
        auto raw = read_reserved();

        return {static_cast<uint8_t>(raw >> builtin_t::reserved_width_c), raw};
    }

    inline user_t read_reserved_as_user() const
    {
        auto raw = read_reserved();

        return {static_cast<uint16_t>(raw >> user_t::reserved_width_c), static_cast<uint16_t>(raw)};
    }

    inline builtin_type_e builtin_type() const
    {
        assert(is_builtin());
        return static_cast<builtin_type_e>(read_reserved_as_builtin().type);
    }

    inline user_type_index_t user_type() const
    {
        assert(is_user());
        return static_cast<user_type_index_t>(read_reserved_as_user().type);
    }

    template <typename T>
    static constexpr data_class_e enumerate_data_class()
    {
        if (std::is_reference_v<T> || std::is_pointer_v<T>) {
            return data_class_e::Reference;
        }

        return data_class_e::Value;
    }

    template <typename T>
    static constexpr type_class_e enumerate_type_class()
    {
        return std::is_fundamental_v<T> ? type_class_e::Builtin : type_class_e::User;
    }

  public:
    inline explicit type_id_t(uint32_t id = 0) // Undefined; Builtin; None
      : id(id)
    {}

    inline type_id_t(type_class_e type_class, data_class_e data_class, builtin_type_e)
      : type_id_t()
    {
        info.type_class = static_cast<uint8_t>(type_class);
        info.data_class = static_cast<uint8_t>(data_class);

        assert(is_builtin());
    }

    template <typename T>
    inline type_id_t()
      : type_id_t()
    {
        info.data_class = enumerate_data_class<T>();
        info.type_class = enumerate_type_class<T>();
    }

    inline explicit operator uint32_t() const { return id; }
    inline operator bool() const { return id; }
    inline bool operator==(type_id_t rhs) const { return id == rhs.id; }
    inline bool operator!=(type_id_t rhs) const { return !(*this == rhs); }

    type_class_e type_class() const { return static_cast<type_class_e>(info.type_class); }
    data_class_e data_class() const { return static_cast<data_class_e>(info.data_class); }

    inline bool is_undefined() const { return !(*this); }
    inline bool is_builtin() const { return type_class() == type_class_e::Builtin; }
    inline bool is_user() const { return type_class() == type_class_e::User; }
    inline bool is_container() const
    {
        return is_builtin() &&
               (builtin_type() == builtin_type_e::Array || builtin_type() == builtin_type_e::Map);
    }
};

} // namespace interop
