NAME = 21sh
CC := gcc

all: $(NAME)

MODULES := src/ast src/env src/exec src/shell src/history_rope includes
LIBDIRS := libft
LIBS := -lncurses -Llibft -lft
CFLAGS := -Wall -Werror -Wextra -Ilibft/includes -Iincludes 
MODNAME := module.mk
SRC :=

include $(patsubst %,%/$(MODNAME),$(MODULES))

OBJ :=  $(patsubst %.c,%.o,$(filter %.c,$(SRC)))
DEP :=	$(patsubst %.c,%.d,$(filter %.c,$(SRC)))

-include $(DEP)

$(NAME): $(OBJ)
	make -C libft
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $@

%.d : %.c
	@./depend.sh $*.o $(CFLAGS) $< > $@
	@printf '\t%s' "$(CC) $(CFLAGS) -c -o $*.o $<" >> $@
	@echo $@ >> all.d

clean:
	make clean -C libft
	rm -f $(OBJ)
	rm -f $(shell cat all.d)
	@rm -f all.d

clean_nolib:
	rm -f $(OBJ)
	rm -f $(shell cat all.d)
	@rm -f all.d

fclean: clean_nolib
	make fclean -C libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
