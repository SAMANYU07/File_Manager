#include <iostream>
#include <gtk-3.0/gtk/gtk.h>

#pragma once

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
        static void on_entering_ffButton(GtkWidget *button, GdkEvent *event, gpointer data)
        {
                const gchar *new_label = static_cast<const gchar *>(data);
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
};