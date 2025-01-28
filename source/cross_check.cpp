#include <cross_check/cross_check.h>

#include <atomic>
#include <cassert>
#include <span>
#include <iostream>
#include <shared_mutex>
#include <spanstream>
#include <stacktrace>
#include <mutex>

namespace cross_check {
    struct shared_mutex {
        std::atomic_uint counters = 0;
        bool try_lock() {
            unsigned expected = 0;
            return counters.compare_exchange_strong(
                expected, 1
            );
        }
        void unlock() {
            auto previous = counters.fetch_sub(1);
            assert(previous & 1);
        }
        bool try_lock_shared() {
            if (!(counters.fetch_add(2) & 1))
                return true;
            counters.fetch_sub(2);
            return false;
        }
        void unlock_shared() {
            auto previous = counters.fetch_sub(2);
            assert(!(previous & 1) && previous != 0);
        }
    };

    struct notes_t {
        struct bucket {
            shared_mutex mutex;
            std::uint64_t key = 0, value;
            char context[1024];
        } global_buckets[1024];
        std::span<bucket> buckets{global_buckets};
    } notes;

    void note(
        value_hash key, 
        value_hash value
    ) {
        auto &bucket = notes.buckets[key.hash % notes.buckets.size()];
        std::unique_lock lock{bucket.mutex, std::try_to_lock};
        if (!lock)
            return;
        bucket.key = key.hash;
        bucket.value = value.hash;
        std::spanbuf context(bucket.context);
        std::ostream stream(&context);
#ifdef __cpp_lib_stacktrace
        stream << std::stacktrace::current(1);
#endif
    }

    void check(
        value_hash key, 
        value_hash value,
        std::string_view message
    ) {
        auto &bucket = notes.buckets[
            key.hash % notes.buckets.size()
        ];
        std::shared_lock lock{bucket.mutex, std::try_to_lock};
        if (!lock)
            return;
        if (bucket.key != key.hash)
            // we don't know the value, assume it's ok
            return;
        if (bucket.value == value.hash)
            return;
        std::cerr << 
            message << "\nin:\n" <<
#ifdef __cpp_lib_stacktrace
            std::stacktrace::current(1) << 
#endif
            "\nbecause of:\n" << bucket.context << 
            std::endl;
        return;
    }
}
