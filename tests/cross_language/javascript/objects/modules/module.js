function test_objects()
{
    let obj = new cpp.module.test(9);

    let errors = [];

    let expect_eq = function(a, b) {
        if (a != b) {
            errors.push("expected: " + a + ", got: " + b);
        }
    };

    let finalize_test = function() {
        if (errors.length != 0) {
            throw 'errors: ' + JSON.stringify(errors);
        }

        return true;
    };

    expect_eq(14, obj.add1(5));
    expect_eq(12, obj.add2(1, 2));
    expect_eq(18, obj.mul_mod(2));
    expect_eq(36, obj.mul_mod(2));

    return finalize_test();
}
