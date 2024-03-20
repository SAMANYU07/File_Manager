#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>

GtkWidget *navPane, *tempelem;

void createNavPane() {
        navPane = gtk_list_box_new();
        for (int i = 0; i < 6; i++) {
                if (i == 0)
                        tempelem = gtk_label_new("Videos");
                if (i == 1)
                        tempelem = gtk_label_new("Pictures");
                if (i == 2)
                        tempelem = gtk_label_new("Music");
                if (i == 3)
                        tempelem = gtk_label_new("Downloads");
                if (i == 4)
                        tempelem = gtk_label_new("Documents");
                if (i == 5)
                        tempelem = gtk_label_new("Home");
                gtk_list_box_prepend(GTK_LIST_BOX(navPane), tempelem);
        }
}