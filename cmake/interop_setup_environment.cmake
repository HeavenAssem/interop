cmake_minimum_required(VERSION 3.4)

if(MSVC)
add_compile_options(/W4 /WX)
else(MSVC)
add_compile_options(-Wall -Wextra -pedantic -Werror)
endif(MSVC)
