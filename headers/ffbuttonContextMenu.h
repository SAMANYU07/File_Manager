#include <iostream>
#include <gtk-3.0/gtk/gtk.h>

GtkWidget *popover;

class FfButtonPopupMenu
{
        public:
        static FfButtonPopupMenu *instance;
        static void deletePopover()
        {
                std::cout << "delete_popover\n";
                if (instance != nullptr)
                {
                        delete instance;
                        instance = nullptr;
                }
        }
        FfButtonPopupMenu(GtkWidget *widget, GdkEventButton *event, gpointer data)
        {
                popover = gtk_popover_new(widget);
                GtkWidget *popover_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
                GtkWidget *test = gtk_label_new("text_label");
                gtk_container_add(GTK_CONTAINER(popover), popover_box);
                gtk_box_pack_start(GTK_BOX(popover_box), test, FALSE, FALSE, 0);
                gtk_widget_show_all(popover);
                // gtk_popover_popdown(GTK_POPOVER(popover));
                // gtk_popover_set_modal(GTK_POPOVER(popover), TRUE);
                gtk_widget_grab_focus(popover);
                g_signal_connect(popover, "focus-out-event", G_CALLBACK(deletePopover), nullptr);
        }
        static gboolean initFfButtonPopupMenu(GtkWidget *widget, GdkEventButton *event, gpointer data)
        {
                if (event->button == GDK_BUTTON_PRIMARY)
                        return FALSE;
                // if (instance == nullptr)
                        instance = new FfButtonPopupMenu(widget, event, data);
                        return TRUE;
                // else
                // {
                //         delete instance;
                //         instance = nullptr;
                // }
                // return instance;
        }
};

FfButtonPopupMenu *FfButtonPopupMenu::instance = nullptr;