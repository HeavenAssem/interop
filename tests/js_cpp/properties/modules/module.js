function test_properties()
{
    let errors = [];

    let finalize_test = function() {
        if (errors.length != 0) {
            throw 'errors: ' + JSON.stringify(errors);
        }

        return true;
    };

    let expect_eq = function(a, b) {
        if (a != b) {
            errors.push("expected: " + a + ", got: " + b);
        }
    };

    {
        let obj = new cpp.module.test(9);

        expect_eq(9, obj.b);

        obj.b = 12;

        expect_eq(12, obj.get());
    }

    return finalize_test();
}
