#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <sstream>
#include <map>

GtkWidget *navPane, *tempelem;
std::list<std::string> diskNameVector;
std::map<int, std::string> diskMap;
int diskID = 6;

std::string runcomm(std::string);

void createNavPane() {
        navPane = gtk_list_box_new();
        std::string userDisks = runcomm("ls /media/" + (std::string)g_get_user_name());
        std::stringstream s(userDisks);
        std::string diskName;
        while (std::getline(s, diskName))
        {
                std::cout << "diskname: " << "'" + diskName + "'" << "\n";
                tempelem = gtk_label_new((const gchar *)diskName.c_str());
                diskNameVector.push_front("\'" + diskName + "\'");
                gtk_list_box_prepend(GTK_LIST_BOX(navPane), tempelem);
                gtk_widget_set_name(tempelem, "tempelem");
        }
        for (auto it = diskNameVector.begin(); it != diskNameVector.end(); ++it)
                diskMap.insert(std::pair<int, std::string>(diskID++, *it));
        std::cout << "diskvecsize: " << diskNameVector.size() << "\n";
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
                GtkListBoxRow *r = gtk_list_box_get_row_at_index((GtkListBox *)navPane, 0);
                gtk_widget_set_name((GtkWidget *)r, "tempelem");
        }

}