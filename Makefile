NAME = minishell

CC = gcc

SRCS =	main.c \
		termfuncs_one.c \
        termfuncs_two.c \
        preparser_one.c \
        preparser_two.c \
        parser.c \
        parse_quotes.c \
        parse_another.c \
        parse_dollar.c \
        parse_utils_one.c \
        parse_utils_two.c \
        parse_utils_three.c \
        initialize.c \
		exec_1.c \
		signals_1.c \
		signals_2.c \
		utils_1.c \
		utils_2.c \
		utils_3.c \
		lists_env_1.c \
		lists_env_2.c \
		lists_env_3.c \
		cmd_path_exec_1.c \
		cmd_path_exec_2.c \
		cmd_path_exec_3.c \
		cmd_path_exec_4.c \
		builtins_1.c \
		builtins_2.c \
		builtins_3.c \
		builtins_4.c \
		builtins_5.c \
		builtins_6.c \
		pps_n_cmd_1.c \
		pps_n_cmd_2.c \
		pps_n_cmd_3.c \
		pps_n_cmd_4.c \
		pps_n_cmd_5.c \
		pps_n_cmd_6.c

OBJS = $(SRCS:.c=.o)

.c.o:
	$(CC) -o $@ -c $^

all: $(NAME)

$(NAME): $(OBJS)
	make bonus -C libft/
	rm -rf $(NAME)
	$(CC) $(OBJS) libft/libft.a -o $(NAME) -ltermcap

debug:
	gcc -g $(SRCS) libft/libft.a -ltermcap

clean:
	make clean -C libft/
	rm -f $(OBJS)

fclean:
	make fclean -C libft/
	rm -f $(OBJS)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
