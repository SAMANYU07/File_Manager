#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include "../StaticMacros.h"

class SettingsPanel
{
        public:
        GtkWidget *win, *grid, *customAppCheckBox, *closeButton, *applyAndCloseButton;
        ConfigManager *configManager = ConfigManager::getInstance();
        std::map<std::string, std::string> configMap = configManager->getAppConfig();

        void processConfigValues()
        {
                if (configMap.at(OPEN_IN).compare(CUSTOM_APP) == 0)
                        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(customAppCheckBox), true);
        }

        static void closeSettingsPanel(GtkWidget *win, gpointer data)
        {
                gtk_main_quit();
                gtk_widget_destroy((GtkWidget *)data);
        }

        static void updateConfigValue(GtkWidget *toggleButton, gpointer data)
        {
                gboolean buttonValue = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggleButton));
                //update config logic
                ConfigManager *configurationManager = static_cast<ConfigManager *>(data);
                if (buttonValue)
                        configurationManager->updateAppConfig(std::pair<std::string, std::string>(OPEN_IN, CUSTOM_APP));
                else
                        configurationManager->updateAppConfig(std::pair<std::string, std::string>(OPEN_IN, DEFAULT_APP));
        }

        SettingsPanel()
        {
                win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                customAppCheckBox = gtk_toggle_button_new_with_label("Open files in custom media viewers.");
                grid = gtk_grid_new();
                closeButton = gtk_button_new_with_label("Close");
                applyAndCloseButton = gtk_button_new_with_label("Apply & Close");
                gtk_grid_attach(GTK_GRID(grid), gtk_label_new("File Manager Settings"), 0, 0, 1, 1);
                gtk_grid_attach(GTK_GRID(grid), customAppCheckBox, 0, 1, 1, 1);
                gtk_grid_attach(GTK_GRID(grid), applyAndCloseButton, 0, 2, 1, 1);
                gtk_grid_attach(GTK_GRID(grid), closeButton, 1, 2, 1, 1);
                g_signal_connect(win, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
                g_signal_connect(customAppCheckBox, "toggled", G_CALLBACK(updateConfigValue), configManager);
                g_signal_connect(closeButton, "clicked", G_CALLBACK(closeSettingsPanel), win);
                gtk_container_add(GTK_CONTAINER(win), grid);
                processConfigValues();
                gtk_widget_set_name(customAppCheckBox, "GtkToggleButton");
                gtk_window_set_default_size(GTK_WINDOW(win), 400, 400);
                gtk_widget_show_all(win);
                gtk_main();
        }

        ~SettingsPanel()
        {
                configManager->saveConfig();
        }
};