#include <cross_check/cross_check.h>

#include <iostream>

using namespace std::string_view_literals;

struct my_object {
    my_object();
};

my_object::my_object() {
    cross_check::note({this, "foo called"}, "no");
}

void foo(my_object &object) {
    cross_check::check(
        {&object, "foo called"}, "no", 
        "foo should only be called once on the same object"
    );
    cross_check::note({&object, "foo called"}, "yes");
}

int main() {
    my_object a, b;
    foo(a);
    foo(b);
    foo(a);
}
