#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <gstreamer-1.0/gst/gst.h>

class videoPlayer
{
        public:
        static int playVideo(std::string targetVideo)
        {
                std::string temp = "";
                for (int i = 0; i < targetVideo.size(); i++)
                {
                        if (targetVideo[i] == '\'')
                                continue;
                        else if (targetVideo[i] == ' ')
                                temp += "%20";
                        else
                                temp += targetVideo[i];
                }
                std::cout << "updated_path: " << temp << "\n";
                targetVideo = temp;
                GtkWidget *win, *videoWidget;
                GstElement *pipeline;
                GstElement *videosink;
                gtk_init(NULL, NULL);
                gst_init(NULL, NULL);
                win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                videoWidget = gtk_drawing_area_new();
                gtk_container_add(GTK_CONTAINER(win), videoWidget);
                const gchar *uri = targetVideo.c_str();
                std::string pipelineDesc = "playbin uri=file://" + targetVideo;
                pipeline = gst_parse_launch(pipelineDesc.c_str(), NULL);
                videosink = gst_element_factory_make("gtksink", "videosink");
                g_object_set(G_OBJECT(pipeline), "video-sink", videosink, NULL);
                gst_element_set_state(pipeline, GST_STATE_PLAYING);
                g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
                gtk_widget_show_all(win);
                gtk_main();
        }
};