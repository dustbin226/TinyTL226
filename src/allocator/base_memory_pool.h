/**
 * Copyright (C) 2020, Yujian Lin, All rights reserved.
 * 
 * @brief
 * 
 */

#ifndef _TTL226_BASE_MEMORY_POOL_H_
#define _TTL226_BASE_MEMORY_POOL_H_

#include "src/config.h"

#include <cstddef>  // for size_t, ptrdiff_t
#include <cstdint>  // for uintptr_t

#include <utility>  // for std::forward()
#include <new>      // for placement new


#if _IN_YUJIANLIN_DEV_MOD_ == 2018202296L
namespace YujianLinDev
{
#endif
namespace TinyTL
{
namespace Memory
{
/**
 * 
 * 
 */ 
template<typename T, size_t pool_size = 4096, uint32_t slot_num = 20>
class BaseMemoryPool
{
public:
    // stl要求的traits接口
    typedef T           value_type;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;

    // stl allocator要求的rebind接口
    template<typename U>
    struct rebind { typedef BaseMemoryPool<U> other; };

    BaseMemoryPool() noexcept;
    BaseMemoryPool(const BaseMemoryPool& dummy) = delete;
    BaseMemoryPool(BaseMemoryPool&& dummy) noexcept;

    template<typename U>
    BaseMemoryPool(const BaseMemoryPool<U>& dummy)  = delete;

    BaseMemoryPool& operator=(const BaseMemoryPool& dummy) = delete;
    BaseMemoryPool& operator=(BaseMemoryPool&& dummy) noexcept;

    ~BaseMemoryPool() noexcept;

    // stl allocator要求实现的接口
    inline pointer address(reference elem) const noexcept;
    inline const_pointer address(const_reference elem) const noexcept;

    pointer allocate(size_type n = 1, const void* hint = nullptr);
    void deallocate(pointer p, size_type n = 1);
    void construct(pointer p, const_reference val);
    void destroy(pointer p);

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args);
    template<typename U>
    void destroy(U* p);

    inline size_type max_size() const noexcept;

    pointer NewElem(const_reference val);
    template<typename... Args>
    pointer NewElem(Args&&... arg);
    void DeleteElem(pointer p);

private:
    union Slot
    {
        value_type elem;
        Slot* next;
    };
    
    Slot* _current_slot;
    Slot* _last_slot;
    Slot* _slots_list_header;

    inline size_type _PadPointer(char* p, size_type align) const noexcept;
    void _AllocateBlock();
    static_assert(pool_size >= slot_num*sizeof(Slot), 
        "--------------------------------------------------\
         * Error (allocator/base_memory_pool):             \
           内存池大小不足以容纳申请的区块 !                    \
         --------------------------------------------------");
};

/**
 * @brief
 *  对void类型的偏特化 
 */ 
template<>
class BaseMemoryPool<void>
{
public:
    // stl要求的traits接口
    typedef void        value_type;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

    typedef void*       pointer;
    typedef const void* const_pointer;

    // stl allocator要求的rebind接口
    template<typename U>
    struct rebind { typedef BaseMemoryPool<U> other; };

    BaseMemoryPool() noexcept;
    BaseMemoryPool(const BaseMemoryPool& dummy) noexcept;
    BaseMemoryPool(BaseMemoryPool&& dummy) noexcept;

    template<typename U>
    BaseMemoryPool(const BaseMemoryPool<U>& dummy) noexcept;

    ~BaseMemoryPool() noexcept;
};

} // namespace Memory
} // namespace TinyTL
#if _IN_YUJIANLIN_DEV_MOD_ == 2018202296L
} // namesapce YujianLinDev
#endif

/* Add New Contents Here */
#endif // _TTL226_BASE_MEMORY_POOL_H_
