/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:31:44 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/05 23:01:20 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <stddef.h>

# define HEX_DIGITS "0123456789abcdef"
# define UHEX_DIGITS "0123456789ABCDEF"
# define NULL_PTR_STR "(null)"
# define NIL_PTR_STR "(nil)"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

void				*ft_memmove(void *dest, void const *src, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
void				*ft_memset(void *s, int c, unsigned long n);
void				ft_bzero(void *s, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(void const *s1, void const *s2, size_t n);
void				*ft_memcpy(void *dest, void const *src, size_t n);
int					ft_isalnum(int c);
int					ft_isprint(int c);
int					ft_isascii(int c);
int					ft_isspace(int c);
int					ft_isalpha(int c);
int					ft_isdigit(int c);
char				*ft_strnstr(char const *big, char const *little,
						size_t len);
char				*ft_strchr(char const *s, int c);
char				*ft_strrchr(char const *s, int c);
char				*ft_strdup(char const *s);
int					ft_strncmp(char const *s1, char const *s2, size_t n);
int					ft_strcmp(char const *s1, char const *s2);
size_t				ft_strlen(char const *s);
char				**ft_split(char const *s, char c);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_substr(char const *s, unsigned int start, size_t len);
int					ft_toupper(int c);
int					ft_tolower(int c);
char				*ft_strtrim(char const *s1, char const *set);
size_t				ft_strlcat(char *dst, char const *src, size_t size);
size_t				ft_strlcat(char *dst, char const *src, size_t size);
size_t				ft_strlcpy(char *dst, char const *src, size_t size);
char				*ft_strmapi(char const *s, char (f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_itoa(int n);
int					ft_atoi(char const *nptr);
double				ft_atof(const char *s);
int					ft_char_in_charset(char c, char const *charset);
unsigned int		ft_abs(int n);
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new_head);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new_tail);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
int					ft_putendl_fd(char *s, int fd);
int					ft_putnbr_fd(int nb, int fd);
int					ft_putchar_fd(char c, int fd);
int					ft_putstr_fd(char *s, int fd);
int					ft_puthex_fd(unsigned long nbr, int uppercase, int fd);
int					ft_puthex(unsigned long nbr, int uppercase);
int					ft_putnbr(int nb);
int					ft_putchar(char c);
int					ft_putstr(char *s);
int					ft_putptr(void *ptr);
int					ft_putptr_fd(void *ptr, int fd);
int					ft_putfmt(const char **fmt, va_list ap);
int					ft_putfmt_fd(int fd, const char **fmt, va_list ap);
int					ft_vprintf(const char *fmt, va_list ap);
int					ft_printf(const char *fmt, ...);
int					ft_vdprintf(int fd, const char *fmt, va_list ap);
int					ft_dprintf(int fd, const char *fmt, ...);

#endif
