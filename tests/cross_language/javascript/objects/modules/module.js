function test_objects()
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

        expect_eq(14, obj.add1(5));
        expect_eq(12, obj.add2(1, 2));
        expect_eq(18, obj.mul_mod(2));
        expect_eq(36, obj.mul_mod(2));
    }

    return finalize_test();
}

function TestObject(a, b, c)
{
    this.a = a;
    this.b = b;
    this.c = c;
}

TestObject.prototype.get_a = function() { return this.a; };
TestObject.prototype.get_b = function() { return this.b; };
TestObject.prototype.get_c = function() { return this.c; };

// I don't know why class declaration is not available from c++, so using here statements
NewTypeClass = class {
    constructor(a, b)
    {
        this.a = a + 2;
        this.b = b + ' was passed from c++';
    }

    get_a() { return this.a; }
    get_b() { return this.b; }
};
