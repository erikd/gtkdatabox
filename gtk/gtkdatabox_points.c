/* $Id: gtkdatabox_points.c 4 2008-06-22 09:19:11Z rbock $ */
/* GtkDatabox - An extension to the gtk+ library
 * Copyright (C) 1998 - 2008  Dr. Roland Bock
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <gtkdatabox_points.h>

G_DEFINE_TYPE(GtkDataboxPoints, gtk_databox_points,
	GTK_DATABOX_TYPE_XYC_GRAPH)

static void gtk_databox_points_real_draw (GtkDataboxGraph * points,
					  GtkDatabox * box);

/**
 * GtkDataboxPointsPrivate
 *
 * A private data structure used by the #GtkDataboxPoints. It shields all internal things
 * from developers who are just using the object.
 *
 **/
typedef struct _GtkDataboxPointsPrivate GtkDataboxPointsPrivate;

struct _GtkDataboxPointsPrivate
{
   GdkPoint *data;
};

static void
points_finalize (GObject * object)
{
   g_free (GTK_DATABOX_POINTS_GET_PRIVATE(object)->data);

   /* Chain up to the parent class */
   G_OBJECT_CLASS (gtk_databox_points_parent_class)->finalize (object);
}

static void
gtk_databox_points_class_init (GtkDataboxPointsClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
   GtkDataboxGraphClass *graph_class = GTK_DATABOX_GRAPH_CLASS (klass);

   gobject_class->finalize = points_finalize;

   graph_class->draw = gtk_databox_points_real_draw;

   g_type_class_add_private (klass, sizeof (GtkDataboxPointsPrivate));
}

static void
gtk_databox_points_complete (GtkDataboxPoints * points)
{
   GTK_DATABOX_POINTS_GET_PRIVATE(points)->data =
      g_new0 (GdkPoint,
	      gtk_databox_xyc_graph_get_length
	      (GTK_DATABOX_XYC_GRAPH (points)));

}

static void
gtk_databox_points_init (GtkDataboxPoints *points)
{
   g_signal_connect (points, "notify::length",
		     G_CALLBACK (gtk_databox_points_complete), NULL);
}

/**
 * gtk_databox_points_new:
 * @len: length of @X and @Y
 * @X: array of horizontal position values of markers
 * @Y: array of vertical position values of markers
 * @color: color of the markers
 * @size: marker size or line width (depending on the @type)
 *
 * Creates a new #GtkDataboxPoints object which can be added to a #GtkDatabox widget.
 *
 * Return value: A new #GtkDataboxPoints object
 **/
GtkDataboxGraph *
gtk_databox_points_new (guint len, gfloat * X, gfloat * Y,
			GdkColor * color, gint size)
{
   GtkDataboxPoints *points;
   g_return_val_if_fail (X, NULL);
   g_return_val_if_fail (Y, NULL);
   g_return_val_if_fail ((len > 0), NULL);

   points = g_object_new (GTK_DATABOX_TYPE_POINTS,
			  "X-Values", X,
			  "Y-Values", Y,
			  "length", len, "color", color, "size", size, NULL);

   return GTK_DATABOX_GRAPH (points);
}

static void
gtk_databox_points_real_draw (GtkDataboxGraph * graph,
			      GtkDatabox* box)
{
   GtkDataboxPoints *points = GTK_DATABOX_POINTS (graph);
   GdkPoint *data;
   GdkGC *gc;
   GdkPixmap *pixmap;
   guint i = 0;
   gfloat *X;
   gfloat *Y;
   guint len;
   gint size = 0;

   g_return_if_fail (GTK_DATABOX_IS_POINTS (points));
   g_return_if_fail (GTK_IS_DATABOX (box));

   pixmap = gtk_databox_get_backing_pixmap (box);

   if (!(gc = gtk_databox_graph_get_gc(graph)))
      gc = gtk_databox_graph_create_gc (graph, box);

   len = gtk_databox_xyc_graph_get_length (GTK_DATABOX_XYC_GRAPH (graph));
   X = gtk_databox_xyc_graph_get_X (GTK_DATABOX_XYC_GRAPH (graph));
   Y = gtk_databox_xyc_graph_get_Y (GTK_DATABOX_XYC_GRAPH (graph));
   size = gtk_databox_graph_get_size (graph);
   data = GTK_DATABOX_POINTS_GET_PRIVATE(points)->data;

   gtk_databox_values_to_pixels (box, len, X, Y, data);

   if (size < 2)
   {
      /* More than 2^16 points will cause X IO error on most XServers
         (Hint from Paul Barton-Davis) */
      for (i = 0; i < len; i += 65536)
      {
	 gdk_draw_points (pixmap, gc, data + i,
			  MIN (65536, len - i));
      }
   }
   else
   {
      for (i = 0; i < len; i++, data++)
      {
	 /* Why on earth is there no gdk_draw_rectangles?? */
	 gdk_draw_rectangle (pixmap, gc, TRUE,
			     data->x - size / 2,
			     data->y - size / 2, size, size);
      }
   }
   return;
}
