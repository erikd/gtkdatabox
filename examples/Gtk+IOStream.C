/* Copyright 2000-2012 Matt Flax <flatmax@flatmax.org>
   This file is part of GTK+ IOStream class set

   GTK+ IOStream is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GTK+ IOStream is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You have received a copy of the GNU General Public License
   along with GTK+ IOStream
 */
#include "gtkInterface.H"
#include "Plot.H"

/** 

This example is a C++ example on how to simply plot using GtkDataBox in a C++ environment
using the Gtk+ IOStream project.

It will plot a straight line and the top of a sinusoid.
It will also plot the area between the two coloured in !

Requirement, please download the latest version of Gtk+ IOStream from here :
http://sourceforge.net/projects/gtkiostream/
You can also get the latest Git repository :
git clone ssh://flatmax@git.code.sf.net/p/gtkiostream/code gtkiostream-code

To compile, run the following :
g++ -I/path/to/GtkIOStream `pkg-config --cflags gtkdatabox` Gtk+IOStream.C -o Gtk+IOStream `pkg-config --libs gtkdatabox`

To test, run the following :
./PlotTest
*/

// Quit button callback
static void quit(void *wid, gpointer data) {
    gtk_main_quit();
}

// some definitions
#define cnt 101
float x[cnt], y[cnt];

#include <time.h>
#include <math.h>

#define TYPE float

// timeout function - shifts the sinusoid along and replots - shows how simple it is to plot dynamically changing data.
gboolean sinusoid(gpointer user_data){
    // shift the sinusoid on.
    int s=time(NULL);
    for (int i=0;i<cnt;i++)
        y[i]=100.0*sin((TYPE)i/((TYPE)cnt-1)*2*M_PI+s);


    // replot the data
    Plot **p = (Plot**) user_data;
    p[0]->replot();
    p[1]->replot();
    return 1;
}


int main(int argc, char *argv[]) {

    gtk_init( &argc, &argv );

    GtkInterface topWindow;

    gtk_widget_set_size_request (topWindow.win, 1000, 500);
    //hBox<< figure.getWidget();

    TYPE x1[cnt], y1[cnt]; // some more data for a second plot
    for (int i=1;i<cnt;i++)
        x[i]=y[i]=(TYPE)i;
    for (int i=1;i<cnt;i++){
        x1[i]=(TYPE)i;
        y1[i]=(TYPE)i+100.0;
    }

    /* ////////////////////////////////////////////////////////*/
    // The actual plotting

    Plot figure;  // create and show a Figure
    figure.show();
    figure.plot(x,y,cnt); // Plot the first line with default colours
    figure.hold(true);
    figure.plot(x1,y1,cnt, "r2");
    figure.limits(1.0,101.0,1.0,201.0);
    figure.set(figure.gca(), "XLabelFormat", "%%.1f", NULL);
    figure.grid(true);

    Plot figure2; // create a second figure for plotting a region
    figure2.show();
    figure2.plot(x,y,y1,cnt); // Plot the first line with default colours
    figure2.limits(1.0,101.0,1.0,201.0);
    figure2.grid(true);

    /* /////////////////////////////////////////////////////////*/

    HBox hBox;
    hBox << BoxIS(TRUE, TRUE)<<figure.getWidget()  // show the figures
         << BoxIS(TRUE, TRUE)<<figure2.getWidget();
    hBox.show(); // show the horizontal box
    topWindow<< hBox; // add it to the top window

    // setup a 1 second timer to change the plot
    Plot *plots[2]; plots[0]=&figure; plots[1]=&figure2;
    g_timeout_add_seconds(1, sinusoid, (gpointer)plots);

    gtk_main();
}
