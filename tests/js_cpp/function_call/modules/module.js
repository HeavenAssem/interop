function test1() {
    return 444;
}

function test2() {
    return "something";
}

function add(a, b) {
    return a + b;
}

function check_modules() {
    return cpp.module != undefined;
}

function cpp_get10() {
    return cpp.module.get10();
}

function cpp_add(a, b) {
    return cpp.module.add(a, b);
}

"OK";
