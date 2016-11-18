/* $Id: addremove.c 4 2008-06-22 09:19:11Z rbock $ */
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_points.h>
#include <gtkdatabox_util.h>
#include <math.h>

#define POINTS 200

GtkDataboxGraph **graphs;

/*----------------------------------------------------------------
 *  databox addremove
 *----------------------------------------------------------------*/
void
add_data (GtkDatabox * box, gint index)
{
   GdkRGBA color;
   gint add_index;

   color.red = 0.5 + 0.5 * (index + 1) / 10;
   color.green = 1 - 0.5 * index / 10;
   color.blue = 1;
   color.alpha = 1;

   gtk_databox_graph_set_color (graphs[index], &color);

   add_index = gtk_databox_graph_add (GTK_DATABOX (box), graphs[index]);

   g_print ("Added graph has index: %d\n", add_index);
}

static gboolean
addremove_func (GtkDatabox * box)
{
   static gboolean remove = TRUE;
   gfloat min_x, max_x;
   gfloat min_y, max_y;
   static gint index;
   static gint hide;

   if (remove)
   {
      index = (int) (10. * rand () / (RAND_MAX + 1.0));
      hide = (int) (2. * rand () / (RAND_MAX + 1.0));

      if (!hide)
      {
	 g_print ("removing %d\n", index);
	 gtk_databox_graph_remove (box, graphs[index]);
	 g_print ("Removed: %d\n", index);
      }
      else
      {
	 g_print ("hiding %d\n", index);
	 gtk_databox_graph_set_hide (graphs[index], TRUE);
      }
   }
   else
   {
      if (!hide)
      {
	 g_print ("Adding graph %d\n", index);
	 add_data (box, index);
      }
      else
      {
	 g_print ("Showing graph %d\n", index);
	 gtk_databox_graph_set_hide (graphs[index], FALSE);

      }
   }
   remove = !remove;

   gtk_databox_auto_rescale (box, 0.05);
   gtk_databox_calculate_extrema (box, &min_x, &max_x, &min_y, &max_y);
   g_print ("minX: %10f, minY:%10f, maxX:%10f, maxY:%10f\n", min_x, min_y,
	    max_x, max_y);

   gtk_widget_queue_draw (GTK_WIDGET(box));

   return (TRUE);
}


static void
create_addremove (void)
{
   GtkWidget *window = NULL;
   GtkWidget *box1;
   GtkWidget *box2;
   GtkWidget *close_button;
   GtkWidget *box;
   GtkWidget *table;
   GtkWidget *label;
   GtkWidget *separator;
   GtkDataboxGraph *graph;
   GdkRGBA color;
   gfloat *X;
   gfloat *Y;
   gint i, j;

   graphs = g_new0 (GtkDataboxGraph *, 10);

   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_size_request (window, 400, 400);

   g_signal_connect (G_OBJECT (window), "destroy",
		     G_CALLBACK (gtk_main_quit), NULL);

   gtk_window_set_title (GTK_WINDOW (window), "GtkDatabox: Basics");
   gtk_container_set_border_width (GTK_CONTAINER (window), 0);

   box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
   gtk_container_add (GTK_CONTAINER (window), box1);

   label =
      gtk_label_new
      ("When you click on the \"Add/Remove\" button, a graph will be removed.\nWhen you click again, it is added again.\n\n(This is just for testing add and remove functions)");
   gtk_box_pack_start (GTK_BOX (box1), label, FALSE, FALSE, 0);
   separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
   gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, FALSE, 0);

   /* Create a GtkDatabox widget along with scrollbars and rulers */
   gtk_databox_create_box_with_scrollbars_and_rulers (&box, &table,
						      TRUE, TRUE, TRUE, TRUE);

   gtk_box_pack_start (GTK_BOX (box1), table, TRUE, TRUE, 0);

   color.red = 0.33;
   color.green = 0.33;
   color.blue = 0.33;
   pgtk_widget_override_background_color (box, GTK_STATE_FLAG_NORMAL, &color);

   X = g_new0 (gfloat, POINTS);

   for (i = 0; i < POINTS; i++)
   {
      X[i] = i * 5;
   }
   for (i = 0; i < 10; i++)
   {
      Y = g_new0 (gfloat, POINTS);
      for (j = 0; j < POINTS; j++)
      {
	 Y[j] = i * 100.0 * sin (i * j / 50.);
      }
      graphs[i] = gtk_databox_points_new (POINTS, X, Y, &color, 1);
      add_data (GTK_DATABOX (box), i);
   }
   X = g_new0 (gfloat, 4);
   Y = g_new0 (gfloat, 4);

   X[0] = -100.;
   Y[0] = -1100.;
   X[1] = -100.;
   Y[1] = +1100.;
   X[2] = +1100.;
   Y[2] = -1100.;
   X[3] = +1100.;
   Y[3] = +1100.;


   color.red = 1;
   color.green = 0;
   color.blue = 0;
   color.alpha = 1;

   graph = gtk_databox_points_new (4, X, Y, &color, 3);
   gtk_databox_graph_add (GTK_DATABOX (box), graph);

   gtk_databox_auto_rescale (GTK_DATABOX (box), 0.05);

   separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
   gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 0);

   box2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
   gtk_box_pack_end (GTK_BOX (box1), box2, FALSE, TRUE, 0);
   close_button = gtk_button_new_with_label ("Add/Remove");
   g_signal_connect_swapped (G_OBJECT (close_button), "clicked",
			     G_CALLBACK (addremove_func), G_OBJECT (box));
   gtk_box_pack_start (GTK_BOX (box2), close_button, TRUE, TRUE, 0);

   close_button = gtk_button_new_with_label ("Close");
   g_signal_connect_swapped (G_OBJECT (close_button), "clicked",
			     G_CALLBACK (gtk_main_quit), G_OBJECT (box));
   gtk_box_pack_start (GTK_BOX (box2), close_button, TRUE, TRUE, 0);
   gtk_widget_set_can_default(close_button, TRUE);
   gtk_widget_grab_default (close_button);

   gtk_widget_show_all (window);
}

gint
main (gint argc, char *argv[])
{
   gtk_init (&argc, &argv);

   create_addremove ();
   gtk_main ();

   return 0;
}
