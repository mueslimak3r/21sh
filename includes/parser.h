enum	e_ast_grammar
{
	PROGRAM,
	COMPLETE_COMMANDS,
	COMPLETE_COMMAND,
	LIST,
	AND_OR,
	PIPELINE,
	PIPE_SEQUENCE,
	COMMAND,
	COMPOUND_COMMAND,
	SUBSHELL,
	COMPOUND_LIST,
	TERM,
	FOR_CLAUSE,
	NAME,
	IN,
	CASE_CLAUSE,
	CASE_LIST_NS,
	CASE_LIST,
	CASE_ITEM_NS,
	CASE_ITEM,
	PATTERN,
	IF_CLAUSE,
	ELSE_PART,
	WHILE_CLAUSE,
	UNTIL_CLAUSE,
	FUNCTION_DEFINITION,
	FUNCTION_BODY,
	FNAME,
	BRACE_GROUP,
	DO_GROUP,
	SIMPLE_COMMAND,
	CMD_NAME,
	CMD_PREFIX,
	CMD_SUFFIX,
	REDIRECT_LIST,
	IO_REDIRECT,
	IO_FILE,
	FILENAME,
	IO_HERE,
	HERE_END,
	NEWLINE_LIST,
	LINEBREAK,
	SEPARATOR_OP,
	SEPARATOR,
	SEQUENTIAL_SEP
};

enum	e_tokens
{
	TYPE_WORD,
	ASSIGNMENT_WORD,
	COMMAND_NAME,
	NEWLINE_TOK,		// '\n'
	IO_NUMBER,

	AND_IF,				// '&&'
	OR_IF,				// '||'
	DSEMI,				// ';;'
	DLESS,				// '<<'
	DGREAT,				// '>>'
	LESSAND,			// '<&'
	GREATAND,			// '>&'
	LESSGREAT,			// '<>'
	DLESSDASH,			// '<<-'
	CLOBBER,			// '>|'
	WORD_HDOC,			// '<<<'

	IF,					// 'if'
	THEN,				// 'then'
	ELSE,				// 'else'
	ELIF,				// 'elif'
	FI,					// 'fi'
	DO,					// 'do'
	DONE,				// 'done'
	CASE,				// 'case'
	ESAC,				// 'esac'
	WHILE,				// 'while'
	UNTIL,				// 'until'
	FOR,				// 'for'
	IN_TOK,				// 'in'

	LEFT_BRACE,			// '{'
	RIGHT_BRACE,		// '}'
	BANG,				// '!'
	AND_TOK,			// '&'
	DOLLAR,				// '$'
	DOLL_BRACE,			// '${'
	DOLL_BRACK,			// '$['
	DOLL_PAREN,			// '$('
	DOLL_DBL_PAREN,		// '$(('
	LEFT_BRACK,			// '['
	DBL_LEFT_BRACK,		// '[['
	LEFT_PAREN,			// '('
	DBL_LEFT_PAREN,		// '(('
	RIGHT_BRACK,		// ']'
	DBL_RIGHT_BRACK,	// ']]'
	RIGHT_PAREN,		// ')'
	DBL_RIGHT_PAREN,	// '))'
	SEMICOLON,			// ';'
	TILDE,				// '~'

	ADDADD,				// '++'
	SUBSUB,				// '--'
	STAR,				// '*'
	POWER,				// '**'
	EQUAL,				// '=='
	NEQUAL,				// '!='
	LEQUAL,				// '<='
	GEQUAL,				// '>='
	ADD_ASSIGN,			// '+='
	SUB_ASSIGN,			// '-='
	MUL_ASSIGN,			// '*='
	DIV_ASSIGN,			// '/='
	MOD_ASSIGN,			// '%='
	AND_ASSIGN,			// '&='
	OR_ASSIGN,			// '|='
	XOR_ASSIGN,			// '^='

	REDIR_OUT,			// '>'
	REDIR_IN,			// '<'
	CMD_IN,				// '<('
	CMD_OUT,			// '>('
	REDIR_PIPE,			// '|'

	PLUS,				// '+'
	MINUS,				// '-'
	QUEST,				// '?'
	ASSIGN,				// '='
	PERC,				// '%'
	HASH,				// '#'
	CARET,				// '^'
	COMMA,				// ','
	AT,					// '@'
	SLASH,				// '/'
	DBL_SLASH,			// '//'
	COLON,				// ':'
	TICK,				// '`'
	QUOTE,				// '''
	DBL_QUOTE,			// '"'

	END_OF_FILE,		// '\0'
	BACK_SLASH			// '\'
};

typedef struct			s_atom
{
	char				*str;
	struct s_atom		*next;
	int					type;
}						t_atom;

t_atom					*atomizer(char *str);
