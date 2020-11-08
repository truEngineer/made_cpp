#include "smart_pointers.h"

namespace task {

    // UniquePtr

    template<typename T>
    UniquePtr<T>::UniquePtr(T* ptr): data(ptr) {}

    template<typename T>
    UniquePtr<T>::UniquePtr(UniquePtr&& other): data(other.data) {
        other.data = nullptr;
    }

    template<typename T>
    UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) {
        reset(other.data);
        other.data = nullptr;
        return *this;
    }

    template<typename T>
    T& UniquePtr<T>::operator*() const {
        return *data;
    }

    template<typename T>
    T& UniquePtr<T>::operator*() {
        return *data;
    }

    template<typename T>
    T* UniquePtr<T>::operator->() const {
        return data;
    }

    template<typename T>
    T* UniquePtr<T>::operator->() {
        return data;
    }

    template<typename T>
    T* UniquePtr<T>::get() const {
        return data;
    }

    template<typename T>
    T* UniquePtr<T>::get() {
        return data;
    }

    template<typename T>
    void UniquePtr<T>::reset(T* ptr) {
        auto tmp_del = data;
        data = ptr;
        delete tmp_del;
    }

    template<typename T>
    T* UniquePtr<T>::release() {
        reset(nullptr);
    }

    template<typename T>
    void UniquePtr<T>::swap(UniquePtr& other) {
        std::swap(data, other.data);
    }

    template<typename T>
    UniquePtr<T>::~UniquePtr() {
        delete data;
    }

    // SharedPtr

    template<typename T>
    SharedPtr<T>::SharedPtr(T* ptr): ctrl_block(new ControlBlock<T>(ptr)) {}

    template<typename T>
    SharedPtr<T>::SharedPtr(const SharedPtr& other): ctrl_block(other.ctrl_block) {
        ctrl_block->r_count += 1;
    }

    template<typename T>
    SharedPtr<T>::SharedPtr(SharedPtr&& other): ctrl_block(other.ctrl_block) {
        other.ctrl_block = nullptr;
    }

    template<typename T>
    SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) {
        if (this == &other) {
            return *this;
        }
        if (ctrl_block) {
            ctrl_block->r_count -= 1;
            if (ctrl_block->r_count == 0) {
                ctrl_block->reset();
            }
            if (ctrl_block->r_count == 0 & ctrl_block->w_count == 0) {
                delete ctrl_block;
            }
        }
        ctrl_block = other.ctrl_block;
        ctrl_block->r_count += 1;
        return *this;
    }

    template<typename T>
    SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) {
        if (this == &other) {
            return *this;
        }
        if (ctrl_block) {
            ctrl_block->r_count -= 1;
            if (ctrl_block->r_count == 0) {
                ctrl_block->reset();
            }
            if (ctrl_block->r_count == 0 & ctrl_block->w_count == 0) {
                delete ctrl_block;
            }
        }
        ctrl_block = other.ctrl_block;
        other.ctrl_block = nullptr;
        return *this;
    }

    template<typename T>
    SharedPtr<T>::SharedPtr(const WeakPtr<T>& other): ctrl_block(other.ctrl_block) {
        if (ctrl_block) {
            ctrl_block->r_count += 1;
        }
    }

    template<typename T>
    T* SharedPtr<T>::get() {
        return ctrl_block->ptr;
    }

    template<typename T>
    T& SharedPtr<T>::operator*() {
        return *(ctrl_block->ptr);
    }

    template<typename T>
    T* SharedPtr<T>::operator->() {
        return ctrl_block ? ctrl_block->ptr : nullptr;
    }

    template<typename T>
    std::size_t SharedPtr<T>::use_count() const {
        if (ctrl_block->ptr) {
            return ctrl_block->r_count;
        }
        return 0;
    }

    template<typename T>
    void SharedPtr<T>::reset(T* ptr) {
        ctrl_block->r_count -= 1;
        if (ctrl_block->r_count == 0) {
            ctrl_block->reset();
        }
        if (ctrl_block->r_count == 0 & ctrl_block->w_count == 0) {
            delete ctrl_block;
        }
        ctrl_block = new ControlBlock(ptr);
    }

    template<typename T>
    void SharedPtr<T>::swap(SharedPtr& other) {
        std::swap(ctrl_block, other.ctrl_block);
    }

    template<typename T>
    SharedPtr<T>::~SharedPtr() {
        if (ctrl_block) {
            ctrl_block->r_count -= 1;
            if (ctrl_block->r_count == 0) {
                ctrl_block->reset();
            }
            if (ctrl_block->r_count == 0 & ctrl_block->w_count == 0) {
                delete ctrl_block;
            }
        }
    }

    // WeakPtr

    template<typename T>
    WeakPtr<T>::WeakPtr(): ctrl_block(new ControlBlock<T>(nullptr)) {}

    template<typename T>
    WeakPtr<T>::WeakPtr(const SharedPtr<T>& other): ctrl_block(other.ctrl_block) {
        if (ctrl_block) {
            ctrl_block->w_count += 1;
        }
    }

    template<typename T>
    WeakPtr<T>::WeakPtr(const WeakPtr& other): ctrl_block(other.ctrl_block) {
        if (ctrl_block) {
            ctrl_block->w_count += 1;
        }
    }

    template<typename T>
    WeakPtr<T>::WeakPtr(WeakPtr&& other): ctrl_block(other.ctrl_block) {
        other.ctrl_block = nullptr;
    }

    template<typename T>
    WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) {
        if (this == &other) {
            return *this;
        }
        ctrl_block->w_count -= 1;
        if (ctrl_block->r_count == 0 & ctrl_block->w_count == 0) {
            delete ctrl_block;
        }
        ctrl_block = other.ctrl_block;
        ctrl_block->w_count += 1;
        return *this;
    }

    template<typename T>
    WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) {
        if (this == &other) {
            return *this;
        }
        ctrl_block = other.ctrl_block;
        other.ctrl_block = nullptr;
        return *this;
    }

    template<typename T>
    WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<T>& other) {
        ctrl_block = other.ctrl_block;
        ctrl_block->w_count += 1;
        return *this;
    }

    template<typename T>
    SharedPtr<T> WeakPtr<T>::lock() {
        return SharedPtr<T>(*this);
    }

    template<typename T>
    std::size_t WeakPtr<T>::use_count() {
        if (ctrl_block) {
            return ctrl_block->r_count;
        }
        return 0;
    }

    template<typename T>
    bool WeakPtr<T>::expired() {
        return use_count() == 0;
    }

    template<typename T>
    void WeakPtr<T>::reset() {
        ctrl_block->w_count -= 1;
        if (ctrl_block->r_count == 0 & ctrl_block->w_count == 0) {
            delete ctrl_block;
        }
    }

    template<typename T>
    void WeakPtr<T>::swap(WeakPtr& other) {
        std::swap(ctrl_block, other.ctrl_block);
    }

    template<typename T>
    WeakPtr<T>::~WeakPtr() {
        if (ctrl_block) {
            ctrl_block->w_count -= 1;
            if (ctrl_block->r_count == 0 & ctrl_block->w_count == 0) {
                delete ctrl_block;
            }
        }
    }

    // ControlBlock

    template<typename T>
    ControlBlock<T>::ControlBlock(T* ptr): ptr(ptr), r_count(1), w_count(0) {}

    template<typename T>
    void ControlBlock<T>::reset() {
        delete ptr;
        ptr = nullptr;
    }

    template<typename T>
    ControlBlock<T>::~ControlBlock() {
        delete ptr;
    }
}