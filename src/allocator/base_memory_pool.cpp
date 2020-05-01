/**
 * Copyright (C) 2020, Yujian Lin, All rights reserved.
 * 
 */

#include "src/config.h"
#include "src/allocator/base_memory_pool.h"

#if _IN_YUJIANLIN_DEV_MOD_ == 2018202296L
namespace YujianLinDev
{
#endif
namespace TinyTL
{
namespace Memory
{
/**
 * @brief
 *      默认构造函数。创建一个空的内存池对象。
 * 
 * @example
 *      BaseMemoryPool<int, 8, 2> my_pool;
 */
template<typename T, size_t pool_size, uint32_t slot_num>
BaseMemoryPool<T, pool_size, slot_num>::BaseMemoryPool() noexcept
{
    this->_current_slot = nullptr;
    this->_last_slot = nullptr;
    this->_slots_list_header = nullptr;
}

/**
 * @brief
 *      拷贝构造函数。拷贝构造和拷贝赋值被禁止。
 */ 
template<typename T, size_t pool_size, uint32_t slot_num>
BaseMemoryPool<T, pool_size, slot_num>::BaseMemoryPool(
    const BaseMemoryPool& dummy) = delete;


/**
 * @brief
 *      移动构造函数。移动完成后，原内存池置空失效。
 * 
 * @example
 *      BaseMemoryPool<int, 8, 2> my_pool(std::move(
 *          BaseMemoryPool<int, 8, 2>()));
 */ 
template<typename T, size_t pool_size, uint32_t slot_num>
BaseMemoryPool<T, pool_size, slot_num>::BaseMemoryPool(
    BaseMemoryPool&& dummy) noexcept
{
    this->_current_slot = dummy._current_slot;
    this->_last_slot = dummy._last_slot;
    this->_free_slots_list = dummy._slots_list_header;
    dummy._current_slot = nullptr;
    dummy._last_slot = nullptr;
    dummy._slots_list_header = nullptr;
}

/**
 * @brief
 *      由其他类型内存池对象拷贝构造。被禁止。
 */ 
template<typename T, size_t pool_size, uint32_t slot_num>
template<typename U>
BaseMemoryPool<T, pool_size, slot_num>::BaseMemoryPool(
    const BaseMemoryPool<U>& dummy) = delete;

/**
 * @brief
 *      拷贝赋值函数。被禁止。 
 */
template<typename T, size_t pool_size, uint32_t slot_num>
BaseMemoryPool<T, pool_size, slot_num>& 
BaseMemoryPool<T, pool_size, slot_num>::operator=(
    const BaseMemoryPool& dummy) = delete;

/**
 * @brief
 *      移动赋值函数。移动后目标原来的内存释放，被移动的右值析构。
 * 
 * @example
 *      BaseMemoryPool<int, 8, 2> a;
 *      a = BaseMemoryPool<int, 8, 2>();
 */
template<typename T, size_t pool_size, uint32_t slot_num>
BaseMemoryPool<T, pool_size, slot_num>&
BaseMemoryPool<T, pool_size, slot_num>::operator=(
    BaseMemoryPool&& dummy) noexcept
{
    if(this != &dummy)
    {
        // 交换内存空间 (包含已分配内存)
        std::swap(this->_slots_list_header, dummy._slots_list_header);
        this->_current_slot = dummy._current_slot;
        this->_last_slot = dummy._last_slot;
        this->_slots_list_header = dummy._slots_list_header;
    }
    dummy.~BaseMemoryPool();
    return *this;
} 

/**
 * @brief
 *      析构函数，释放所有内存块 (包含已分配内存块)。
 */
template<typename T, size_t pool_size, uint32_t slot_num>
BaseMemoryPool<T, pool_size, slot_num>::~BaseMemoryPool() noexcept
{
    Slot* curr_slot = this->_slots_list_header;
    Slot* next_slot = nullptr;
    while(curr_slot != nullptr)
    {
        next_slot = curr_slot->next;
        ::operator delete(reinterpret_cast<void*>(curr_slot));
        curr_slot = next_slot;
    }
    this->_current_slot = nullptr;
    this->_last_slot = nullptr;
    this->_slots_list_header = nullptr;
} 

} // namespace Memory
} // namespace TinyTL
#if _IN_YUJIANLIN_DEV_MOD_ == 2018202296L
} // namespace YujianLinDev
#endif