#include <iostream>
#include <gtk-3.0/gtk/gtk.h>


GtkWidget *popover = nullptr;

class FfButtonPopupMenu {
public:
    static FfButtonPopupMenu *instance;

    static void deletePopover(GtkWidget *widget, gpointer data) {
        std::cout << "delete_popover\n";
        if (instance != nullptr) {
            gtk_widget_destroy(popover);  // Properly destroy the widget
            delete instance;
            instance = nullptr;
        }
    }

    FfButtonPopupMenu(GtkWidget *widget, GdkEventButton *event) {
        popover = gtk_popover_new(widget);
        gtk_popover_set_position(GTK_POPOVER(popover), GTK_POS_BOTTOM);
        gtk_popover_set_modal(GTK_POPOVER(popover), TRUE); // Optional, helps keep it open

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        GtkWidget *label = gtk_label_new("Options:");
        gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

        // Optional: add a button to close the popover
        GtkWidget *close_btn = gtk_button_new_with_label("Close");
        GtkWidget *copyButton = gtk_button_new_with_label("Copy");
        gtk_box_pack_start(GTK_BOX(box), close_btn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), copyButton, false, false, 0);
        g_signal_connect(close_btn, "clicked", G_CALLBACK(deletePopover), nullptr);
        g_signal_connect(copyButton, "clicked", G_CALLBACK(deletePopover), nullptr);

        gtk_container_add(GTK_CONTAINER(popover), box);
        gtk_widget_show_all(popover);

        gtk_popover_popup(GTK_POPOVER(popover));
    }

    static gboolean initFfButtonPopupMenu(GtkWidget *widget, GdkEventButton *event, gpointer data) {
        if (event->button == GDK_BUTTON_PRIMARY)
            return FALSE;

        if (instance != nullptr)
            deletePopover(nullptr, nullptr);

        instance = new FfButtonPopupMenu(widget, event);
        return TRUE;
    }
};

FfButtonPopupMenu *FfButtonPopupMenu::instance = nullptr;