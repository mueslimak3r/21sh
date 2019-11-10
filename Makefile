NAME = 21sh
CC := gcc

all: $(NAME)

MODULES := src/ast src/env src/exec src/shell src/history_rope includes
LIBDIRS := libft
LIBS := -lncurses -Llibft -lft
CFLAGS := -Wall -Werror -Wextra -Ilibft/ft_printf/includes  -Ilibft -Iincludes 
MODNAME := module.mk
SRC :=

include $(patsubst %,%/$(MODNAME),$(MODULES))

OBJ :=  $(patsubst %.c,%.o,$(filter %.c,$(SRC)))
DEP :=	$(patsubst %.c,%.d,$(filter %.c,$(SRC)))

include $(DEP)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $@
	@echo $(patsubst %.o,%.d,$(OBJ)) > all.d

%.d : %.c
	./depend.sh $*.o $(CFLAGS) $< > $@
	echo "\t$(CC) $(CFLAGS) -c -o $*.o $<" >> $@

clean:
	@make clean -C libft
	@rm -f $(OBJ)
	rm -f `cat all.d`
	@rm -f all.d

fclean: clean
	@make fclean -C libft
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
