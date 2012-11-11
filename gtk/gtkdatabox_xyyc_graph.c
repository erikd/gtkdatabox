/* $Id: gtkdatabox_xyyc_graph.c 4 2008-06-22 09:19:11Z rbock $ */
/* GtkDatabox - An extension to the gtk+ library
 * Copyright (C) 1998 - 2008  Dr. Roland Bock
 * Copyright (C) 2012  Dr. Matt Flax <flatmax@>
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

#include <gtkdatabox_xyyc_graph.h>

G_DEFINE_TYPE(GtkDataboxXYYCGraph, gtk_databox_xyyc_graph,
	GTK_DATABOX_TYPE_GRAPH)

static gint gtk_databox_xyyc_graph_real_calculate_extrema (GtkDataboxGraph *
							  xyyc_graph,
							  gfloat * min_x,
							  gfloat * max_x,
							  gfloat * min_y,
							  gfloat * max_y);

/* IDs of properties */
enum
{
   PROP_X = 1,
   PROP_Y1,
   PROP_Y2,
   PROP_LEN
};

/**
 * GtkDataboxXYYCGraphPrivate
 *
 * A private data structure used by the #GtkDataboxXYYCGraph. It shields all internal things
 * from developers who are just using the object.
 *
 **/
typedef struct _GtkDataboxXYYCGraphPrivate GtkDataboxXYYCGraphPrivate;

struct _GtkDataboxXYYCGraphPrivate
{
   guint len;
   gfloat *X;
   gfloat *Y1;
   gfloat *Y2;
};

static void
gtk_databox_xyyc_graph_set_X (GtkDataboxXYYCGraph * xyyc_graph, gfloat * X)
{
   g_return_if_fail (GTK_DATABOX_IS_XYYC_GRAPH (xyyc_graph));
   g_return_if_fail (X);

   GTK_DATABOX_XYYC_GRAPH_GET_PRIVATE(xyyc_graph)->X = X;

   g_object_notify (G_OBJECT (xyyc_graph), "X-Values");
}

static void
gtk_databox_xyyc_graph_set_Y1 (GtkDataboxXYYCGraph * xyyc_graph, gfloat * Y1)
{
   g_return_if_fail (GTK_DATABOX_IS_XYYC_GRAPH (xyyc_graph));
   g_return_if_fail (Y1);

   GTK_DATABOX_XYYC_GRAPH_GET_PRIVATE(xyyc_graph)->Y1 = Y1;

   g_object_notify (G_OBJECT (xyyc_graph), "Y1-Values");
}

static void
gtk_databox_xyyc_graph_set_Y2 (GtkDataboxXYYCGraph * xyyc_graph, gfloat * Y2)
{
   g_return_if_fail (GTK_DATABOX_IS_XYYC_GRAPH (xyyc_graph));
   g_return_if_fail (Y2);

   GTK_DATABOX_XYYC_GRAPH_GET_PRIVATE(xyyc_graph)->Y2 = Y2;

   g_object_notify (G_OBJECT (xyyc_graph), "Y2-Values");
}

static void
gtk_databox_xyyc_graph_set_length (GtkDataboxXYYCGraph * xyyc_graph, guint len)
{
   g_return_if_fail (GTK_DATABOX_IS_XYYC_GRAPH (xyyc_graph));
   g_return_if_fail (len > 0);

   GTK_DATABOX_XYYC_GRAPH_GET_PRIVATE(xyyc_graph)->len = len;

   g_object_notify (G_OBJECT (xyyc_graph), "length");
}

static void
gtk_databox_xyyc_graph_set_property (GObject * object,
				    guint property_id,
				    const GValue * value, GParamSpec * pspec)
{
   GtkDataboxXYYCGraph *xyyc_graph = GTK_DATABOX_XYYC_GRAPH (object);

   switch (property_id)
   {
   case PROP_X:
      {
	 gtk_databox_xyyc_graph_set_X (xyyc_graph,
				      (gfloat *) g_value_get_pointer (value));
      }
      break;
   case PROP_Y1:
      {
	 gtk_databox_xyyc_graph_set_Y1 (xyyc_graph,
				      (gfloat *) g_value_get_pointer (value));
      }
      break;
   case PROP_Y2:
      {
	 gtk_databox_xyyc_graph_set_Y2 (xyyc_graph,
				      (gfloat *) g_value_get_pointer (value));
      }
      break;
   case PROP_LEN:
      {
	 gtk_databox_xyyc_graph_set_length (xyyc_graph,
					   g_value_get_int (value));
      }
      break;
   default:
      /* We don't have any other property... */
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
   }
}

/**
 * gtk_databox_xyyc_graph_get_X:
 * @xyyc_graph: A #GtkDataboxXYYCGraph object
 *
 * Gets the X values of the @xzc_graph.
 *
 * Return value: Pointer to X values
 */
gfloat *
gtk_databox_xyyc_graph_get_X (GtkDataboxXYYCGraph * xyyc_graph)
{
   g_return_val_if_fail (GTK_DATABOX_IS_XYYC_GRAPH (xyyc_graph), NULL);

   return GTK_DATABOX_XYYC_GRAPH_GET_PRIVATE(xyyc_graph)->X;
}

/**
 * gtk_databox_xyyc_graph_get_Y1:
 * @xyyc_graph: A #GtkDataboxXYYCGraph object
 *
 * Gets the Y1 values of the @xzc_graph.
 *
 * Return value: Pointer to Y1 values
 */
gfloat *
gtk_databox_xyyc_graph_get_Y1 (GtkDataboxXYYCGraph * xyyc_graph)
{
   g_return_val_if_fail (GTK_DATABOX_IS_XYYC_GRAPH (xyyc_graph), NULL);

   return GTK_DATABOX_XYYC_GRAPH_GET_PRIVATE(xyyc_graph)->Y1;
}

/**
 * gtk_databox_xyyc_graph_get_Y2:
 * @xyyc_graph: A #GtkDataboxXYYCGraph object
 *
 * Gets the Y2 values of the @xzc_graph.
 *
 * Return value: Pointer to Y2 values
 */
gfloat *
gtk_databox_xyyc_graph_get_Y2 (GtkDataboxXYYCGraph * xyyc_graph)
{
   g_return_val_if_fail (GTK_DATABOX_IS_XYYC_GRAPH (xyyc_graph), NULL);

   return GTK_DATABOX_XYYC_GRAPH_GET_PRIVATE(xyyc_graph)->Y2;
}

/**
 * gtk_databox_xyyc_graph_get_length:
 * @xyyc_graph: A #GtkDataboxXYYCGraph object
 *
 * Gets the the length of the X and Y values arrays.
 *
 * Return value: Length of X/Y arrays.
 */
guint
gtk_databox_xyyc_graph_get_length (GtkDataboxXYYCGraph * xyyc_graph)
{
   g_return_val_if_fail (GTK_DATABOX_IS_XYYC_GRAPH (xyyc_graph), 0);

   return GTK_DATABOX_XYYC_GRAPH_GET_PRIVATE(xyyc_graph)->len;
}

static void
gtk_databox_xyyc_graph_get_property (GObject * object,
				    guint property_id,
				    GValue * value, GParamSpec * pspec)
{
   GtkDataboxXYYCGraph *xyyc_graph = GTK_DATABOX_XYYC_GRAPH (object);

   switch (property_id)
   {
   case PROP_X:
      {
	 g_value_set_pointer (value, gtk_databox_xyyc_graph_get_X (xyyc_graph));
      }
      break;
   case PROP_Y1:
      {
	 g_value_set_pointer (value, gtk_databox_xyyc_graph_get_Y1 (xyyc_graph));
      }
      break;
   case PROP_Y2:
      {
	 g_value_set_pointer (value, gtk_databox_xyyc_graph_get_Y2 (xyyc_graph));
      }
      break;
   case PROP_LEN:
      {
	 g_value_set_int (value,
			  gtk_databox_xyyc_graph_get_length (xyyc_graph));
      }
      break;
   default:
      /* We don't have any other property... */
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
   }
}

static void
gtk_databox_xyyc_graph_class_init (GtkDataboxXYYCGraphClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
   GtkDataboxGraphClass *graph_class = GTK_DATABOX_GRAPH_CLASS (klass);
   GParamSpec *xyyc_graph_param_spec;

   gobject_class->set_property = gtk_databox_xyyc_graph_set_property;
   gobject_class->get_property = gtk_databox_xyyc_graph_get_property;

   xyyc_graph_param_spec = g_param_spec_pointer ("X-Values",
						"X coordinates",
						"X values of data",
						G_PARAM_CONSTRUCT_ONLY |
						G_PARAM_READWRITE);

   g_object_class_install_property (gobject_class,
				    PROP_X, xyyc_graph_param_spec);

   xyyc_graph_param_spec = g_param_spec_pointer ("Y1-Values",
						"Y1 coordinates",
						"Y1 values of data",
						G_PARAM_CONSTRUCT_ONLY |
						G_PARAM_READWRITE);

   g_object_class_install_property (gobject_class,
				    PROP_Y1, xyyc_graph_param_spec);

   xyyc_graph_param_spec = g_param_spec_pointer ("Y2-Values",
						"Y2 coordinates",
						"Y2 values of data",
						G_PARAM_CONSTRUCT_ONLY |
						G_PARAM_READWRITE);

   g_object_class_install_property (gobject_class,
				    PROP_Y2, xyyc_graph_param_spec);

   xyyc_graph_param_spec = g_param_spec_int ("length", "length of X, Y1 and Y2", "number of data points", G_MININT, G_MAXINT, 0,	/* default value */
					    G_PARAM_CONSTRUCT_ONLY |
					    G_PARAM_READWRITE);

   g_object_class_install_property (gobject_class,
				    PROP_LEN, xyyc_graph_param_spec);

   graph_class->calculate_extrema =
      gtk_databox_xyyc_graph_real_calculate_extrema;

   g_type_class_add_private (klass, sizeof (GtkDataboxXYYCGraphPrivate));
}

static void
gtk_databox_xyyc_graph_init (GtkDataboxXYYCGraph * xyyc_graph)
{
}

static gint
gtk_databox_xyyc_graph_real_calculate_extrema (GtkDataboxGraph * graph,
					      gfloat * min_x, gfloat * max_x,
					      gfloat * min_y, gfloat * max_y)
{
   GtkDataboxXYYCGraphPrivate *priv = GTK_DATABOX_XYYC_GRAPH_GET_PRIVATE (graph);
   guint i;

   g_return_val_if_fail (GTK_DATABOX_IS_XYYC_GRAPH (graph), -1);
   g_return_val_if_fail (min_x, -1);
   g_return_val_if_fail (max_x, -1);
   g_return_val_if_fail (min_y, -1);
   g_return_val_if_fail (max_y, -1);
   g_return_val_if_fail (priv->len, -1);

   *min_x = *max_x = priv->X[0];
   *min_y = *max_y = priv->Y1[0];

   for (i = 1; i < priv->len; ++i)
   {
      if (priv->X[i] < *min_x)
        *min_x = priv->X[i];
      else if (priv->X[i] > *max_x)
        *max_x = priv->X[i];
      if (priv->Y1[i] < *min_y)
        *min_y = priv->Y1[i];
      else if (priv->Y1[i] > *max_y)
        *max_y = priv->Y1[i];
      if (priv->Y2[i] < *min_y)
        *min_y = priv->Y2[i];
      else if (priv->Y2[i] > *max_y)
        *max_y = priv->Y2[i];
   }

   return 0;
}
