words that describe style:
1. snake_case
2. suffixes
3. verbose

main statements:
1. all identifiers are written in snake case except MACRO_ARGUMENTS, TemplateArguments and EnumValues
2. all identifiers that are written in snake case except (member) variables that are not constants
and (member) functions have suffixes:
    1. _t: classes, aliases
    2. _m: C marcos
    3. _c: constants
    4. _e: enums
    5. _p: constructor parameters that will be moved in initializer list
3. always use full names, but for common abbreviations such as "arg" you can make an exception
