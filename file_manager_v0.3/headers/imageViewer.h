#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

GtkWidget *main_image, *imageWin, *scwin3;
GdkPixbuf *main_image_pixbuf;
GdkPixbufAnimation *main_image_pixbuf_animation;
GdkPixbufAnimationIter *main_image_pixbuf_animation_iter;

class imageViewer
{
public:
        static void destroyImageWin()
        {
                gtk_main_quit();
                gtk_widget_destroy(imageWin);
        }

        static gboolean playGIF(gpointer user_data)
        {
                GTimeVal time;
                g_get_current_time(&time);
                gdk_pixbuf_animation_iter_advance(main_image_pixbuf_animation_iter, &time);
                main_image_pixbuf = gdk_pixbuf_animation_iter_get_pixbuf(main_image_pixbuf_animation_iter);
                main_image_pixbuf = gdk_pixbuf_scale_simple(main_image_pixbuf, 600, 600, GDK_INTERP_BILINEAR);
                g_timeout_add(gdk_pixbuf_animation_iter_get_delay_time(main_image_pixbuf_animation_iter), playGIF, NULL);
                main_image = gtk_image_new_from_pixbuf(main_image_pixbuf);
                gtk_widget_show(main_image);
                return false;
        }

        static void openImage(std::string targetPath)
        {
                GTimeVal time;
                g_get_current_time(&time);
                imageWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                scwin3 = gtk_scrolled_window_new(NULL, NULL);
                std::string imageLocation = "";
                for (int i = 0; i < targetPath.size(); i++)
                {
                        if (targetPath[i] != '\'')
                                imageLocation += targetPath[i];
                }
                if (targetPath.find(".gif") != std::string::npos)
                {
                        // GTimeVal time;
                        // main_image_pixbuf_animation = gdk_pixbuf_animation_new_from_file(imageLocation.c_str(), NULL);
                        // g_get_current_time(&time);
                        // main_image_pixbuf_animation_iter = gdk_pixbuf_animation_get_iter(main_image_pixbuf_animation, &time);
                        // gdk_pixbuf_animation_iter_advance(main_image_pixbuf_animation_iter, &time);
                        // main_image_pixbuf = gdk_pixbuf_animation_iter_get_pixbuf(main_image_pixbuf_animation_iter);
                        // main_image_pixbuf = gdk_pixbuf_scale_simple(main_image_pixbuf, 600, 600, GDK_INTERP_BILINEAR);
                        // g_timeout_add(gdk_pixbuf_animation_iter_get_delay_time(main_image_pixbuf_animation_iter), playGIF, NULL);
                        // main_image = gtk_image_new_from_pixbuf(main_image_pixbuf);
                        // playGIF(NULL);
                        // gtk_widget_show(main_image);
                        main_image_pixbuf = gdk_pixbuf_new_from_file(imageLocation.c_str(), NULL);
                        main_image_pixbuf_animation = gdk_pixbuf_animation_new_from_file(imageLocation.c_str(), NULL);
                        main_image = gtk_image_new_from_animation(main_image_pixbuf_animation);
                        int winw = gdk_pixbuf_animation_get_width(main_image_pixbuf_animation);
                        int winh = gdk_pixbuf_animation_get_height(main_image_pixbuf_animation);
                        gtk_window_resize(GTK_WINDOW(imageWin), winw, winh);
                }
                else
                {
                        main_image_pixbuf = gdk_pixbuf_new_from_file(imageLocation.c_str(), NULL);
                        main_image_pixbuf = gdk_pixbuf_scale_simple(main_image_pixbuf, 600, 600, GDK_INTERP_BILINEAR);
                        main_image = gtk_image_new_from_pixbuf(main_image_pixbuf);
                }
                gtk_image_set_pixel_size(GTK_IMAGE(main_image), 100);
                gtk_container_add(GTK_CONTAINER(scwin3), main_image);
                gtk_container_add(GTK_CONTAINER(imageWin), scwin3);
                g_signal_connect(imageWin, "delete-event", G_CALLBACK(destroyImageWin), NULL);
                gtk_window_set_default_size(GTK_WINDOW(imageWin), 800, 800);
                gtk_widget_show_all(imageWin);
                gtk_window_set_title(GTK_WINDOW(imageWin), "Image Viewer");
                gtk_main();
        }
};