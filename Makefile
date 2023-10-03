# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tosuman <timo42@proton.me>                 +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/28 14:29:52 by tosuman           #+#    #+#              #
#    Updated: 2023/10/03 01:55:33 by tosuman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile for Linux systems

NAME         = fractol
LIBFT        = libft.a
LIBFT_       = $(patsubst lib%,%,$(patsubst %.a,%,$(LIBFT)))
MINILIBX     = libmlx_Linux.a
MINILIBX_    = $(patsubst lib%,%,$(patsubst %.a,%,$(MINILIBX)))

SRC          = fractol.c mlx_extras.c
OBJ          = $(SRC:.c=.o)
HEADERS		 = fractol.h
LIBFT_DIR    = ./libft
MINILIBX_DIR = ./minilibx-linux

CC           = cc
CFLAGS       = -O3 -Wall -Wextra -Werror \
		       -pedantic -std=c89 -Wconversion
CPPFLAGS     = -I$(LIBFT_DIR) -I$(MINILIBX_DIR) -I.
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

$(OBJ): $(HEADERS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) $@
	make -C $(MINILIBX_DIR) $@
	$(RM) $(OBJ)

fclean: clean
	make -C $(LIBFT_DIR) $@
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all
