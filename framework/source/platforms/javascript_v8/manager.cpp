#include "manager.hpp"
#include "base_module.h"
#include "node.hpp"

using namespace std;

namespace interop {

object_manager_t::object_context_t::object_context_t(object_ptr_t object,
                                                     v8::Local<v8::Object> bind_to)
  : object{std::move(object)}
  , weak_ref{v8::Isolate::GetCurrent(), bind_to}
{}

object_manager_t::object_manager_t(class_id_t class_id)
  : class_id{class_id}
{}

base_module_t & object_manager_t::get_module() const
{
    return node_t::get().get(class_id.module_id);
}

const std::string & object_manager_t::get_class_name() const
{
    return get_module().get_object_metadata(class_id).name;
}

void object_manager_t::manage_lifetime(object_ptr_t object, v8::Local<v8::Object> bind_to)
{
    const auto & [it, inserted] =
        managed_objects.emplace(piecewise_construct, forward_as_tuple(object.get()),
                                forward_as_tuple(std::move(object), bind_to));
    interop_invariant_m(inserted, "attempt to manage lifetime of already managed object");

    auto & [_, context] = *it;

    context.weak_ref.SetWeak<void>(nullptr, weak_callback, v8::WeakCallbackType::kInternalFields);
}

void object_manager_t::weak_callback(const v8::WeakCallbackInfo<void> & info)
{
    auto object_ptr = static_cast<object_view_t *>(info.GetInternalField(0));
    auto self       = static_cast<object_manager_t *>(info.GetInternalField(1));

    auto it = self->managed_objects.find(object_ptr);
    interop_invariant_m(it != self->managed_objects.end(), "no Persistent<> for " << object_ptr);
    it->second.weak_ref.Reset();
    self->managed_objects.erase(it);
}

object_manager_t & class_manager_t::get_manager_for_class(class_id_t id)
{
    return managed_classes.emplace(id, id).first->second;
}

} // namespace interop
