DIR := src/parser_2/type_methods
SRC +=	$(DIR)/t_program.c, $(DIR)/t_complete_commands.c, $(DIR)/t_complete_command.c,
		$(DIR)/t_list.c, $(DIR)/t_and_or.c, $(DIR)/t_pipeline.c, $(DIR)/t_pipe_sequence.c,
		$(DIR)/t_command.c, $(DIR)/t_compound_command.c, $(DIR)/t_subshell.c,
		$(DIR)/t_compound_list.c, $(DIR)/t_term.c, $(DIR)/t_for_clause.c, $(DIR)/t_name.c,
		$(DIR)/t_in.c, $(DIR)/t_case_clause.c, $(DIR)/t_case_lis$(DIR)/t_ns.c, $(DIR)/t_case_list.c,
		$(DIR)/t_case_item_ns.c, $(DIR)/t_case_item.c, $(DIR)/t_pattern.c,
		$(DIR)/t_if_clause.c, $(DIR)/t_else_part.c, $(DIR)/t_while_clause.c,
		$(DIR)/t_until_clause.c, $(DIR)/t_function_definition.c, $(DIR)/t_function_body.c,
		$(DIR)/t_fname.c, $(DIR)/t_brace_group.c, $(DIR)/t_do_group.c,
		$(DIR)/t_simple_commmand.c, $(DIR)/t_cmd_name.c, $(DIR)/t_cmd_prefix.c,
		$(DIR)/t_cmd_suffix.c, $(DIR)/t_redirec$(DIR)/t_list.c, $(DIR)/t_io_redirect.c,
		$(DIR)/t_io_file.c, $(DIR)/t_filename.c, $(DIR)/t_io_here.c, $(DIR)/t_here_end.c,
		$(DIR)/t_newline_list.c, $(DIR)/t_linebreak.c, $(DIR)/t_separator_op.c,
		$(DIR)/t_separator.c, $(DIR)/t_sequential_sep.c, $(DIR)/t_wordlist.c
