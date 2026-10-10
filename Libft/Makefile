NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar
ARFLAGS = rcs
SRCS = $(wildcard *.c)
BUILD_DIR = build
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

all: $(NAME)

bonus: all

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.o: %.c libft.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re