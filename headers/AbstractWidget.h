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

#pragma once

class AbstractButton: public GtkWidget
{
    public:
    GtkWidget *button;
    std::string fullLabel;
    AbstractButton(GtkWidget *button, std::string fullLabel): button(button), fullLabel(fullLabel) {}
    template<typename T>
    typename std::enable_if<std::is_same<T, const gchar *>::value || std::is_same<T, std::string>::value
    || std::is_same<T, gchar *>::value, void>::type
    setFullLabel(T value)
    {
        if constexpr (std::is_same<T, std::string>::value)
            this->fullLabel = value;
        else if constexpr (std::is_same<T, const gchar *>::value || std::is_same<T, gchar *>::value)
            this->fullLabel = (std::string)value;
    }
    GtkWidget *getButton() { return this->button; }
    operator GtkWidget *() const
    {
        return this->button;
    }
};

// class AbstractButton: public GtkWidget
// {
//     public:
//     std::string fullLabel;
//     AbstractButton(std::string fullLabel): fullLabel(fullLabel) {}
//     template<typename T>
//     typename std::enable_if<std::is_same<T, const gchar *>::value || std::is_same<T, std::string>::value
//     || std::is_same<T, gchar *>::value, void>::type
//     setFullLabel(T value)
//     {
//         if constexpr (std::is_same<T, std::string>::value)
//             this->fullLabel = value;
//         else if constexpr (std::is_same<T, const gchar *>::value || std::is_same<T, gchar *>::value)
//             this->fullLabel = (std::string)value;
//     }
// };

/*
void ffpaneInsert(const std::string &ffList)
{
    if (f == 0)
    {
        x = 0, y = 0;
        gtk_widget_destroy(ffbutton);
        gtk_widget_destroy(ffpane);
        scwin = gtk_scrolled_window_new(NULL, NULL);
        ffpane = gtk_table_new(8, 8, 0);
        gtk_container_add(GTK_CONTAINER(scwin), ffpane);
        gtk_table_attach(GTK_TABLE(table1), scwin, 2, 8, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
        gtk_widget_show(scwin);
        gtk_widget_show(ffpane);
        gtk_widget_set_size_request(scwin, 1488, 500);
        gtk_widget_set_name(scwin, "scwin");
    }
    else
    {
        f = 0;
    }

    std::stringstream s(ffList);
    std::string ffname;
    if (!addtoPaneList)
    {
        x = 0, y = 0;
        addtoPaneList = 0;
    }

    while (std::getline(s, ffname))
    {
        std::string truncatedLabel = ffname.substr(0, 10);
        if (ffname.size() > 10)
            truncatedLabel += "...";
          
        for (int i = 0; i < ffname.size(); i++)
            if (i % 10 == 0)
                ffname.insert(i, "\n");

        GtkWidget *ffbuttonimage;
        if (ffname.find('.') != std::string::npos)
            ffbuttonimage = gtk_image_new_from_icon_name("document", GTK_ICON_SIZE_BUTTON);
        else
            ffbuttonimage = gtk_image_new_from_icon_name("folder", GTK_ICON_SIZE_BUTTON);

        AbstractButton *ffbutton = new AbstractButton(gtk_button_new(), ffname);
        ffbutton->setFullLabel(ffname);

        GtkWidget *ffbuttonlabel = gtk_label_new(truncatedLabel.c_str());
        GtkWidget *ffbuttonbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_container_add(GTK_CONTAINER(ffbutton->getButton()), ffbuttonbox);
        gtk_box_pack_start(GTK_BOX(ffbuttonbox), ffbuttonimage, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(ffbuttonbox), ffbuttonlabel, FALSE, FALSE, 0);
        gtk_widget_show_all(ffbutton->getButton());
        gtk_widget_set_name(ffbutton->getButton(), "ffbutton");
        gtk_table_attach(GTK_TABLE(ffpane), ffbutton->getButton(), x, x + 1, y, y + 1, GTK_FILL, GTK_FILL, 0, 0);
      
        if ((x + 1) == 10) {
            x = -1;
            y++;
        }
        x++;
        gtk_widget_show(ffbutton->getButton());
        gtk_widget_set_size_request(ffbutton->getButton(), 100, 80);
        
        g_signal_connect(ffbutton->getButton(), "enter-notify-event", G_CALLBACK(ggh::on_entering_ffButton), g_strdup(ffname.c_str()));
        g_signal_connect(ffbutton->getButton(), "leave-notify-event", G_CALLBACK(ggh::on_leaving_ffButton), g_strdup(truncatedLabel.c_str()));
        gtk_widget_set_name(ffbutton->getButton(), "ffbutton");
        g_signal_connect(ffbutton->getButton(), "clicked", G_CALLBACK(ffClickedF), ffbutton->getButton());
        g_signal_connect(ffbutton->getButton(), "pressed", G_CALLBACK(ffPressedF), ffbutton->getButton());
        g_signal_connect(ffbutton->getButton(), "clicked", G_CALLBACK(ffClickedorPressedF), ffbutton->getButton());
        g_signal_connect(ffbutton->getButton(), "pressed", G_CALLBACK(ffClickedorPressedF), ffbutton->getButton());
    }

    pathRibbonComponent::updateAddressBar(currentPath);
}
*/