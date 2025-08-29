#ifndef JMN_HYDROGEN_INCLUDED
#  define JMN_HYDROGEN_INCLUDED

#  include <cstdint>
#  include <cstring>
#  include <cuchar>

#  ifdef JMN_FORCE_INLINE
#    define JMN_INLINE __forceinline
#  else
#    define JMN_INLINE inline
#  endif

#  define JMN_BREAK() __debugbreak()
#  define JMN_ASSERT(expr) do { if (!(expr)) JMN_BREAK(); } while (0)
#  define JMN_CHECK(expr, resvar, resval, jmplbl) do { if (!(expr)) { resvar = resval; goto jmplbl; } } while (0)
#  define JMN_REALLOC_DECL(name) B8 name(Addr user_data, Addr old_addr, Size old_size, Size new_size, Size alignment, Addr &new_addr, Result &result)
#  define JMN_LENGTH(static_array) (sizeof(static_array) / sizeof(static_array[0]))
#  define JMN_MIN(a, b) ((a) < (b) ? (a) : (b))
#  define JMN_MAX(a, b) ((a) > (b) ? (a) : (b))


namespace jmn
{

    using B8  = uint8_t;
    using B16 = uint16_t;
    using B32 = uint32_t;
    using B64 = uint64_t;

    using U8  = uint8_t;
    using U16 = uint16_t;
    using U32 = uint32_t;
    using U64 = uint64_t;

    using S8  = int8_t;
    using S16 = int16_t;
    using S32 = int32_t;
    using S64 = int64_t;

    using F32 = float;
    using F64 = double;

    using C8  = char;
    using C16 = char16_t;
    using C32 = char32_t;

    using Addr   = U64;
    using Size   = U64;
    using Handle = Addr;

    static inline constexpr Addr   NullAddr   = (Addr)0;
    static inline constexpr Handle NullHandle = (Handle)0;

    enum class Result : S32
    {
        ErrorGeneric      = INT32_MIN,
        ErrorNotSupported = -3,
        ErrorOutOfMemory  = -2,
        ErrorNull         = -1,
        Success           = 0,
        WarningNotFound   = 1,
        WarningGeneric    = INT32_MAX,
    };

    typedef JMN_REALLOC_DECL((*PFN_Realloc));

    struct Allocator
    {
        Addr        user_data;
        PFN_Realloc realloc;

        B8 Alloc(Size new_size, Size alignment, Addr &new_addr, Result &result);
        B8 Realloc(Addr old_addr, Size old_size, Size new_size, Size alignment, Addr &new_addr, Result &result);
        B8 Free(Addr old_addr, Size old_size, Size &free_size, Result &result);
    };

    struct MemoryArena
    {
        struct TemporaryMemory
        {
            MemoryArena &arena;
            Size   const point;
            TemporaryMemory(MemoryArena &arena) : arena(arena), point(arena.used) {}
            TemporaryMemory(TemporaryMemory const &) = delete;
            TemporaryMemory(TemporaryMemory &&) = delete;
            ~TemporaryMemory() { arena.used = point; }
            TemporaryMemory &operator=(TemporaryMemory const &) = delete;
            TemporaryMemory &operator=(TemporaryMemory &&) = delete;
        };

        Addr base;
        Size used;
        Size size;

        void Initialize(Addr base, Size size);
        B8 Push(Size bytes, Addr &addr, Result &result);
        B8 Push(Size bytes, Size alignment, Addr &addr, Result &result);
        template<typename T> B8 Push(Size count, T *&ptr, Result &result) { return Push(sizeof(T) * count, alignof(T), (Addr &)ptr, result); }

        TemporaryMemory MakeTemporaryMemory() { return { *this }; }
    };

    struct MemoryHeap
    {
        static inline constexpr Size DefaultMinBlockSize = 8;

        struct Block
        {
            Block *prev;
            Block *next;
            Size   offs;
            Size   size;
        };
        Block *used;
        Block *free;
        Size   min_block_size;
        Size   block_count;
        Size   bytes_used;

        void Initialize(Addr addr, Size size, Size min_block_size = DefaultMinBlockSize);
        B8 Alloc(Size new_size, Size alignment, Addr &new_addr, Result &result);
        B8 Realloc(Addr old_addr, Size old_size, Size new_size, Size alignment, Addr &new_addr, Result &result);
        B8 Free(Addr old_addr, Size old_size, Size &free_size, Result &result);
    };

    template<typename T, Size N> JMN_INLINE constexpr Size Length(T(&)[N]) { return N; }
    template<typename T> JMN_INLINE T &&Min(T &&a, T &&b) { return JMN_MIN(a, b); }
    template<typename T> JMN_INLINE T &&Max(T &&a, T &&b) { return JMN_MIN(a, b); }

    JMN_INLINE Addr Zero(Addr dst_addr,                Size size) { return (Addr)memset ((void *)dst_addr,                      0, size); }
    JMN_INLINE Addr Copy(Addr dst_addr, Addr src_addr, Size size) { return (Addr)memcpy ((void *)dst_addr, (void const *)src_addr, size); }
    JMN_INLINE Addr Move(Addr dst_addr, Addr src_addr, Size size) { return (Addr)memmove((void *)dst_addr, (void const *)src_addr, size); }
    template<typename T> JMN_INLINE T *Zero(T *dst              , Size count = 1) { return (T *)Zero((Addr)dst           , sizeof(T) * count); }
    template<typename T> JMN_INLINE T *Copy(T *dst, T const *src, Size count = 1) { return (T *)Copy((Addr)dst, (Addr)src, sizeof(T) * count); }
    template<typename T> JMN_INLINE T *Move(T *dst, T const *src, Size count = 1) { return (T *)Move((Addr)dst, (Addr)src, sizeof(T) * count); }

    JMN_INLINE constexpr Size KiB(Size x) { return    (x) * (Size)1024; }
    JMN_INLINE constexpr Size MiB(Size x) { return KiB(x) * (Size)1024; }
    JMN_INLINE constexpr Size GiB(Size x) { return MiB(x) * (Size)1024; }
    JMN_INLINE constexpr Size TiB(Size x) { return GiB(x) * (Size)1024; }
    JMN_INLINE constexpr Size PiB(Size x) { return TiB(x) * (Size)1024; }
    JMN_INLINE constexpr Size EiB(Size x) { return PiB(x) * (Size)1024; }

    JMN_INLINE constexpr Size ComputeAlignmentOffset(Addr addr, Size alignment) { return (alignment - (addr & (alignment - 1))) & (alignment - 1); }

}

#endif // JMN_HYDROGEN_INCLUDED

#ifdef __INTELLISENSE__
#  define JMN_HYDROGEN_IMPLEMENTATION
#endif // __INTELLISENSE__

#ifdef JMN_HYDROGEN_IMPLEMENTATION
#  ifndef JMN_HYDROGEN_IMPLEMENTATED
#    define JMN_HYDROGEN_IMPLEMENTATED

#    ifdef JMN_HYDROGEN_SINGLE_FILE
#      define JMN_HYDROGEN_IMPL_INLINE JMN_INLINE
#    else
#      define JMN_HYDROGEN_IMPL_INLINE
#    endif // JMN_HYDROGEN_SINGLE_FILE

namespace jmn
{

    namespace internal
    {

        static JMN_INLINE Addr GetMemoryHeapBlockAddress(MemoryHeap::Block &block) { return (Addr)(&block + 1) + block.offs; }

        static JMN_INLINE MemoryHeap::Block *GetEndOfMemoryHeapBlock(MemoryHeap::Block &block) { return (MemoryHeap::Block *)((Addr)(&block + 1) + block.size); }

        static JMN_INLINE void ConnectMemoryHeapBlock(MemoryHeap::Block &block, MemoryHeap::Block *prev, MemoryHeap::Block *next)
        {
            if (prev) prev->next = &block;
            if (next) next->prev = &block;
            block.prev = prev;
            block.next = next;
        }

        static JMN_INLINE void DisconnectMemoryHeapBlock(MemoryHeap::Block &block)
        {
            if (block.prev) block.prev->next = block.next;
            if (block.next) block.next->prev = block.prev;
            block.prev = NULL;
            block.next = NULL;
        }

        static JMN_INLINE void MemoryHeapConnectFreeBlock(MemoryHeap &heap, MemoryHeap::Block &block, MemoryHeap::Block *prev, MemoryHeap::Block *next)
        {
            ConnectMemoryHeapBlock(block, prev, next);
            // If we're connecting a block to the beginning of the free list, we must update the heap's pointer to the beginning of the list
            // This happens when we provide no prev block and provided next block is the heap's free list pointer
            // (This logic only works because we already generically added the block to the list)
            if ((!prev) && (next == heap.free)) heap.free = &block;
        }

        static JMN_INLINE void MemoryHeapDisconnectFreeBlock(MemoryHeap &heap, MemoryHeap::Block &block)
        {
            // If we're disconnecting a block that is the heap's pointer to the free list, we advance the pointer to the next block (Null or not)
            if (heap.free == &block) heap.free = heap.free->next;
            DisconnectMemoryHeapBlock(block);
        }

        // Same logic as the free list helper
        static JMN_INLINE void MemoryHeapConnectUsedBlock(MemoryHeap &heap, MemoryHeap::Block &block, MemoryHeap::Block *prev, MemoryHeap::Block *next)
        {
            ConnectMemoryHeapBlock(block, prev, next);
            // If we're connecting a block to the beginning of the used list, we must update the heap's pointer to the beginning of the list
            // This happens when we provide no prev block and provided next block is the heap's used list pointer
            // (This logic only works because we already generically added the block to the list)
            if ((!prev) && (next == heap.used)) heap.used = &block;
        }

        // Same logic as the free list helper
        static JMN_INLINE void MemoryHeapDisconnectUsedBlock(MemoryHeap &heap, MemoryHeap::Block &block)
        {
            // If we're disconnecting a block that is the heap's pointer to the used list, we advance the pointer to the next block (Null or not)
            if (heap.used == &block) heap.used = heap.used->next;
            DisconnectMemoryHeapBlock(block);
        }

        // Returns the number of blocks that were combined
        static U8 CombineAdjacentBlocks(MemoryHeap::Block *prev, MemoryHeap::Block &curr, MemoryHeap::Block *next)
        {
            // Compute the blocks immediately adjacent to prev and curr blocks
            auto const prev_next = prev ? GetEndOfMemoryHeapBlock(*prev) : NULL; // If we provide no prev, we default to Null
            auto const curr_next =        GetEndOfMemoryHeapBlock( curr);

            if (prev)
            {
                if (prev_next == &curr) // The prev block is adjacent to curr
                {
                    if (curr_next == next) // The next block is adjacent to curr
                    {
                        // We keep prev, disconnect curr and next, and merge the sizes
                        DisconnectMemoryHeapBlock(curr);
                        prev->size += sizeof(MemoryHeap::Block);
                        prev->size += curr.size;
                        DisconnectMemoryHeapBlock(*next);
                        prev->size += sizeof(MemoryHeap::Block);
                        prev->size += next->size;
                        return 2;
                    }
                    else
                    {
                        // We keep prev, disconnect curr, and merge the sizes
                        DisconnectMemoryHeapBlock(curr);
                        prev->size += sizeof(MemoryHeap::Block);
                        prev->size += curr.size;
                        return 1;
                    }
                }
                else if (curr_next == next)  // The next block is adjacent to curr
                {
                    // We keep curr, disconnect next, and merge the sizes
                    DisconnectMemoryHeapBlock(*next);
                    curr.size += sizeof(MemoryHeap::Block);
                    curr.size += next->size;
                    return 1;
                }
            }
            else // There is no prev block, so we only check next
            {
                if (curr_next == next) // The next block is adjacent to curr
                {
                    // We keep curr, disconnect curr, and merge the sizes
                    DisconnectMemoryHeapBlock(*next);
                    curr.size += sizeof(MemoryHeap::Block);
                    curr.size += next->size;
                    return 1;
                }
            }
            return 0;
        }

    }

    JMN_HYDROGEN_IMPL_INLINE void MemoryArena::Initialize(Addr new_base, Size new_size)
    {
        base = new_base;
        used = 0;
        size = new_size;
    }

    JMN_HYDROGEN_IMPL_INLINE B8 MemoryArena::Push(Size bytes, Addr &addr, Result &result)
    {
        JMN_CHECK((used + bytes) <= size, result, Result::ErrorOutOfMemory, ex0);
        addr = base + used;
        used += bytes;
        return true;
    ex0:return false;
    }

    JMN_HYDROGEN_IMPL_INLINE B8 MemoryArena::Push(Size bytes, Size alignment, Addr &addr, Result &result)
    {
        Addr base_addr = base + used;
        Size offset = ComputeAlignmentOffset(base_addr, alignment);
        JMN_CHECK((used + offset + bytes) <= size, result, Result::ErrorOutOfMemory, ex0);
        used += offset;
        used += bytes;
        addr = base_addr + offset;
        return true;
    ex0:return false;
    }

    JMN_HYDROGEN_IMPL_INLINE void MemoryHeap::Initialize(Addr addr, Size size, Size mbs)
    {
        free           = (Block *)addr;
        used           = NULL;
        min_block_size = mbs;
        block_count    = 1;
        bytes_used     = 0;

        free->prev = NULL;
        free->next = NULL;
        free->offs = 0;
        free->size = size - sizeof(Block);
    }

    B8 MemoryHeap::Alloc(Size new_size, Size alignment, Addr &new_addr, Result &result)
    {
        new_addr = NullAddr;

        if (new_size) // 0 sized allocations aren't allowed
        {
            Block *curr_free = free;
            Block *prev_used = NULL;
            Block *next_used = used;
            while (curr_free != NULL)
            {
                // Advances the pointers of the adjacent used blocks to keep them centered on the curr free block
                while (next_used && next_used < curr_free)
                {
                    prev_used = next_used;
                    next_used = next_used->next;
                }

                auto const curr_addr = internal::GetMemoryHeapBlockAddress(*curr_free);
                auto const curr_offs = ComputeAlignmentOffset(curr_addr, alignment);
                auto const aligned_size = (Size)(curr_offs + new_size);
                if (aligned_size <= curr_free->size)
                {
                    auto const remaining_size = (Size)(curr_free->size - aligned_size);

                    // We split this free block if the resulting split is large enough to not hit the min block size threshold
                    if (remaining_size >= (sizeof(Block) + min_block_size))
                    {
                        auto const new_free = (Block *)(curr_addr + aligned_size);
                        new_free ->prev = NULL;
                        new_free ->next = NULL;
                        new_free ->offs = 0;
                        new_free ->size = remaining_size - sizeof(Block);
                        curr_free->size = aligned_size;
                        internal::MemoryHeapConnectFreeBlock(*this, *new_free, curr_free, curr_free->next);
                        block_count += 1;
                    }

                    internal::MemoryHeapDisconnectFreeBlock(*this, *curr_free);

                    auto const new_used = curr_free; // Variable used as a semantic rename
                    new_used->offs = curr_offs;
                    internal::MemoryHeapConnectUsedBlock(*this, *new_used, prev_used, next_used);

                    bytes_used += (sizeof(Block) + new_used->size);
                    new_addr = curr_addr + curr_offs;
                    break;
                }

                curr_free = curr_free->next;
            }

            JMN_CHECK(new_addr != NullAddr, result, Result::ErrorOutOfMemory, ex0);
        }
        return true;
    ex0:return false;
    }

    B8 MemoryHeap::Realloc(Addr old_addr, Size old_size, Size new_size, Size alignment, Addr &new_addr, Result &result)
    {
        new_addr = NullAddr;
        if (!Free(old_addr, old_size, old_size, result)) goto ex0;
        if (!Alloc(new_size, alignment, new_addr, result)) goto ex0;
        // Move the data if we changed addresses
        if (new_addr != old_addr) Move(new_addr, old_addr, JMN_MIN(old_size, new_size));
        return true;
    ex0:return false;
    }

    B8 MemoryHeap::Free(Addr old_addr, Size old_size, Size &free_size, Result &result)
    {
        (void)old_size;

        B8 found = false;
        if (old_addr)
        {
            Block *curr_used = used;
            Block *prev_free = NULL;
            Block *next_free = free;
            while (curr_used != NULL)
            {
                // Advances the pointers of the adjacent free blocks to keep them centered on the curr used block
                while (next_free && next_free < curr_used)
                {
                    prev_free = next_free;
                    next_free = next_free->next;
                }

                auto const curr_addr = internal::GetMemoryHeapBlockAddress(*curr_used);
                if (curr_addr == old_addr)
                {
                    free_size = curr_used->size - curr_used->offs;

                    internal::MemoryHeapDisconnectUsedBlock(*this, *curr_used);

                    auto const new_free = curr_used; // Variable used as a semantic rename
                    new_free->offs = 0;
                    internal::MemoryHeapConnectFreeBlock(*this, *new_free, prev_free, next_free);

                    bytes_used  -= (sizeof(Block) + new_free->size);
                    block_count -= internal::CombineAdjacentBlocks(prev_free, *new_free, next_free);
                    found = true;
                }

                curr_used = curr_used->next;
            }
        }
        // Freeing a null addr or invalid address is not necessarily an error, just a warning
        if (!found) result = Result::WarningNotFound;
        return true;
    }

}

#  endif // JMN_HYDROGEN_IMPLEMENTATED
#endif // JMN_HYDROGEN_IMPLEMENTATION
