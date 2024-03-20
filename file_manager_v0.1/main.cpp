#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdlib.h>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include "headers/navPane.h"
#include "headers/ribbon.h"
#include "headers/pathRibbon.h"
#include "headers/filePreviewer.h"
#include "headers/imageViewer.h"

GtkWidget *win, *table1, *ribbon, *mypcDropDown, *testButton, *homeBtn, *docBtn, *musicBtn, *videoBtn, *pictureBtn, *temp, *ffpane, *scwin, *ffbutton;
GtkListBoxRow *navPaneR;
std::string fop, username = (std::string)g_get_user_name();
bool f = 1;
bool addtoPaneList = 0;
int indexOfLastFolder = 0;
guint x = 0, y = 0;

void ffpaneInsert(std::string ffList);
std::string openFolder(std::string targetPath);

std::string runcomm(std::string cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int isFile(std::string currPath)
{
        std::string com = runcomm("ls -ldF " + currPath);
        // std::cout << "isfilecomm = " << com[com.length()-2] << "\n";
        // std::cout << "gfd";
        if (com[com.length()-2] == '/')
                return -1;
        else
                return 1;
}

static void pasteFF()
{
        std::string targetFile = fileOriginalLocation + "/'" + fileToBeCopied + "'";
        std::cout << targetFile;
        std::cout << " -> " << currentPath << std::endl;
        if (copyMode)
        {
                if (fileOrFolder)
                        runcomm("cp -r " + targetFile + " " + currentPath);
                else
                        runcomm("cp " + targetFile + " " + currentPath);
        }
        std::cout << "cutmode: " << cutMode << "\n";
        if (cutMode)
        {
                runcomm("mv " + targetFile + " " + currentPath);
                std::cout << "cut comm run\n";
        }
        pasteMode = 0;
        copyMode = 0;
        cutMode = 0;
        f = 1;
        addtoPaneList = 1;
        ffpaneInsert(fileToBeCopied);
}

void backButtonfunc()
{
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(fileBuffer), "", 0);
        // currentPath= currentPath.substr(0, indexOfLastFolder);
        if (slashpos.size() == 1)
                return;
        currentPath.erase(slashpos[slashpos.size()-1], currentPath.size());
        slashpos.pop_back();
        ffpaneInsert(runcomm("ls " + currentPath));
}

void refreshButtonfunc()
{
        ffpaneInsert(runcomm("ls " + currentPath));
}

static int ffButtonFunc(GtkWidget *w)
{
        std::cout << "\nButton location: " << w;
        // const gchar *targetffName = gtk_button_get_label((GtkButton *)w);
        // std::string targetPath = currentPath + "/" + (std::string)targetffName;
        // std::cout << "\nisfile : " << isFile(targetPath) << "\n";
        // currentPath = targetPath;
        // ffpaneInsert(runcomm("ls " + targetPath));
        const gchar *targetffName = gtk_button_get_label((GtkButton *)w);
        std::string temp1 = (std::string)targetffName;
        for (int i = 0; i < temp1.size(); i++)
                if (temp1[i] == '\n')
                        temp1.erase(i, 1);
        // std::cout << "temp1: " << temp1[0] << std::endl;
        if (deleteMode)
        {
                fileToBeCopied = temp1;
                fileOriginalLocation = currentPath;
                if (isFile(fileOriginalLocation + "/'" + fileToBeCopied + "'") == 1)
                        runcomm("rm " + fileOriginalLocation + "/'" + fileToBeCopied + "'");
                if (isFile(fileOriginalLocation + "/'" + fileToBeCopied + "'") == -1)
                        runcomm("rm -r " + fileOriginalLocation + "/'" + fileToBeCopied + "'");
                std::cout << "file_deleted...\n";
                deleteMode = 0;
                refreshButtonfunc();
                return -1;
        }
        if (copyMode && !pasteMode)
        {
                fileToBeCopied = temp1;
                fileOriginalLocation = currentPath;
                if (isFile(fileOriginalLocation + "/'" + fileToBeCopied + "'") == 1)
                        fileOrFolder = 0;
                        // std::cout << "is_file: " << 1 << "\n";
                else if (isFile(fileOriginalLocation + "/'" + fileToBeCopied + "'") == -1)
                        fileOrFolder = 1;
                        // std::cout << "is_file: " << 0 << "\n";
                        
                std::cout << "file_to_be_copied" << fileOriginalLocation  + "/'" + fileToBeCopied + "'"<< std::endl;
                pasteMode = 1;
                return -1;
        }
        if (cutMode && !pasteMode)
        {
                fileToBeCopied = temp1;
                fileOriginalLocation = currentPath;
                std::cout << "file_to_be_cut" << fileToBeCopied << std::endl;
                pasteMode = 1;
                return -1;
        }
        std::string targetPath = currentPath + "/'" + temp1 + "'";
        indexOfLastFolder = (currentPath + "/").length()-1;
        slashpos.push_back(indexOfLastFolder);
        std::cout << "\n" << targetPath << indexOfLastFolder << "\n";
        std::cout << std::endl;
        // std::cout << isFile(targetPath) << "\n";
        std::cout << "target_path: " << targetPath << "\n";
        if (isFile(targetPath) == 1)
        {
                // std::cout << 1 << "\n";
                if (targetPath.find(".jpg") != std::string::npos || targetPath.find(".png") != std::string::npos || targetPath.find(".gif") != std::string::npos || targetPath.find(".jpeg") != std::string::npos)
                {
                        std::cout << "image opened in imageViewer: \n";
                        imageViewer::openImage(targetPath);
                }
                std::string fileData = runcomm("cat " + targetPath);
                // std::cout << fileData << "\n";
                gtk_text_buffer_set_text(GTK_TEXT_BUFFER(fileBuffer), fileData.c_str(), fileData.size());
        }
        else if (isFile(targetPath) == -1)
        {
                // currentPath = targetPath;
                // std::cout << -1 << "\n";
                // std::cout << targetPath << "\n";
                currentPath = targetPath;
                pathRibbonComponent::updateslashpos();
                ffpaneInsert(runcomm("ls " + currentPath));
        }
        else
                std::cout << "unknown error occured\n";
}

void ffpaneInsert(std::string ffList)
{
        if (f == 0)
        {
                x = 0, y = 0;
                gtk_widget_destroy(ffpane);
                scwin = gtk_scrolled_window_new(NULL, NULL);
                ffpane = gtk_table_new(8,8,0);
                gtk_container_add(GTK_CONTAINER(scwin), ffpane);
                gtk_table_attach(GTK_TABLE(table1), scwin, 2, 8, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
                gtk_widget_show(scwin);
                gtk_widget_show(ffpane);
                gtk_widget_set_size_request(scwin, 1488, 500);
                gtk_widget_set_name(scwin, "scwin");
        }
        else
                f = 0;
        std::stringstream s(ffList);
        std::string ffname;
        if (!addtoPaneList)
        {
                x = 0, y = 0;
                addtoPaneList = 0;
        }
        while (std::getline(s, ffname))
        {
                // std::cout << "ffname: " << ffname << "\n";
                for (int i = 0; i < ffname.size(); i++)
                        if (i % 10 == 0)
                                ffname.insert(i, "\n");
                ffbutton = gtk_button_new_with_label((const gchar *)ffname.c_str());
                gtk_widget_set_name(ffbutton, "ffbutton");
                gtk_table_attach(GTK_TABLE(ffpane), ffbutton, x, x+1, y, y+1, GTK_FILL, GTK_FILL, 0, 0);
                if ((x+1) == 5) {
                        x = -1;
                        y++;
                }
                x++;
                // std::cout << (const gchar *)ffname.c_str() << "\n";
                // gtk_label_set_line_wrap((GtkLabel*)gtk_button_get_label((GtkButton*)ffbutton), true);
                gtk_widget_set_name(ffbutton, "ffbutton");
                gtk_widget_show(ffbutton);
                gtk_widget_set_size_request(ffbutton, 100, 40);
                g_signal_connect(ffbutton, "clicked", G_CALLBACK(ffButtonFunc), ffbutton);
        }
        pathRibbonComponent::updateAddressBar(currentPath);
}


void createNewFolder(GtkWidget *w)
{
        std::string folderName = "";
        if ((std::string)gtk_button_get_label((GtkButton *)w) == "Create Folder")
        {
                folderName = ribbonComponent::createButtonFunc(1);
                if (folderName == "-1")
                        return;
                runcomm("mkdir " + currentPath + "/" + folderName);
        }
        if ((std::string)gtk_button_get_label((GtkButton *)w) == "Create File")
        {
                folderName = ribbonComponent::createButtonFunc(0);
                if (folderName == "-1")
                        return;
                runcomm("touch " + currentPath + "/" + folderName);
        }
        f = 1;
        addtoPaneList = 1;
        ffpaneInsert(folderName);
        refreshButtonfunc();
        std::cout << "button name: " << (std::string)gtk_button_get_label((GtkButton *)w) << "\n";
}

std::string openFolder(std::string targetPath)
{
        temp = (GtkWidget*)gtk_list_box_get_selected_row(GTK_LIST_BOX(navPane));
        gint id = gtk_list_box_row_get_index((GtkListBoxRow*)temp);
        // std::cout << id << "\n";
        slashpos.push_back(17);
        if (id == 0)
        {
                currentPath = "/'home'/'" + username + "'/";
                ffpaneInsert(runcomm("ls /home/" + username));
        }
        if (id == 1)
        {
                currentPath = "/'home'/'" + username + "'/'Documents'";
                ffpaneInsert(runcomm("ls /home/" + username  + "/Documents"));
        }
        if (id == 2)
        {
                currentPath = "/'home'/'" + username + "'/'Downloads'";
                ffpaneInsert(runcomm("ls /home/" + username + "/Downloads"));
        }
        if (id == 3)
        {
                currentPath = "/'home'/'" + username + "'/'Music'";
                ffpaneInsert(runcomm("ls /home/" + username + "/Music"));
        }
        if (id == 4)
        {
                currentPath = "/'home'/'" + username + "'/'Pictures'";
                ffpaneInsert(runcomm("ls /home/" + username + "/Pictures"));
        }
        if (id == 5)
        {
                currentPath = "/'home'/'" + username + "'/'Videos'";
                ffpaneInsert(runcomm("ls /home/" + username + "/Videos"));
        }
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(fileBuffer), "", 0);
}
// static int navPaneIns()

void addcss()
{
        // gtk_widget_set_name(testButton, "testButton");
        gtk_widget_set_name(navPane, "navpasteButtonPane");
        gtk_widget_set_name(ribbon, "ribbon");
        gtk_widget_set_name(scwin, "scwin");
        // gtk_widget_set_name(, "scwin");
        GtkCssProvider *prov1 = gtk_css_provider_new();
        gtk_css_provider_load_from_path(prov1, "fm.css", NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(prov1), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

int main(int argc, char *argv[])
{
        gtk_init(&argc, &argv);
        createNavPane();
        ribbonComponent::createRibbbon();
        pathRibbonComponent::createPathRibbon();
        filePreviewerComponent::createFileViewer();
        win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        table1 = gtk_table_new(8, 8, 0);
        ribbon = gtk_table_new(8, 8, 0);
        ffpane = gtk_table_new (8, 8, 0);
        pathRibbon = gtk_table_new(8, 8, 0);
        // temp = gtk_button_new_with_label("temp");
        // navPane = gtk_list_box_new();
        // mypcDropDown = gtk_list_box_new();
        testButton = gtk_button_new_with_label("Test Button");
        // homeBtn = gtk_button_new_with_label("Home");
        // docBtn = gtk_button_new_with_label("Documents");
        // musicBtn = gtk_button_new_with_label("Music");
        // pictureBtn = gtk_button_new_with_label("Pictures");
        // videoBtn = gtk_button_new_with_label("Video");
        scwin = gtk_scrolled_window_new(NULL, NULL);
        addcss();
        // navPaneIns();
        // mypcDropDownIns();
        gtk_table_attach(GTK_TABLE(table1), ribbon, 0, 8, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(table1), pathRibbon, 0, 8, 1, 2, GTK_FILL, GTK_FILL, 0, 0);
        // gtk_table_attach(GTK_TABLE(ffpane), temp, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

        gtk_table_attach(GTK_TABLE(ribbon), makeFolderButton, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(ribbon), makeFileButton, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(ribbon), copyButton, 2, 3, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(ribbon), cutButton, 3, 4, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(ribbon), pasteButton, 4, 5, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(ribbon), deleteButton, 5, 6, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(ribbon), refreshButton, 6, 7, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

        gtk_table_attach(GTK_TABLE(pathRibbon), backButton, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(pathRibbon), addressBar, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

        gtk_table_attach(GTK_TABLE(table1), navPane, 0, 1, 2, 8, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(table1), scwin2, 2, 3, 3, 4, GTK_FILL, GTK_FILL, 10, 4);
        g_signal_connect(win, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(navPane, "row-activated", G_CALLBACK(openFolder), NULL);
        g_signal_connect(backButton, "clicked", G_CALLBACK(backButtonfunc), NULL);
        g_signal_connect(makeFolderButton, "clicked", G_CALLBACK(createNewFolder), makeFolderButton);
        g_signal_connect(makeFileButton, "clicked", G_CALLBACK(createNewFolder), makeFileButton);
        g_signal_connect(copyButton, "clicked", G_CALLBACK(ribbonComponent::toggleCopyMode), NULL);
        g_signal_connect(cutButton, "clicked", G_CALLBACK(ribbonComponent::toggleCutMode), NULL);
        g_signal_connect(pasteButton, "clicked", G_CALLBACK(pasteFF), NULL);
        g_signal_connect(deleteButton, "clicked", G_CALLBACK(ribbonComponent::toggleDeleteMode), NULL);
        g_signal_connect(refreshButton, "clicked", G_CALLBACK(refreshButtonfunc), NULL);
        // gtk_container_add(GTK_CONTAINER(navPane), mypcDropDown);
        gtk_container_add(GTK_CONTAINER(win), table1);
        gtk_container_add(GTK_CONTAINER(scwin), ffpane);
        gtk_window_set_default_size(GTK_WINDOW(win), 1800, 800);
        gtk_window_set_resizable(GTK_WINDOW(win), FALSE);
        gtk_table_attach(GTK_TABLE(table1), scwin, 2, 8, 2, 3, GTK_FILL, GTK_FILL, 0, 0);
        gtk_widget_set_size_request(scwin, 1488, 500);
        gtk_widget_set_size_request(navPane, 300, 800);
        gtk_widget_set_size_request(makeFolderButton, 110, 70);
        gtk_widget_set_size_request(makeFileButton, 110, 70);
        gtk_widget_set_size_request(copyButton, 110, 70);
        gtk_widget_set_size_request(cutButton, 110, 70);
        gtk_widget_set_size_request(pasteButton, 110, 70);
        gtk_widget_set_size_request(deleteButton, 110, 70);
        gtk_widget_set_size_request(refreshButton, 110, 70);
        gtk_widget_set_size_request(addressBar, 1500, 30);
        gtk_widget_set_size_request(scwin2, 1488, 200);
        gtk_widget_show_all(win);
        gtk_window_set_title(GTK_WINDOW(win), "File Explorer");
        currentPath = "/'home'/'" + username + "'/";
        ffpaneInsert(runcomm("ls /home/" + username));
        gtk_main();
        // std::string op = runcomm("ls");
        // std::cout << op << "\n";
}

// g++ main.cpp -o main `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` && ./main