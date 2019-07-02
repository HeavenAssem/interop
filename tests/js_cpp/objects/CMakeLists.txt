cmake_minimum_required(VERSION 3.4)

add_subdirectory(modules)
configure_file(configuration.json configuration.json COPYONLY)
add_test(js_cpp.objects ${TESTS_NODE} -c configuration.json)
