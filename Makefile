# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tosuman <timo42@proton.me>                 +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/28 14:29:52 by tosuman           #+#    #+#              #
#    Updated: 2023/10/02 05:14:40 by tosuman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile for Linux systems

NAME = fractol
LIBFT = libft.a
LIBFT_ = $(patsubst lib%,%,$(patsubst %.a,%,$(LIBFT)))
MINILIBX = libmlx_Linux.a
MINILIBX_ = $(patsubst lib%,%,$(patsubst %.a,%,$(MINILIBX)))

SRC = fractol.c
OBJ = $(SRC:.c=.o)
LIBFT_DIR = ./libft
MINILIBX_DIR = ./minilibx-linux

CC = cc
# CFLAGS = -O3 -I$(LIBFT_DIR) -I$(MINILIBX_DIR)
CFLAGS = -O3 -Wall -Wextra -Werror -pedantic \
		 -std=c89 -Wconversion -I$(LIBFT_DIR) -I$(MINILIBX_DIR)
LDFLAGS = -L$(LIBFT_DIR) -L$(MINILIBX_DIR) \
		  -l$(LIBFT_) -l$(MINILIBX_) -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(MINILIBX_DIR)/$(MINILIBX) $(LIBFT_DIR)/$(LIBFT) $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)
	make

$(LIBFT_DIR)/$(LIBFT):
	make -C $(LIBFT_DIR)

$(MINILIBX_DIR)/$(MINILIBX):
	make -C $(MINILIBX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) $@
	make -C $(MINILIBX_DIR) $@
	$(RM) $(OBJ)

fclean: clean
	make -C $(LIBFT_DIR) $@
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all
