
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PIPE
#  define PIPE 124
# endif
# ifndef D
#  define D 34
# endif
# ifndef S
#  define S 39
# endif
# define SEP		1
# define COM		2
# define REDIR_OUT	3
# define REDIR_IN	4
# define HERE_DOC	5

extern int			minishell_global;

typedef struct s_dlist
{
	char			*str;
	int				quote;
	struct s_dlist	*prev;
	struct s_dlist	*next;
}					t_dlist;

typedef struct s_tlist
{
	char			*comd;
	char			**flargs;
	char			*op;
	char			**env;
	char			**path;
	pid_t			pid;
	char			*doc;
	struct s_tlist	*prev;
	struct s_tlist	*next;
}					t_tlist;

// Valeria'part
typedef struct s_env
{
	char			*str;
	struct s_env	*next;
}					t_env;


typedef struct s_tokens
{
	char			*val;
	int				type;
	int				ind_command;
	int				ind_word;
	struct s_tokens	*next;
	struct s_tokens	*prev;
}		t_tokens;

typedef struct s_data
{
	char			**comd;
	char			*comd_line;
	char			end_of_cmd;
	int				index;
	char			*check_input;
	int				i;
	int				j;
	int				at;
	int				exit;
	pid_t			*pid;
	int				out;
	// valeria's
	int				exit_st;
	t_env			*env;
	t_env			*env_orig;
}					t_data;

typedef struct s_pipex
{
	pid_t			*pids;
	int				ac;
	int				redir_in;
	int				redir_out;
	int				rem_fd;
	int				pipes[2];
	int				here_doc;
	char			*paths;
	char			*command;
	char			**args;
	char			**com_paths;
	char			**valid_env;
	t_tokens		*tokens;
	t_data			*data;
}					t_pipex;

/* env.c */
int					ft_store_env(t_data *data, char **env_orig, char structt);
int					ft_free_env(t_env *env);
int					ft_env_init(t_data *data, char **env);

/* built ins */
int					ft_env(t_data *data, t_pipex *list);
int					ft_pwd(t_data *data, t_pipex *list);

// free.c
int					free_n_exit(t_data *d, int i);
void				*save_free(void *s1, void *s2);
char				*ft_strjoin_free(char const *s1, char const *s2);

// expander.c
char				*ft_expander(char *str, t_data *data);

// input_check.c
int					syntax_errors(t_data *d, char c);
int					check_input(t_data *d, char *s);

// input_utils.c
int					ft_strcmp(char *s1, char *s2);

/* exec.c */
int					ft_exec(int ac, char **av, t_data *data);

/* exec_utils.c */
char				*ft_bcheck_paths(t_data *data, t_env *env);
char				**ft_env_to_twod_arr(t_data *data, t_env *env_list);
void				ft_check_kid(int i, t_pipex *list);
char				*ft_gimme_com(char *str, t_pipex *list);
char				**ft_tok_to_args(t_tokens *toks, int i);

/* redirects.c */
void				ft_redirects(int i, t_tokens **toks_orig, t_pipex *list);

/* tokenizing.c */
t_tokens    		*ft_gimme_tokens(char **strs);

/* builtins.c */
void				ft_check_builtins(t_pipex *list);
void				ft_builtins_p(int i, t_tokens *toks);

/* main.c */
int					ft_count_words(char **av);

// quote.c
int					is_quote(int c);
int					ft_is_space(char s);
int					check_open_quote(t_data *d, char *s);

// singal.c
void				get_sigint(int sig);
void				get_sigint_child(int sig);

// token.c
t_dlist				*ft_create_dlist(char *s, int quote);
void				ft_add_dlist_back(t_dlist **dl, char *str, int quote);
void				ft_token_loop(char *s, int *q, int *i, int **sum_q);

/* utils.c */
int					ft_find_index(char *s, char c);
void				ft_list_free(t_pipex *list);
int					ft_error_msg(t_data *data, char *msg, int msg_len);
char				*ft_strjoin_char(char *str, char c);

char				**ft_command_split(char *s);
char				**ft_remove_quotes(char **av);

#endif