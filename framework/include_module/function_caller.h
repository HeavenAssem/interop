#pragma once
//
// Created by islam on 27.05.17.
//


namespace mosaic {
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

        virtual void call() = 0;

        virtual ~function_caller() {};
    };

}

