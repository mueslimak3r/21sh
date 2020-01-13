#!/bin/bash
i="0"
name=("handle_program" "handle_complete_commands" "handle_complete_command" \
		"handle_list" "handle_and_or" "handle_pipeline" "handle_pipe_sequence" \
		"handle_command" "handle_compound_command" "handle_subshell" \
		"handle_compound_list" "handle_term" "handle_for_clause" "handle_name" \
		"handle_in" "handle_case_clause" "handle_case_list_ns" "handle_case_list" \
		"handle_case_item_ns" "handle_case_item" "handle_pattern" \
		"handle_if_clause" "handle_else_part" "handle_while_clause" \
		"handle_until_clause" "handle_function_definition" "handle_function_body" \
		"handle_fname" "handle_brace_group" "handle_do_group" \
		"handle_simple_commmand" "handle_cmd_name" "handle_cmd_prefix" \
		"handle_cmd_suffix" "handle_redirect_list" "handle_io_redirect" \
		"handle_io_file" "handle_filename" "handle_io_here" "handle_here_end" \
		"handle_newline_list" "handle_linebreak" "handle_separator_op" \
		"handle_separator" "handle_sequential_sep" "handle_wordlist")
s_name=()
t_name=()

for j in ${!name[@]}; do
    s_name+=( "s$(echo ${name[$j]} | cut -c7-)" )
    t_name+=( "t$(echo ${name[$j]} | cut -c7-)" )
done

dir=./temp
defs=../../../includes/parser.h
for i in ${!name[@]}; do
    filename="$dir/${name[$i]}.c"
    printf "#include \"ftshell.h\"\n\nstatic void pretty_print(${t_name[$i]} *node, unsigned int depth)\n{\n" > $filename
    printf "\tstatic char pad[] = \"                                                                         \";\n" >> $filename
    printf "\tprintf(\"%.*s${t_name[$i]}\\" >> $filename
    printf "n\", depth * 2, pad);\n" >> $filename
    objects=$(sed -e "0,/struct\t\t\t\t\t${s_name[$i]}/d" $defs | sed -e "0,/{/d" | sed -e "/}/,\$d")
    echo ${t_name[i]}
    echo "YY"
    echo $objects
    echo "XX"
    IFS=';'
    for var in $objects; do
        printf "\nfound $var \nend\n"
        awk -F"t_" '{sub(/ .*/,"",$2);print $2}' <<<"$var"
    done
    printf "}\n" >> $filename
done
