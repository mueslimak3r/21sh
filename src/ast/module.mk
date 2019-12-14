DIR := src/ast
SRC +=	$(DIR)/lexer.c $(DIR)/lexer_redirection.c $(DIR)/lexer_op_handler.c	\
		$(DIR)/lexer_create.c $(DIR)/parser.c $(DIR)/parser_classify.c		\
		$(DIR)/parser_create.c $(DIR)/parser_utils.c $(DIR)/tree_redir.c	\
		$(DIR)/tree_traverse.c $(DIR)/tree_redir2.c	$(DIR)/tree_traverse2.c
