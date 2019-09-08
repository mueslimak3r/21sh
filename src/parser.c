# include "ftshell.h"

# define REDIR_L '<'
# define REDIR_R '>'
# define REDIR_LL "<<"
# define REDIR_RR ">>"
# define PIPE '|'
# define AND '&'
# define WAIT ';'

typedef struct s_cdata  t_cdata;

struct s_cdata
{
    char *command;
    char **args;

};

static int	is_token(char c)
{
	if (c == PIPE || c == WAIT)
		return (1);
	return (0);
}

/*static char	*str_thing(const char *input, int pos, int len)
{
	char	*ret;

	ret = malloc(sizeof(char) * len);
	ret[len--] = 0;
	while (len)
	{
		ret[len] = input[pos + len];
		len--;
	}
	return (ret);
}*/

void	parse_input(char *input)
{
	char	**operands;
	char	*operators;
	size_t	size;
	int	a;
	int	b;
	int	c;

	size = ft_strlen(input);
	operators = malloc(sizeof(char) * size);
	operands = malloc(sizeof(char*) * size);
	ft_bzero(operators, size);
	ft_bzero(operands, size);
	a = 0;
	b = 0;
	c = 0;
	while (input[a] != '\n')
	{
		ft_printf("%c", input[a]);
		if (is_token(input[a]))
		{
			ft_printf(" is a token\n");
			operators[b] = input[a];
			ft_printf("a = %d, c = %d\n", a, c);
			operands[b++] = ft_strsub(input, c, a - c);
			c = a + 1;
		}
		else
			ft_printf(" is not a token\n");
		a++;
	}
	operands[b] = ft_strsub(input, c, a - c);
	ft_printf("[%s][%c][%s]", operands[0], operators[0], operands[1]);
}
