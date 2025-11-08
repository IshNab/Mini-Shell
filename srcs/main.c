/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:09:11 by maborges          #+#    #+#             */
/*   Updated: 2025/11/06 16:22:53 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//REPL
// READ -> EVALUATE->PRINT->EXECUTE->LOOP

static t_env	*init_env(char **envp)
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

static void	init_shell(t_mshell *shell, char **envp)
{
	shell->env = init_env(envp);
	shell->ast = NULL; //still waiting for the parsing team
	shell->cmd_count = 0;
	shell->exit_status = 0;
	shell->must_exit = 0;
	shell->shell_pid = 0;
}

static void	cleanup_shell(t_mshell *shell)
{
	t_env	*current;
	t_env	*next;

	current = shell->env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_mshell	shell;
	t_ast		*ast;

	if (argc > 1)
		return (error_msg("Minishell does not accept arguments", 1, NULL));
	(void)argv;
	line = NULL;
	print_banner(); //UNCOMMENT BEFORE RELEASE
	using_history();
	init_shell(&shell, envp);
	setup_interactive_signals();
	//printf("Shell initialized"); // debugger
	//debug_print_shell(&shell);
	//debug_print_env(shell.env);
	while (1)
	{
		line = readline("minishell$");
		// If SIGINT was received, refresh prompt like bash and continue
		if (g_signal_received == SIGINT)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			g_signal_received = 0;
			continue ;
		}
		if (!line) // This handles Ctrl+D (EOF)
		{
			printf("exit\n");
			break ;
		}
		if (line)
			add_history(line);
		//printf("About to parse command");
		ast = parser(line, envp, &shell);
		if (ast)
			execute_ast(ast, &shell);
		free_ast(ast);
		free(line);
	}
	cleanup_shell(&shell);
	return (shell.exit_status);
}
