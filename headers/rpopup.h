#include <iostream>
#include <gtk-3.0/gtk/gtk.h>

void ffpaneInsert(std::string);
void createNewFolder(GtkWidget*w);

std::string stringToBeSent = "";

class Popup
{
private:
        static Popup *firstInstance;
        int value;
        static void ArrangeByDate()
        {
                std::string ffList = runcomm("ls -t " + currentPath);
                std::cout << "latest curpath: " << currentPath << "\n";
                ffpaneInsert(ffList);
        }
        static void ArrangeByName()
        {
                std::string ffList = runcomm("ls " + currentPath);

                std::cout << "latest curpath: " << currentPath << "\n";
                ffpaneInsert(ffList);
        }
        static void PopupDelete()
        {
                std::cout << "Popup deleted...\n";
                sendString = false;
                stringToBeSent = "";
        }
        static void menuOption3f()
        {
                stringToBeSent = "Create Folder";
                createNewFolder(NULL);
        }
        static void menuOption4f()
        {
                stringToBeSent = "Create File";
                createNewFolder(NULL);
        }
        Popup(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
        {
                // std::cout << "Code to run once\n";
                const gchar *wn = gtk_widget_get_name(widget);
                std::cout << "widget: " << &widget << " " << user_data << " " << wn << std::endl;
                if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_SECONDARY && strcmp(wn, "ffbutton") != 0)
                {
                        std::string fdstr = "Create Folder";
                        std::string flstr = "Create File";
                        std::string *fdstr_ptr = new std::string(fdstr);
                        sendString = true;
                        GtkWidget *menu = gtk_menu_new();
                        GtkWidget *menuOption1 = gtk_menu_item_new_with_label("Arrange by name");
                        GtkWidget *menuOption2 = gtk_menu_item_new_with_label("Arrange by Date");
                        GtkWidget *menuOption3 = gtk_menu_item_new_with_label("Create New Folder");
                        GtkWidget *menuOption4 = gtk_menu_item_new_with_label("Create New File");
                        gtk_widget_set_name(menuOption1, "menuOption");
                        gtk_widget_set_name(menuOption2, "menuOption");
                        gtk_widget_set_name(menuOption3, "menuOption");
                        gtk_widget_set_name(menuOption4, "menuOption");
                        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuOption1);
                        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuOption2);
                        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuOption3);
                        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuOption4);
                        g_signal_connect(menuOption1, "activate", G_CALLBACK(ArrangeByName), NULL);
                        g_signal_connect(menuOption2, "activate", G_CALLBACK(ArrangeByDate), NULL);
                        g_signal_connect(menuOption3, "activate", G_CALLBACK(menuOption3f), NULL);
                        g_signal_connect(menuOption4, "activate", G_CALLBACK(menuOption4f), NULL);
                        g_signal_connect(menu, "deactivate", G_CALLBACK(PopupDelete), NULL);
                        gtk_widget_show_all(menu);
                        gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);
                }
        }

public:
        static Popup *getInstance(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
        {
                sendString = false;
                if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY || event->button == GDK_BUTTON_SECONDARY)
                {
                        delete firstInstance;
                        firstInstance = nullptr;
                        sendString = false;
                }
                if (firstInstance == nullptr)
                        firstInstance = new Popup(widget, event, user_data);
                return firstInstance;
        }
        static void deleteInstance()
        {
                if (firstInstance != nullptr)
                {
                        delete firstInstance;
                        firstInstance = nullptr;
                        sendString = false;
                }
        }
        int getValue() const
        {
                return value;
        }
        ~Popup()
        {
                std::cout << "Destructor exec\n";
                sendString = false;
                // delete firstInstance;
        }
        Popup(const Popup &) = delete;
        Popup &operator=(const Popup &) = delete;
};

Popup *Popup::firstInstance = nullptr;

// public:
//         static void createPopup(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
//         {
// const gchar *wn = gtk_widget_get_name(widget);
// std::cout << "widget: " << &widget << " " << user_data << " " << wn << std::endl;
// if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_SECONDARY)
// {
//         GtkWidget *menu = gtk_menu_new();
//         GtkWidget *menuOption1 = gtk_menu_item_new_with_label("Arrange by name");
//         gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuOption1);
//         gtk_widget_show_all(menu);
//         gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);
// }
//         }
// }
;