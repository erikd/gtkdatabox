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

#include <gtkdatabox.h>
#include <gtkdatabox_marshal.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_util.h>
#include <gtk/gtk.h>
#include <math.h>


void
pgtk_style_context_set_background (GtkStyleContext *stylecontext, GtkWidget *widget)
{
	cairo_t *cr;
    GtkAllocation allocation;

    cr = pgdk_cairo_create (gtk_widget_get_window (widget));
    gtk_widget_get_allocation (widget, &allocation);
    gtk_render_background(stylecontext, cr, 0, 0, allocation.width, allocation.height);
    cairo_destroy(cr);
}

cairo_t *
pgdk_cairo_create (GdkWindow *window)
{
	cairo_t * cr ;
	G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	/* Very recent deprecation. No idea how to replace it. */
	cr = gdk_cairo_create (window);
	G_GNUC_END_IGNORE_DEPRECATIONS
	return cr ;
}


void
pgtk_widget_override_background_color (GtkWidget * widget, GtkStateFlags state, const GdkRGBA * color)
{
	/* This one is complicated. See:
	 * https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-override-background-color
	 */
	G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	gtk_widget_override_background_color (widget, state, color) ;
	G_GNUC_END_IGNORE_DEPRECATIONS
}

GdkCursor *
pgdk_cursor_new (GdkCursorType cursor_type)
{
	return gdk_cursor_new_for_display(gdk_display_get_default(), cursor_type) ;
}
