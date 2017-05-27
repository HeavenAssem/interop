#pragma once
//
// Created by islam on 27.05.17.
//


#include <exceptions.h>
#include "function_caller.h"
#include "module_metadata.h"

namespace mosaic {
    class local_function_caller : public function_caller {
        static void * vm;
        static uint32_t count;

        const function_metadata * metadata;
        uint8_t argument_index;
    public:
        void push_arg(double arg) override;

        void push_arg(int arg) override;

        void push_arg(bool arg) override;

        void push_arg(char arg) override;

        void push_arg(float arg) override;

        void push_arg(long arg) override;

        void push_arg(long long arg) override;

        void push_arg(void * arg) override;

        void push_arg(short arg) override;

        void call() override;

        local_function_caller(const function_metadata * metadata);

        virtual ~local_function_caller();

    private:
        template<typename T>
        bool cast_argument(T arg) {
            if (metadata->arguments.size() <= argument_index) {
                throw function_call_error("while calling function \"" + metadata->name + "\": too many arguments");
            }

            const auto & argument_metadata = metadata->arguments[argument_index];
            if (argument_metadata.type != enumerate_type<T>()) {
                switch (argument_metadata.type) {
                    case type_enum::TE_BOOL:
                        if constexpr (std::is_convertible<T, bool>::value) {
                            push_arg(static_cast<bool>(arg));
                        }
                        break;
                    case type_enum::TE_CHAR:
                        if constexpr (std::is_convertible<T, char>::value) {
                            push_arg(static_cast<char>(arg));
                        }
                        break;
                    case type_enum::TE_SHORT:
                        if constexpr (std::is_convertible<T, short>::value) {
                            push_arg(static_cast<short>(arg));
                        }
                        break;
                    case type_enum::TE_FLOAT:
                        if constexpr (std::is_convertible<T, float>::value) {
                            push_arg(static_cast<float>(arg));
                        }
                        break;
                    case type_enum::TE_DOUBLE:
                        if constexpr (std::is_convertible<T, double>::value) {
                            push_arg(static_cast<double>(arg));
                        }
                        break;
                    case type_enum::TE_INT:
                        if constexpr (std::is_convertible<T, int>::value) {
                            push_arg(static_cast<int>(arg));
                        }
                        break;
                    case type_enum::TE_LONG:
                        if constexpr (std::is_convertible<T, long>::value) {
                            push_arg(static_cast<long>(arg));
                        }
                        break;
                    case type_enum::TE_LONG_LONG:
                        if constexpr (std::is_convertible<T, long long>::value) {
                            push_arg(static_cast<long long>(arg));
                        }
                        break;
                    case type_enum::TE_POINTER:
                        if constexpr (std::is_convertible<T, void *>::value) {
                            push_arg(static_cast<void *>(arg));
                        }
                        break;
                    default:
                        break;
                }
                return true;
            }
            return false;
        }
    };
}



