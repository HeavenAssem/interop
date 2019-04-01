cmake_minimum_required(VERSION 3.4)

add_subdirectory(modules)
configure_file(configuration.json configuration.json COPYONLY)
add_test(cross_language.javascript.objects ${TESTS_NODE} -c configuration.json)
