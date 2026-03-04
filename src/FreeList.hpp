#pragma once

#include <climits>
#include <memory>
#include <cstdint>
#include <cassert>
#include <vector>

typedef uint32_t Handle;

template<typename T>
class FreeListShared {
    FreeListShared(size_t initial_capacity) {
        s_ptrs.reserve(initial_capacity);
        next.reserve(initial_capacity);
    }

    inline Handle create(std::shared_ptr<T> sptr) {
        uint32_t idx = obtain_free_index();
        s_ptrs[idx] = sptr;
    }

    inline void remove(Handle handle) {
        s_ptrs[handle].reset();
        next[handle] = free_head;
        free_head = handle;
    }

    inline std::shared_ptr<T>& operator[](Handle handle) {
        return s_ptrs[handle];
    }
private:
    inline uint32_t obtain_free_index() { 
        if (free_head != invalid_index) { 
            uint32_t idx = free_head; 
            free_head = next[idx]; 
            return idx; 
        } 
        next.emplace_back(); 
        s_ptrs.emplace_back();
        return static_cast<uint32_t>(next.size() - 1); 
    }

    static constexpr uint32_t invalid_index = std::numeric_limits<uint32_t>::max();

    std::vector<std::shared_ptr<T>> s_ptrs;
    std::vector<uint32_t> next;

    int free_head = invalid_index;
};