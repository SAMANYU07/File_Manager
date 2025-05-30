#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <vector>
#include "ggh.h"

#pragma once

GtkWidget *pathRibbon, *backButton, *addressBar, *searchEntry, *searchButton;
GtkEntryBuffer *addressBarBuffer;
std::vector<int> slashpos{0, 7};
std::string currentPath;

class pathRibbonComponent
{
        public:
        static int createPathRibbon()
        {
                backButton = gtk_button_new();
                addressBar = gtk_entry_new();
                searchEntry = gtk_entry_new();
                searchButton = gtk_button_new();
                gtk_entry_set_buffer(GTK_ENTRY(addressBar), addressBarBuffer);
                gtk_entry_set_text(GTK_ENTRY(addressBar), "Address->");
                gtk_widget_set_name(backButton, "backButton");
                gtk_widget_set_name(addressBar, "addressBar");
                gtk_widget_set_name(searchButton, "searchButton");
                gtk_widget_set_name(searchEntry, "searchEntry");
                gtk_editable_set_editable((GtkEditable *)addressBar, false);
                gtk_widget_set_can_focus(addressBar, false);
                GdkColor addressBarColor;
                gdk_color_parse("#141414", &addressBarColor);
                gtk_widget_modify_bg(addressBar, GTK_STATE_NORMAL, &addressBarColor);
                gtk_widget_set_name(addressBar, "addressBar");
                gtk_widget_set_name(backButton, "backButton");
                ggh::attach_label_icon_with_button(backButton, "Go Back", "go-previous");
                ggh::attach_label_icon_with_button(searchButton, "Search", "edit-find");
        }

        static void updateAddressBar(std::string newAddress)
        {
                gtk_entry_set_text(GTK_ENTRY(addressBar), newAddress.c_str());
        }

        static void updateslashpos()
        {
                slashpos.clear();
                for (int i = 0; i < currentPath.size(); i++)
                        if (currentPath[i] == '/')
                                slashpos.push_back(i);
                for (int i = 0; i < slashpos.size(); i++)
                        std::cout << slashpos[i] << " ";
                std::cout << std::endl;
        }

        static std::string searchFunc()
        {
                const gchar *inputQuery = gtk_entry_get_text((GtkEntry *)searchEntry);
                // std::cout << "inp_query: " << (std::string)inputQuery << "\n";
                // ggh::runcomm("cd " + currentPath);
                std::string queryResult = ggh::runcomm("ls " + currentPath + " | grep '" + (std::string)inputQuery + "'");
                std::cout << "search res: \n" << queryResult << "\n";
                return queryResult;
        }
};