# CrossCheck

Library to unintrusively add checks to functions.

## Examples

Let's say you have some class `my_object` and a function `foo` that should be called at most once per object. You don't want to or can't extend the class, but you'd still like to be able to check this constraint. Perhaps only in debug builds. With this library you could write `foo` like so:

```c++
my_object::my_object() {
    cross_check::note({this, "foo called"}, "no");

    // Construct...
}

void foo(my_object &object) {
    cross_check::check(
        {&object, "foo called"}, "no", 
        "foo should only be called once on the same object"
    );

    // Do something with object

    cross_check::note({&object, "foo called"}, "yes");
}
```

Now we call the function a few times.

```c++
int main() {
    my_object a, b;
    foo(a);
    foo(b);
    foo(a);
}
```

The second call of `foo(a)` will write a message to stderr containing the message and stacktraces of the calls to `foo` related to `a`.

```
foo should only be called once on the same object
in:
0> D:\Felix\Documents\C++ projects\cross_check\samples\main.cpp(14): samples!foo+0xAD
1> D:\Felix\Documents\C++ projects\cross_check\samples\main.cpp(25): samples!main+0x22
2> D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl(288): samples!__scrt_common_main_seh+0x10C
3> KERNEL32!BaseThreadInitThunk+0x14
4> ntdll!RtlUserThreadStart+0x21
because of:
0> D:\Felix\Documents\C++ projects\cross_check\samples\main.cpp(15): samples!foo+0x110
1> D:\Felix\Documents\C++ projects\cross_check\samples\main.cpp(22): samples!main+0xE
2> D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl(288): samples!__scrt_common_main_seh+0x10C
3> KERNEL32!BaseThreadInitThunk+0x14
4> ntdll!RtlUserThreadStart+0x21
```

`note` adds a key-value pair to a map. `check` looks up a key and writes out a message and a stacktrace if it doesn't match the given value. Both key and value can be any object that can be hashed through `std::hash`, `const char[]` or an `std::initializer_list` of such objects. If the key is absent in the map, check will pass.

The library is thread-safe. It does not resize the map so there is a chance of false negatives. Further, it only stores and compares the hashes of keys and values so there is a small chance of false positives.
