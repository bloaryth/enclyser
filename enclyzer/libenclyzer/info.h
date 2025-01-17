#ifndef enclyzer_LIBenclyzer_INFO

#define enclyzer_LIBenclyzer_INFO

/**
 * @brief the defines and functions that are exclusive to trusted libraries
 *
 */
#ifdef NAMESPACE_SGX_YES

/**
 * @brief Invokes OCALL to display the enclave buffer to the terminal.
 *
 * @param fmt the format string to written to stdout
 * @param ... a list of values to be used to replace a format specifier in the format string
 * @return int On success, the total number of characters written is returned.
 */
int printf(const char *fmt, ...);

#endif

/**
 * @brief the defines and functions that are exclusive to untrusted libraries
 *
 */
#ifdef NAMESPACE_SGX_NO

#endif

/**
 * @brief the defines and functions that are shared by trusted libraries and untrusted libraries
 *
 */
#ifdef NAMESPACE_SGX_SHARED

#include <stdio.h>
#include <errno.h>

#define ASSERT(cond)                                                                \
    do                                                                              \
    {                                                                               \
        if (!(cond))                                                                \
        {                                                                           \
            printf("%s: %x", "[" __FILE__ "] assertion '" #cond "' failed", errno); \
            abort();                                                                \
        }                                                                           \
    } while (0)

#define EXPECT(cond)                                                                \
    do                                                                              \
    {                                                                               \
        if (!(cond))                                                                \
        {                                                                           \
            printf("%s: %x", "[" __FILE__ "] assertion '" #cond "' failed", errno); \
        }                                                                           \
    } while (0)

#define INFO(msg, ...)                                     \
    do                                                     \
    {                                                      \
        printf("[" __FILE__ "] " msg "\n", ##__VA_ARGS__); \
    } while (0)

#endif

#endif