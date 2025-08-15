#include <iostream>
#include <gtk-3.0/gtk/gtk.h>

#define COPY_ACTION 0
#define COMPRESS_ACTION 1
#define DELETE_ACTION 2
#define RENAME_ACTION 3
#define CUT_ACTION 4
#define PROPERTIES_ACTION 5

GtkWidget *popover = nullptr;

void ffButtonPopupMenuHandler(GtkWidget *w, gpointer data);

struct ButtonActionPayload
{
    GtkWidget *button;
    int action;
    ButtonActionPayload(GtkWidget *button, int action): button(button), action(action) {}
};


class FfButtonPopupMenu {
public:
    static FfButtonPopupMenu *instance;

    static void deletePopover(GtkWidget *widget, gpointer data) {
        std::cout << "delete_popover\n";
        if (instance != nullptr) {
            gtk_widget_destroy(popover);
            delete instance;
            instance = nullptr;
        }
    }

    FfButtonPopupMenu(GtkWidget *widget, GdkEventButton *event) {
        popover = gtk_popover_new(widget);
        gtk_popover_set_position(GTK_POPOVER(popover), GTK_POS_BOTTOM);
        gtk_popover_set_modal(GTK_POPOVER(popover), TRUE);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        GtkWidget *label = gtk_label_new("Options:");
        gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
        // GtkWidget *close_btn = gtk_button_new_with_label("Close");
        GtkWidget *copyButton = gtk_button_new_with_label("Copy");
        GtkWidget *cutButton = gtk_button_new_with_label("Cut");
        GtkWidget *deleteButton = gtk_button_new_with_label("Delete");
        GtkWidget *renameButton = gtk_button_new_with_label("Rename");
        GtkWidget *compressButton = gtk_button_new_with_label("Compress");
        GtkWidget *propertiesButton = gtk_button_new_with_label("Properties");
        // gtk_box_pack_start(GTK_BOX(box), close_btn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), copyButton, false, false, 0);
        gtk_box_pack_start(GTK_BOX(box), cutButton, false, false, 0);
        gtk_box_pack_start(GTK_BOX(box), deleteButton, false, false, 0);
        gtk_box_pack_start(GTK_BOX(box), renameButton, false, false, 0);
        gtk_box_pack_start(GTK_BOX(box), compressButton, false, false, 0);
        gtk_box_pack_start(GTK_BOX(box), propertiesButton, false, false, 0);
        GList *boxChildren = gtk_container_get_children(GTK_CONTAINER(box));
        // if (boxChildren != nullptr)
        // {
        //     for (GList *iter = boxChildren; iter != NULL; iter = iter->next)
        //     {
        //         GtkWidget *child = GTK_WIDGET(iter->data);
        //         if (GTK_IS_BUTTON(child))
        //         {
        //             std::cout << "child: " << gtk_button_get_label(GTK_BUTTON(child)) << "\n";
        //             gtk_widget_set_name(child, "ffbuttonContextMenuChild");
        //         }
        //     }
        // }
                    gtk_widget_set_name(copyButton, "ffbuttonContextMenuChild");
        gtk_widget_set_name(popover, "contextMenu");
        gtk_widget_set_name(copyButton, "contextMenuItem");
        gtk_widget_set_name(cutButton, "contextMenuItem");
        gtk_widget_set_name(deleteButton, "contextMenuItem");
        gtk_widget_set_name(renameButton, "contextMenuItem");
        gtk_widget_set_name(compressButton, "contextMenuItem");
        gtk_widget_set_name(propertiesButton, "contextMenuItem");
        // g_signal_connect(close_btn, "clicked", G_CALLBACK(deletePopover), nullptr);
        ButtonActionPayload *copyPayload = new ButtonActionPayload(widget, COPY_ACTION);
        ButtonActionPayload *cutPayload = new ButtonActionPayload(widget, CUT_ACTION);
        ButtonActionPayload *deletePayload = new ButtonActionPayload(widget, DELETE_ACTION);
        ButtonActionPayload *renamePayload = new ButtonActionPayload(widget, RENAME_ACTION);
        ButtonActionPayload *compressPayload = new ButtonActionPayload(widget, COMPRESS_ACTION);
        ButtonActionPayload *propertiesPayload = new ButtonActionPayload(widget, PROPERTIES_ACTION);
        g_signal_connect(copyButton, "clicked", G_CALLBACK(ffButtonPopupMenuHandler), copyPayload);
        g_signal_connect(cutButton, "clicked", G_CALLBACK(ffButtonPopupMenuHandler), cutPayload);
        g_signal_connect(deleteButton, "clicked", G_CALLBACK(ffButtonPopupMenuHandler), deletePayload);
        g_signal_connect(renameButton, "clicked", G_CALLBACK(ffButtonPopupMenuHandler), renamePayload);
        g_signal_connect(compressButton, "clicked", G_CALLBACK(ffButtonPopupMenuHandler), compressPayload);
        g_signal_connect(propertiesButton, "clicked", G_CALLBACK(ffButtonPopupMenuHandler), propertiesPayload);
        // g_signal_connect(copyButton, "clicked", G_CALLBACK(ffButtonPopupMenuHandler), GINT_TO_POINTER(COPY_ACTION));

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