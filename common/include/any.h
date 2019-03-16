#pragma once

#include "exceptions.h"
#include "utils/typing.h"

#include <cassert>
#include <cstring>
#include <limits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace interop {
namespace details {
struct i_type_wrapper_t {
    template <typename T>
    inline bool is_same() const
    {
        return is_same(typeid(T));
    }

    virtual bool is_same(const std::type_info & type) const                = 0;
    virtual const std::type_info & get_type() const                        = 0;
    virtual std::string get_name() const                                   = 0;
    virtual size_t get_size() const                                        = 0;
    virtual void copy(void * memory_to, const void * memory_from) const    = 0;
    virtual void move(void * memory_to, void * memory_from) const noexcept = 0;
    virtual void assign(void * memory_to, const void * memory_from) const  = 0;
    virtual void move_assign(void * memory_to, void * memory_from) const   = 0;
    virtual void destroy(void * memory) const noexcept                     = 0;
    virtual ~i_type_wrapper_t()                                            = default;
};

template <class T, class Interface>
struct type_wrapper_t: public Interface {
    bool is_same(const std::type_info & type) const override { return get_type() == type; }

    static constexpr T & as_object(void * memory) { return *static_cast<T *>(memory); }

    static constexpr const T & as_object(const void * memory)
    {
        return *static_cast<const T *>(memory);
    }

    type_wrapper_t() {}

    const std::type_info & get_type() const override { return typeid(T); }
    std::string get_name() const override { return utils::name_of_type<T>(); }
    size_t get_size() const override { return sizeof(T); }

    void copy(void * memory_to, const void * memory_from) const override
    {
        new (memory_to) T(as_object(memory_from));
    }

    void move(void * memory_to, void * memory_from) const noexcept override
    {
        new (memory_to) T(std::move(as_object(memory_from)));
    }

    void assign(void * memory_to, const void * memory_from) const override
    {
        as_object(memory_to) = as_object(memory_from);
    }

    void move_assign(void * memory_to, void * memory_from) const override
    {
        as_object(memory_to) = std::move(as_object(memory_from));
    }

    void destroy(void * memory) const noexcept override { as_object(memory).~T(); }
};

} // namespace details

template <class TypeWrapperInterface, template <typename, typename> class TypeWrapper>
struct type_eraser_t {
    using type_wrapper_ptr = const TypeWrapperInterface *;
    template <typename T>
    using type_wrapper_t = TypeWrapper<T, TypeWrapperInterface>;

    static_assert(std::is_base_of<details::i_type_wrapper_t, TypeWrapperInterface>::value,
                  "interface must be derived from i_type_wrapper");

    template <typename T>
    static type_wrapper_ptr wrap()
    {
        static_assert(std::is_base_of<TypeWrapperInterface, type_wrapper_t<T>>::value,
                      "interface must be base of wrapper");
        static const type_wrapper_t<T> instance;
        return &instance;
    }
};

using default_type_eraser_t = type_eraser_t<details::i_type_wrapper_t, details::type_wrapper_t>;

struct malloc_allocator_t {
    inline static void * allocate(size_t size) { return malloc(size); }

    inline static void free(void * memory) { ::free(memory); }
};

/*!
 * Allows to store values of any type.
 * Note:
 *  any_basic_t move doesn't trigger move constructor of held object
 *    explanation:
 *      we are moving ownership of object to another any_basic_t,
 *      but not to another object of the same type, thus no need to call move
 * constructor
 *
 *  any_basic_t copy, on contrary, does trigger copy constructor of held object
 *   since copy of held object needs to be created.
 */
template <size_t MinStaticCapacity = 0, class TypeEraser = default_type_eraser_t,
          class Allocator = malloc_allocator_t>
class any_basic_t {
  protected:
    constexpr static size_t get_additional_in_place_memory_size()
    {
        return std::max(sizeof(void *), MinStaticCapacity) - sizeof(void *);
    }

    union {
        // Resulting size is determined by size of
        // this struct: we need pointer and flag
        struct {
            void * memory;
            uint8_t _additional_in_place_memory[get_additional_in_place_memory_size()];
            bool _;
        } on_heap;

        // Small Object Optimization
        struct {
            uint8_t memory[sizeof on_heap - 1];
            bool _;
        } in_place;

        // Flag is last byte
        struct {
            uint8_t _[sizeof on_heap - 1];
            bool is_dynamic; // Owns memory on heap?
        } memory;

    } inner;

    typename TypeEraser::type_wrapper_ptr wrapped_type;

    // ---- inner utils ----
    inline const any_basic_t * c_this() const noexcept { return this; }

    template <typename>
    struct is_any: std::false_type {};

    template <size_t MC, typename T>
    struct is_any<any_basic_t<MC, T>>: std::true_type {};

#define not_lvalue_ref_m(T)                                                                        \
    typename = typename std::enable_if<!std::is_lvalue_reference<T>::value>::type
#define of_other_type_m(T) typename = typename std::enable_if<!is_any<T>::value>::type
#define not_void_m(T) typename = typename std::enable_if<!std::is_void<T>::value>::type
#define is_void_m(T) typename = typename std::enable_if<std::is_void<T>::value>::type
    // ---- inner utils end ----

    inline void reset() noexcept
    {
        wrapped_type            = nullptr;
        inner.memory.is_dynamic = false;
    }

    inline void init(std::size_t size)
    {
        inner.on_heap.memory    = Allocator::allocate(size);
        inner.memory.is_dynamic = true;
    }

    inline void init_in_place() { inner.memory.is_dynamic = false; }

    inline void copy(const void * object, std::size_t size)
    {
        init(size);
        wrapped_type->copy(inner.on_heap.memory, object);
    }

    inline void copy_in_place(const void * object)
    {
        init_in_place();
        wrapped_type->copy(inner.in_place.memory, object);
    }

    inline void move(void * object, std::size_t size)
    {
        init(size);
        wrapped_type->move(inner.on_heap.memory, object);
    }

    inline void move_in_place(void * object) noexcept
    {
        init_in_place();
        wrapped_type->move(inner.in_place.memory, object);
    }

    inline void set(void * data) noexcept
    {
        inner.on_heap.memory    = data;
        inner.memory.is_dynamic = true;
    }

    inline void set_in_place(const void * object, size_t size) noexcept
    {
        init_in_place();
        memcpy(inner.in_place.memory, object, size);
    }

    template <typename T>
    inline void copy(const T & object)
    {
        wrapped_type = TypeEraser::template wrap<T>();

        if (sizeof(T) > in_place_capacity()) {
            copy(&object, sizeof(T));
        } else {
            copy_in_place(&object);
        }
    }

    inline void copy(const any_basic_t & other)
    {
        if (other.empty()) {
            return;
        }

        wrapped_type = other.wrapped_type;

        if (other.inner.memory.is_dynamic) {
            copy(other.inner.on_heap.memory, other.size());
        } else {
            copy_in_place(other.inner.in_place.memory);
        }
    }

    template <typename T>
    inline void move(T && object)
    {
        wrapped_type = TypeEraser::template wrap<T>();

        if (sizeof(T) > in_place_capacity()) {
            move(&object, sizeof(T));
        } else {
            move_in_place(&object);
        }
    }

    inline void move(any_basic_t && other) noexcept
    {
        if (other.empty()) {
            return;
        }

        wrapped_type = other.wrapped_type;

        if (other.inner.memory.is_dynamic) {
            set(other.inner.on_heap.memory);
        } else {
            set_in_place(other.inner.in_place.memory, other.size());
        }

        other.reset();
    }

    template <typename T, typename... Args>
    inline T & emplace_unsafe(Args &&... args)
    {
        wrapped_type = TypeEraser::template wrap<T>();

        if (sizeof(T) > in_place_capacity()) {
            init(sizeof(T));
        } else {
            init_in_place();
        }

        new (data()) T(std::forward<Args>(args)...);

        return as<T>();
    }

    inline void delete_dirty() noexcept
    {
        if (!empty()) {
            wrapped_type->destroy(data());
            if (inner.memory.is_dynamic) {
                Allocator::free(inner.on_heap.memory);
            }
        }
    }

  public:
#pragma region constructors
    any_basic_t() { reset(); }

    template <bool NoAlloc = false, typename T>
    any_basic_t(const T & object)
      : any_basic_t()
    {
        static_assert(!NoAlloc || sizeof(T) <= in_place_capacity(), "object doesn't fit in place");
        copy(object);
    }

    template <typename T, not_lvalue_ref_m(T), of_other_type_m(T)>
    any_basic_t(T && object)
      : any_basic_t()
    {
        move(std::move(object));
    }

    // Treat C array as C++ array
    template <typename T, int N>
    any_basic_t(const T (&c_array)[N])
    {
        auto & array = emplace_unsafe<std::array<T, N>>();
        std::copy(std::begin(c_array), std::end(c_array), array.begin());
    }

    // Treat C string as C++ string
    any_basic_t(const char * c_string)
      : any_basic_t(std::string(c_string))
    {}

    // Invokes T's copy constructor
    any_basic_t(const any_basic_t & other)
      : any_basic_t()
    {
        copy(other);
    }

    // Does not invoke T's move constructor
    any_basic_t(any_basic_t && other) noexcept
      : any_basic_t()
    {
        move(std::move(other));
    }

#pragma endregion

    template <typename T, typename... Args>
    inline T & emplace(Args &&... args)
    {
        clear();
        return emplace_unsafe<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    inline T & emplace_in_place(Args &&... args)
    {
        static_assert(sizeof(T) <= in_place_capacity(), "object size > SOO capacity");
        return emplace<T>(std::forward<Args>(args)...);
    }

#pragma region operators
    template <typename T>
    T & operator=(const T & object)
    {
        clear();
        copy(object);
        return as<T>();
    }

    template <typename T, not_lvalue_ref_m(T), of_other_type_m(T)>
    T & operator=(T && object)
    {
        clear();
        move(std::move(object));
        return as<T>();
    }

    template <typename T, int N>
    std::array<T, N> & operator=(T (&c_array)[N])
    {
        clear();
        auto & array = emplace_unsafe<std::array<T, N>>();
        std::copy(std::begin(c_array), std::end(c_array), array.begin());
        return array;
    }

    std::string & operator=(const char * c_string) { return operator=(std::string(c_string)); }

    any_basic_t & operator=(const any_basic_t & rhs)
    {
        if (&rhs != this) {
            clear();
            copy(rhs);
        }
        return *this;
    }

    any_basic_t & operator=(any_basic_t && rhs) noexcept
    {
        if (&rhs != this) {
            clear();
            move(std::move(rhs));
        }
        return *this;
    }

    template <typename T>
    operator const T &() const
    {
        return as<T>();
    }

    template <typename T>
    operator T &()
    {
        return as<T>();
    }
#pragma endregion

    constexpr static std::size_t in_place_capacity() { return sizeof(inner.in_place.memory); }

    std::size_t size() const { return wrapped_type ? wrapped_type->get_size() : 0; }

    const void * data() const
    {
        return inner.memory.is_dynamic ? static_cast<const void *>(inner.on_heap.memory)
                                       : static_cast<const void *>(inner.in_place.memory);
    }

    void * data() { return const_cast<void *>(c_this()->data()); }

    const std::type_info & type() const
    {
        static const auto & default_type_info = typeid(void);

        return empty() ? default_type_info : wrapped_type->get_type();
    }

    std::string type_name() const { return empty() ? "void" : wrapped_type->get_name(); }

    template <typename T>
    bool is() const
    {
        return wrapped_type && wrapped_type->template is_same<T>();
    }

    bool is_in_place() const { return !inner.memory.is_dynamic; }

    template <typename T, not_void_m(T)>
    const T & as() const
    {
        if (empty()) {
            throw type_mismatch_error_t("storing: <none> casting to: " + utils::name_of_type<T>());
        }

        if (!is<T>()) {
            assert(false);
            throw type_mismatch_error_t("storing: " + wrapped_type->get_name() +
                                        " casting to: " + utils::name_of_type<T>());
        }

        return *static_cast<const T *>(data());
    }

    template <typename T, is_void_m(T)>
    void as() const
    {
        if (!empty()) {
            throw type_mismatch_error_t("storing: " + wrapped_type->get_name() +
                                        " casting to void");
        }
        return;
    }

    template <typename T>
    T & as()
    {
        return const_cast<T &>(c_this()->template as<T>());
    }

    void clear()
    {
        delete_dirty();
        reset();
    }

    bool empty() const { return !wrapped_type; }

    ~any_basic_t() noexcept { delete_dirty(); }
};

#undef is_void_m
#undef not_void_m
#undef of_other_type_m
#undef not_lvalue_ref_m

// TODO: avoid mentioning template args
template <typename T, size_t P1, class P2, class P3>
bool operator==(const T & lhs, const any_basic_t<P1, P2, P3> & rhs)
{
    return lhs == rhs.template as<T>();
}

template <typename T, size_t P1, class P2, class P3>
bool operator==(const any_basic_t<P1, P2, P3> & lhs, const T & rhs)
{
    return rhs == lhs;
}

using any_t = any_basic_t<>;

} // namespace interop
