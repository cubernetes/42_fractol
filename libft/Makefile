# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tosuman <timo42@proton.me>                 +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/12 10:04:04 by tosuman           #+#    #+#              #
#    Updated: 2023/10/05 23:40:12 by tischmid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
unexport CFLAGS
unexport LDFLAGS
CFLAGS += -O3 -Wall -Wextra -Werror -pedantic -std=c89 -Wconversion
AR = ar rcs
RM = /bin/rm -f

NAME = libft.a
OBJDIR = .
SRCDIR = .
INCDIR = .

unexport _SRC
_SRC += ft_atoi.c
_SRC += ft_atoi.c
_SRC += ft_atof.c
_SRC += ft_bzero.c
_SRC += ft_calloc.c
_SRC += ft_isalnum.c
_SRC += ft_isalpha.c
_SRC += ft_isascii.c
_SRC += ft_isdigit.c
_SRC += ft_isprint.c
_SRC += ft_memchr.c
_SRC += ft_memcmp.c
_SRC += ft_memcpy.c
_SRC += ft_memmove.c
_SRC += ft_memset.c
_SRC += ft_strchr.c
_SRC += ft_strdup.c
_SRC += ft_strlcat.c
_SRC += ft_strlcpy.c
_SRC += ft_strlen.c
_SRC += ft_strncmp.c
_SRC += ft_strcmp.c
_SRC += ft_strnstr.c
_SRC += ft_strrchr.c
_SRC += ft_tolower.c
_SRC += ft_toupper.c
_SRC += ft_putchar_fd.c
_SRC += ft_putendl_fd.c
_SRC += ft_putnbr_fd.c
_SRC += ft_putstr_fd.c
_SRC += ft_putendl.c
_SRC += ft_putstr.c
_SRC += ft_putchar.c

_SRC += ft_itoa.c
_SRC += ft_strtrim.c
_SRC += ft_substr.c
_SRC += ft_strjoin.c
_SRC += ft_split.c
_SRC += ft_striteri.c
_SRC += ft_strmapi.c

_SRC += ft_putfmt.c
_SRC += ft_putfmt_fd.c
_SRC += ft_puthex_fd.c
_SRC += ft_putptr_fd.c
_SRC += ft_puthex.c
_SRC += ft_putptr.c
_SRC += ft_printf.c
_SRC += ft_dprintf.c

_SRC_BONUS += ft_lstnew.c
_SRC_BONUS += ft_lstadd_front.c
_SRC_BONUS += ft_lstsize.c
_SRC_BONUS += ft_lstlast.c
_SRC_BONUS += ft_lstadd_back.c
_SRC_BONUS += ft_lstclear.c
_SRC_BONUS += ft_lstdelone.c
_SRC_BONUS += ft_lstiter.c
_SRC_BONUS += ft_lstmap.c

_SRC += ft_char_in_charset.c
_SRC += ft_isspace.c
_SRC += ft_abs.c

unexport _DEPS
_DEPS += libft.h

_OBJ = $(_SRC:.c=.o)
_OBJ_BONUS = $(_SRC_BONUS:.c=.o)

OBJ = $(addprefix $(OBJDIR)/,$(_OBJ))
OBJ_BONUS = $(addprefix $(OBJDIR)/,$(_OBJ_BONUS))
SRC = $(addprefix $(SRCDIR)/,$(_SRC))
SRC_BONUS = $(addprefix $(SRCDIR)/,$(_SRC_BONUS))
DEPS = $(addprefix $(INCDIR)/,$(_DEPS))

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $@ $^

$(OBJDIR)/%.o: %c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ -I$(INCDIR) $<

clean:
	$(RM) $(OBJ) $(OBJ_BONUS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus: $(OBJ) $(OBJ_BONUS)
	$(AR) $(NAME) $^

.PHONY: re fclean clean all bonus
