/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 21:50:21 by marvin            #+#    #+#             */
/*   Updated: 2023/10/23 21:50:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

void	lexer(t_data *data)
{
	int	i;

	i = 0;
	data->cmd = ft_strtrim(data->cmd, " ");
	add_history(data->cmd);
	while (data->cmd[i])
	{
		while (is_space(data->cmd[i]))
			i++;
		if (data->cmd[i] == '|')
			i = add_pipe(data, i);
		if (data->cmd[i] == '&')
			i = add_ampersand(data, i);
		if (data->cmd[i] == ';')
			i = add_semicolon(data, i);
		if (is_redirection(data->cmd[i]))
			i = add_redirection(data, i);
		if (is_parenthesis(data->cmd[i]))
			i = add_parenthesis(data, i);
		if (data->cmd[i] && data->cmd[i] != '|' && data->cmd[i] != '&'
			&& !is_space(data->cmd[i]) && !is_redirection(data->cmd[i]))
			i = add_word(data, i);
	}
	free(data->cmd);
}

void	ft_free_lexer(t_lexer *lexer)
{
	t_lexer	*curr;
	t_lexer	*next;

	curr = lexer;
	while (curr)
	{
		next = curr->next;
		free(curr->val);
		free(curr);
		curr = next;
	}
	curr = NULL;
	next = NULL;
}
