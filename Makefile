# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tosuman <timo42@proton.me>                 +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/28 14:29:52 by tosuman           #+#    #+#              #
#    Updated: 2023/10/06 00:40:07 by tischmid         ###   ########.fr        #
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
_HEADERS	 = fractol.h definitions.h math_utils.h parsing.h \
			   typedefs.h viewport.h window.h
LIBFT_DIR    = ./libft
MINILIBX_DIR = ./minilibx-linux

SRCDIR       = src
OBJDIR       = obj
INCLUDEDIR   = include
SRC          = $(addprefix $(SRCDIR)/,$(_SRC))
OBJ          = $(addprefix $(OBJDIR)/,$(_OBJ))
INCLUDE      = $(addprefix $(INCLUDEDIR)/,$(_HEADERS))

CC           = cc
CFLAGS       = -O3 -Wall -Wextra -Werror \
		       -std=c89 -pedantic -Wconversion
CPPFLAGS     = -I$(LIBFT_DIR) -I$(MINILIBX_DIR) -I$(INCLUDEDIR)
LDFLAGS      = -L$(LIBFT_DIR) -L$(MINILIBX_DIR) \
		       -l$(LIBFT_) -l$(MINILIBX_) -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(MINILIBX_DIR)/$(MINILIBX) $(LIBFT_DIR)/$(LIBFT) $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)
	$(MAKE)

$(LIBFT_DIR)/$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MINILIBX_DIR)/$(MINILIBX): $(MINILIBX_DIR)
	$(MAKE) -C $(MINILIBX_DIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ): $(INCLUDE) | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(MINILIBX_DIR):
	@printf '\033[31m%s\033[m\n' "Downloading minilibx-linux.tgz"
	wget -q 'https://cdn.intra.42.fr/document/document/20211/minilibx-linux.tgz'
	tar xf minilibx-linux.tgz
	rm -rf minilibx-linux.tgz
	@printf '\033[31m%s\033[m\n' "Finished downloading"

clean: | $(MINILIBX_DIR)
	$(MAKE) -C $(LIBFT_DIR) $@
	if [ -f "$(MINILIBX_DIR)/$(MINILIBX)" ] ; then $(MAKE) -C $(MINILIBX_DIR) $@; fi
	$(RM) $(OBJ)
	$(RM) -rf $(OBJDIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) $@
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all
