#include "object.hpp"
#include "common.hpp"

#include <object_view.hpp>

#include <v8.h>

using namespace v8;

// namespace interop {
// namespace {
/*
 * Construct a new c++ object and wrap it in a js object
 */
// template <typename T, typename... Args>
// Persistent<Object> make_object(Handle<Object> object, Args &&... args)
// {
//     auto x   = new T(std::forward<Args>(args)...);
//     auto obj = Persistent<Object>::New(object);
//     obj->SetInternalField(0, External::New(x));

//     obj.SetWeak(x, [](Persistent<Value> obj, void * data) {
//         auto x = static_cast<T *>(data);
//         delete x;

//         obj.Dispose();
//         obj.Clear();
//     });

//     return obj;
// }
// } // namespace

// void expose_object_view(Isolate * isolate, const object_view_t & object_view)
// {

// }
// } // namespace
