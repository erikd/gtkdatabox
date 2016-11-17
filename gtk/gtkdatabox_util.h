/* GtkDatabox - An extension to the gtk+ library
 * Copyright (C) 2016  Erik de Castro Lopo <erikd@mega-nerd.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __GTK_DATABOX_UTIL_H__
#define __GTK_DATABOX_UTIL_H__

#ifdef UNUSED
#elif defined (__GNUC__)
#       define UNUSED(x) UNUSED_ ## x __attribute__ ((unused))
#elif defined (__LCLINT__)
#       define UNUSED(x) /*@unused@*/ x
#else
#       define UNUSED(x) x
#endif

// Replacements for functions which the GTK+ developers have chosen to
// deprecate. Prepend `private_` to the original function names.

cairo_t * pgdk_cairo_create (GdkWindow *window) ;

void pgtk_style_context_set_background (GtkStyleContext *stylecontext, GtkWidget *widget) ;

void pgtk_widget_override_background_color (GtkWidget *widget, GtkStateFlags state, const GdkRGBA *color);

GdkCursor * pgdk_cursor_new (GdkCursorType cursor_type);

#endif
