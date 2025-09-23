static void	skip_whitespace(const char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}

char *ft_strtok(char *str, const char *delim)
{
    static char *save_ptr = NULL;
    char *token_start;
    int i;

    if (str)
        save_ptr = str;
    if (!save_ptr || *save_ptr == '\0')
        return NULL;

    // 跳过前导分隔符
    while (*save_ptr && strchr(delim, *save_ptr))
        save_ptr++;
    if (*save_ptr == '\0')
        return NULL;

    token_start = save_ptr;
    // 查找下一个分隔符
    while (*save_ptr && !strchr(delim, *save_ptr))
        save_ptr++;
    if (*save_ptr)
    {
        *save_ptr = '\0';
        save_ptr++;
    }
    else
    {
        save_ptr = NULL;
    }
    return token_start;
}

// Helper: append char to dynamic string
static char	*str_append(char *s, char c)
{
	char	*new_s;
	size_t	len;

	len = 0;
	if (s)
		len = ft_strlen(s);
	new_s = (char *)malloc(len + 2);
	if (!new_s)
		return (NULL);
	if (s)
		ft_strlcpy(new_s, s, len + 1);
	new_s[len] = c;
	new_s[len + 1] = '\0';
	free(s);
	return (new_s);
}

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static char	*ms_tokenize_next(const char *input, int *i, int *in_squote, int *in_dquote)
{
	char	*token;

	token = NULL;
	while (input[*i])
	{
		if (!*in_squote && !*in_dquote && (input[*i] == ' ' || input[*i] == '\t'))
			break ;
		if (input[*i] == '\\' && input[*i + 1])
		{
			(*i)++;
			token = str_append(token, input[*i]);
		}
		else if (input[*i] == '\'' && !*in_dquote)
			*in_squote = !*in_squote;
		else if (input[*i] == '"' && !*in_squote)
			*in_dquote = !*in_dquote;
		else
			token = str_append(token, input[*i]);
		(*i)++;
	}
	return (token);
}

char	**ms_tokenize(const char *input)
{
	char	**tokens;
	int		t;
	int		i;
	int		in_squote;
	int		in_dquote;
	char	*token;

	tokens = (char **)malloc(sizeof(char *) * 256);
	t = 0;
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		token = ms_tokenize_next(input, &i, &in_squote, &in_dquote);
		if (token)
			tokens[t++] = token;
	}
	tokens[t] = NULL;
	return (tokens);
}

char	*ms_remove_quotes(const char *token)
{
	char	*res;
	int		in_squote;
	int		in_dquote;
	int		i;

	res = NULL;
	in_squote = 0;
	in_dquote = 0;
	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (token[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else
			res = str_append(res, token[i]);
		i++;
	}
	return (res);
}

char	*get_env_value(const char *var, char **envp)
{
	int	k;
	int	len;

	k = 0;
	len = ft_strlen(var);
	while (envp && envp[k])
	{
		if (!ft_strncmp(envp[k], var, len) && envp[k][len] == '=')
			return (envp[k] + len + 1);
		k++;
	}
	return (NULL);
}

char	*ms_expand_token(const char *token, char **envp, int last_status)
{
	char	*res;
	int		in_squote;
	int		in_dquote;
	int		i;

	res = NULL;
	in_squote = 0;
	in_dquote = 0;
	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (token[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else if (token[i] == '$' && !in_squote)
		{
			if (token[i + 1] == '?')
			{
				char buf[16];
				ft_snprintf(buf, sizeof(buf), "%d", last_status);
				res = ft_strjoin_free(res, buf);
				i++;
			}
			else if (ft_isalnum(token[i + 1]) || token[i + 1] == '_')
			{
				int j = i + 1;
				while (ft_isalnum(token[j]) || token[j] == '_')
					j++;
				char var[128];
				int len = j - i - 1;
				ft_strlcpy(var, token + i + 1, len + 1);
				char *val = get_env_value(var, envp);
				if (val)
					res = ft_strjoin_free(res, val);
				i = j - 1;
			}
			else
				res = str_append(res, '$');
		}
		else
			res = str_append(res, token[i]);
		i++;
	}
	if (!res)
		return (ft_strdup(""));
	return (res);
}

char	*expand_status(int last_status)
{
	char	*buf;

	buf = (char *)malloc(16);
	if (!buf)
		return (NULL);
	ft_snprintf(buf, 16, "%d", last_status);
	return (buf);
}
