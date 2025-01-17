#include "enclyzer/libenclyzer/flush_reload.h"

/**
 * @brief the defines and functions that are exclusive to trusted libraries
 * 
 */
#ifdef NAMESPACE_SGX_YES

#include "enclyzer/libenclyzer/flush_reload_t.h"

#endif

/**
 * @brief the defines and functions that are exclusive to untrusted libraries
 * 
 */
#ifdef NAMESPACE_SGX_NO

#include "enclyzer/libenclyzer/flush_reload_u.h"

#include <ctype.h> // isprint function

void flush(buffer_t *encoding_buffer, buffer_t *printing_buffer)
{
    int i;

    for (i = 0; i < encoding_buffer->size; i += ENCODING_BUFFER_SLOT_SHIFT)
    {
        asm volatile("clflush (%0)\n" ::"r"(encoding_buffer->buffer + i));
    }

    asm volatile("mfence\n");
}

/**
 * @brief Get the time used to access the memory address, which indicates its location 
 * in the memory hierarchy.
 * 
 * @param address the memory address to be accessed.
 * @return the time used to access the memory address.
 * 
 * @see How to Benchmark Code Execution Times on Intel® IA-32 and IA-64 
 *     Instruction Set Architectures
 */
static uint32_t access_time(uint64_t address)
{
    uint32_t cycles;

    asm volatile(
        "cpuid\n"
        "rdtsc\n"
        "movl %%eax, %0\n"
        "movq (%1), %%rax\n"
        "rdtscp\n"
        "subl %0, %%eax\n"
        "movl %%eax, %0\n"
        "cpuid\n"
        : "=r"(cycles), "+r"(address)
        :
        : "rax", "rbx", "rcx", "rdx");

    return cycles;
}

void reload(buffer_t *encoding_buffer, buffer_t *printing_buffer)
{
    int i;
    uint64_t dt;

    asm volatile("mfence\n");

    for (i = 0; i < encoding_buffer->size; i += ENCODING_BUFFER_SLOT_SIZE)
    {
        dt = access_time((uint64_t)(encoding_buffer->buffer + i));
        if (dt < TIME_LIMIT)
        {
            printing_buffer->buffer[i / ENCODING_BUFFER_SLOT_SIZE]++;
        }
    }
}

void reset(buffer_t *printing_buffer)
{
    int i;

    for (i = 0; i < printing_buffer->size; i++)
    {
        printing_buffer->buffer[i] = 0;
    }
}

void print(buffer_t *printing_buffer, uint8_t printing_bar)
{
    int i;

    printf("{--------------------\n");
    for (i = 0; i < printing_buffer->size; i++)
    {
        if (printing_buffer->buffer[i] > printing_bar)
        {
            printf("%08u: %02x (%c)\n", printing_buffer->buffer[i], (uint32_t)i,
                   isprint(i) ? (uint32_t)i : '?');
        }
    }
    printf("--------------------}\n");

    reset(printing_buffer);
}

#endif

/**
 * @brief the defines and functions that are shared by trusted libraries and untrusted libraries
 * 
 */
#ifdef NAMESPACE_SGX_SHARED

#endif