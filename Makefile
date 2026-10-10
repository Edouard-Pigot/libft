NAME = test_libft
LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
TEST_CFLAGS = $(CFLAGS) -Wno-unused-function

LIBFT_SRCS = $(wildcard $(LIBFT_DIR)/*.c)
LIBFT_HEADERS = $(wildcard $(LIBFT_DIR)/*.h)

all: $(NAME)

$(LIBFT): $(LIBFT_SRCS) $(LIBFT_HEADERS) $(LIBFT_DIR)/Makefile
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): test_libft.c $(LIBFT) Makefile
	$(CC) $(TEST_CFLAGS) -I$(LIBFT_DIR) test_libft.c $(LIBFT) -o $@

test: $(NAME)
	./$(NAME)

clean:
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all test clean fclean re
