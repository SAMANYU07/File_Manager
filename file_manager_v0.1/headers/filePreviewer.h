#include <iostream>
#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>

GtkWidget *filePreviewer, *scwin2;
GtkTextBuffer *fileBuffer;

class filePreviewerComponent
{
        public:
        static int createFileViewer()
        {
                filePreviewer = gtk_text_view_new();
                scwin2 = gtk_scrolled_window_new(NULL, NULL);
                fileBuffer = gtk_text_buffer_new(NULL);
                gtk_text_view_set_buffer(GTK_TEXT_VIEW(filePreviewer), fileBuffer);
                // gtk_text_buffer_set_text(GTK_TEXT_BUFFER(fileBuffer), txt.c_str(), txt.length());
                gtk_container_add(GTK_CONTAINER(scwin2), filePreviewer);
                gtk_widget_set_name(filePreviewer, "filePreviewer");
        }
};