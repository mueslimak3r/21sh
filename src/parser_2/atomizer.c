#include "ftshell.h"
//#include "parser.h"

char	*g_atoms[] = 
{
	"WORD", "ASSIGNMENT_WORD",
	"NAME", "\n", "IO_NUMBER",

	"&&", "||", ";;", "<<",
	">>", "<&", ">&", "<<-",
	">|", "<<<",

	"if", "then", "else",
	"elif", "fi", "do",
	"done", "case", "esac",
	"while", "until", "for",
	"in",

	"{", "}",
	"!", "&", "$",
	"${", "$[", "$(",
	"$((", "[", "[[",
	"(", "((", "]", "]]",
	")", "))", ";", "~",

	"++", "--", "*", "**",
	"==", "!=", "<=", ">=",
	"+=", "-=", "*=", "/=",
	"%=", "&=", "|=", "^=",

	">", "<", "<(", ">(", "|",

	"+", "-", "?", "=", "%",
	"#", "^", ",", "@", "/",
	"//", ":", "`", "\'",
	"\"", "\0", "\\", NULL
};

char	*g_reserved_words[] =
{
	"if", "then", "else",
	"elif", "fi", "do",
	"done", "case", "esac",
	"while", "until", "for",
	"in", NULL
};

char	*g_op[] =
{
	"\n",

	"&&", "||", ";;", "<<",
	">>", "<&", ">&", "<<-",
	">|", "<<<",

	"{", "}",
	"!", "&", "$",
	"${", "$[", "$(",
	"$((", "[", "[[",
	"(", "((", "]", "]]",
	")", "))", ";", "~",

	"++", "--", "*", "**",
	"==", "!=", "<=", ">=",
	"+=", "-=", "*=", "/=",
	"%=", "&=", "|=", "^=",

	">", "<", "<(", ">(", "|",

	"+", "-", "?", "=", "%",
	"#", "^", ",", "@", "/",
	"//", ":", "`", "\'",
	"\"", "\0", "\\", NULL
};

char	*g_atom_types[] =
{
	"TYPE_WORD",
	"ASSIGNMENT_WORD",
	"COMMAND_NAME",
	"NEWLINE_TOK",
	"IO_NUMBER",
	"AND_IF",
	"OR_IF",
	"DSEMI",
	"DLESS",
	"DGREAT",
	"LESSAND",
	"GREATAND",
	"LESSGREAT",
	"DLESSDASH",
	"CLOBBER",
	"WORD_HDOC",
	"IF",
	"THEN",
	"ELSE",
	"ELIF",
	"FI",
	"DO",
	"DONE",
	"CASE",
	"ESAC",
	"WHILE",
	"UNTIL",
	"FOR",
	"IN_TOK",
	"LEFT_BRACE",
	"RIGHT_BRACE",
	"BANG",
	"AND_TOK",
	"DOLLAR",
	"DOLL_BRACE",
	"DOLL_BRACK",
	"DOLL_PAREN",
	"DOLL_DBL_PAREN",
	"LEFT_BRACK",
	"DBL_LEFT_BRACK",
	"LEFT_PAREN",
	"DBL_LEFT_PAREN",
	"RIGHT_BRACK",
	"DBL_RIGHT_BRACK",
	"RIGHT_PAREN",
	"DBL_RIGHT_PAREN",
	"SEMICOLON",
	"TILDE",
	"ADDADD",
	"SUBSUB",
	"STAR",
	"POWER",
	"EQUAL",
	"NEQUAL",
	"LEQUAL",
	"GEQUAL",
	"ADD_ASSIGN",
	"SUB_ASSIGN",
	"MUL_ASSIGN",
	"DIV_ASSIGN",
	"MOD_ASSIGN",
	"AND_ASSIGN",
	"OR_ASSIGN",
	"XOR_ASSIGN",
	"REDIR_OUT",
	"REDIR_IN",
	"CMD_IN",
	"CMD_OUT",
	"REDIR_PIPE",
	"PLUS",
	"MINUS",
	"QUEST",
	"ASSIGN",
	"PERC",
	"HASH",
	"CARET",
	"COMMA",
	"AT",
	"SLASH",
	"DBL_SLASH",
	"COLON",
	"TICK",
	"QUOTE",
	"DBL_QUOTE",
	"EOF",
	"BACKSLASH"
};

char	*add_char(char *str, char c, int to_free)
{
	char	*new;
	size_t	sz;
	size_t	i;

	sz = str ? ft_strlen(str) : 0;
	i = 0;
	new = malloc(sz + 2);
	while ((size_t)i < sz)
	{
		new[i] = str[i];
		i++;
	}
	new[i] = c;
	new[i + 1] = '\0';
	to_free && str ? free(str) : 0;
	str = to_free ? NULL : str;
	return (new);
}

t_atom	*new_atom(void)
{
	t_atom	*new;

	new = malloc(sizeof(*new));
	new->str = NULL,
	new->type = 0;
	new->next = NULL;
	return (new);
}

int		is_reserved_word(t_atom *atom)
{
	int	i;

	i = 0;
	if (!atom || !atom->str)
		return (atom->type);
	while (g_atoms[i])
		if (!ft_strcmp(atom->str, g_atoms[i++]))
			return (i);
	return (atom->type);
}

t_atom	*delimit(t_atom *atom)
{
	if (!atom)
		return (atom);
	atom->type = is_reserved_word(atom);
	return (atom->next = new_atom());
}

int		classifier(t_atom **ref, char c, int adv)
{
	int		i;
	char	*new;
	t_atom	*atom;

	atom = *ref;
	i = 0;
	new = adv ? add_char(atom->str, c, 0) : ft_strdup(atom->str);
	while (g_atoms[i])
	{
		if (!ft_strcmp(new, g_atoms[i]))
		{
			atom->type = i;
			atom->str ? free(atom->str) : 0;
			atom->str = new;
			*ref = atom;
			return (i);
		}
		i++;
	}
	if (atom->type > 0 && !g_atoms[i])
	{
		*ref = delimit(*ref);
		(*ref)->str = add_char(NULL, c, 0);
		(*ref)->type = classifier(ref, 0, 0);
		return ((*ref)->type);
	}
	else
	{
		atom->str ? free(atom->str) : 0;
		atom->str = new;
		atom->type = 0;
	}
	return (TYPE_WORD);
}

void	print_atoms(t_atom *list)
{
	t_atom	*tmp;

	tmp = list;
	while (tmp)
	{
		ft_printf_fd(STDERR_FILENO, "|%s| %s\n", tmp->str ? tmp->str : "NULL", g_atom_types[tmp->type]);
		tmp = tmp->next;
	}
	ft_printf_fd(STDERR_FILENO, "\n");
}

int		can_delimit(char *str, int pos)
{
	int	i;

	i = 0;
	while (g_op[i])
	{
		if (str[pos] == g_op[i][0])
			return (i);
		i++;
	}
	return (-1);
}

int		is_quote(char c)
{
	return (c == '\'' || c == '\"' || c == '(' || c == ')' ||
		c == '{' || c == '}' || c == '[' || c == ']' ? 1 : 0);
}

int		is_matching_quote(char a, char b)
{
	if (a == '\'' && b == '\'')
		return (1);
	if (a == '\"' && b == '\"')
		return (1);
	if (a == '(' && b == ')')
		return (2);
	if (a == '{' && b == '}')
		return (2);
	if (a == '[' && b == ']')
		return (2);
	return (0);
}

int		quotes_balanced(char *str)
{
	t_list	*tmp;
	t_list	*stack;
	int		i;
	int		res;

	tmp = NULL;
	stack = NULL;
	i = 0;
	while (str[i])
	{
		if (is_quote(str[i]))
		{
			res = 0;
			if (!stack)
				stack = ft_lstnew(str + i, 1);
			else if (stack && (res = is_matching_quote(((char*)stack->content)[0], str[i])) == 1)
			{
				tmp = stack->next;
				free(stack->content);
				free(stack);
				stack = tmp;
				if (!stack)
					ft_printf_fd(STDERR_FILENO, "here\n");
			}
			else if (res == 2)
			{
				if (str[i] == '(' || str[i] == '[' || str[i] == '{')
				{
					tmp = ft_lstnew(str + i, 1);
					tmp->next = stack;
					stack = tmp;
				}
				else if (str[i] == ')' || str[i] == ']' || str[i] == '}')
				{
					tmp = stack->next;
					free(stack->content);
					free(stack);
					stack = tmp;
					if (!stack)
						ft_printf_fd(STDERR_FILENO, "done\n");
				}
			}
			else if (str[i] != '\'' && str[i] != '\"')
			{
				tmp = ft_lstnew(str + i, 1);
				tmp->next = stack;
				stack = tmp;
			}
		}
		i++;
	}
	stack ? free(stack->content) : 0;
	stack ? free(stack) : 0;
	return (stack ? 0 : 1);
}

t_atom	*atomizer(char *str)
{
	t_atom	*tmp;
	t_atom	*head;
	int		i;
	int		del;
	int		quoting;

	i = 0;
	del = 0;
	quoting = 0;
	if (!quotes_balanced(str))
	{
		// FIXME: NEEDS TO SPAWN HEREDOCS
		ft_printf_fd(STDERR_FILENO, "-wtsh: bad quotes (replace me with heredocs)\n");
	}
	if (!str || !*str)
		return (NULL);
	tmp = new_atom();
	tmp->type = -1;
	head = tmp;
	while (str[i])
	{
		
		del = can_delimit(str, i);
		if ((str[i] == '\'' || str[i] == '\"') && (quoting == 0 || quoting == str[i]))
		{
			quoting = quoting ? 0 : str[i];
			i++;
		}
		else if (ft_isspace(str[i]) && !quoting)
		{
			tmp = tmp->str ? delimit(tmp) : tmp;
			while (ft_isspace(str[i]))
				i++;
		}	
		else if ((tmp->type == TYPE_WORD && del < 0) || quoting)
		{
			classifier(&tmp, str[i], 1);
			tmp->type = TYPE_WORD;
			i++;
		}
		else if (del >= 0)
		{
			if (tmp->type == TYPE_WORD)
				tmp = tmp->str ? delimit(tmp) : tmp;
			classifier(&tmp, str[i], 1);
			i++;
		}
		else if (can_delimit(str, i) < 0 && tmp->type > TYPE_WORD)
		{
			tmp = tmp->str ? delimit(tmp) : tmp;
			classifier(&tmp, str[i], 1);
			i++;
		}
		else if (classifier(&tmp, str[i], 1) > TYPE_WORD || can_delimit(str, i + 1) >= 0)
		{
			tmp = tmp->str ? delimit(tmp) : tmp;
			if (!ft_isspace(str[i]))
				i++;
			else
				while (str[i] && ft_isspace(str[i]))
					i++;
		}
		else
			i++;
	}
	tmp->str ? delimit(tmp) : 0;
	print_atoms(head);
	return (head);
}
