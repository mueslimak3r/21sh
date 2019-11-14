# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: calamber <calamber@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/29 23:01:53 by calamber          #+#    #+#              #
#    Updated: 2019/11/14 07:06:51 by calamber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#CC := gcc
#INCLUDE := -Ift_printf/includes -I.
#FLAGS := -Wall -Wextra -Werror # -g -fsanitize=address
#NAME := libft.a
#SRC_LIST :=
#include ft_printf/Makefile
#OBJ := $(SRC_LIST:.c=.o)

#$(NAME): $(OBJ)
#	@ar rc $(NAME) $(OBJ)
#	@ranlib $(NAME)

#%.o: %.c
#	@$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

NAME = libft.a
CC := gcc

all: $(NAME)

MODULES := src ft_printf includes
LIBDIRS :=
LIBS :=
CFLAGS += -Wall -Werror -Wextra -Iincludes 
MODNAME := module.mk
SRC :=

include $(patsubst %,%/$(MODNAME),$(MODULES))

OBJ :=  $(patsubst %.c,%.o,$(filter %.c,$(SRC)))
DEP :=	$(patsubst %.c,%.d,$(filter %.c,$(SRC)))

-include $(DEP)

$(NAME): $(OBJ)
	@ar rc $@ $(OBJ)
	@ranlib $@

%.d : %.c
	@./depend.sh $*.o $(CFLAGS) $< > $@
	@printf '\t%s' "$(CC) $(CFLAGS) -c -o $*.o $<" >> $@
	@echo $@ >> all.log

clean:
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean rmname fclean re\
