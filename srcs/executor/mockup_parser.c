#include "../../inc/minishell.h"

// Mockup function to create a simple command structure
t_command	*create_mockup_command(char *input_line)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);

    // Simple split by spaces (you'll replace this with real parsing)
    cmd->args = ft_split(input_line, ' ');
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = 0;
    cmd->next = NULL;

    return (cmd);
}

// Mockup parser function
t_command	*mockup_parse(char *input, char **envp, int last_status)
{
	(void)envp;
	(void)last_status;
	if (!input || !*input)
		return (NULL);

    // For now, just create a simple command
    // Later, your partner will replace this with proper parsing
    return (create_mockup_command(input));
}

// Helper function to free command structure
void	free_command(t_command *cmd)
{
    int i = 0;
    if (!cmd)
        return;

    if (cmd->args)
    {
        while (cmd->args[i])
            free(cmd->args[i++]);
        free(cmd->args);
    }
    free(cmd->input_file);
    free(cmd->output_file);
    free(cmd);
}
