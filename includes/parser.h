#ifndef PARSER_H
# define PARSER_H

typedef struct s_atom t_atom;

typedef struct	s_program			t_program;
typedef struct	s_complete_commands	t_complete_commands;
typedef struct	s_complete_command	t_complete_command;
typedef struct	s_list_node			t_list_node;
typedef struct	s_and_or			t_and_or;
typedef struct	s_pipeline			t_pipeline;
typedef struct	s_pipe_sequence		t_pipe_sequence;
typedef struct	s_command			t_command;
typedef struct	s_compound_command	t_compound_command;
typedef struct	s_subshell			t_subshell;
typedef struct	s_compound_list		t_compound_list;
typedef struct	s_term_node			t_term_node;
typedef struct	s_for_clause		t_for_clause;
typedef struct	s_name				t_name;
typedef struct	s_in				t_in;
typedef struct	s_wordlist			t_wordlist;
typedef struct	s_case_clause		t_case_clause;
typedef struct	s_case_list_ns		t_case_list_ns;
typedef struct	s_case_list			t_case_list;
typedef struct	s_case_item_ns		t_case_item_ns;
typedef struct	s_case_item			t_case_item;
typedef struct	s_pattern			t_pattern;
typedef struct	s_if_clause			t_if_clause;
typedef struct	s_else_part			t_else_part;
typedef struct	s_while_clause		t_while_clause;
typedef struct	s_until_clause		t_until_clause;
typedef struct	s_function_definition	t_function_definition;
typedef struct	s_function_body		t_function_body;
typedef struct	s_fname				t_fname;
typedef struct	s_brace_group		t_brace_group;
typedef struct	s_do_group			t_do_group;
typedef struct	s_simple_command	t_simple_command;
typedef struct	s_cmd_name			t_cmd_name;
typedef struct	s_cmd_word			t_cmd_word;
typedef struct	s_cmd_prefix		t_cmd_prefix;
typedef struct	s_cmd_suffix		t_cmd_suffix;
typedef struct	s_redirect_list		t_redirect_list;
typedef struct	s_io_redirect		t_io_redirect;
typedef struct	s_io_file			t_io_file;
typedef struct	s_filename			t_filename;
typedef struct	s_io_here			t_io_here;
typedef struct	s_here_end			t_here_end;
typedef struct	s_newline_list		t_newline_list;
typedef struct	s_linebreak			t_linebreak;
typedef struct	s_separator_op		t_separator_op;
typedef struct	s_separator			t_separator;
typedef struct	s_sequential_sep	t_sequential_sep;

enum	e_type_methods
{
	CREATE,
	VISIT,
	PRINT
};

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
	SEQUENTIAL_SEP,
	WORDLIST
};

typedef struct	s_parse_tree
{
	t_atom				*atom;
	int					type;
	int					terminal;
	struct s_parse_tree	*children[16];
}				t_parse_tree;

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

struct					s_atom
{
	char				*str;
	t_atom				*next;
	int					type;
	int					peek;
};

struct					s_separator_op
{
	int					id;
	t_atom				*atom;
};

struct					s_newline_list
{
	int					id;
	t_atom				*atom;
	t_newline_list		*next;
};

struct					s_linebreak
{
	int					id;
	t_newline_list		*list;
};

struct					s_sequential_sep
{
	int					id;
	t_atom				*atom;
	t_linebreak			*linebreak;
	t_newline_list		*newline_list;
};

struct					s_separator
{
	int					id;
	t_separator_op		*separator_op;
	t_linebreak			*linebreak;
	t_newline_list		*newline_list;
};

struct					s_here_end
{
	int					id;
	t_atom				*word;
};

struct					s_io_here
{
	int					id;
	t_atom				*atom;
	t_here_end			*end;
};

struct					s_filename
{
	int					id;
	t_atom				*word;
};

struct					s_io_file
{
	int					id;
	t_atom				*op;
	t_filename			*filename;
};

struct					s_io_redirect
{
	int					id;
	t_atom				*io_number;
	t_io_file			*io_file;
	t_io_here			*io_here;
};

struct					s_redirect_list
{
	int					id;
	t_io_redirect		*io_redirect;
	t_redirect_list		*next;
};

struct					s_cmd_suffix
{
	int					id;
	t_io_redirect		*io_redir;
	t_atom				*word;
	t_cmd_suffix		*next;
};

struct					s_cmd_prefix
{
	int					id;
	t_io_redirect		*io_redir;
	t_atom				*assignment_word;
	t_cmd_prefix		*next;
};

struct					s_cmd_word
{
	int					id;
	t_atom				*word;			// word
};

struct					s_cmd_name
{
	int					id;
	t_atom				*name;			// word
};

struct					s_simple_command
{
	int					id;
	t_cmd_prefix		*cmd_prefix;
	t_cmd_word			*cmd_word;
	t_cmd_suffix		*cmd_suffix;
	t_cmd_name			*cmd_name;
};

struct					s_do_group
{
	int					id;
	t_atom				*word;			// 'do'
	t_compound_list		*compound_list;
	t_atom				*done;			// 'done'
};

struct					s_brace_group
{
	int					id;
	t_atom				*lbrace;		// '{'
	t_compound_list		*compound_list;
	t_atom				*rbrace;		// '}'
};

struct					s_fname
{
	int					id;
	t_atom				*fname;			// word
};

struct					s_function_body
{
	int					id;
	t_compound_command	*command;
	t_redirect_list		*redirect_list;
};

struct					s_function_definition
{
	int					id;
	t_fname				*fname;
	t_atom				*lparen;		// '('
	t_atom				*rparen;		// ')'
	t_linebreak			*linebreak;
	t_function_body		*function_body;
};

struct					s_until_clause
{
	int					id;
	t_atom				*word;			// 'until'
	t_compound_list		*compound_list;
	t_do_group			*do_group;
};

struct					s_while_clause
{
	int					id;
	t_atom				*word;			// 'while'
	t_compound_list		*compound_list;
	t_do_group			*do_group;
};

struct					s_else_part
{
	int					id;
	t_atom				*elif;
	t_compound_list		*compound_list;
	t_atom				*then;
	t_else_part			*else_part;
	t_atom				*word;			// 'else'
};

struct					s_if_clause
{
	int					id;
	t_atom				*word;			// 'if'
	t_compound_list		*compound_list;
	t_atom				*then;
	t_compound_list		*compound_list2;
	t_else_part			*else_part;
	t_atom				*fi;
};

struct					s_case_item
{
	int					id;
	t_atom				*lparen;
	t_pattern			*pattern;
	t_atom				*rparen;
	t_linebreak			*linebreak;
	t_compound_list		*compound_list;
	t_atom				*dsemi;
	t_linebreak			*linebreak2;
};

struct					s_pattern
{
	int					id;
	t_pattern			*pattern;
	t_atom				*pipe;
	t_atom				*word;
};

struct					s_case_item_ns
{
	int					id;
	t_atom				*lparen;
	t_pattern			*pattern;
	t_atom				*rparen;
	t_compound_list		*compound_list;
	t_linebreak			*linebreak;
};

struct					s_case_list
{
	int					id;
	t_case_list			*list;
	t_case_item			*item;
};

struct					s_case_list_ns
{
	int					id;
	t_case_list			*list;
	t_case_item_ns		*item;
};

struct					s_case_clause
{
	int					id;
	t_atom				*case_word;
	t_atom				*word;
	t_linebreak			*linebreak;
	t_in				*in;
	t_linebreak			*linebreak2;
	t_case_list			*case_list;
	t_case_list_ns		*case_list_ns;
	t_atom				*esac;
};

struct					s_wordlist
{
	int					id;
	t_wordlist			*list;
	t_atom				*word;
};

struct					s_in
{
	int					id;
	t_atom				*word;
};

struct					s_name
{
	int					id;
	t_atom				*word;
};

struct					s_for_clause
{
	int					id;
	t_atom				*for_word;
	t_name				*name;
	t_linebreak			*linebreak;
	t_in				*in;
	t_wordlist			*wordlist;
	t_sequential_sep	*sequential_sep;
	t_do_group			*do_group;
};

struct					s_term_node
{
	int					id;
	t_term_node			*term;
	t_separator			*separator;
	t_and_or			*and_or;
};

struct					s_compound_list
{
	int					id;
	t_linebreak			*linebreak;
	t_term_node			*term;
	t_separator			*separator;
};

struct					s_subshell
{
	int					id;
	t_atom				*lparen;
	t_compound_list		*compound_list;
	t_atom				*rparen;
};

struct					s_compound_command
{
	int					id;
	t_brace_group		*brace_group;
	t_subshell			*subshell;
	t_for_clause		*for_clause;
	t_case_clause		*case_clause;
	t_if_clause			*if_clause;
	t_while_clause		*while_clause;
	t_until_clause		*until_clause;
};

struct					s_command
{
	int					id;
	t_simple_command	*simple_command;
	t_compound_command	*compound_command;
	t_redirect_list		*redirect_list;
	t_function_definition	*function_definition;
};

struct					s_pipe_sequence
{
	int					id;
	t_pipe_sequence		*next;
	t_atom				*pipe;
	t_linebreak			*linebreak;
	t_command			*command;
};

struct					s_pipeline
{
	int					id;
	t_atom				*bang;
	t_pipe_sequence		*pipe_sequence;
};

struct					s_and_or
{
	int					id;
	t_and_or			*next;
	t_atom				*and_if;
	t_atom				*or_if;
	t_linebreak			*linebreak;
	t_pipeline			*pipeline;
};

struct					s_list_node
{
	int					id;
	t_list_node			*next;
	t_separator_op		*separator_op;
	t_and_or			*and_or;
};

struct					s_complete_command
{
	int					id;
	t_list_node			*list;
	t_separator			*separator;
};

struct					s_complete_commands
{
	t_complete_commands	*complete_commands;
	t_newline_list		*newline_list;
	t_complete_command	*complete_command;
};

struct					s_program
{
	t_linebreak			*linebreak;
	t_complete_commands	*complete_commands;
	t_linebreak			*linebreak2;
};

typedef struct			s_molecule
{
	t_program			*program;
}						t_molecule;

void					*handle_program(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_complete_commands(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_complete_command(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_list(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_and_or(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_pipeline(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_pipe_sequence(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_command(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_compound_command(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_subshell(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_compound_list(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_term(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_for_clause(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_name(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_in(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_case_clause(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_case_list_ns(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_case_list(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_case_item_ns(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_case_item(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_pattern(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_if_clause(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_else_part(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_while_clause(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_until_clause(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_function_definition(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_function_body(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_fname(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_brace_group(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_do_group(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_simple_commmand(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_cmd_name(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_cmd_prefix(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_cmd_suffix(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_redirect_list(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_io_redirect(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_io_file(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_filename(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_io_here(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_here_end(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_newline_list(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_linebreak(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_separator_op(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_separator(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_sequential_sep(void *node, unsigned int type, unsigned int method, unsigned int depth);
void					*handle_wordlist(void *node, unsigned int type, unsigned int method, unsigned int depth);



t_atom					*atomizer(char *str);
t_molecule				*moleculizer(t_atom	*atoms);

t_sequential_sep		*make_sequential_sep(t_atom **atoms);
t_separator				*make_separator(t_atom **atoms);
t_separator_op			*make_separator_op(t_atom **atoms);
t_linebreak				*make_linebreak(t_atom **atoms);
t_newline_list			*make_newline_list(t_atom **atoms);
t_here_end				*make_here_end(t_atom **atoms);
t_io_here				*make_io_here(t_atom **atoms);
t_filename				*name_filename(t_atom **atoms);
t_io_file				*make_io_file(t_atom **atoms);
t_io_redirect			*make_io_redirect(t_atom **atoms);
t_redirect_list			*make_redirect_list(t_atom **atoms);
t_cmd_suffix			*make_cmd_suffix(t_atom **atoms);
t_cmd_prefix			*make_cmd_prefix(t_atom **atoms);
t_cmd_word				*make_cmd_word(t_atom **atoms);
t_cmd_name				*make_cmd_name(t_atom **atoms);
t_simple_command		*make_simple_command(t_atom **atoms);
t_do_group				*make_do_group(t_atom **atoms);
t_brace_group			*make_brace_group(t_atom **atoms);
t_fname					*make_fname(t_atom **atoms);
t_function_body			*make_function_body(t_atom **atoms);
t_function_definition	*make_function_definition(t_atom **atoms);
t_until_clause			*make_until_clause(t_atom **atoms);
t_while_clause			*make_while_clause(t_atom **atoms);
t_else_part				*make_else_part(t_atom **atoms);
t_if_clause				*make_if_clause(t_atom **atoms);
t_pattern				*make_pattern(t_atom **atoms);
t_case_item				*make_case_item(t_atom **atoms);
t_case_item_ns			*make_case_item_ns(t_atom **atoms);
t_case_list				*make_case_list(t_atom **atoms);
t_case_list_ns			*make_case_list_ns(t_atom **atoms);
t_case_clause			*make_case_clause(t_atom **atoms);
t_wordlist				*make_wordlist(t_atom **atoms);
t_in					*make_in(t_atom **atoms);
t_name					*make_name(t_atom **atoms);
t_for_clause			*make_for_clause(t_atom **atoms);
t_term_node				*make_term_node(t_atom **atoms);
t_compound_list			*make_compound_list(t_atom **atoms);
t_subshell				*make_subshell(t_atom **atoms);
t_compound_command		*make_compound_command(t_atom **atoms);
t_command				*make_command(t_atom **atoms);
t_pipe_sequence			*make_pipe_sequence(t_atom **atoms);
t_pipeline				*make_pipeline(t_atom **atoms);
t_and_or				*make_and_or(t_atom **atoms);
t_list_node				*make_list_node(t_atom **atoms);
t_complete_command		*make_complete_command(t_atom **atoms);
t_complete_commands		*make_complete_commands(t_atom **atoms);
t_program				*make_program(t_atom **atoms);

#endif
