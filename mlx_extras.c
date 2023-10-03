/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_extras.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 01:00:13 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 03:38:37 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minilibx-linux/mlx.h"
#include "./minilibx-linux/mlx_int.h"
#include <X11/Xlib.h>
#include <libft.h>
#include <mlx.h>

int	mlx_get_window_dim(void *mlx_ptr, void *win_ptr, int *width, int *height)
{
	XWindowAttributes	w_attr;
	int					status;

	status = XGetWindowAttributes(((t_xvar *)mlx_ptr)->display,
			((t_win_list *)win_ptr)->window, &w_attr);
	*width = w_attr.width;
	*height = w_attr.height;
	return (status);
}

int	mlx_int_wait_first_expose(t_xvar *xvar, Window win)
{
	XEvent	ev;

	XWindowEvent(xvar->display, win, ExposureMask, &ev);
	XPutBackEvent(xvar->display, &ev);
	return (0);
}

/* I have no idea what I'm doing, but ChatGPT tells me
 * these could be sensible default values. Had to do this,
 * other wise valgrind scream because of uninitialized values
 */
int	mlx_int_resizable_win(t_xvar *xvar, Window win, int w, int h)
{
	XSizeHints			hints;
	long				toto;
	XWindowAttributes	w_attr;

	XGetWMNormalHints(xvar->display, win, &hints, &toto);
	XGetWindowAttributes(xvar->display, xvar->root, &w_attr);
	hints.width = w;
	hints.height = h;
	hints.min_width = 10;
	hints.min_height = 10;
	hints.max_width = w_attr.width;
	hints.max_height = w_attr.height;
	hints.flags = USPosition | USSize | PMinSize | PMaxSize | PAspect;
	hints.win_gravity = 1;
	hints.x = 0;
	hints.y = 0;
	hints.width_inc = 1;
	hints.height_inc = 1;
	hints.base_width = 100;
	hints.base_height = 100;
	hints.max_aspect.x = 2;
	hints.max_aspect.y = 1;
	hints.min_aspect.x = 1;
	hints.min_aspect.y = 2;
	toto = 0;
	XSetWMNormalHints(xvar->display, win, &hints);
	return (0);
}

void	*mlx_new_resizable_window(t_xvar *xvar, int size_x, int size_y,
		char *title)
{
	t_win_list				*new_win;
	XSetWindowAttributes	xswa;
	XGCValues				xgcv;

	xswa.background_pixel = 0;
	xswa.border_pixel = (unsigned long)-1;
	xswa.colormap = xvar->cmap;
	xswa.event_mask = 0xFFFFFF;
	new_win = malloc(sizeof(*new_win));
	if (!new_win)
		return ((void *)0);
	new_win->window = XCreateWindow(xvar->display, xvar->root, 0, 0,
			(unsigned int)size_x, (unsigned int)size_y, 0, CopyFromParent,
			InputOutput, xvar->visual,
			CWEventMask | CWBackPixel | CWBorderPixel | CWColormap, &xswa);
	mlx_int_resizable_win(xvar, new_win->window, size_x, size_y);
	XStoreName(xvar->display, new_win->window, title);
	XSetWMProtocols(xvar->display, new_win->window, &(xvar->wm_delete_window),
		1);
	xgcv.foreground = (unsigned long)-1;
	xgcv.function = GXcopy;
	xgcv.plane_mask = AllPlanes;
	new_win->gc = XCreateGC(xvar->display, new_win->window,
			GCFunction | GCPlaneMask | GCForeground, &xgcv);
	new_win->next = xvar->win_list;
	xvar->win_list = new_win;
	ft_bzero(&(new_win->hooks), sizeof(new_win->hooks));
	XMapRaised(xvar->display, new_win->window);
	mlx_int_wait_first_expose(xvar, new_win->window);
	return (new_win);
}
