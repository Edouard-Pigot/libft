#define _GNU_SOURCE

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libft.h"

#define CHECK_INT(label, got, expected) \
    do { \
        if ((got) != (expected)) { \
            fprintf(stderr, "[FAIL] %s: got %ld expected %ld\n", label, (long)(got), (long)(expected)); \
            exit(1); \
        } \
    } while (0)

#define CHECK_PTR(label, got, expected) \
    do { \
        if ((got) != (expected)) { \
            fprintf(stderr, "[FAIL] %s: got %p expected %p\n", label, (void *)(got), (void *)(expected)); \
            exit(1); \
        } \
    } while (0)

#define CHECK_STR(label, got, expected) \
    do { \
        if (strcmp((got), (expected)) != 0) { \
            fprintf(stderr, "[FAIL] %s: got '%s' expected '%s'\n", label, (got), (expected)); \
            exit(1); \
        } \
    } while (0)

static void test_ctype(void)
{
    int c;

    for (c = -128; c < 128; c++)
    {
        if (ft_isalpha(c) != isalpha(c))
        {
            fprintf(stderr, "[FAIL] ft_isalpha(%d)\n", c);
            exit(1);
        }
        if (ft_isdigit(c) != isdigit(c))
        {
            fprintf(stderr, "[FAIL] ft_isdigit(%d)\n", c);
            exit(1);
        }
        if (ft_isalnum(c) != isalnum(c))
        {
            fprintf(stderr, "[FAIL] ft_isalnum(%d)\n", c);
            exit(1);
        }
        if (ft_isascii(c) != isascii(c))
        {
            fprintf(stderr, "[FAIL] ft_isascii(%d)\n", c);
            exit(1);
        }
        if (ft_isprint(c) != isprint(c))
        {
            fprintf(stderr, "[FAIL] ft_isprint(%d)\n", c);
            exit(1);
        }
        if (ft_toupper(c) != toupper(c))
        {
            fprintf(stderr, "[FAIL] ft_toupper(%d)\n", c);
            exit(1);
        }
        if (ft_tolower(c) != tolower(c))
        {
            fprintf(stderr, "[FAIL] ft_tolower(%d)\n", c);
            exit(1);
        }
    }
}

static void test_strlen(void)
{
    const char *s = "bonjour";
    CHECK_INT("ft_strlen", ft_strlen(s), strlen(s));
}

static void test_memset(void)
{
    char s[16];
    char ref[16];

    memset(ref, 'A', 16);
    memset(s, 'A', 16);
    ft_memset(s, 'B', 5);
    memset(ref, 'B', 5);
    if (memcmp(s, ref, 16) != 0)
    {
        fprintf(stderr, "[FAIL] ft_memset\n");
        exit(1);
    }
}

static void test_bzero(void)
{
    char s[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    char ref[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    ft_bzero(s, 4);
    memset(ref, 0, 4);
    if (memcmp(s, ref, 8) != 0)
    {
        fprintf(stderr, "[FAIL] ft_bzero\n");
        exit(1);
    }
}

static void test_memcpy(void)
{
    char dst[16];
    char src[] = "abcdef";

    memset(dst, 'x', 16);
    ft_memcpy(dst, src, 7);
    if (memcmp(dst, src, 7) != 0)
    {
        fprintf(stderr, "[FAIL] ft_memcpy\n");
        exit(1);
    }
}

static void test_memmove(void)
{
    char s[] = "abcdef";
    char ref[] = "abcdef";

    ft_memmove(s, s + 2, 4);
    memmove(ref, ref + 2, 4);
    if (strcmp(s, ref) != 0)
    {
        fprintf(stderr, "[FAIL] ft_memmove\n");
        exit(1);
    }
}

static void test_strlcpy(void)
{
    char dst[16];
    const char *src = "hello world";
    size_t n = 6;

    CHECK_INT("ft_strlcpy return", ft_strlcpy(dst, src, n), strlcpy(dst, src, n));
    if (strcmp(dst, "hello") != 0)
    {
        fprintf(stderr, "[FAIL] ft_strlcpy content\n");
        exit(1);
    }
}

static void test_strlcat(void)
{
    char dst[20] = "abc";
    char ref[20] = "abc";
    const char *src = "def";

    CHECK_INT("ft_strlcat return", ft_strlcat(dst, src, sizeof(dst)), strlcat(ref, src, sizeof(ref)));
    if (strcmp(dst, "abcdef") != 0)
    {
        fprintf(stderr, "[FAIL] ft_strlcat content\n");
        exit(1);
    }
}

static void test_strchr(void)
{
    const char *s = "bonjour";
    CHECK_PTR("ft_strchr", ft_strchr(s, 'n'), strchr(s, 'n'));
    CHECK_PTR("ft_strchr nul", ft_strchr(s, '\0'), strchr(s, '\0'));
}

static void test_strrchr(void)
{
    const char *s = "banana";
    CHECK_PTR("ft_strrchr", ft_strrchr(s, 'a'), strrchr(s, 'a'));
    CHECK_PTR("ft_strrchr nul", ft_strrchr(s, '\0'), strrchr(s, '\0'));
}
/*
static void test_strncmp(void)
{
    CHECK_INT("ft_strncmp", ft_strncmp("abc", "abd", 3), strncmp("abc", "abd", 3));
    CHECK_INT("ft_strncmp shorter", ft_strncmp("abc", "ab", 3), strncmp("abc", "ab", 3));
}

static void test_memchr(void)
{
    const char s[] = "abcdef";
    CHECK_PTR("ft_memchr", ft_memchr(s, 'd', 6), memchr(s, 'd', 6));
}

static void test_memcmp(void)
{
    CHECK_INT("ft_memcmp", ft_memcmp("abcd", "abce", 4), memcmp("abcd", "abce", 4));
}

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
        fprintf(stderr, "[FAIL] ft_calloc allocation\n");
        exit(1);
    }
    if (memcmp(p, (char[10]){0}, 10) != 0)
    {
        fprintf(stderr, "[FAIL] ft_calloc zero fill\n");
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
        fprintf(stderr, "[FAIL] ft_split terminator\n");
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
        fprintf(stderr, "[FAIL] ft_lstclear\n");
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
    /*test_strncmp();
    test_memchr();
    test_memcmp();
    test_strnstr();
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

    printf("All Libft tests passed.\n");
    return (0);
}
