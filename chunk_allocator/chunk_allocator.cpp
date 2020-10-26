#include <iostream>
#include <vector>
#include <list>

class Chunk {
public:
    static const int size = 64; // for tests

    Chunk() {
        used = new uint8_t[size];
        free = used;
    }

    ~Chunk() {
        delete[] used;
    }

    uint8_t* alloc_block(size_t block_size) { // allocate memory block in chunk
        if (size - (free - used) < block_size) {
            return nullptr;
        }
        free += block_size;
        return free;
    }

private:
    uint8_t* used; // used chunk memory ptr (data section begin)
    uint8_t* free; // free chunk memory ptr (no_data section begin)
};

template <class T>
class ChunkAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    template<class U> struct rebind {
        typedef ChunkAllocator<U> other;
    };

    ChunkAllocator() {
        chunk_mgr = new ChunkManager;
    }

    ChunkAllocator(const ChunkAllocator& other) {
        chunk_mgr = other.chunk_mgr;
        chunk_mgr->counter += 1;
    }

    ~ChunkAllocator() {
        chunk_mgr->counter -= 1;
        if (chunk_mgr->counter == 0) {
            chunk_mgr->chunk_list.clear();
            delete chunk_mgr;
        }
    }

    ChunkAllocator& operator=(const ChunkAllocator& other) {
        if (this == &other) {
            return *this;
        }
        chunk_mgr->counter -= 1;
        if (chunk_mgr->counter == 0) {
            chunk_mgr->chunk_list.clear();
            delete chunk_mgr;
        }
        chunk_mgr = other.chunk_mgr;
        chunk_mgr->counter += 1;
    }

    T* allocate(size_t n) {
        std::cout << "--> allocate " << n * sizeof(T) << " bytes" << std::endl; // for tests
        if (n * sizeof(T) > Chunk::size) {
            std::cout << "ERROR! Chunk size is: " << Chunk::size << " bytes" << std::endl; // for tests
            return nullptr;
        }
        for (Chunk& chunk : chunk_mgr->chunk_list) { // first-fit search
            uint8_t* ptr = chunk.alloc_block(n * sizeof(T));
            if (ptr != nullptr) {
                return reinterpret_cast<T*>(ptr);
            }
        }
        chunk_mgr->chunk_list.emplace_back(); // create new chunk, Chunk()
        uint8_t* ptr = chunk_mgr->chunk_list.back().alloc_block(n * sizeof(T));
        return reinterpret_cast<T*>(ptr);
    }

    void deallocate(T* ptr, size_t n) {
        std::cout << "<-- deallocate " << n * sizeof(T) << " bytes" << std::endl; // for tests
    }

    template<class U, class... Args>
    void construct(U* ptr, Args&&... args) {
        ::new((void*)ptr) U(std::forward<Args>(args)...);
    }

    void destroy(T* ptr) {
        ptr->~T();
    }

    size_t chunk_count() {
        return chunk_mgr->chunk_list.size();
    }

private:
    class ChunkManager {
    public:
        size_t counter = 1; // count of references
        std::list<Chunk> chunk_list;
    };
    ChunkManager* chunk_mgr;
};

int main() {
    
    std::vector<int, ChunkAllocator<int>> vec;
    for (int i = 1; i <= 17; ++i) { // add 17 elements
        std::cout << ">> add " << i << std::endl;
        vec.push_back(i);
        std::cout << "* vec data size: " << vec.size() << ", size in memory: ";
        std::cout << vec.size() * sizeof(int) << " bytes" << std::endl;
        std::cout << "* vec buffer size: " << vec.capacity() << ", size in memory: ";
        std::cout << vec.capacity() * sizeof(int) << " bytes" << std::endl;
        std::cout << "* vec full size in memory: "; // sizeof(vec) = 32 bytes (4 pointers * 8 bytes)
        std::cout << vec.capacity() * sizeof(int) + sizeof(vec) << " bytes" << std::endl;
        std::cout << "* chunk count (memory size): "<< vec.get_allocator().chunk_count() << " (";
        std::cout << vec.get_allocator().chunk_count() * Chunk::size << " bytes) " << std::endl << std::endl;
    }

    /**
     * std::vec<int> vec = {1,2,..,16}
     * vec.size() = 16 elements * 4 bytes = 64 bytes, equals to current vec.capacity()
     *
     *  |  |  |  |  |  |  |  | heap-allocated buffer
     *   |
     * |ptr| std::vector instance
     *
     * It is impossible to allocate vector with 17 elements! (it requires 128 bytes chunk size)
     * */

    return 0;
}