/* $Id: gtkdatabox_regions.c 4 2008-06-22 09:19:11Z rbock $ */
/* GtkDatabox - An extension to the gtk+ library
 * Copyright (C) 2011 - 2012  Dr. Matt Flax <flatmax@>
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

#include <gtkdatabox_regions.h>

G_DEFINE_TYPE(GtkDataboxRegions, gtk_databox_regions,
	GTK_DATABOX_TYPE_XYYC_GRAPH)

static void gtk_databox_regions_real_draw (GtkDataboxGraph * regions,
					GtkDatabox* box);

static void
gtk_databox_regions_class_init (GtkDataboxRegionsClass *klass)
{
   GtkDataboxGraphClass *graph_class = GTK_DATABOX_GRAPH_CLASS (klass);

   graph_class->draw = gtk_databox_regions_real_draw;
}

static void
gtk_databox_regions_init (GtkDataboxRegions *regions)
{
}

/**
 * gtk_databox_regions_new:
 * @len: length of @X, @Y1  and @Y2
 * @X: array of ordinates
 * @Y1: array of co-ordinates
 * @Y2: array of co-ordinates
 * @color: color of the markers
 *
 * Creates a new #GtkDataboxRegions object which can be added to a #GtkDatabox widget
 *
 * Return value: A new #GtkDataboxRegions object
 **/
GtkDataboxGraph *
gtk_databox_regions_new (guint len, gfloat * X, gfloat * Y1, gfloat * Y2, GdkColor * color)
{
   GtkDataboxRegions *regions;
   g_return_val_if_fail (X, NULL);
   g_return_val_if_fail (Y1, NULL);
   g_return_val_if_fail (Y2, NULL);
   g_return_val_if_fail ((len > 0), NULL);

   regions = g_object_new (GTK_DATABOX_TYPE_REGIONS,
			"X-Values", X,
			"Y1-Values", Y1,
			"Y2-Values", Y2,
			"length", len, "color", color,NULL);

   return GTK_DATABOX_GRAPH (regions);
}

static void
gtk_databox_regions_real_draw (GtkDataboxGraph * graph,
			    GtkDatabox* box)
{
   GtkDataboxRegions *regions = GTK_DATABOX_REGIONS (graph);
   GdkPoint data[4];
   GdkGC *gc;
   GdkPixmap *pixmap;
   guint i = 0;
   gfloat *X;
   gfloat *Y1;
   gfloat *Y2;
   guint len;

   g_return_if_fail (GTK_DATABOX_IS_REGIONS (regions));
   g_return_if_fail (GTK_IS_DATABOX (box));

   pixmap = gtk_databox_get_backing_pixmap (box);

   if (gtk_databox_get_scale_type_y (box) == GTK_DATABOX_SCALE_LOG)
      g_warning
	 ("gtk_databox_regions do not work well with logarithmic scale in Y axis");

   if (!(gc = gtk_databox_graph_get_gc(graph)))
      gc = gtk_databox_graph_create_gc (graph, box);

   len = gtk_databox_xyyc_graph_get_length (GTK_DATABOX_XYYC_GRAPH (graph));
   X = gtk_databox_xyyc_graph_get_X (GTK_DATABOX_XYYC_GRAPH (graph));
   Y1 = gtk_databox_xyyc_graph_get_Y1 (GTK_DATABOX_XYYC_GRAPH (graph));
   Y2 = gtk_databox_xyyc_graph_get_Y2 (GTK_DATABOX_XYYC_GRAPH (graph));

   data[2].x = gtk_databox_value_to_pixel_x (box, *X);
   data[2].y = gtk_databox_value_to_pixel_y (box, *Y2);
   data[3].x = gtk_databox_value_to_pixel_x (box, *X);
   data[3].y = gtk_databox_value_to_pixel_y (box, *Y1);
   X++; Y1++; Y2++;
   for (i = 0; i < len-1; i++, X++, Y1++, Y2++)
   {
      data[1].x = data[2].x; /* 4 points in the polygon */
      data[1].y = data[2].y;
      data[0].x = data[3].x;
      data[0].y = data[3].y;
      data[2].x = gtk_databox_value_to_pixel_x (box, *X);
      data[2].y = gtk_databox_value_to_pixel_y (box, *Y2);
      data[3].x = gtk_databox_value_to_pixel_x (box, *X);
      data[3].y = gtk_databox_value_to_pixel_y (box, *Y1);
      gdk_draw_polygon (pixmap, gc, 1, /* 1 for a filled polygon*/
			 data, 4);
   }
   return;
}
