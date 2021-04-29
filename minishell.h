#ifndef MINISHELL_H
# define MINISHELL_H

# include <curses.h>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/mman.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <termios.h>
# include "libft/libft.h"
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <term.h>
# include <sys/errno.h>
# include <sys/stat.h>

# define PROMT "\033[32mbush-0.0$ \033[0m"
# define ERROR "error: \0"
# define HISTORY "/goinfre/bush_hist.txt"

typedef struct			s_kv
{
	void				*key;
	void				*value;
	struct s_kv			*next;
}						t_kv;

typedef struct			s_shell
{
	//----------------------------
	char				**env_args;
	char				**cmd_n_args;
	int					fl_arg[1024];
	int					err[2];
	int					cntr;
	//----------------------------
	int					num_args;
	char				*prev_home;
	char				*prev_pwd;
	char				**prev_path;
	char				*input;
	char				*p_in;
	char				*p_out;
	int					fl_rdr_in;
	int					fl_rdr_out;
	int					fl_rdr_app;
	int					pp_l[2];
	int					pp_r[2];
	int					*ind_arg;
	int					pps;
	int					len_env;
	int					stts;
	int					in_fd;
	int					out_fd;
	int					rdr_i;
	char				*buf;
	char				*parsed_buf;
	char				**hist;
	char				**env;
	char				**dup_env;
	int					env_n_l;
	int					sq;
	int					dq;
	int					f;
	int					ff[3];
	int					cc;
	int					isfound;
	int					pips;
	t_list				*ls_hist;
	t_list				*list;
	t_kv				*kv;
	t_kv				*kv_head;
	char				*exp_key;
	char				*exp_val;
}						t_shell;

//Global variables
int		g_exit;

// LOGIC

void	exec(t_shell *shell);
int		len_of_2d_array(t_shell *shell);
int		tokens(t_shell *shell);
int		pps_exc(t_shell *shell);
int 	old_ord(t_shell *shell);
int		new_ord(t_shell *shell);
void	manage_order(t_shell *shell, int i);
int		chk_tkn(char *str, char *line);
int		cmd_exec(t_shell *shell);
void	parent(t_shell *shell, int i);
void	child(t_shell *shell, int i);
void	pps_free(t_shell *shell);
int		flag_chk(t_shell *shell);
int		fl_chk_rdr(t_shell *shell, int i);
void	num_of_rdrs(t_shell *shell);
void	init(t_shell *shell);
int		len_of_2d_array(t_shell *shell);
int		chk_tkn(char *str, char *line);
void	err_out(char *str);
int		exec_other_bins(t_shell * shell);
void	signals_start(int cat_time);
void	disp_cmd_ctrl_slash(int fr);
void	cmd_ctrl_slash(int fr);
void	disp_cmd_crtl_c(int sg);
void	cmd_crtl_c(int sg);
void	get_env(char **env, t_shell *sh);
char	*find_key_value(char *key, t_kv **kv);
void	exec_child(t_shell *shell, char *cmd_n_path, int stts);
void	exec_parent(t_shell *shell, int stts);
void	err_cmd(t_shell *shell);
char	*bin_path(t_shell *shell, char *cmd);
int		file_dir(char *cmd);
char	*cmd_chk(char **path, char *cmd);
int		rdr_in(t_shell *shell, int i);
int		rdr_out(t_shell *shell);
int		rdr_app(t_shell *shell);
void	pps_fd(t_shell *shell, int i);
void	pps_fd_1(t_shell *shell);
void	pps_fd_2(t_shell *shell);
void	pps_fd_3(t_shell *shell);
void	cmd_not_found(t_shell *shell);
void	close_change_fd(t_shell *shell, int i);
int		chk_pps_tkns(t_shell *shell, int i);
int		chk_pps_tkns_2(t_shell *shell, int i);
int		flag_on(t_shell *shell, char **path, int i, int *fl);
int		op_path(t_shell *shell, int fd, char **path);
void	builtins(t_shell * shell);
void	mini_echo(char **cmd_n_args);
void	mini_pwd(t_shell *shell);
void	mini_env(t_shell *shell);
void	mini_cd(t_shell *shell);
char	**ft_shell_lvl(char **env);
void	display_export(t_shell *shell);
void	mini_export(t_shell *shell);
void	export_add(t_shell *shell);
void	export_add_2(t_shell *shell);
void	export_value(t_shell *shell, int *i, int *j);
t_kv	*ft_lstkv(void *key, void *value);
void	ft_kvadd_back(t_kv **lst, t_kv *new);
t_kv	*ft_kvlast(t_kv *lst);
void	mini_unset(t_shell *shell);
void	mini_unset_2(t_shell *shell, t_kv **sacrifice, t_kv **tmp, int i);
void	err_is_dir();
void	err_perm_deny();
void	err_no_file_dir();
void	err_cmd_n_found();
void	err_num(int rax, char **cmd_n_args);
int		neg_pos_dig(char *cmd_n_args, int pos_neg);
void	mini_exit(char **cmd_n_args);
int		ex_args(char **cmd_n_args);
void	err_no_dir_cd(char *dir);
int		check_path(t_shell *shell, char **home);
void	parse_env(t_shell *sh);
char	**ft_freeall(char **arr);

//PARSER
//Term functions and history
void    term_func(t_shell *sh);
void    print_promt(t_shell *sh);
void    setup_term(t_shell *sh);
void    term_backup(int i);
int     up_and_down(t_shell *sh, int pos);
void    backslash_and_tab(t_shell *sh, int pos, int i);
void    history_record_and_call_parser(t_shell *sh, int pos);
void    join_buf(char **hist, char *command, t_shell *sh);
void    get_hist(t_shell *sh);
int		break_chr(char c);

//Preparsing
void    preparser(t_shell *sh);
void    check_shielding(char *line, t_shell *sh);
void    check_quotes(char *line, t_shell *sh);
void    check_redirects(char *line, t_shell *sh);
void    check_pipes(char *line, t_shell *sh);
void    check_semicolons(char *line, t_shell *sh);
//Parsing
void    parser(t_shell *sh);
void    processor(t_shell *sh, char *line);
int     parse_if_dollar(t_shell *sh, char *line, int i);
int     parse_if_double_quotes(t_shell *sh, char *line, int i);
int     parse_if_single_quotes(t_shell *sh, char *line, int i);
int     parse_if_all_the_rest(t_shell *sh, char *line, int i);
void    one_two_dollars(t_shell *sh, char **str, char **oldstr, int *j);
char    *dollar_one(t_shell *sh, char *str, char *oldstr, int j);
char    *dollar_two(t_shell *sh, char *str, int *j);
char    *replace_env_n_value(t_shell *sh,
char *str, char *env_key, char *env_value);
void    save_str_to_structure(t_shell *sh, char *str);
void    save_str_or_oldstr(t_shell *sh, char *str, char *oldstr);
void    switch_space(t_shell *sh, int one, int two, int three);
void    history_record_and_call_parser(t_shell *sh, int pos);
//Utils
void    initialize(t_shell *sh);
void    qfl(char *line, int i, t_shell *sh);
int     num_of_chars(char *s, int c);
int     remove_backslash(char *line, int i);
int     ft_strslen(char **line);
int     skipspaces(char *s);
void    do_exit(char *msg);
void    print_error(const char *error_msg, t_shell *sh);
void    print_cmd_error(const char *error_msg, t_shell *sh);
void    errcode(t_shell *sh, int i, int errcode);
void    free_maker(char **str);
int     len_2d(char **array);

#endif
