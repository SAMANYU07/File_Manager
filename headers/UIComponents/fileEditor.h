#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "../ggh.h"

GtkWidget *filePreviewer, *scwin2, *undoChangesButton, *saveChangesButton;
GtkTextBuffer *fileBuffer;
GtkTextIter start, end;
std::string currentFile;

class fileEditor
{
        public:

        static void saveFile()
        {
                gtk_text_buffer_get_bounds(fileBuffer, &start, &end);
                std::string newFileData = (std::string)gtk_text_buffer_get_text(GTK_TEXT_BUFFER(fileBuffer), &start, &end, NULL);
                std::fstream fp;
                std::string fname;
                for (int i = 0; i < currentFile.size(); i++)
                {
                        if (currentFile[i] == '\'')
                                continue;
                        // if (currentFile[i] == ' ')
                                // fname += "\\";
                        fname += currentFile[i];
                }
                try
                {
                        fp.open(fname, std::ios::out | std::ios::trunc);
                }
                catch(const std::exception& e)
                {
                        std::cerr << e.what() << '\n';
                }

                if (!fp)
                {
                        std::cout << "cant edit\n";
                        std::cout << "fname: " << fname << "\n";
                        return;
                        exit(0);
                }
                std::string line;
                std::stringstream ndata(newFileData);
                while (std::getline(ndata, line))
                {
                        fp << line;
                        fp << "\n";
                }
                fp.close();

        }

        static void loadFile(std::string fileName)
        {
                gtk_text_buffer_set_text(fileBuffer, "", 0);
                std::fstream fp;
                std::string fname;
                for (int i = 0; i < fileName.size(); i++)
                {
                        if (fileName[i] == '\'')
                                continue;
                        // if (fileName[i] == ' ')
                                // fname += "\\";
                        fname += fileName[i];
                }
                try
                {
                        fp.open(fname);
                }
                catch(const std::exception& e)
                {
                        std::cerr << e.what() << '\n';
                }

                if (!fp) {
                        std::cout << "cant open\n";
                        std::cout << "fname: " << fname << "\n";
                        return;
                        // exit(0);
                }
                std::string fileData, line;
                while (!fp.eof())
                {
                        std::getline(fp, line);
                        // fp >> line;
                        fileData += line + "\n";
                }
                // std::string fileData = runcomm("cat "+ fileName);
                gtk_text_buffer_set_text(GTK_TEXT_BUFFER(fileBuffer), fileData.c_str(), fileData.size());
                fp.close();
                g_signal_connect(undoChangesButton, "clicked", G_CALLBACK(reloadFile), NULL);
                gtk_widget_set_name(scwin2, "scwin2");
                gtk_widget_set_can_focus(filePreviewer, true);
        }

        static void reloadFile()
        {
                loadFile(currentFile);
        }

        static int createFileViewer()
        {
                filePreviewer = gtk_text_view_new();
                scwin2 = gtk_scrolled_window_new(NULL, NULL);
                fileBuffer = gtk_text_buffer_new(NULL);
                undoChangesButton = gtk_button_new();
                saveChangesButton = gtk_button_new();
                gtk_widget_set_name(undoChangesButton, "undoChangesButton");
                ggh::attach_label_icon_with_button(undoChangesButton, "Undo Changes", "edit-undo-symbolic");
                gtk_widget_set_name(saveChangesButton, "saveChangesButton");
                ggh::attach_label_icon_with_button(saveChangesButton, "Save", "document-save");
                gtk_text_view_set_buffer(GTK_TEXT_VIEW(filePreviewer), fileBuffer);
                // gtk_text_buffer_set_text(GTK_TEXT_BUFFER(fileBuffer), txt.c_str(), txt.length());
                gtk_container_add(GTK_CONTAINER(scwin2), filePreviewer);
                gtk_widget_set_can_focus(filePreviewer, false);
                GdkColor scwin2bgcolor;
                gdk_color_parse("#141414", &scwin2bgcolor);
                gtk_widget_modify_bg(filePreviewer, GTK_STATE_NORMAL, &scwin2bgcolor);
                gtk_widget_set_name(filePreviewer, "filePreviewer");
        }
};