#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <archive.h>
#include <archive_entry.h>
#include <fstream>
#include <array>
#include <memory>
// #include <array>

#pragma once

struct GtkButtonWrapper
{
        public:
        GtkButton *button;
        const gchar *buttonLabel;
        GtkButtonWrapper(GtkButton *button, const gchar *buttonLabel): button(button), buttonLabel(buttonLabel)
        {
                // std::cout << "GtkButtonWrapper-> buttonLabel: " << this->buttonLabel << "\n";
        }
};

class ggh
{
public:
        static void attach_label_icon_with_button(GtkWidget *button, const gchar *label, const gchar *icon)
        {
                GtkWidget *buttonIcon = gtk_image_new_from_icon_name(icon, GTK_ICON_SIZE_BUTTON);
                GtkWidget *buttonLabel = gtk_label_new(label);
                GtkWidget *buttonAlignment = gtk_alignment_new(0.5, 0.5, 0, 0);
                GtkWidget *buttonGrid = gtk_grid_new();
                gtk_container_add(GTK_CONTAINER(buttonAlignment), buttonGrid);
                gtk_container_add(GTK_CONTAINER(button), buttonAlignment);
                gtk_grid_attach(GTK_GRID(buttonGrid), buttonIcon, 0, 0, 1, 1);
                gtk_grid_attach(GTK_GRID(buttonGrid), buttonLabel, 1, 0, 1, 1);
                gtk_button_set_image_position((GtkButton *)button, GTK_POS_LEFT);
        }
        static std::string get_button_label(GtkWidget *button)
        {
                GtkWidget *buttonAlignment = gtk_bin_get_child(GTK_BIN(button));
                if (GTK_IS_ALIGNMENT(buttonAlignment))
                {
                        GtkWidget *buttonGrid = gtk_bin_get_child(GTK_BIN(buttonAlignment));
                        if (GTK_IS_GRID(buttonGrid))
                        {
                                GList *children = gtk_container_get_children(GTK_CONTAINER(buttonGrid));
                                if (children)
                                {
                                        GtkWidget *label = GTK_WIDGET(children->data);
                                        if (GTK_IS_LABEL(label))
                                        {
                                                const gchar *text = gtk_label_get_text(GTK_LABEL(label));
                                                g_list_free(children);
                                                return text;
                                        }
                                        g_list_free(children);
                                }
                        }
                }
                return nullptr;
        }
        static std::string extractLabelFromButton(GtkWidget *button)
        {
                GtkWidget *ffbuttonbox = gtk_bin_get_child(GTK_BIN(button));
                GList *children = gtk_container_get_children(GTK_CONTAINER(ffbuttonbox));
                GtkWidget *labelWidget = nullptr;
                if (g_list_length(children) > 1)
                {
                        labelWidget = GTK_WIDGET(g_list_nth_data(children, 1));
                }
                const gchar *labelText = gtk_label_get_text(GTK_LABEL(labelWidget));
                // std::string label = labelText ? labelText : "";
                std::string label;
                if (labelText)
                {
                        for (const gchar *c = labelText; *c; ++c)
                                if (*c != '\n')
                                        label += *c;
                }
                g_list_free(children);
                return label;
        }
        static GtkWidget *extractButtonFromButton(GtkWidget *button)
        {
                GtkWidget *parent = gtk_widget_get_parent(button);
                while (parent != nullptr)
                {
                        if (GTK_IS_BUTTON(parent))
                        return parent;

                        parent = gtk_widget_get_parent(parent);
                }
                return nullptr;
        }

        static void on_entering_ffButton(GtkWidget *button, GdkEvent *event, gpointer data)
        {
                const gchar *new_label = static_cast<const gchar *>(data);
                // std::cout << "new_label: " << *new_label << "\n";
                GtkWidget *box = gtk_bin_get_child(GTK_BIN(button));
                if (GTK_IS_BOX(box))
                {
                        GList *children = gtk_container_get_children(GTK_CONTAINER(box));
                        if (children != nullptr)
                        {
                                GtkWidget *label_widget = GTK_WIDGET(g_list_nth_data(children, 1));
                                if (GTK_IS_LABEL(label_widget))
                                {
                                        gtk_label_set_text(GTK_LABEL(label_widget), new_label);
                                }
                                g_list_free(children);
                        }
                }
        }

        static void on_leaving_ffButton(GtkWidget *button, GdkEvent *event, gpointer data)
        {
                const gchar *old_label = static_cast<const gchar *>(data);
                GtkWidget *box = gtk_bin_get_child(GTK_BIN(button));
                if (GTK_IS_BOX(box))
                {
                        GList *children = gtk_container_get_children(GTK_CONTAINER(box));
                        if (children != nullptr)
                        {
                                GtkWidget *label_widget = GTK_WIDGET(g_list_nth_data(children, 1));

                                if (GTK_IS_LABEL(label_widget))
                                {
                                        gtk_label_set_text(GTK_LABEL(label_widget), old_label);
                                }
                                g_list_free(children);
                        }
                }
        }
        static void ffbutton_change_label(GtkWidget *button, GdkEvent *event, gpointer data)
        {
                const gchar *old_label = static_cast<const gchar *>(data);
                std::cout << "old_label: " << old_label << "\n";
        }
        static std::string runcomm(std::string cmd)
        {
                std::array<char, 128> buffer;
                std::string result;
                std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
                if (!pipe)
                {
                        throw std::runtime_error("popen() failed!");
                }
                while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
                {
                        result += buffer.data();
                }
                return result;
        }
        static int isFile(std::string currPath)
        {
                std::string com = runcomm("ls -ldF " + currPath);
                // std::cout << "isfilecomm = " << com[com.length()-2] << "\n";
                // std::cout << "gfd";
                if (com[com.length() - 2] == '/')
                        return -1;
                else
                        return 1;
        }
        static std::string stringChecker(std::string str)
        {
                for (int i = 0; i < str.size(); i++)
                {
                        if (str[i] == '\'')
                        {
                                str.insert(i++, "\'");
                                str.insert(i++, "\\");
                                str.insert(i++, "\'");
                        }
                }
                std::cout << "Updated String: " << str << std::endl;
                return str;
        }
        static void compressToTarGz(const std::string &inputFilePath, const std::string &tarGzFilePath)
        {
                // runcomm("tar -czf " + inputFilePath)
                std::cout << "DEBUG_inputFilePath: " << inputFilePath << "\nDEBUG_tarGzFilePath: " << tarGzFilePath << "\n";
                // std::cout << "DEBUG_inputFilePath: " << getPathFromFilePath(inputFilePath) << "\nDEBUG_tarGzFilePath: " << tarGzFilePath << "\n";
                std::cout << "\nRunning command: " << "tar -czf " + tarGzFilePath + " -C " + inputFilePath << "\n";
                std::cout << "getFileAndPath" << getPathFromFilePath(inputFilePath) << " path: " << getFileFromFilePath(inputFilePath) << "\n";
                runcomm("tar -czf " + tarGzFilePath + " -C " + getPathFromFilePath(inputFilePath) + " " + getFileFromFilePath(inputFilePath));
                // runcomm("tar -czf " + tarGzFilePath + " -C " + );
                // struct archive *archivePtr;
                // struct archive_entry *entry;
                // archivePtr = archive_write_new();
                // archive_write_set_format_pax_restricted(archivePtr);
                // if (archive_write_add_filter_gzip(archivePtr) != ARCHIVE_OK)
                // {
                //         std::cerr << "Error adding gzip filter: " << archive_error_string(archivePtr) << std::endl;
                //         archive_write_free(archivePtr);
                //         return;
                // }
                // if (archive_write_open_filename(archivePtr, tarGzFilePath.c_str()) != ARCHIVE_OK)
                // {
                //         std::cerr << "Error opening output file: " << archive_error_string(archivePtr) << std::endl;
                //         std::cout << "errorFilePath: " << archivePtr << "\n";
                //         archive_write_free(archivePtr);
                //         return;
                // }
                // entry = archive_entry_new();
                // archive_entry_set_pathname(entry, inputFilePath.c_str());
                // std::ifstream inputFile(inputFilePath, std::ios::binary | std::ios::ate);
                // if (!inputFile)
                // {
                //         std::cerr << "Error opening input file: " << inputFilePath << std::endl;
                //         archive_entry_free(entry);
                //         archive_write_free(archivePtr);
                //         return;
                // }
                // std::streamsize fileSize = inputFile.tellg();
                // archive_entry_set_size(entry, fileSize);
                // archive_entry_set_filetype(entry, AE_IFREG);
                // archive_entry_set_perm(entry, 0644);
                // if (archive_write_header(archivePtr, entry) != ARCHIVE_OK)
                // {
                //         std::cerr << "Error writing header: " << archive_error_string(archivePtr) << std::endl;
                //         archive_entry_free(entry);
                //         archive_write_free(archivePtr);
                //         return;
                // }
                // inputFile.seekg(0);
                // char buffer[8192];
                // while (inputFile)
                // {
                //         inputFile.read(buffer, sizeof(buffer));
                //         std::streamsize bytesRead = inputFile.gcount();
                //         if (bytesRead > 0)
                //         {
                //                 archive_write_data(archivePtr, buffer, bytesRead);
                //         }
                // }
                // inputFile.close();
                // archive_entry_free(entry);
                // archive_write_finish_entry(archivePtr);
                // archive_write_close(archivePtr);
                // archive_write_free(archivePtr);

                std::cout << "Compressed " << inputFilePath << " to " << tarGzFilePath << std::endl;
        }

        static std::string getPathFromFilePath(std::string filePath)
        {
                for (int i = filePath.size() - 1; i >= 0; i--)
                {
                        if (filePath[i] == '/')
                                return filePath.substr(0, i);
                }
        }
        static std::string getFileFromFilePath(std::string filePath)
        {
                for (int i = filePath.size() - 1; i >= 0; i--)
                {
                        if (filePath[i] == '/')
                                return filePath.substr(i + 1);
                }
        }
        static void exchangeVisibleAndHiddenLabel(GtkWidget *button)
        {
                GtkWidget *buttonBox = gtk_bin_get_child(GTK_BIN(button));
                if (GTK_IS_BOX(buttonBox))
                {
                        GList *children = gtk_container_get_children(GTK_CONTAINER(buttonBox));
                        if (children != nullptr)
                        {
                                GtkWidget *visibleLabel = GTK_WIDGET(g_list_nth_data(children, 1));
                                GtkWidget *hidden = GTK_WIDGET(g_list_nth_data(children, 2));
                                if (GTK_IS_LABEL(hidden) && GTK_IS_LABEL(visibleLabel))
                                {
                                        const gchar *currentVisibleLabel = g_strdup(gtk_label_get_text(GTK_LABEL(visibleLabel)));
                                        const gchar *currentHiddenLabel = g_strdup(gtk_label_get_text(GTK_LABEL(hidden)));
                                        gtk_label_set_text(GTK_LABEL(hidden), currentVisibleLabel);
                                        gtk_label_set_text(GTK_LABEL(visibleLabel), currentHiddenLabel);
                                }
                        }
                }
        }
};

/*
gst-plugins-good
gst-plugins-bad (for additional formats)
gst-plugins-ugly (for less open-source friendly formats)
gst-libav (optional, includes libav for various codecs)
command: sudo apt install gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-libav
*/