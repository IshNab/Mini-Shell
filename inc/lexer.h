/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/23 11:51:44 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

char	**ms_tokenize(const char *input);
char	*ms_remove_quotes(const char *token);
char	*ms_expand_token(const char *token, char **envp, int last_status);

#endif
