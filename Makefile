# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tosuman <timo42@proton.me>                 +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/28 14:29:52 by tosuman           #+#    #+#              #
#    Updated: 2023/10/03 10:12:41 by tosuman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile for Linux systems

NAME         = fractol
LIBFT        = libft.a
LIBFT_       = $(patsubst lib%,%,$(patsubst %.a,%,$(LIBFT)))
MINILIBX     = libmlx_Linux.a
MINILIBX_    = $(patsubst lib%,%,$(patsubst %.a,%,$(MINILIBX)))

_SRC         = viewport.c change_params.c fractol.c render.c color_utils.c \
			   complex_math_utils.c fractals.c math_utils.c hooks.c \
			   string_utils.c parsing.c help.c mlx_extras.c img.c \
			   param_parsing1.c param_parsing2.c param_parsing3.c \
			   zoom_and_translation.c
_OBJ         = $(_SRC:.c=.o)
HEADERS		 = fractol.h
LIBFT_DIR    = ./libft
MINILIBX_DIR = ./minilibx-linux

SRCDIR       = src
OBJDIR       = obj
INCLUDE      = include
SRC          = $(addprefix $(SRCDIR)/,$(_SRC))
OBJ          = $(addprefix $(OBJDIR)/,$(_OBJ))

CC           = cc
# TODO: Add -std=c89
CFLAGS       = -O3 -Wall -Wextra -Werror \
		       -pedantic -Wconversion
CPPFLAGS     = -I$(LIBFT_DIR) -I$(MINILIBX_DIR) -I$(INCLUDE)
LDFLAGS      = -L$(LIBFT_DIR) -L$(MINILIBX_DIR) \
		       -l$(LIBFT_) -l$(MINILIBX_) -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(MINILIBX_DIR)/$(MINILIBX) $(LIBFT_DIR)/$(LIBFT) $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)
	make

$(LIBFT_DIR)/$(LIBFT):
	make -C $(LIBFT_DIR)

$(MINILIBX_DIR)/$(MINILIBX):
	make -C $(MINILIBX_DIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ): $(INCLUDE)/$(HEADERS) | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	make -C $(LIBFT_DIR) $@
	make -C $(MINILIBX_DIR) $@
	$(RM) $(OBJ)
	$(RM) -rf $(OBJDIR)

fclean: clean
	make -C $(LIBFT_DIR) $@
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all
