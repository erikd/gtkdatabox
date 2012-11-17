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

static void gtk_databox_points_real_draw (GtkDataboxGraph * points,
					  GtkDatabox * box);

struct _GtkDataboxPointsPrivate
{
   gint16 *xpixels;
   gint16 *ypixels;
   guint pixelsalloc;
};

static gpointer parent_class = NULL;

static void
points_finalize (GObject * object)
{
   GtkDataboxPoints *points = GTK_DATABOX_POINTS (object);

   g_free (points->priv->xpixels);
   g_free (points->priv->ypixels);
   g_free (points->priv);

   /* Chain up to the parent class */
   G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtk_databox_points_class_init (gpointer g_class /*, gpointer g_class_data */ )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS (g_class);
   GtkDataboxGraphClass *graph_class = GTK_DATABOX_GRAPH_CLASS (g_class);
   GtkDataboxPointsClass *klass = GTK_DATABOX_POINTS_CLASS (g_class);

   parent_class = g_type_class_peek_parent (klass);

   gobject_class->finalize = points_finalize;

   graph_class->draw = gtk_databox_points_real_draw;
}

static void
gtk_databox_points_instance_init (GTypeInstance * instance	/*,
								   gpointer g_class */ )
{
   GtkDataboxPoints *points = GTK_DATABOX_POINTS (instance);

   points->priv = g_new0 (GtkDataboxPointsPrivate, 1);
   points->priv->xpixels = NULL;
   points->priv->ypixels = NULL;
   points->priv->pixelsalloc = 0;
}

GType
gtk_databox_points_get_type (void)
{
   static GType type = 0;

   if (type == 0)
   {
      static const GTypeInfo info = {
	 sizeof (GtkDataboxPointsClass),
	 NULL,			/* base_init */
	 NULL,			/* base_finalize */
	 (GClassInitFunc) gtk_databox_points_class_init,	/* class_init */
	 NULL,			/* class_finalize */
	 NULL,			/* class_data */
	 sizeof (GtkDataboxPoints),	/* instance_size */
	 0,			/* n_preallocs */
	 (GInstanceInitFunc) gtk_databox_points_instance_init,	/* instance_init */
	 NULL,			/* value_table */
      };
      type = g_type_register_static (GTK_DATABOX_TYPE_XYC_GRAPH,
				     "GtkDataboxPoints", &info, 0);
   }

   return type;
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
			  "xstart", 0,
			  "ystart", 0,
			  "xstride", 1,
			  "ystride", 1,
			  "xtype", G_TYPE_FLOAT,
			  "ytype", G_TYPE_FLOAT,
			  "length", len,
			  "maxlen", len,
			  "color", color, "size", size, NULL);

   return GTK_DATABOX_GRAPH (points);
}

/**
 * gtk_databox_points_new_full:
 * @maxlen: maximum length of @X and @Y
 * @len: actual number of @X and @Y values to plot
 * @X: array of horizontal position values of markers
 * @Y: array of vertical position values of markers
 * @xstart: the first element in the X array to plot (usually 0)
 * @ystart: the first element in the Y array to plot (usually 0)
 * @xstride: successive elements in the X array are separated by this much (1 if array, ncols if matrix)
 * @ystride: successive elements in the Y array are separated by this much (1 if array, ncols if matrix)
 * @xtype: the GType of the X array elements.  G_TYPE_FLOAT, G_TYPE_DOUBLE, etc.
 * @ytype: the GType of the Y array elements.  G_TYPE_FLOAT, G_TYPE_DOUBLE, etc.
 * @color: color of the markers
 * @size: marker size or line width (depending on the @type)
 *
 * Creates a new #GtkDataboxPoints object which can be added to a #GtkDatabox widget.
 *
 * Return value: A new #GtkDataboxPoints object
 **/
GtkDataboxGraph *
gtk_databox_points_new_full (guint maxlen, guint len,
			void * X, guint xstart, guint xstride, GType xtype,
			void * Y, guint ystart, guint ystride, GType ytype,
			GdkColor * color, gint size)
{
   GtkDataboxPoints *points;
   g_return_val_if_fail (X, NULL);
   g_return_val_if_fail (Y, NULL);
   g_return_val_if_fail ((len > 0), NULL);

   points = g_object_new (GTK_DATABOX_TYPE_POINTS,
			  "X-Values", X,
			  "Y-Values", Y,
			  "xstart", xstart,
			  "ystart", ystart,
			  "xstride", xstride,
			  "ystride", ystride,
			  "xtype", xtype,
			  "ytype", ytype,
			  "length", len,
			  "maxlen", maxlen,
			  "color", color, "size", size, NULL);

   return GTK_DATABOX_GRAPH (points);
}

static void
gtk_databox_points_real_draw (GtkDataboxGraph * graph,
			      GtkDatabox* box)
{
   GtkDataboxPoints *points = GTK_DATABOX_POINTS (graph);
   guint i = 0;
   void *X;
   void *Y;
   guint len, maxlen;
   gint pointsize = 0;
   cairo_t *cr;
   gint16 *xpixels, *ypixels;
   guint xstart, xstride, ystart, ystride;
   GType xtype, ytype;

   g_return_if_fail (GTK_DATABOX_IS_POINTS (points));
   g_return_if_fail (GTK_IS_DATABOX (box));

   cr = gtk_databox_graph_create_gc (graph, box);

   len = gtk_databox_xyc_graph_get_length (GTK_DATABOX_XYC_GRAPH (graph));
   maxlen = gtk_databox_xyc_graph_get_maxlen (GTK_DATABOX_XYC_GRAPH (graph));

   if (points->priv->pixelsalloc < len)
   {
   	points->priv->pixelsalloc = len;
	points->priv->xpixels = (gint16 *)g_realloc(points->priv->xpixels, len * sizeof(gint16));
	points->priv->ypixels = (gint16 *)g_realloc(points->priv->ypixels, len * sizeof(gint16));
   }

   xpixels = points->priv->xpixels;
   ypixels = points->priv->ypixels;

   X = gtk_databox_xyc_graph_get_X (GTK_DATABOX_XYC_GRAPH (graph));
   xstart = gtk_databox_xyc_graph_get_xstart (GTK_DATABOX_XYC_GRAPH (graph));
   xstride = gtk_databox_xyc_graph_get_xstride (GTK_DATABOX_XYC_GRAPH (graph));
   xtype = gtk_databox_xyc_graph_get_xtype (GTK_DATABOX_XYC_GRAPH (graph));
   gtk_databox_values_to_xpixels(box, xpixels, X, xtype, maxlen, xstart, xstride, len);

   Y = gtk_databox_xyc_graph_get_Y (GTK_DATABOX_XYC_GRAPH (graph));
   ystart = gtk_databox_xyc_graph_get_ystart (GTK_DATABOX_XYC_GRAPH (graph));
   ystride = gtk_databox_xyc_graph_get_ystride (GTK_DATABOX_XYC_GRAPH (graph));
   ytype = gtk_databox_xyc_graph_get_ytype (GTK_DATABOX_XYC_GRAPH (graph));
   gtk_databox_values_to_ypixels(box, ypixels, Y, ytype, maxlen, ystart, ystride, len);

   pointsize = gtk_databox_graph_get_size (graph);

   for (i = 0; i < len; i++, xpixels++, ypixels++)
      cairo_rectangle(cr, *xpixels - pointsize / 2, *ypixels - pointsize / 2, pointsize, pointsize);

   cairo_fill(cr);
   cairo_destroy(cr);
   return;
}
