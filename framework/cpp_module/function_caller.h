#pragma once
//
// Created by islam on 27.05.17.
//


namespace interop {
    class function_caller {
    public:
        virtual void push_arg(double arg) = 0;
        virtual void push_arg(int arg) = 0;
        virtual void push_arg(bool arg) = 0;
        virtual void push_arg(char arg) = 0;
        virtual void push_arg(float arg) = 0;
        virtual void push_arg(long arg) = 0;
        virtual void push_arg(long long arg) = 0;
        virtual void push_arg(void * arg) = 0;
        virtual void push_arg(short arg) = 0;

        virtual double  call_double() = 0;
        virtual int     call_int() = 0;
        virtual bool    call_bool() = 0;
        virtual char    call_char() = 0;
        virtual float   call_float() = 0;
        virtual long    call_long() = 0;
        virtual long long   call_long_long() = 0;
        virtual void *  call_ptr() = 0;
        virtual void    call_void() = 0;
        virtual short   call_short() = 0;

        virtual ~function_caller() {};
    };

}
