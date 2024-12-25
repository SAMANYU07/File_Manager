#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include "ggh.h"

GtkWidget *makeFolderButton, *makeFileButton, *copyButton, *cutButton, *pasteButton, *deleteButton, *refreshButton, *renameButton, *oldLabel, *newLabel, *oldName;
GtkWidget *tempWin, *entry1, *tempTable, *ques, *createButton, *cancelButton, *compressButton, *fileName, *archivePath, *browseButton, *archivePathLabel, *compressBtn;
const gchar *inp, *compressOutputFileName;
bool copyMode = 0;
bool pasteMode = 0;
bool cutMode = 0;
bool deleteMode = 0;
bool fileOrFolder = 0;
bool makeFileorFolder = 0;
bool renameMode = 0;
bool confirmDelete = 0;
bool compressMode = 0;
std::string tempStr;
GtkEntryBuffer *entryBuffer = gtk_entry_buffer_new(NULL, NULL);
std::string fileToBeCopied, fileOriginalLocation;

class ribbonComponent
{
        public:
        static int createRibbbon()
        {
                makeFolderButton = gtk_button_new();
                makeFileButton = gtk_button_new();
                copyButton = gtk_button_new();
                cutButton = gtk_button_new();
                pasteButton = gtk_button_new();
                deleteButton = gtk_button_new();
                refreshButton = gtk_button_new();
                renameButton = gtk_button_new();
                compressButton = gtk_button_new();
                ggh::attach_label_icon_with_button(makeFolderButton, "Create Folder", "folder-symbolic"); //button se label nikalna hai
                ggh::attach_label_icon_with_button(makeFileButton, "Create File", "document-new");
                ggh::attach_label_icon_with_button(copyButton, "Copy", "edit-copy");
                ggh::attach_label_icon_with_button(cutButton, "Cut", "edit-cut");
                ggh::attach_label_icon_with_button(pasteButton, "Paste", "edit-paste");
                ggh::attach_label_icon_with_button(deleteButton, "Delete", "edit-delete");
                ggh::attach_label_icon_with_button(refreshButton, "Refresh", "view-refresh");
                ggh::attach_label_icon_with_button(renameButton, "Rename", "edit-rename");
                ggh::attach_label_icon_with_button(compressButton, "Compress", "application-x-tar");
        }

        static void dst()
        {
                // inp = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(entryBuffer));
                tempStr = gtk_entry_get_text(GTK_ENTRY(entry1));
                std::cout << "tempstr: " << tempStr << "\n";
                gtk_main_quit();
                gtk_widget_destroy(tempWin);
        }

        static void destroyWin()
        {
                deleteMode = 0;
                tempStr = "-1";
                confirmDelete = 0;
                gtk_main_quit();
                gtk_widget_destroy(tempWin);
        }

        static void toggleCopyMode()
        {
                cutMode == 1 ? cutMode = 0 : NULL;
                copyMode == 0 ? copyMode = 1 : copyMode = 0;
                std::cout << "copy_mode_toggled " << copyMode << "\n";
        }

        static void toggleCutMode()
        {
                copyMode == 1 ? copyMode = 0 : NULL;
                cutMode == 0 ? cutMode = 1 : cutMode = 0;
                std::cout << "cut_mode_toggled " << cutMode << "\n";
        }

        static void toggleDeleteMode()
        {
                deleteMode == 0 ? deleteMode = 1: deleteMode = 0;
                std::cout << "delete_mode_toggled " << deleteMode << "\n";
        }

        static void toggleRenameMode()
        {
                renameMode == 0 ? renameMode = 1: renameMode = 0;
        }

        static void toggleCompressMode()
        {
                compressMode == 0 ? compressMode = 1 : compressMode = 0;
        }

        static void createTempWin(int fof)
        {
                tempWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                entry1 = gtk_entry_new();
                if (fof)
                {
                        ques = gtk_label_new("Enter name to create folder:");
                        gtk_window_set_title(GTK_WINDOW(tempWin), "Create Folder");
                }
                if (!fof)
                {
                        ques = gtk_label_new("Enter name to create file:");
                        gtk_window_set_title(GTK_WINDOW(tempWin), "Create File");
                }
                tempTable = gtk_table_new(5, 5, 0);
                createButton = gtk_button_new_with_label("Create");
                cancelButton = gtk_button_new_with_label("Cancel");
                // gtk_entry_buffer_set_text(entryBuffer, "1234", 2);
                gtk_table_attach(GTK_TABLE(tempTable), ques, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), entry1, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), createButton, 0, 1, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), cancelButton, 0, 1, 4, 5, GTK_FILL, GTK_FILL, 0, 0);
                g_signal_connect(createButton, "clicked", G_CALLBACK(dst), NULL);
                g_signal_connect(cancelButton, "clicked", G_CALLBACK(destroyWin), NULL);
                gtk_container_add(GTK_CONTAINER(tempWin), tempTable);
                gtk_window_set_default_size(GTK_WINDOW(tempWin), 400, 300);
                gtk_widget_set_name(ques, "ques");
                gtk_widget_set_name(entry1, "entry1");
                gtk_widget_set_name(createButton, "createButton");
                gtk_widget_set_name(cancelButton, "cancelButton");
                gtk_widget_set_name(tempWin, "tempWin");
                gtk_widget_show_all(tempWin);
                gtk_main();
        }

        static std::string createButtonFunc(int fof)
        {
                if (fof)
                        makeFileorFolder = 1;
                if (!fof)
                        makeFileorFolder = 0;
                createTempWin(fof);
                return tempStr;
        }

        static std::string renameFunc(GtkWidget *w)
        {
                renameButtonFunc(w);
                return tempStr;
        }

        static void renameButtonFunc(GtkWidget *w)
        {
                tempWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                tempTable = gtk_table_new(8, 8, 0);
                entry1 = gtk_entry_new();
                createButton = gtk_button_new_with_label("Rename");
                cancelButton = gtk_button_new_with_label("Cancel");
                oldLabel = gtk_label_new("Old Name: ");
                newLabel = gtk_label_new("New Name: ");
                // oldName = gtk_label_new(gtk_button_get_label((GtkButton *)w));
                oldName = gtk_label_new((const gchar *)ggh::extractLabelFromButton(w).c_str());
                std::cout << "\noldname: " << oldName << "\n";
                gtk_table_attach(GTK_TABLE(tempTable), oldLabel, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), oldName, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), newLabel, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), entry1, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), createButton, 0, 2, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), cancelButton, 0, 2, 3, 4, GTK_FILL, GTK_FILL, 0, 0);
                g_signal_connect(createButton, "clicked", G_CALLBACK(dst), NULL);
                g_signal_connect(cancelButton, "clicked", G_CALLBACK(destroyWin), NULL);
                gtk_container_add(GTK_CONTAINER(tempWin), tempTable);
                gtk_window_set_default_size(GTK_WINDOW(tempWin), 400, 300);
                gtk_window_set_title(GTK_WINDOW(tempWin), "Rename File");
                gtk_widget_show_all(tempWin);
                gtk_widget_set_name(entry1, "entry1");
                gtk_widget_set_name(createButton, "createButton");
                gtk_widget_set_name(cancelButton, "cancelButton");
                gtk_widget_set_name(tempWin, "tempWin");
                gtk_main();
        }
        static void iDeleteFunc()
        {
                confirmDelete = 1;
                deleteMode = 0;
                gtk_main_quit();
                gtk_widget_destroy(tempWin);
        }

        static void destroyCompressWin()
        {
                compressMode = 0;
                gtk_main_quit();
                gtk_widget_destroy(tempWin);
        }

        static void tempDeleteWin(std::string fileName)
        {
                tempWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                tempTable = gtk_table_new(8, 8, 0);
                const gchar *fn = fileName.c_str();
                const gchar *q = "will be deleted permanently!";
                const gchar *f = g_strdup_printf("%s %s", fn, q);
                ques = gtk_label_new(f);
                createButton = gtk_button_new_with_label("Delete");
                cancelButton = gtk_button_new_with_label("Cancel");
                gtk_table_attach(GTK_TABLE(tempTable), ques, 0, 8, 0, 1, GTK_FILL, GTK_FILL, 60, 40);
                gtk_table_attach(GTK_TABLE(tempTable), createButton, 0, 8, 1, 2, GTK_FILL, GTK_FILL, 100, 10);
                gtk_table_attach(GTK_TABLE(tempTable), cancelButton, 0, 8, 2, 3, GTK_FILL, GTK_FILL, 100, 0);
                g_signal_connect(createButton, "clicked", G_CALLBACK(iDeleteFunc), NULL);
                g_signal_connect(cancelButton, "clicked", G_CALLBACK(destroyWin), NULL);
                g_signal_connect(tempWin, "delete-event", G_CALLBACK(destroyWin), NULL);
                gtk_container_add(GTK_CONTAINER(tempWin), tempTable);
                gtk_window_set_default_size(GTK_WINDOW(tempWin), 200, 200);
                gtk_widget_show_all(tempWin);
                gtk_window_set_title(GTK_WINDOW(tempWin), "Delete Item?");
                gtk_widget_set_name(createButton, "confDelButton");
                gtk_widget_set_name(cancelButton, "delCancelButton");
                gtk_main();
        }
        static int confirmDeletionFunc(std::string fileName)
        {
                tempDeleteWin(fileName);
                return confirmDelete;
        }
        static void convertToTar(GtkWidget *w, gpointer data)
        {
                compressOutputFileName = gtk_entry_get_text(GTK_ENTRY(entry1));
                // std::pair<std::string, std::string> *fileDetails = static_cast<std::pair<std::string, std::string>*>(data);
                // std::string fileToCompress = fileDetails->first;
                // std::cout << "File To Compress: " << fileToCompress << "\n" << "currentPath: " << ggh::getPathFromFilePath(fileToCompress) << "\n";
                std::cout << "fileName: " << gtk_entry_get_text(GTK_ENTRY(entry1)) << " path: " << gtk_entry_get_text(GTK_ENTRY(archivePath)) << "\n";
                std::string fileName = gtk_entry_get_text(GTK_ENTRY(entry1));
                std::string targetFilePath = gtk_entry_get_text(GTK_ENTRY(archivePath));
                std::string *inputFileName = static_cast<std::string *>(data);
                std::cout << "fileName: " << fileName << " path: " << targetFilePath << " inputfilename: " << *(static_cast<std::string *>(data)) << "getfilefromfilepath: " << ggh::getPathFromFilePath(*(static_cast<std::string *>(data))) << "\n";
                // ggh::compressToTarGz(*fileToCompress, ggh::getPathFromFilePath(*fileToCompress) + );
                ggh::compressToTarGz(*(static_cast<std::string *>(data)), ggh::getPathFromFilePath(*(static_cast<std::string *>(data))) + "/" + fileName + ".tar.gz");
                gtk_main_quit();
                gtk_widget_destroy(tempWin);
        }
        static void compressFile(std::string inputFile)
        {
                std::cout << "Compress Input file: " << inputFile << "\n";
                tempWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                entry1 = gtk_entry_new();
                fileName = gtk_label_new("Filename: ");
                archivePathLabel = gtk_label_new("Archive Path: ");
                archivePath = gtk_entry_new();
                browseButton = gtk_button_new_with_label("Browse");
                compressBtn = gtk_button_new_with_label("Compress");
                cancelButton = gtk_button_new_with_label("Cancel");
                tempTable = gtk_table_new(4, 4, 0);
                gtk_table_attach(GTK_TABLE(tempTable), fileName, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), entry1, 1, 3, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
                gtk_table_attach(GTK_TABLE(tempTable), archivePathLabel, 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 10);
                gtk_table_attach(GTK_TABLE(tempTable), archivePath, 1, 3, 1, 2, GTK_FILL, GTK_FILL, 0, 10);
                gtk_table_attach(GTK_TABLE(tempTable), browseButton, 3, 4, 1, 2, GTK_FILL, GTK_FILL, 0, 10);
                gtk_table_attach(GTK_TABLE(tempTable), compressBtn, 0, 2, 3, 4, GTK_FILL, GTK_FILL, 10, 10);
                gtk_table_attach(GTK_TABLE(tempTable), cancelButton, 2, 4, 3, 4, GTK_FILL, GTK_FILL, 10, 10);
                g_signal_connect(tempWin, "delete-event", G_CALLBACK(destroyCompressWin), NULL);
                g_signal_connect(compressBtn, "clicked", G_CALLBACK(convertToTar), &inputFile);
                g_signal_connect(cancelButton, "clicked", G_CALLBACK(destroyCompressWin), NULL);
                gtk_container_add(GTK_CONTAINER(tempWin), tempTable);
                // gtk_window_set_default_size(GTK_WINDOW(tempWin), 400, 300);
                gtk_widget_show_all(tempWin);
                gtk_window_set_title(GTK_WINDOW(tempWin), "Compress To Archive");
                gtk_widget_set_name(tempWin, "tempWin");
                gtk_widget_set_name(compressBtn,"compressBtn");
                gtk_widget_set_name(cancelButton,"cancelButton2");
                gtk_widget_set_name(browseButton,"browseButton");
                // gtk_widget_set_name(compressBtn,"compressBtn");
                gtk_main();
        }
};