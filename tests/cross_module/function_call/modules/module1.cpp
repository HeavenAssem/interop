//
// Created by islam on 13.05.17.
//

#include "module1.h"

#include <iostream>

void hello_world(int n, float k, double a)
{
    std::cout << "hello_world invoked! n is: " << n << "; k is: " << k << " a is: " << a
              << std::endl;
}

int add(int a, int b) { return a + b; }

const char * say_hello() { return "say hello yourself!"; }

test::test(int a)
  : b(a)
{
    std::cout << "test object " << this << ": constructor int called" << std::endl;
}

test::test(double a)
  : b(static_cast<int>(a))
{
    std::cout << "test object " << this << ": constructor double called" << std::endl;
}

void test::member1() {}

double test::member2(int arg)
{
    std::cout << "member2 invoked! b is: " << b << "; arg is: " << arg << "; object: " << this
              << std::endl;
    return 1.5 * arg;
}

double test::member3(float arg1, double arg2)
{
    std::cout << "member3 invoked! arg1 is: " << arg1 << "; arg2 is: " << arg2
              << "; object: " << this << std::endl;

    return arg1 + arg2;
}

int test::get()
{
    std::cout << "get invoked! b is: " << b << "; object: " << this << std::endl;
    return b;
}

test::~test() { std::cout << "test object " << this << ": destructor called" << std::endl; }

namespace interop {
void INTEROP_MODULE_REGISTER()
{
    register_function("hello_world", hello_world);
    register_function("add", add);
    register_function("say_hello", say_hello);
    register_function("non_capturing_lambda", [](int a) { return 8080 + a; });

    int value = 552;
    register_function("capturing_lambda", [value](int a) { return value + a; });

    register_class<test, constructor<int>, constructor<double>>("test")
        .member<&test::member1>("member1")
        .member<&test::member2>("member2")
        .member<&test::member3>("member3")
        .member<&test::get>("get");
}
} // namespace interop
