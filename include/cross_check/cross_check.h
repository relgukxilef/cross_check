#include <cstdint>
#include <initializer_list>
#include <string_view>

namespace cross_check {
    struct value_hash {
        template<typename T>
        value_hash(const T &t) {
            hash = std::hash<T>()(t);
        }
        value_hash(const char t[]) : value_hash(std::string_view(t)) {}
        value_hash(
            std::initializer_list<value_hash> values
        ) {
            for (auto value : values)
                hash = hash * 713387040358173221ul + value.hash;
        }
        auto operator==(const value_hash &v) const {
            return hash == v.hash;
        }
        std::uint64_t hash = 0;
    };

    void note(value_hash key, value_hash value);
    void check(
        value_hash key, value_hash value,
        std::string_view message
    );
}
