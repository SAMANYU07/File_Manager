#include <iostream>
#include <sstream>
#include <gtk-3.0/gtk/gtk.h>
#include <filesystem>
#include "ggh.h"

class PropertiesPanel
{
        public:
        GtkWidget *win;
        static void destroy_win(GtkWidget *w, gpointer data)
        {
                gtk_main_quit();
                PropertiesPanel *p = (PropertiesPanel *) data;
                gtk_widget_destroy(p->win);
        }
        GtkWidget *PropertiesPanel_init(std::string fileName, std::string filePath)
        {
                std::string filePathWithName = filePath + "/" + fileName;
                for (int i = 0; i < filePathWithName.size(); i++)
                        if (filePathWithName[i] == '\'')
                                filePathWithName.replace(i, 1, "");
                GtkWidget *nameLabel, *nameValueLabel, *typeLabel, *typeValueLabel;
                GtkWidget *sizeLabel, *sizeValueLabel, *locationLabel, *locationValueLabel;
                GtkWidget *table;
                GtkWidget *closeButton;
                win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                table = gtk_table_new(8, 8, 0);
                nameLabel = gtk_label_new("Name");
                nameValueLabel = gtk_label_new(fileName.c_str());
                typeLabel = gtk_label_new("Type");
                std::cout << "fileName:: " << fileName << "\n";
                if (std::filesystem::is_directory(filePathWithName))
                        std::cout << "its dir\n";
                else
                        std::cout << "its file\n";
                std::string fileExtension = std::filesystem::is_directory(filePathWithName) ? fileName : fileName.substr(fileName.find("."), fileName.size() - fileName.find("."));
                typeValueLabel = gtk_label_new(fileExtension.c_str());
                sizeLabel = gtk_label_new("Size");
                std::string fileLocation = "";
                std::cout << "newFilePAth: " << filePathWithName << "\n";
                std::uintmax_t fileSize = std::filesystem::file_size(filePathWithName);
                sizeValueLabel = gtk_label_new(std::to_string(fileSize).c_str());
                locationLabel = gtk_label_new("Location");
                locationValueLabel = gtk_label_new(filePath.c_str());
                closeButton = gtk_button_new_with_label("Close");
                gtk_widget_set_name(closeButton, "cancelButton");
                // std::filesystem::file_time_type fs_lastModifiedTime = std::filesystem::last_write_time(filePathWithName);
                // std::chrono::system_clock::time_point sysTimePoint = std::chrono::time_point_cast<std::chrono::system_clock::duration>(fs_lastModifiedTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
                // std::time_t lastModifiedTime = std::chrono::system_clock::to_time_t(sysTimePoint);
                // std::tm tm = *std::localtime(&lastModifiedTime);
                // std::stringstream ss;
                // ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
                // std::string lastModifiedTimeStr;
                // ss >> lastModifiedTimeStr;
                gtk_table_attach(GTK_TABLE(table), nameLabel, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(table), nameValueLabel, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(table), typeLabel, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(table), typeValueLabel, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(table), sizeLabel, 0, 1, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(table), sizeValueLabel, 1, 2, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(table), locationLabel, 0, 1, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(table), locationValueLabel, 1, 2, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(table), closeButton, 0, 2, 4, 5, GTK_FILL, GTK_FILL, 0, 0);
                g_signal_connect(closeButton, "clicked", G_CALLBACK(destroy_win), this);
                gtk_container_add(GTK_CONTAINER(win), table);
                gtk_widget_show_all(win);
                gtk_main();
        }
};