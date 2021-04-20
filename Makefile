NAME = minishell

CC = gcc

SRCS =	minishell.c \
		main.c \
		lists_env.c \
		exec.c \
		pps_n_cmd.c \
		cmd_path_exec.c \
		signals.c \
		utils.c \
		builtins.c

OBJS = $(SRCS:.c=.o)

.c.o:
	$(CC) -o $@ -c $^

all: $(NAME)

$(NAME): $(OBJS)
	# make bonus -C libft/
	rm -rf $(NAME)
	$(CC) $(OBJS) libft/libft.a -o $(NAME) -ltermcap

debug:
	gcc -g $(SRCS) libft/libft.a -ltermcap

clean:
	# make clean -C libft/
	rm -f $(OBJS)

fclean:
	# make fclean -C libft/
	rm -f $(OBJS)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
