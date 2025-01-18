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
#include <libgen.h>
#include <thread>
#include <X11/Xlib.h>
#include "headers/navPane.h"
#include "headers/ribbon.h"
#include "headers/pathRibbon.h"
#include "headers/fileEditor.h"
#include "headers/imageViewer.h"
#include "headers/videoPlayer.h"
#include "headers/rpopup.h"

GtkWidget *win, *table1, *ribbon, *mypcDropDown, *testButton, *homeBtn, *docBtn, *musicBtn, *videoBtn, *pictureBtn, *temp, *ffpane, *scwin, *ffbutton;
GtkListBoxRow *navPaneR;
std::string fop, username = (std::string)g_get_user_name();
std::string targetPath;
bool f = 1;
bool addtoPaneList = 0;
int indexOfLastFolder = 0;
guint x = 0, y = 0, hold_timer_id = 0;
bool ffClicked = false;
bool ffHold = false;

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
        gtk_widget_set_can_focus(filePreviewer, false);
}

void refreshButtonfunc()
{
        ffpaneInsert(runcomm("ls " + currentPath));
}

int ffButtonPreProcessing(GtkWidget *w)
{
                std::cout << "\nButton location: " << w;
        // const gchar *targetffName = gtk_button_get_label((GtkButton *)w);
        // std::string targetPath = currentPath + "/" + (std::string)targetffName;
        // std::cout << "\nggh::isFile : " << ggh::isFile(targetPath) << "\n";
        // currentPath = targetPath;
        // ffpaneInsert(runcomm("ls " + targetPath));
        ggh::exchangeVisibleAndHiddenLabel(w);
        const gchar *targetffName = ggh::extractLabelFromButton(w).c_str();
        GtkWidget *ffbuttonbox = gtk_bin_get_child(GTK_BIN(w));
        GList *children = gtk_container_get_children(GTK_CONTAINER(ffbuttonbox));
        GtkWidget *labelWidget = nullptr;
        if (g_list_length(children) > 1)
        {
                labelWidget = GTK_WIDGET(g_list_nth_data(children, 1));
        }
        const gchar *labelText = gtk_label_get_text(GTK_LABEL(labelWidget));
        // ggh::exchangeVisibleAndHiddenLabel(w);
        // std::string label = labelText ? labelText : "";
        std::string label;
        if (labelText)
        {
                for (const gchar *c = labelText; *c; ++c)
                        if (*c != '\n')
                                label += *c;
        }
        // const gchar *targetffName = gtk_button_get_label((GtkButton *)w);
        // std::string temp1 = targetffName;
        std::string temp1 = ggh::stringChecker(label);
        // for (int i = 0; i < temp1.size(); i++)
                // if (temp1[i] == '\n')
                        // temp1.erase(i, 1);
        std::cout << "modified target label: " << temp1 << std::endl;
        // std::cout << "temp1: " << temp1[0] << std::endl;
        if (compressMode)
        {
                ribbonComponent::compressFile(currentPath + "/'" + (ggh::extractLabelFromButton(w)).substr(0, (ggh::extractLabelFromButton(w)).size()) + "'");
                compressMode = 0;
                return -1;
        }
        else if (renameMode)
        {
                std::string newFname = "/'" + ribbonComponent::renameFunc(w) + "'";
                std::cout << "newFname: " << newFname << "\n";
                if (newFname.substr(2, newFname.size()-3) == "-1")
                {
                        renameMode = 0;
                        return -1;
                }
                std::string currentName = currentPath + "/'" + (ggh::extractLabelFromButton(w)).substr(0, (ggh::extractLabelFromButton(w)).size()) + "'";
                std::cout <<  "mv " + currentName + " " + currentPath  + newFname << "\n";
                runcomm( "mv " + currentName + " " + currentPath  + newFname);
                // gtk_button_set_label(GTK_BUTTON(w), (const gchar *)newFname.substr(2, newFname.size()-3).c_str());
                refreshButtonfunc();
                renameMode = 0;
                return -1;
        }
        else if (deleteMode)
        {
                fileToBeCopied = temp1;
                fileOriginalLocation = currentPath;
                int r = ribbonComponent::confirmDeletionFunc(fileToBeCopied);
                if (!r)
                        return -1;
                if (ggh::isFile(fileOriginalLocation + "/'" + fileToBeCopied + "'") == 1)
                        runcomm("rm " + fileOriginalLocation + "/'" + fileToBeCopied + "'");
                if (ggh::isFile(fileOriginalLocation + "/'" + fileToBeCopied + "'") == -1)
                        runcomm("rm -r " + fileOriginalLocation + "/'" + fileToBeCopied + "'");
                std::cout << "file_deleted...\n";
                gtk_widget_destroy(w);
                deleteMode = 0;
                refreshButtonfunc();
                return -1;
        }
        else if (copyMode && !pasteMode)
        {
                fileToBeCopied = temp1;
                fileOriginalLocation = currentPath;
                if (ggh::isFile(fileOriginalLocation + "/'" + fileToBeCopied + "'") == 1)
                        fileOrFolder = 0;
                        // std::cout << "is_file: " << 1 << "\n";
                else if (ggh::isFile(fileOriginalLocation + "/'" + fileToBeCopied + "'") == -1)
                        fileOrFolder = 1;
                        // std::cout << "is_file: " << 0 << "\n";

                std::cout << "file_to_be_copied" << fileOriginalLocation  + "/'" + fileToBeCopied + "'"<< std::endl;
                pasteMode = 1;
                return -1;
        }
        else if (cutMode && !pasteMode)
        {
                fileToBeCopied = temp1;
                fileOriginalLocation = currentPath;
                std::cout << "file_to_be_cut" << fileToBeCopied << std::endl;
                pasteMode = 1;
                return -1;
        }
        else
        {
                targetPath = currentPath + "/'" + temp1 + "'";
                indexOfLastFolder = (currentPath + "/").length()-1;
                slashpos.push_back(indexOfLastFolder);
                std::cout << "\n" << targetPath << indexOfLastFolder << "\n";
                std::cout << std::endl;
                // std::cout << ggh::isFile(targetPath) << "\n";
                std::cout << "target_path: " << targetPath << "\n";
        }
}

int ffButtonHoldFunc(GtkWidget *w)
{
        if (ffClicked)
                return 0;
        if (ffButtonPreProcessing(w) == -1)
                return -1;
        if (ggh::isFile(targetPath) == 1)
        {
                currentFile = targetPath;
                // std::string fRes = runcomm("xdg-open " + currentFile + "");
                std::thread holdOpenFileThread(runcomm, "xdg-open " + currentFile + "");
                holdOpenFileThread.detach();
                std::cout << "targetPath (custom): " << targetPath << "\n";
        }
        else if (ggh::isFile(targetPath) == -1)
        {
                // currentPath = targetPath;
                // std::cout << -1 << "\n";
                // std::cout << targetPath << "\n";
                gtk_widget_set_can_focus(filePreviewer, false);
                gtk_text_buffer_set_text(fileBuffer, "", 0);
                currentPath = targetPath;
                pathRibbonComponent::updateslashpos();
                ffpaneInsert(runcomm("ls " + currentPath));
        }
        else
                std::cout << "unknown error occured\n";
        // ffClicked = false;
        ffHold = true;
        return G_SOURCE_REMOVE;
}

int ffButtonFunc(GtkWidget *w, GdkEventButton *event)
{
        // ggh::exchangeVisibleAndHiddenLabel(w);
        if (!ffClicked)
                return 0;
        if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_SECONDARY)
        {
                std::cout << "ffButton right clicked.\n";
                return 0;
        }
        if (ffButtonPreProcessing(w) == -1)
                return -1;
        if (ggh::isFile(targetPath) == 1)
        {
                // std::cout << 1 << "\n";
                if (targetPath.find(".mp4") != std::string::npos)
                {
                        // videoPlayer::openVideo(targetPath, NULL, NULL);
                        std::thread openVideoThread(videoPlayer::playVideo, targetPath);
                        openVideoThread.detach();
                        // videoPlayer::playVideo(targetPath, NULL, NULL);
                }
                else if (targetPath.find(".jpg") != std::string::npos || targetPath.find(".png") != std::string::npos || targetPath.find(".gif") != std::string::npos || targetPath.find(".jpeg") != std::string::npos || targetPath.find(".bmp") != std::string::npos)
                {
                        std::cout << "image opened in imageViewer: \n";
                        std::thread openImageThread(imageViewer::openImage, targetPath);
                        openImageThread.detach();
                        // imageViewer::openImage(targetPath);
                }
                else if (targetPath.find(".txt") != std::string::npos || targetPath.find(".cpp") != std::string::npos || targetPath.find(".c") != std::string::npos || targetPath.find(".py") != std::string::npos || targetPath.find(".svg") != std::string::npos || targetPath.find(".css") != std::string::npos  || targetPath.find(".html") != std::string::npos || targetPath.find(".java") != std::string::npos || targetPath.find(".js") != std::string::npos || targetPath.find(".jsx") != std::string::npos || targetPath.find(".conf") != std::string::npos || targetPath.find(".ipynb") != std::string::npos || targetPath.find(".toml") != std::string::npos || targetPath.find(".conf") != std::string::npos)
                {

                        fileEditor::loadFile(targetPath);
                        currentFile = targetPath;
                        std::cout << "targetPath: " << targetPath << "\n";
                }
                else
                {
                        currentFile = targetPath;
                        std::thread clickOpenFileThread(runcomm, "xdg-open " + currentFile + "");
                        clickOpenFileThread.detach();
                        std::cout << "targetPath (custom): " << targetPath << "\n";
                }
                // std::string fileData = runcomm("cat " + targetPath);
                // std::cout << fileData << "\n";
                // gtk_text_buffer_set_text(GTK_TEXT_BUFFER(fileBuffer), fileData.c_str(), fileData.size());
        }
        else if (ggh::isFile(targetPath) == -1)
        {
                // currentPath = targetPath;
                // std::cout << -1 << "\n";
                // std::cout << targetPath << "\n";
                gtk_widget_set_can_focus(filePreviewer, false);
                gtk_text_buffer_set_text(fileBuffer, "", 0);
                currentPath = targetPath;
                pathRibbonComponent::updateslashpos();
                ffpaneInsert(runcomm("ls " + currentPath));
        }
        else
                std::cout << "unknown error occured\n";
        ggh::exchangeVisibleAndHiddenLabel(w);
        return G_SOURCE_REMOVE;
}

gboolean ffButtonPressed(GtkWidget *w)
{
        std::cout << "ffButtonPressed exec...\n";
        hold_timer_id = g_timeout_add(280, (GSourceFunc)ffButtonHoldFunc, w);
        g_signal_connect(win, "focus-out-event", G_CALLBACK(g_source_remove), &hold_timer_id);
        return false;
}

void ffClickedorPressedF(GtkWidget *w, GdkEventButton *event)
{
        std::cout << "ffClickedorPressedF exec...\n";
        if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_SECONDARY)
                return;
        if (ffClicked && !ffHold)
        {
                std::cout << "ffButtonFunc called...\n";
                ffButtonFunc(w, event);
        }
        else
        {
                std::cout << "ffButtonPressed called...\n";
                ffButtonPressed(w);
        }
        if (ffHold)
                ffHold = false;
        // ffClicked = false;
}

void ffClickedF(GdkEventButton *event)
{
        std::cout << "ffClickedF exec...\n";
        if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_SECONDARY)
                return;
        ffClicked = true;
}

gboolean ffPressedF(GtkWidget *w, GdkEventButton *event)
{
        std::cout << "ffPressedF exec...\n";
        if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_SECONDARY)
                return false;
        if (ffClicked)
                ffClicked = false;
        if (!ffClicked)
        {
                std::cout << "ffButtonPressed called...\n";
                ffButtonPressed(w);
        }
        return false;
}

void ffpaneInsert(std::string ffList)
{
        if (f == 0)
        {
                x = 0, y = 0;
                gtk_widget_destroy(ffbutton);
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
                std::string truncatedLabel = ffname.substr(0, 10);
                if (ffname.size() > 10)
                        truncatedLabel += "...";
                // for (int i = 0; i < ffname.size(); i++)
                //         if (i % 10 == 0)
                //                 ffname.insert(i, "\n");
                // ffbutton = gtk_button_new_with_label((const gchar *)ffname.c_str());
                GtkWidget *ffbuttonimage;
                if (ffname.find('.') != std::string::npos)
                        ffbuttonimage = gtk_image_new_from_icon_name("document", GTK_ICON_SIZE_BUTTON);
                else
                        ffbuttonimage = gtk_image_new_from_icon_name("folder", GTK_ICON_SIZE_BUTTON);
                ffbutton = gtk_button_new();
                GtkWidget *ffbuttonlabel = gtk_label_new(truncatedLabel.c_str());
                GtkWidget *ffbuttonbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
                GtkWidget *hiddenLabel = gtk_label_new(ffname.c_str());
                gtk_container_add(GTK_CONTAINER(ffbutton), ffbuttonbox);
                gtk_box_pack_start(GTK_BOX(ffbuttonbox), ffbuttonimage, FALSE, FALSE, 0);
                gtk_box_pack_start(GTK_BOX(ffbuttonbox), ffbuttonlabel, FALSE, FALSE, 0);
                gtk_box_pack_start(GTK_BOX(ffbuttonbox), hiddenLabel, FALSE, FALSE, 0);
                gtk_widget_show_all(ffbutton);
                gtk_widget_hide(hiddenLabel);
                gtk_widget_set_name(ffbutton, "ffbutton");
                gtk_table_attach(GTK_TABLE(ffpane), ffbutton, x, x+1, y, y+1, GTK_FILL, GTK_FILL, 0, 0);
                if ((x+1) == 10) {
                        x = -1;
                        y++;
                }
                x++;
                // std::cout << (const gchar *)ffname.c_str() << "\n";
                // gtk_label_set_line_wrap((GtkLabel*)gtk_button_get_label((GtkButton*)ffbutton), true);
                gtk_widget_show(ffbutton);
                gtk_widget_set_tooltip_text(ffbutton, ffname.c_str());
                gtk_widget_set_size_request(ffbutton, 100, 80);
                // g_signal_connect(ffbutton, "enter-notify-event", G_CALLBACK(ggh::on_entering_ffButton), g_strdup(ffname.c_str()));
                // g_signal_connect(ffbutton, "leave-notify-event", G_CALLBACK(ggh::on_leaving_ffButton), g_strdup(truncatedLabel.c_str()));
                gtk_widget_set_name(ffbutton, "ffbutton");
                // g_signal_connect(ffbutton, "button-press-event", G_CALLBACK(Popup::getInstance), ffbutton);
                g_signal_connect(ffbutton, "clicked", G_CALLBACK(ffClickedF), ffbutton);
                g_signal_connect(ffbutton, "pressed", G_CALLBACK(ffPressedF), ffbutton);
                g_signal_connect(ffbutton, "clicked", G_CALLBACK(ffClickedorPressedF), ffbutton);
                g_signal_connect(ffbutton, "pressed", G_CALLBACK(ffClickedorPressedF), ffbutton);
        }
        // std::string tempData = "tempData";
        pathRibbonComponent::updateAddressBar(currentPath);
}


void createNewFolder(GtkWidget *w)
{
        std::string folderName = "";
        std::string button_label;
        if (stringToBeSent.size() == 0)
                button_label = ggh::get_button_label(w);
        else
                button_label = stringToBeSent;
        std::cout << "button_label: " << button_label << std::endl;
        // return;
        // if ((std::string)gtk_button_get_label((GtkButton *)w) == "Create Folder")
        if (button_label == "Create Folder")
        {
                folderName = ribbonComponent::createButtonFunc(1);
                if (folderName == "-1")
                        return;
                runcomm("mkdir " + currentPath + "/" + folderName);
        }
        if (button_label == "Create File")
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
        // std::cout << "button name: " << (std::string)gtk_button_get_label((GtkButton *)w) << "\n";
        std::cout << "button name: " << button_label << "\n";
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
        if (id >= 6)
        {
                for (auto it = diskMap.begin(); it != diskMap.end(); ++it)
                {
                        if (id == it->first)
                        {
                                std::cout << "first: " << it->first  << "secondL " << it->second << "\n";
                                currentPath = "/'media'/'" + username + "'/" + it->second;
                                ffpaneInsert(runcomm("ls /media/" + username + "/"+ it->second));
                        }
                }
                // currentPath = "/'media'/'" + username + "'/" + diskNameVector[diskNameVector.size()-1];
                // ffpaneInsert(runcomm("ls /media/" + username + "/"+diskNameVector[diskNameVector.size()-1]));
        }
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(fileBuffer), "", 0);
}
// static int navPaneIns()

void searchQueryFunc()
{
        std::string searchResults = pathRibbonComponent::searchFunc();
        ffpaneInsert(searchResults);
}

void addcss()
{
        // gtk_widget_set_name(testButton, "testButton");
        gtk_widget_set_name(navPane, "navPane");
        gtk_widget_set_name(ribbon, "ribbon");
        gtk_widget_set_name(scwin, "scwin");
        gtk_widget_set_name(win, "win");
        gtk_widget_set_name(scwin2, "scwin2");
        gtk_widget_set_name(ribbon, "ribbon");

        gtk_widget_set_name(makeFolderButton, "makeFolderButton");
        gtk_widget_set_name(makeFileButton, "makeFileButton");
        gtk_widget_set_name(copyButton, "copyButton");
        gtk_widget_set_name(cutButton, "cutButton");
        gtk_widget_set_name(pasteButton, "pasteButton");
        gtk_widget_set_name(deleteButton, "deleteButton");
        gtk_widget_set_name(refreshButton, "refreshButton");
        gtk_widget_set_name(renameButton, "renameButton");
        gtk_widget_set_name(compressButton, "compressButton");

        // gtk_widget_set_name(, "scwin");
        char executable_path[1024];
        ssize_t count = readlink("/proc/self/exe", executable_path, sizeof(executable_path) - 1);
        if (count != -1)
        {
                executable_path[count] = '\0';
                std::string executable_dir = dirname(executable_path);
                chdir(executable_dir.c_str());
                GtkCssProvider *prov1 = gtk_css_provider_new();
                gtk_css_provider_load_from_path(prov1, "fm.css", NULL);
                gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(prov1), GTK_STYLE_PROVIDER_PRIORITY_USER);
        }
        else
                g_warning("Failed to get executable directory.");
}

int main(int argc, char *argv[])
{
        XInitThreads();
        gtk_init(&argc, &argv);
        createNavPane();
        ribbonComponent::createRibbbon();
        pathRibbonComponent::createPathRibbon();
        fileEditor::createFileViewer();
        win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        table1 = gtk_table_new(8, 8, 0);
        ribbon = gtk_table_new(8, 9, 0);
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
        gtk_table_attach(GTK_TABLE(table1), ribbon, 0, 8, 0, 1, GTK_FILL, GTK_FILL, 10, 8);
        gtk_table_attach(GTK_TABLE(table1), pathRibbon, 0, 8, 1, 2, GTK_FILL, GTK_FILL, 0, 2);
        // gtk_table_attach(GTK_TABLE(ffpane), temp, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

        gtk_table_attach(GTK_TABLE(ribbon), makeFolderButton, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 10, 0);
        gtk_table_attach(GTK_TABLE(ribbon), makeFileButton, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 10, 0);
        gtk_table_attach(GTK_TABLE(ribbon), copyButton, 2, 3, 0, 1, GTK_FILL, GTK_FILL, 10, 0);
        gtk_table_attach(GTK_TABLE(ribbon), cutButton, 3, 4, 0, 1, GTK_FILL, GTK_FILL, 10, 0);
        gtk_table_attach(GTK_TABLE(ribbon), pasteButton, 4, 5, 0, 1, GTK_FILL, GTK_FILL, 10, 0);
        gtk_table_attach(GTK_TABLE(ribbon), deleteButton, 5, 6, 0, 1, GTK_FILL, GTK_FILL, 10, 0);
        gtk_table_attach(GTK_TABLE(ribbon), refreshButton, 6, 7, 0, 1, GTK_FILL, GTK_FILL, 10, 0);
        gtk_table_attach(GTK_TABLE(ribbon), renameButton, 7, 8, 0, 1, GTK_FILL, GTK_FILL, 10, 0);
        gtk_table_attach(GTK_TABLE(ribbon), compressButton, 8, 9, 0, 1, GTK_FILL, GTK_FILL, 10, 0);

        gtk_table_attach(GTK_TABLE(pathRibbon), backButton, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(pathRibbon), addressBar, 1, 6, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(pathRibbon), searchEntry, 6, 7, 0, 1, GTK_FILL, GTK_FILL, 10, 0);
        gtk_table_attach(GTK_TABLE(pathRibbon), searchButton, 7, 8, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

        gtk_table_attach(GTK_TABLE(table1), navPane, 0, 1, 2, 8, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(table1), scwin2, 2, 8, 3, 4, GTK_FILL, GTK_FILL, 10, 4);
        gtk_table_attach(GTK_TABLE(table1), undoChangesButton, 3, 4, 4, 5, GTK_FILL, GTK_FILL, 0, 0);
        gtk_table_attach(GTK_TABLE(table1), saveChangesButton, 6, 7, 4, 5, GTK_FILL, GTK_FILL, 0, 0);
        g_signal_connect(win, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
        // g_signal_connect(win, "clicked", G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(navPane, "row-activated", G_CALLBACK(openFolder), NULL);
        g_signal_connect(backButton, "clicked", G_CALLBACK(backButtonfunc), NULL);
        g_signal_connect(makeFolderButton, "clicked", G_CALLBACK(createNewFolder), makeFolderButton);
        g_signal_connect(makeFileButton, "clicked", G_CALLBACK(createNewFolder), makeFileButton);
        g_signal_connect(copyButton, "clicked", G_CALLBACK(ribbonComponent::toggleCopyMode), NULL);
        g_signal_connect(cutButton, "clicked", G_CALLBACK(ribbonComponent::toggleCutMode), NULL);
        g_signal_connect(pasteButton, "clicked", G_CALLBACK(pasteFF), NULL);
        g_signal_connect(deleteButton, "clicked", G_CALLBACK(ribbonComponent::toggleDeleteMode), NULL);
        g_signal_connect(refreshButton, "clicked", G_CALLBACK(refreshButtonfunc), NULL);
        g_signal_connect(renameButton, "clicked", G_CALLBACK(ribbonComponent::toggleRenameMode), NULL);
        g_signal_connect(compressButton, "clicked", G_CALLBACK(ribbonComponent::toggleCompressMode), NULL);
        g_signal_connect(saveChangesButton, "clicked", G_CALLBACK(fileEditor::saveFile), NULL);
        g_signal_connect(searchButton, "clicked", G_CALLBACK(searchQueryFunc), NULL);
        g_signal_connect(win, "button-press-event", G_CALLBACK(Popup::getInstance), NULL);
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
        gtk_widget_set_size_request(renameButton, 110, 70);
        gtk_widget_set_size_request(compressButton, 110, 70);
        gtk_widget_set_size_request(addressBar, 1400, 30);
        gtk_widget_set_size_request(searchEntry, 200, 30);
        gtk_widget_set_size_request(searchButton, 100, 30);
        gtk_widget_set_size_request(scwin2, 1488, 200);
        gtk_widget_show_all(win);
        gtk_window_set_title(GTK_WINDOW(win), "File Explorer 0.4");
        currentPath = "/'home'/'" + username + "'/";
        ffpaneInsert(runcomm("ls /home/" + username));
        gtk_main();
        // std::string op = runcomm("ls");
        // std::cout << op << "\n";
}

// g++ main.cpp -o main `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` && ./main
// g++ main.cpp -o main `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` `pkg-config --cflags gstreamer-1.0` -fPIC `pkg-config --libs gstreamer-1.0 gobject-2.0 glib-2.0` && ./main
// g++ main.cpp -o main `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` `pkg-config --cflags gstreamer-1.0` -fPIC `pkg-config --libs gstreamer-1.0 gobject-2.0 glib-2.0` && ./main
// g++ main.cpp -o main -lX11 -pthread `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` `pkg-config --cflags gstreamer-1.0` -fPIC `pkg-config --libs gstreamer-1.0 gobject-2.0 glib-2.0` && ./main
//