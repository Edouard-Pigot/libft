#define _GNU_SOURCE

#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "libft.h"

static int g_failures;

static void print_bytes(FILE *stream, const unsigned char *bytes, size_t length)
{
    size_t i;

    for (i = 0; i < length; i++)
        fprintf(stream, "%s%02x", i == 0 ? "" : " ", bytes[i]);
}

static void check_bytes(const char *label, const void *current,
    const void *expected, size_t length)
{
    if (memcmp(current, expected, length) != 0)
    {
        g_failures++;
        fprintf(stderr, "[FAIL] %s: expected bytes [", label);
        print_bytes(stderr, expected, length);
        fprintf(stderr, "], current bytes [");
        print_bytes(stderr, current, length);
        fprintf(stderr, "]\n");
    }
}

#define CHECK_INT(label, got, expected) \
    do { \
        long current_value = (long)(got); \
        long expected_value = (long)(expected); \
        if (current_value != expected_value) \
        { \
            g_failures++; \
            fprintf(stderr, "[FAIL] %s: expected %ld, current %ld\n", label, expected_value, current_value); \
        } \
    } while (0)

#define CHECK_TRUTH(label, got, expected) \
    do { \
        int current_value = (got); \
        int expected_value = (expected); \
        if (!!current_value != !!expected_value) \
        { \
            g_failures++; \
            fprintf(stderr, "[FAIL] %s: expected %s (raw %d), current %s (raw %d)\n", \
                label, expected_value ? "true" : "false", expected_value, \
                current_value ? "true" : "false", current_value); \
        } \
    } while (0)

#define CHECK_ORDER(label, got, expected) \
    do { \
        int current_value = (got); \
        int expected_value = (expected); \
        int current_sign = (current_value > 0) - (current_value < 0); \
        int expected_sign = (expected_value > 0) - (expected_value < 0); \
        if (current_sign != expected_sign) \
        { \
            g_failures++; \
            fprintf(stderr, "[FAIL] %s: expected sign %d (raw %d), current sign %d (raw %d)\n", \
                label, expected_sign, expected_value, current_sign, current_value); \
        } \
    } while (0)

#define CHECK_PTR(label, got, expected) \
    do { \
        const void *current_value = (got); \
        const void *expected_value = (expected); \
        if (current_value != expected_value) \
        { \
            g_failures++; \
            fprintf(stderr, "[FAIL] %s: expected %p, current %p\n", label, expected_value, current_value); \
        } \
    } while (0)

#define CHECK_STR(label, got, expected) \
    do { \
        const char *current_value = (got); \
        const char *expected_value = (expected); \
        if (!current_value || !expected_value || strcmp(current_value, expected_value) != 0) \
        { \
            g_failures++; \
            fprintf(stderr, "[FAIL] %s: expected '%s', current '%s'\n", label, \
                expected_value ? expected_value : "(null)", \
                current_value ? current_value : "(null)"); \
        } \
    } while (0)

static void test_ctype(void)
{
    int c;

    printf("=== TEST ft_isalpha()\n");
    for (c = EOF; c <= UCHAR_MAX; c++)
    {
        CHECK_TRUTH("ft_isalpha", ft_isalpha(c), isalpha(c));
    }
    printf("=== TEST ft_isdigit()\n");
    for (c = EOF; c <= UCHAR_MAX; c++)
    {
        CHECK_TRUTH("ft_isdigit", ft_isdigit(c), isdigit(c));
    }
    printf("=== TEST ft_isalnum()\n");
    for (c = EOF; c <= UCHAR_MAX; c++)
    {
        CHECK_TRUTH("ft_isalnum", ft_isalnum(c), isalnum(c));
    }
    printf("=== TEST ft_isascii()\n");
    for (c = EOF; c <= UCHAR_MAX; c++)
    {
        CHECK_TRUTH("ft_isascii", ft_isascii(c), c >= 0 && c <= 127);
    }
    printf("=== TEST ft_isprint()\n");
    for (c = EOF; c <= UCHAR_MAX; c++)
    {
        CHECK_TRUTH("ft_isprint", ft_isprint(c), isprint(c));
    }
    printf("=== TEST ft_toupper()\n");
    for (c = EOF; c <= UCHAR_MAX; c++)
    {
        CHECK_INT("ft_toupper", ft_toupper(c), toupper(c));
    }
    printf("=== TEST ft_tolower()\n");
    for (c = EOF; c <= UCHAR_MAX; c++)
    {
        CHECK_INT("ft_tolower", ft_tolower(c), tolower(c));
    }
}

static void test_strlen(void)
{
    printf("=== TEST ft_strlen()\n");
    const char embedded_nul[] = {'a', 'b', '\0', 'c', '\0'};

    CHECK_INT("ft_strlen empty", ft_strlen(""), strlen(""));
    CHECK_INT("ft_strlen single", ft_strlen("x"), strlen("x"));
    CHECK_INT("ft_strlen ordinary", ft_strlen("bonjour"), strlen("bonjour"));
    CHECK_INT("ft_strlen stops at NUL", ft_strlen(embedded_nul),
        strlen(embedded_nul));
}

static void test_memset(void)
{
    printf("=== TEST ft_memset()\n");
    unsigned char actual[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    unsigned char expected[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    CHECK_PTR("ft_memset return", ft_memset(actual, 0x1ff, 5), actual);
    memset(expected, 0x1ff, 5);
    check_bytes("ft_memset count and byte conversion", actual, expected,
        sizeof(actual));
    CHECK_PTR("ft_memset zero return", ft_memset(actual, 0, 0), actual);
    check_bytes("ft_memset zero count unchanged", actual, expected,
        sizeof(actual));
}

static void test_bzero(void)
{
    printf("=== TEST ft_bzero()\n");
    unsigned char actual[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    unsigned char expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    ft_bzero(actual, 0);
    check_bytes("ft_bzero zero count unchanged", actual, expected,
        sizeof(actual));
    ft_bzero(actual, 4);
    memset(expected, 0, 4);
    check_bytes("ft_bzero partial", actual, expected, sizeof(actual));
    ft_bzero(actual, sizeof(actual));
    memset(expected, 0, sizeof(expected));
    check_bytes("ft_bzero whole buffer", actual, expected, sizeof(actual));
}

static void test_memcpy(void)
{
    printf("=== TEST ft_memcpy()\n");
    unsigned char dst[8] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
    const unsigned char src[4] = {0x00, 0x7f, 0x80, 0xff};
    unsigned char expected[8] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

    CHECK_PTR("ft_memcpy return", ft_memcpy(dst, src, sizeof(src)), dst);
    memcpy(expected, src, sizeof(src));
    check_bytes("ft_memcpy binary bytes", dst, expected, sizeof(dst));
    CHECK_PTR("ft_memcpy zero return", ft_memcpy(dst, src, 0), dst);
    check_bytes("ft_memcpy zero count unchanged", dst, expected, sizeof(dst));
}

static void test_memmove(void)
{
    printf("=== TEST ft_memmove()\n");
    char actual[] = "abcdef";
    char expected[] = "abcdef";

    CHECK_PTR("ft_memmove non-overlapping return",
        ft_memmove(actual + 4, actual, 2), actual + 4);
    memmove(expected + 4, expected, 2);
    check_bytes("ft_memmove non-overlapping", actual, expected,
        sizeof(actual));
    CHECK_PTR("ft_memmove overlapping return",
        ft_memmove(actual, actual + 2, 4), actual);
    memmove(expected, expected + 2, 4);
    check_bytes("ft_memmove destination before source", actual, expected,
        sizeof(actual));
    CHECK_PTR("ft_memmove overlapping return",
        ft_memmove(actual + 1, actual, 5), actual + 1);
    memmove(expected + 1, expected, 5);
    check_bytes("ft_memmove destination after source", actual, expected,
        sizeof(actual));
    CHECK_PTR("ft_memmove identical return",
        ft_memmove(actual, actual, sizeof(actual)), actual);
    CHECK_PTR("ft_memmove zero return", ft_memmove(actual, actual + 1, 0),
        actual);
}

static void check_strlcpy_case(const char *label, const char *src,
    const char *initial, size_t dsize)
{
    char actual[32];
    char expected[32];
    size_t actual_len;
    size_t expected_len;

    memset(actual, 0xa5, sizeof(actual));
    memset(expected, 0xa5, sizeof(expected));
    strcpy(actual, initial);
    strcpy(expected, initial);
    actual_len = ft_strlcpy(actual, src, dsize);
    expected_len = strlcpy(expected, src, dsize);
    CHECK_INT(label, actual_len, expected_len);
    check_bytes(label, actual, expected, sizeof(actual));
}

static void test_strlcpy(void)
{
    printf("=== TEST ft_strlcpy()\n");

    check_strlcpy_case("ft_strlcpy zero size", "hello", "old", 0);
    check_strlcpy_case("ft_strlcpy one-byte size", "hello", "old", 1);
    check_strlcpy_case("ft_strlcpy truncation", "hello world", "old", 6);
    check_strlcpy_case("ft_strlcpy exact fit", "hello", "old", 6);
    check_strlcpy_case("ft_strlcpy larger destination", "hello", "old", 12);
    check_strlcpy_case("ft_strlcpy empty source", "", "old", 8);
    check_strlcpy_case("ft_strlcpy empty source and size zero", "", "old", 0);
}

static void check_strlcat_case(const char *label, const char *initial,
    const char *src, size_t dsize)
{
    char actual[32];
    char expected[32];
    size_t actual_len;
    size_t expected_len;

    memset(actual, 0xa5, sizeof(actual));
    memset(expected, 0xa5, sizeof(expected));
    strcpy(actual, initial);
    strcpy(expected, initial);
    actual_len = ft_strlcat(actual, src, dsize);
    expected_len = strlcat(expected, src, dsize);
    CHECK_INT(label, actual_len, expected_len);
    check_bytes(label, actual, expected, sizeof(actual));
}

static void test_strlcat(void)
{
    printf("=== TEST ft_strlcat()\n");

    check_strlcat_case("ft_strlcat zero size", "abc", "de", 0);
    check_strlcat_case("ft_strlcat size shorter than destination", "abcdef",
        "xy", 3);
    check_strlcat_case("ft_strlcat size equals destination length", "abc",
        "de", 3);
    check_strlcat_case("ft_strlcat one-byte size", "", "de", 1);
    check_strlcat_case("ft_strlcat truncation", "abc", "def", 5);
    check_strlcat_case("ft_strlcat exact fit", "abc", "de", 6);
    check_strlcat_case("ft_strlcat larger destination", "abc", "de", 12);
    check_strlcat_case("ft_strlcat empty source", "abc", "", 12);
    check_strlcat_case("ft_strlcat empty destination", "", "abc", 8);
}

static void test_strchr(void)
{
    printf("=== TEST ft_strchr()\n");
    const char *s = "bonjour";

    CHECK_PTR("ft_strchr first match", ft_strchr(s, 'o'), strchr(s, 'o'));
    CHECK_PTR("ft_strchr", ft_strchr(s, 'n'), strchr(s, 'n'));
    CHECK_PTR("ft_strchr nul", ft_strchr(s, '\0'), strchr(s, '\0'));
    CHECK_PTR("ft_strchr missing", ft_strchr(s, 'z'), strchr(s, 'z'));
    CHECK_PTR("ft_strchr empty string", ft_strchr("", '\0'), strchr("", '\0'));
}

static void test_strrchr(void)
{
    printf("=== TEST ft_strrchr()\n");
    const char *s = "banana";
    CHECK_PTR("ft_strrchr", ft_strrchr(s, 'a'), strrchr(s, 'a'));
    CHECK_PTR("ft_strrchr nul", ft_strrchr(s, '\0'), strrchr(s, '\0'));
    CHECK_PTR("ft_strrchr missing", ft_strrchr(s, 'z'), strrchr(s, 'z'));
    CHECK_PTR("ft_strrchr empty string", ft_strrchr("", '\0'),
        strrchr("", '\0'));
}

static void test_strncmp(void)
{
    printf("=== TEST ft_strncmp()\n");
    const char high_byte[] = {(char)0x80, '\0'};
    const char ascii_byte[] = {(char)0x7f, '\0'};

    CHECK_ORDER("ft_strncmp zero count", ft_strncmp("a", "b", 0),
        strncmp("a", "b", 0));
    CHECK_ORDER("ft_strncmp equal", ft_strncmp("abc", "abc", 3),
        strncmp("abc", "abc", 3));
    CHECK_ORDER("ft_strncmp", ft_strncmp("abc", "abd", 3), strncmp("abc", "abd", 3));
    CHECK_ORDER("ft_strncmp shorter", ft_strncmp("abc", "ab", 3), strncmp("abc", "ab", 3));
    CHECK_ORDER("ft_strncmp difference outside limit",
        ft_strncmp("abc", "abd", 2), strncmp("abc", "abd", 2));
    CHECK_ORDER("ft_strncmp compares unsigned bytes",
        ft_strncmp(high_byte, ascii_byte, 1),
        strncmp(high_byte, ascii_byte, 1));
}

static void test_memchr(void)
{
    printf("=== TEST ft_memchr()\n");
    const unsigned char bytes[] = {0x41, 0x00, 0x80, 0xff, 0x41};

    CHECK_PTR("ft_memchr first match", ft_memchr(bytes, 0x41, sizeof(bytes)),
        memchr(bytes, 0x41, sizeof(bytes)));
    CHECK_PTR("ft_memchr zero byte", ft_memchr(bytes, 0, sizeof(bytes)),
        memchr(bytes, 0, sizeof(bytes)));
    CHECK_PTR("ft_memchr after zero byte",
        ft_memchr(bytes, 0xff, sizeof(bytes)),
        memchr(bytes, 0xff, sizeof(bytes)));
    CHECK_PTR("ft_memchr missing", ft_memchr(bytes, 0x42, sizeof(bytes)),
        memchr(bytes, 0x42, sizeof(bytes)));
    CHECK_PTR("ft_memchr zero count", ft_memchr(bytes, 0x41, 0),
        memchr(bytes, 0x41, 0));
    CHECK_PTR("ft_memchr match outside limit", ft_memchr(bytes + 2, 0x41, 2),
        memchr(bytes + 2, 0x41, 2));
}

static void test_memcmp(void)
{
    printf("=== TEST ft_memcmp()\n");
    const unsigned char with_zero[] = {0x00, 0xff};
    const unsigned char with_zero_later[] = {0x00, 0x01};
    const unsigned char high_byte[] = {0x80};
    const unsigned char lower_byte[] = {0x7f};

    CHECK_ORDER("ft_memcmp zero count", ft_memcmp("a", "b", 0),
        memcmp("a", "b", 0));
    CHECK_ORDER("ft_memcmp equal", ft_memcmp("abcd", "abcd", 4),
        memcmp("abcd", "abcd", 4));
    CHECK_ORDER("ft_memcmp difference", ft_memcmp("abcd", "abce", 4),
        memcmp("abcd", "abce", 4));
    CHECK_ORDER("ft_memcmp compares after zero",
        ft_memcmp(with_zero, with_zero_later, sizeof(with_zero)),
        memcmp(with_zero, with_zero_later, sizeof(with_zero)));
    CHECK_ORDER("ft_memcmp compares unsigned bytes",
        ft_memcmp(high_byte, lower_byte, sizeof(high_byte)),
        memcmp(high_byte, lower_byte, sizeof(high_byte)));
}

static void test_strnstr(void)
{
    const char *big = "hello world";
    const char repeated[] = "ababa";

    printf("=== TEST ft_strnstr()\n");
    CHECK_PTR("ft_strnstr match", ft_strnstr(big, "world", 20), big + 6);
    CHECK_PTR("ft_strnstr exact boundary", ft_strnstr(big, "world", 11),
        big + 6);
    CHECK_PTR("ft_strnstr match exceeds length",
        ft_strnstr(big, "world", 10), NULL);
    CHECK_PTR("ft_strnstr partial match exceeds length",
        ft_strnstr("ab", "ab", 1), NULL);
    CHECK_PTR("ft_strnstr missing", ft_strnstr(big, "mars", 20), NULL);
    CHECK_PTR("ft_strnstr empty needle", ft_strnstr(big, "", 0), big);
    CHECK_PTR("ft_strnstr empty haystack", ft_strnstr("", "a", 1), NULL);
    CHECK_PTR("ft_strnstr first of repeated matches",
        ft_strnstr(repeated, "aba", 5), repeated);
}

static void test_atoi(void)
{
    char min_string[32];

    printf("=== TEST ft_atoi()\n");
    CHECK_INT("ft_atoi negative", ft_atoi("-123"), atoi("-123"));
    CHECK_INT("ft_atoi plus", ft_atoi("+42"), atoi("+42"));
    CHECK_INT("ft_atoi leading whitespace",
        ft_atoi("\t\v\f\r\n  42"), atoi("\t\v\f\r\n  42"));
    CHECK_INT("ft_atoi negative zero", ft_atoi("-0"), atoi("-0"));
    CHECK_INT("ft_atoi no digits", ft_atoi("  +word"), atoi("  +word"));
    CHECK_INT("ft_atoi stops at non-digit",
        ft_atoi("123abc456"), atoi("123abc456"));
    CHECK_INT("ft_atoi sign without digits", ft_atoi("-"), atoi("-"));
    snprintf(min_string, sizeof(min_string), "%d", INT_MIN);
    CHECK_INT("ft_atoi int minimum", ft_atoi(min_string), atoi(min_string));
}

static void test_calloc(void)
{
    unsigned char *p;
    size_t i;

    printf("=== TEST ft_calloc()\n");
    p = (unsigned char *)ft_calloc(4, sizeof(*p) * 3);
    if (!p)
    {
        g_failures++;
        fprintf(stderr, "[FAIL] ft_calloc allocation: expected non-NULL allocation, current NULL\n");
    }
    else
    {
        for (i = 0; i < 12; i++)
        {
            if (p[i] != 0)
            {
                g_failures++;
                fprintf(stderr, "[FAIL] ft_calloc zero fill at byte %zu\n", i);
                break;
            }
        }
        free(p);
    }
    p = (unsigned char *)ft_calloc(0, 8);
    free(p);
    p = (unsigned char *)ft_calloc(8, 0);
    free(p);
    CHECK_PTR("ft_calloc multiplication overflow",
        ft_calloc(SIZE_MAX, 2), NULL);
}

static void test_strdup(void)
{
    char source[] = "test";
    char *copy;

    printf("=== TEST ft_strdup()\n");
    copy = ft_strdup("");
    CHECK_STR("ft_strdup empty", copy, "");
    if (copy)
        free(copy);
    copy = ft_strdup(source);
    CHECK_STR("ft_strdup", copy, "test");
    if (copy)
    {
        copy[0] = 'b';
        CHECK_STR("ft_strdup copy mutation", copy, "best");
        CHECK_STR("ft_strdup independent copy", source, "test");
        free(copy);
    }
}

// static void test_substr(void)
// {
//     char *s = ft_substr("abcdef", 2, 3);
//     CHECK_STR("ft_substr", s, "cde");
//     free(s);
// }

// static void test_strjoin(void)
// {
//     char *s = ft_strjoin("ab", "cd");
//     CHECK_STR("ft_strjoin", s, "abcd");
//     free(s);
// }

// static void test_strtrim(void)
// {
//     char *s = ft_strtrim("  abc  ", " ");
//     CHECK_STR("ft_strtrim", s, "abc");
//     free(s);
// }

// static void test_split(void)
// {
//     char **tab = ft_split("one two three", ' ');
//     CHECK_STR("ft_split[0]", tab[0], "one");
//     CHECK_STR("ft_split[1]", tab[1], "two");
//     CHECK_STR("ft_split[2]", tab[2], "three");
//     if (tab[3] != NULL)
//     {
//         g_failures++;
//         fprintf(stderr, "[FAIL] ft_split terminator: expected NULL, current %p\n",
//             (void *)tab[3]);
//         exit(1);
//     }
//     free(tab[0]);
//     free(tab[1]);
//     free(tab[2]);
//     free(tab);
// }

// static void test_itoa(void)
// {
//     char *s = ft_itoa(-123);
//     CHECK_STR("ft_itoa", s, "-123");
//     free(s);
// }

// static char ft_mapi_test(unsigned int i, char c)
// {
//     return (char)(c + i);
// }

// static void test_strmapi(void)
// {
//     char *s = ft_strmapi("abc", ft_mapi_test);
//     CHECK_STR("ft_strmapi", s, "ace");
//     free(s);
// }

// static void ft_iter_test(unsigned int i, char *c)
// {
//     (void)i;
//     *c = *c + 1;
// }

// static void test_striteri(void)
// {
//     char s[] = "abc";
//     ft_striteri(s, ft_iter_test);
//     CHECK_STR("ft_striteri", s, "bcd");
// }

// static void test_putchar_fd(void)
// {
//     int fd = open(".libft_putchar_fd.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     ft_putchar_fd('Z', fd);
//     close(fd);
//     fd = open(".libft_putchar_fd.out", O_RDONLY);
//     char c;
//     read(fd, &c, 1);
//     close(fd);
//     CHECK_INT("ft_putchar_fd", c, 'Z');
//     unlink(".libft_putchar_fd.out");
// }

// static void test_putstr_fd(void)
// {
//     int fd = open(".libft_putstr_fd.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     ft_putstr_fd("OK", fd);
//     close(fd);
//     fd = open(".libft_putstr_fd.out", O_RDONLY);
//     char buf[8] = {0};
//     read(fd, buf, 2);
//     close(fd);
//     CHECK_STR("ft_putstr_fd", buf, "OK");
//     unlink(".libft_putstr_fd.out");
// }

// static void test_putendl_fd(void)
// {
//     int fd = open(".libft_putendl_fd.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     ft_putendl_fd("END", fd);
//     close(fd);
//     fd = open(".libft_putendl_fd.out", O_RDONLY);
//     char buf[16] = {0};
//     read(fd, buf, 5);
//     close(fd);
//     CHECK_STR("ft_putendl_fd", buf, "END\n");
//     unlink(".libft_putendl_fd.out");
// }

// static void test_putnbr_fd(void)
// {
//     int fd = open(".libft_putnbr_fd.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     ft_putnbr_fd(-42, fd);
//     close(fd);
//     fd = open(".libft_putnbr_fd.out", O_RDONLY);
//     char buf[32] = {0};
//     read(fd, buf, 32);
//     close(fd);
//     CHECK_STR("ft_putnbr_fd", buf, "-42");
//     unlink(".libft_putnbr_fd.out");
// }

// static void test_list(void)
// {
//     t_list *head = NULL;
//     t_list *node1;
//     t_list *node2;
//     int *a = malloc(sizeof(int));
//     int *b = malloc(sizeof(int));

//     *a = 10;
//     *b = 20;

//     node1 = ft_lstnew(a);
//     node2 = ft_lstnew(b);
//     ft_lstadd_front(&head, node1);
//     ft_lstadd_back(&head, node2);

//     CHECK_INT("ft_lstsize", ft_lstsize(head), 2);
//     CHECK_INT("ft_lstlast value", *(int *)ft_lstlast(head)->content, 20);

//     ft_lstclear(&head, free);
//     if (head != NULL)
//     {
//         g_failures++;
//         fprintf(stderr, "[FAIL] ft_lstclear: expected NULL, current %p\n",
//             (void *)head);
//         exit(1);
//     }
// }

int main(void)
{
    test_ctype();
    test_strlen();
    test_memset();
    test_bzero();
    test_memcpy();
    test_memmove();
    test_strlcpy();
    test_strlcat();
    test_strchr();
    test_strrchr();
    test_strncmp();
    test_memchr();
    test_memcmp();
    test_strnstr();
    test_atoi();
    test_calloc();
    test_strdup();
    // test_substr();
    // test_strjoin();
    // test_strtrim();
    // test_split();
    // test_itoa();
    // test_strmapi();
    // test_striteri();
    // test_putchar_fd();
    // test_putstr_fd();
    // test_putendl_fd();
    // test_putnbr_fd();
    // test_list();

    if (g_failures != 0)
    {
        fprintf(stderr, "%d test failure(s).\n", g_failures);
        return (1);
    }
    printf("All Libft tests passed.\n");
    return (0);
}
