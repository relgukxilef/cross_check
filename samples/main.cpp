#include <cross_check/cross_check.h>

#include <iostream>

using namespace std::string_view_literals;

struct my_object {};

void foo(my_object &object) {
    cross_check::check(
        {&object, "foo called"sv}, "no"sv, 
        "foo should only be called once on the same object"sv
    );
    cross_check::note({&object, "foo called"sv}, "yes"sv);
}

int main() {
    my_object a, b;

    foo(a);

    foo(b);
    
    foo(a);
}
