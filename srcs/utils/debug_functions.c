/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 15:38:19 by maborges          #+#    #+#             */
/*   Updated: 2025/10/07 14:46:09 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	debug_print_env(t_env *env)
{
	if (!DEBUG)
		return ;
	printf("\n=== ENVIRONMENT VARIABLES ===\n");
	while (env)
	{
		printf("Key: '%s' | Value: '%s'\n", env->key, env->value);
		env = env->next;
	}
	printf("=== END ENV ===\n\n");
}

/* void	debug_print_command(t_command *cmd)
{
	int	i;

	if (!DEBUG)
		return ;
	i = 0;
	printf("\n=== COMMAND STRUCTURE ===\n");
	if (!cmd)
	{
		printf("Command is NULL\n");
		return ;
	}
	printf("Args: ");
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			printf("'%s' ", cmd->args[i]);
			i++;
		}
	}
	printf("\n");
	printf("Input file: %s\n", cmd->input_file ? cmd->input_file : "NULL");
	printf("Output file: %s\n", cmd->output_file ? cmd->output_file : "NULL");
	printf("Append mode: %d\n", cmd->append);
	printf("Next command: %s\n", cmd->next ? "EXISTS" : "NULL");
	printf("=== END COMMAND ===\n\n");
} */

void	debug_print_shell(t_mshell *shell)
{
	if (!DEBUG)
		return ;

	printf("\n=== SHELL STATE ===\n");
	printf("Exit status: %d\n", shell->exit_status);
	printf("Must exit: %d\n", shell->must_exit);
	printf("Command count: %d\n", shell->cmd_count);
	printf("Shell PID: %d\n", shell->shell_pid);
	printf("=== END SHELL STATE ===\n\n");
}

// Generic debug function with location info
void	debug_checkpoint(const char *function, int line, const char *message)
{
	if (!DEBUG)
		return ;

	printf("\n[DEBUG] %s:%d - %s\n", function, line, message);
}

// Macro for easy checkpoint calls
#define DEBUG_CHECKPOINT(msg) debug_checkpoint(__FUNCTION__, __LINE__, msg)
