/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yerilee <yerilee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 16:43:57 by yerilee           #+#    #+#             */
/*   Updated: 2023/10/12 16:43:57 by yerilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# define WORD 1
# define PIPE 2
# define AMPERSAND 3
# define SEMICOLON 4
# define REDIRECTION 5
# define PARENTHESIS 6

typedef struct s_gvar
{
	int	g_exit_status;
}	t_gvar;

typedef struct s_lexer
{
	char			*val;
	int				type;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_env
{
	char			*key;
	char			*val;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

typedef struct s_cmd
{
	char			**cmd;
	int				fd_in;
	int				fd_out;
	int				heredoc_num;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_split
{
	char	**p;
	int		len;
	int		d_flag;
	int		s_flag;
}	t_split;

typedef struct s_data
{
	int		ac;
	char	**av;
	char	**env;
	char	**paths;
	char	**end;
	char	*cmd;
	int		heredoc_num;
	t_lexer	*lexer_list;
	t_env	*env_list;
	t_cmd	*cmd_list;
}	t_data;

char	*readline(const char *prompt);

/* Libft */
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, char *src, size_t dstsize);
char	*ft_strtrim(char *s1, char *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(char *s1, char *s2, size_t n);
char	*ft_strjoin(char *s1, char *s2);

/* lexer */
void	lexer(t_data *data);
void	ft_free_lexer(t_lexer *lexer);
void	print_lexer_list(t_lexer *lexer);

t_lexer	*new_lexer_node(char *token, int type);
t_lexer	*add_token_to_lexer(t_lexer *lexer, char *token, int type);

int		is_space(char c);
int		is_word(char c);
int		is_redirection(char c);
int		is_parenthesis(char c);
int		ft_word_len(char *cmd, int i);
int		add_word(t_data *data, int i);
int		add_pipe(t_data *data, int i);
int		add_ampersand(t_data *data, int i);
int		add_semicolon(t_data *data, int i);
int		add_redirection(t_data *data, int i);
int		add_parenthesis(t_data *data, int i);

/* syntax */
int		check_syntax(t_lexer *lexer);
int		check_token_start(t_lexer *lexer);

int		check_pipe(t_lexer *lexer);
int		check_pipe_start_end(t_lexer *lexer);
int		check_pipe_len(t_lexer *lexer);
int		check_command_between_pipes(t_lexer *lexer);

int		check_quotes(t_lexer *lexer);
int		check_closed_quote(int double_flag, int single_flag);

int		check_redirection(t_lexer *lexer);
int		check_redirection_end(t_lexer *lexer);
int		check_redirection_len(t_lexer *lexer);
int		check_command_after_redirection(t_lexer *lexer);

/* env */
t_env	*new_env_node(char *str);
t_env	*add_env_to_list(t_env *env, char *str);
void	print_env_list(t_env *env);

/* expanding */
int		has_variable(char *value);
void	expanding(t_data *data);
void	ft_expanding(t_data *data, t_lexer *lexer);

int		is_valid_env(t_data *data, char *var);
int		is_alnum(int c);
char	*find_env(char *str, int *i);
char	*get_value(t_data *data, char *key);

int		len_before_env(char *value, int *i);
int		len_after_env(char *value);
int		location_after_env(int i, char *value);

char	*str_after_env(char *value);
char	*strjoin_after_env(char *replaced_str, int i, char *lexer);
char	*str_without_env(char *lexer, char *replaced_val);

/* cmd_list */
t_cmd	*new_command_node(char *command, int fd_in, int fd_out);
t_cmd	*add_command_to_list(t_data *data, int *fd, int *red_type, int red_num);

/* parsing */
void	parsing(t_data *data);

void	handle_heredoc(t_data *data, t_lexer *lexer);
void	count_heredoc(t_data *data, t_lexer *lexer);
void	delete_heredoc(t_cmd *node);
char	**get_new_command(t_cmd *cmd);
char	**replace_with_new_command(char **new, char **cmd);

void	handle_command(t_data *data);
void	make_command_list(t_data *data, int red_num, int *red_type);
void	print_command_list(t_cmd *cmd_list);

int		get_red_num(t_data *data);
char	*get_file_name(char *value);
int		get_fd_in(int *fd, int *red_type, int red_num);
int		get_fd_out(int *fd, int *red_type, int red_num);
int		*set_red_type(t_data *data, int red_num);
int		set_fd(char *filename, int red_type);
char	*set_command(t_data *data);

size_t	word_count(char *s, char c);
void	word_len(t_split *split, char c, int flag);
char	**ft_free_word(char	**p, int i);
void	ft_free_list(char **p);
char	**split_command(char *s, char c);

int		has_redirection_in_lexer(t_data *data);
void	delete_node(t_data *data, int position);
void	delete_redirection_and_filename(t_data *data, int position, int flag);
void	delete_redirection(t_data *data);

/* delete_quotes */
int		has_quotes(char *value);
char	*get_new_string(char *s, int position);
char	*ft_delete_quotes(char *s, int i, int double_flag, int single_flag);
char	*delete_quotes_in_str(char *str);
void	delete_quotes_in_list(t_cmd *cmd_list);

/* init_data */
void	setting_env(t_data *data);
int		init_data1(t_data *data, int argc, char **env);
void	init_data2(t_data *data);
void	init_split(t_split *split, char *s, char c);

/* main */
int		minishell(t_data *data);

#endif