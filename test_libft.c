#define _GNU_SOURCE

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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
    for (c = 0; c < 128; c++)
    {
        CHECK_TRUTH("ft_isalpha", ft_isalpha(c), isalpha(c));
    }
    printf("=== TEST ft_isdigit()\n");
    for (c = 0; c < 128; c++)
    {
        CHECK_TRUTH("ft_isdigit", ft_isdigit(c), isdigit(c));
    }
    printf("=== TEST ft_isalnum()\n");
    for (c = 0; c < 128; c++)
    {
        CHECK_TRUTH("ft_isalnum", ft_isalnum(c), isalnum(c));
    }
    printf("=== TEST ft_isascii()\n");
    for (c = 0; c < 128; c++)
    {
        CHECK_TRUTH("ft_isascii", ft_isascii(c), isascii(c));
    }
    printf("=== TEST ft_isprint()\n");
    for (c = 0; c < 128; c++)
    {
        CHECK_TRUTH("ft_isprint", ft_isprint(c), isprint(c));
    }
    printf("=== TEST ft_toupper()\n");
    for (c = 0; c < 128; c++)
    {
        if (ft_toupper(c) != toupper(c))
            fprintf(stderr, "[FAIL] ft_toupper(%d): expected %d, current %d\n",
                c, toupper(c), ft_toupper(c));
    }
    printf("=== TEST ft_tolower()\n");
    for (c = 0; c < 128; c++)
    {
        if (ft_tolower(c) != tolower(c))
            fprintf(stderr, "[FAIL] ft_tolower(%d): expected %d, current %d\n",
                c, tolower(c), ft_tolower(c));
    }
}

static void test_strlen(void)
{
    printf("=== TEST ft_strlen()\n");
    const char *s = "bonjour";
    CHECK_INT("ft_strlen", ft_strlen(s), strlen(s));
}

static void test_memset(void)
{
    printf("=== TEST ft_memset()\n");
    char s[16];
    char ref[16];

    memset(ref, 'A', 16);
    memset(s, 'A', 16);
    ft_memset(s, 'B', 5);
    memset(ref, 'B', 5);
    check_bytes("ft_memset", s, ref, sizeof(s));
}

static void test_bzero(void)
{
    printf("=== TEST ft_bzero()\n");
    char s[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    char ref[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    ft_bzero(s, 4);
    memset(ref, 0, 4);
    check_bytes("ft_bzero", s, ref, sizeof(s));
}

static void test_memcpy(void)
{
    printf("=== TEST ft_memcpy()\n");
    char dst[16];
    char src[] = "abcdef";

    memset(dst, 'x', 16);
    ft_memcpy(dst, src, 7);
    check_bytes("ft_memcpy", dst, src, 7);
}

static void test_memmove(void)
{
    printf("=== TEST ft_memmove()\n");
    char s[] = "abcdef";
    char ref[] = "abcdef";

    ft_memmove(s, s + 2, 4);
    memmove(ref, ref + 2, 4);
    check_bytes("ft_memmove", s, ref, sizeof(s));
}

static void test_strlcpy(void)
{
    printf("=== TEST ft_strlcpy()\n");
    char dst[16];
    const char *src = "hello world";
    size_t n = 6;

    CHECK_INT("ft_strlcpy return", ft_strlcpy(dst, src, n), strlcpy(dst, src, n));
    CHECK_STR("ft_strlcpy content", dst, "hello");
}

static void test_strlcat(void)
{
    printf("=== TEST ft_strlcat()\n");
    char dst[20] = "abc";
    char ref[20] = "abc";
    const char *src = "def";

    CHECK_INT("ft_strlcat return", ft_strlcat(dst, src, sizeof(dst)), strlcat(ref, src, sizeof(ref)));
    CHECK_STR("ft_strlcat content", dst, "abcdef");
}

static void test_strchr(void)
{
    printf("=== TEST ft_strchr()\n");
    const char *s = "bonjour";
    CHECK_PTR("ft_strchr", ft_strchr(s, 'n'), strchr(s, 'n'));
    CHECK_PTR("ft_strchr nul", ft_strchr(s, '\0'), strchr(s, '\0'));
}

static void test_strrchr(void)
{
    printf("=== TEST ft_strrchr()\n");
    const char *s = "banana";
    CHECK_PTR("ft_strrchr", ft_strrchr(s, 'a'), strrchr(s, 'a'));
    CHECK_PTR("ft_strrchr nul", ft_strrchr(s, '\0'), strrchr(s, '\0'));
}

static void test_strncmp(void)
{
    printf("=== TEST ft_strncmp()\n");
    CHECK_ORDER("ft_strncmp", ft_strncmp("abc", "abd", 3), strncmp("abc", "abd", 3));
    CHECK_ORDER("ft_strncmp shorter", ft_strncmp("abc", "ab", 3), strncmp("abc", "ab", 3));
}

static void test_memchr(void)
{
    printf("=== TEST ft_memchr()\n");
    const char s[] = "abcdef";
    CHECK_PTR("ft_memchr", ft_memchr(s, 'd', 6), memchr(s, 'd', 6));
}

static void test_memcmp(void)
{
    printf("=== TEST ft_memcmp()\n");
    CHECK_ORDER("ft_memcmp", ft_memcmp("abcd", "abce", 4), memcmp("abcd", "abce", 4));
}
/*
static void test_strnstr(void)
{
    const char *big = "hello world";
    const char *little = "world";
    CHECK_PTR("ft_strnstr", ft_strnstr(big, little, 20), strstr(big, little));
}

static void test_atoi(void)
{
    CHECK_INT("ft_atoi", ft_atoi("-123"), atoi("-123"));
    CHECK_INT("ft_atoi plus", ft_atoi("+42"), atoi("+42"));
}

static void test_calloc(void)
{
    char *p;
    size_t n = 10;

    p = (char *)ft_calloc(n, sizeof(char));
    if (!p)
    {
        g_failures++;
        fprintf(stderr, "[FAIL] ft_calloc allocation: expected non-NULL allocation, current NULL\n");
        exit(1);
    }
    if (memcmp(p, (char[10]){0}, 10) != 0)
    {
        check_bytes("ft_calloc zero fill", p, (char[10]){0}, 10);
        free(p);
        exit(1);
    }
    free(p);
}

static void test_strdup(void)
{
    char *s = ft_strdup("test");
    CHECK_STR("ft_strdup", s, "test");
    free(s);
}

static void test_substr(void)
{
    char *s = ft_substr("abcdef", 2, 3);
    CHECK_STR("ft_substr", s, "cde");
    free(s);
}

static void test_strjoin(void)
{
    char *s = ft_strjoin("ab", "cd");
    CHECK_STR("ft_strjoin", s, "abcd");
    free(s);
}

static void test_strtrim(void)
{
    char *s = ft_strtrim("  abc  ", " ");
    CHECK_STR("ft_strtrim", s, "abc");
    free(s);
}

static void test_split(void)
{
    char **tab = ft_split("one two three", ' ');
    CHECK_STR("ft_split[0]", tab[0], "one");
    CHECK_STR("ft_split[1]", tab[1], "two");
    CHECK_STR("ft_split[2]", tab[2], "three");
    if (tab[3] != NULL)
    {
        g_failures++;
        fprintf(stderr, "[FAIL] ft_split terminator: expected NULL, current %p\n",
            (void *)tab[3]);
        exit(1);
    }
    free(tab[0]);
    free(tab[1]);
    free(tab[2]);
    free(tab);
}

static void test_itoa(void)
{
    char *s = ft_itoa(-123);
    CHECK_STR("ft_itoa", s, "-123");
    free(s);
}

static char ft_mapi_test(unsigned int i, char c)
{
    return (char)(c + i);
}

static void test_strmapi(void)
{
    char *s = ft_strmapi("abc", ft_mapi_test);
    CHECK_STR("ft_strmapi", s, "ace");
    free(s);
}

static void ft_iter_test(unsigned int i, char *c)
{
    (void)i;
    *c = *c + 1;
}

static void test_striteri(void)
{
    char s[] = "abc";
    ft_striteri(s, ft_iter_test);
    CHECK_STR("ft_striteri", s, "bcd");
}

static void test_putchar_fd(void)
{
    int fd = open(".libft_putchar_fd.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ft_putchar_fd('Z', fd);
    close(fd);
    fd = open(".libft_putchar_fd.out", O_RDONLY);
    char c;
    read(fd, &c, 1);
    close(fd);
    CHECK_INT("ft_putchar_fd", c, 'Z');
    unlink(".libft_putchar_fd.out");
}

static void test_putstr_fd(void)
{
    int fd = open(".libft_putstr_fd.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ft_putstr_fd("OK", fd);
    close(fd);
    fd = open(".libft_putstr_fd.out", O_RDONLY);
    char buf[8] = {0};
    read(fd, buf, 2);
    close(fd);
    CHECK_STR("ft_putstr_fd", buf, "OK");
    unlink(".libft_putstr_fd.out");
}

static void test_putendl_fd(void)
{
    int fd = open(".libft_putendl_fd.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ft_putendl_fd("END", fd);
    close(fd);
    fd = open(".libft_putendl_fd.out", O_RDONLY);
    char buf[16] = {0};
    read(fd, buf, 5);
    close(fd);
    CHECK_STR("ft_putendl_fd", buf, "END\n");
    unlink(".libft_putendl_fd.out");
}

static void test_putnbr_fd(void)
{
    int fd = open(".libft_putnbr_fd.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ft_putnbr_fd(-42, fd);
    close(fd);
    fd = open(".libft_putnbr_fd.out", O_RDONLY);
    char buf[32] = {0};
    read(fd, buf, 32);
    close(fd);
    CHECK_STR("ft_putnbr_fd", buf, "-42");
    unlink(".libft_putnbr_fd.out");
}

static void test_list(void)
{
    t_list *head = NULL;
    t_list *node1;
    t_list *node2;
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));

    *a = 10;
    *b = 20;

    node1 = ft_lstnew(a);
    node2 = ft_lstnew(b);
    ft_lstadd_front(&head, node1);
    ft_lstadd_back(&head, node2);

    CHECK_INT("ft_lstsize", ft_lstsize(head), 2);
    CHECK_INT("ft_lstlast value", *(int *)ft_lstlast(head)->content, 20);

    ft_lstclear(&head, free);
    if (head != NULL)
    {
        g_failures++;
        fprintf(stderr, "[FAIL] ft_lstclear: expected NULL, current %p\n",
            (void *)head);
        exit(1);
    }
}*/

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
    /*test_strnstr();
    test_atoi();
    test_calloc();
    test_strdup();
    test_substr();
    test_strjoin();
    test_strtrim();
    test_split();
    test_itoa();
    test_strmapi();
    test_striteri();
    test_putchar_fd();
    test_putstr_fd();
    test_putendl_fd();
    test_putnbr_fd();
    test_list();*/

    if (g_failures != 0)
    {
        fprintf(stderr, "%d test failure(s).\n", g_failures);
        return (1);
    }
    printf("All Libft tests passed.\n");
    return (0);
}
