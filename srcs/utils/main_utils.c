/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:09:11 by maborges          #+#    #+#             */
/*   Updated: 2025/11/14 16:49:21 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//REPL
// READ -> EVALUATE->PRINT->EXECUTE->LOOP

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new_node;
	char	*equal;
	int		i;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			new_node = safe_malloc(sizeof(t_env));
			new_node->key = ft_substr(envp[i], 0, equal - envp[i]);
			new_node->value = ft_strdup(equal + 1);
			new_node->next = head;
			head = new_node;
		}
		i++;
	}
	return (head);
}

void	init_shell(t_mshell *shell, char **envp)
{
	shell->env = init_env(envp);
	shell->ast = NULL;
	shell->cmd_count = 0;
	shell->exit_status = 0;
	shell->must_exit = 0;
	shell->shell_pid = 0;
}

void	signal_on_main(t_mshell *shell, char **line)
{
	shell->exit_status = 130;
	if (*line)
		free(*line);
	*line = NULL;
	g_signal_received = 0;
}

void	process_command(t_mshell *shell, char *line)
{
	t_ast	*ast;

	if (isatty(STDIN_FILENO))
		add_history(line);
	ast = parser(line, shell);
	if (ast)
	{
		execute_ast(ast, shell);
		setup_interactive_signals();
		g_signal_received = 0;
	}
	free_ast(ast);
}

void	repl_loop(t_mshell *shell)
{
	char		*line;

	while (1)
	{
		line = readline("minishell$");
		if (g_signal_received == SIGINT)
		{
			signal_on_main(shell, &line);
			continue ;
		}
		if (!line && printf("exit\n"))
			break ;
		if ((*line == '\0' || !ft_strcmp(line, "\n")) && (free(line), 1))
			continue ;
		process_command(shell, line);
		free(line);
	}
}
