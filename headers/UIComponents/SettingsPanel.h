#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include "../StaticMacros.h"

class SettingsPanel
{
        public:
        GtkWidget *win, *grid, *customAppCheckBox, *closeButton, *applyAndCloseButton;

        void processConfigValues()
        {
                ConfigManager *configManager = ConfigManager::getInstance();
                std::map<std::string, std::string> configMap = configManager->getAppConfig();
                if (configMap.at(OPEN_IN).compare(CUSTOM_APP) == 0)
                        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(customAppCheckBox), true);
        }

        SettingsPanel()
        {
                win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                customAppCheckBox = gtk_toggle_button_new_with_label("Open files in custom media viewers.");
                gtk_widget_set_name(customAppCheckBox, "GtkToggleButton");
                grid = gtk_grid_new();
                gtk_grid_attach(GTK_GRID(grid), gtk_label_new("File Manager Settings"), 0, 0, 1, 1);
                gtk_grid_attach(GTK_GRID(grid), customAppCheckBox, 0, 1, 1, 1);
                g_signal_connect(win, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
                gtk_container_add(GTK_CONTAINER(win), grid);
                processConfigValues();
                gtk_window_set_default_size(GTK_WINDOW(win), 400, 400);
                gtk_widget_show_all(win);
                gtk_main();
        }
};