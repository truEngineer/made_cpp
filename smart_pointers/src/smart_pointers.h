#pragma once
#include <iostream>

namespace task {

    template<class T>
    class UniquePtr {
    public:
        UniquePtr() = default;
        explicit UniquePtr(T* ptr);
        UniquePtr(const UniquePtr& other) = delete;
        UniquePtr(UniquePtr&& other);
        UniquePtr operator=(const UniquePtr& other) = delete;
        UniquePtr& operator=(UniquePtr&& other);
        T& operator*() const;
        T& operator*();
        T* operator->() const;
        T* operator->();
        T* get() const;
        T* get();
        T* release();
        void reset(T* ptr = nullptr);
        void swap(UniquePtr& other);
        ~UniquePtr();
    private:
        T* data = nullptr;
    };

    template<class T>
    class WeakPtr;

    template<class T>
    class ControlBlock;

    template<typename T>
    class SharedPtr {
    public:
        SharedPtr(T* ptr = nullptr);
        SharedPtr(const SharedPtr& other);
        SharedPtr(SharedPtr&& other);
        SharedPtr& operator=(const SharedPtr& other);
        SharedPtr& operator=(SharedPtr&& other);
        SharedPtr(const WeakPtr<T>& other);
        T& operator*();
        T* operator->();
        T* get();
        std::size_t use_count() const;
        void reset(T* ptr = nullptr);
        void swap(SharedPtr& other);
        ~SharedPtr();
    private:
        friend class WeakPtr<T>;
        ControlBlock<T>* ctrl_block;
    };

    template<typename T>
    class WeakPtr {
    public:
        WeakPtr();
        WeakPtr(const SharedPtr<T>& other);
        WeakPtr(const WeakPtr& other);
        WeakPtr(WeakPtr&& other);
        WeakPtr& operator=(const WeakPtr& other);
        WeakPtr& operator=(WeakPtr&& other);
        WeakPtr& operator=(const SharedPtr<T>& other);
        SharedPtr<T> lock();
        std::size_t use_count();
        bool expired();
        void reset();
        void swap(WeakPtr& other);
        ~WeakPtr();
    private:
        friend class SharedPtr<T>;
        ControlBlock<T>* ctrl_block;
    };

    template<typename T>
    struct ControlBlock {
        T* ptr = nullptr;
        std::size_t r_count;
        std::size_t w_count;
        ControlBlock(T* ptr);
        void reset();
        ~ControlBlock();
    };

}  // namespace task


#include "smart_pointers.tpp"