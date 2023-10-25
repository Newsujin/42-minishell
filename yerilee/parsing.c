/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yerilee <yerilee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:27:26 by yerilee           #+#    #+#             */
/*   Updated: 2023/10/25 21:39:21 by yerilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

void	count_heredoc(t_data *data, t_lexer *lexer)
{
	while (lexer)
	{
		if (!ft_strcmp(lexer->val, "<<"))
			data->heredoc_num++;
		lexer = lexer->next;
	}
}

void	handle_heredoc(t_data *data, t_lexer *lexer)
{
	int	i;

	i = 0;
	count_heredoc(data, lexer);
	if (data->heredoc_num)
	{
		data->end = malloc(sizeof(char *) * (data->heredoc_num + 1));
		if (!data->end)
			exit (1);
		lexer = data->lexer_list;
		while (lexer)
		{
			if (ft_strcmp(lexer->val, "<<") == 0)
			{
				lexer = lexer->next;
				data->end[i] = ft_substr(lexer->val, 0, ft_strlen(lexer->val));
				// quotes 지우기
				i++;
			}
			lexer = lexer->next;
		}
		data->end[i] = NULL;
	}
}

/* $? 처리 -> heredoc 처리 -> command_list 만들기 -> quotes 제거 */
void	parsing(t_data *data)
{
	t_lexer	*lexer;

	lexer = data->lexer_list;

	// $? 처리
	handle_heredoc(data, lexer);
	// command_list 만들기
	// quotes 제거
}
