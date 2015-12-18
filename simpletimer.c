#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gtk/gtk.h>
#include <glib.h>

char* ICON_RUNNING = "media-playback-pause-symbolic";
char* ICON_STOPPED = "media-playback-start-symbolic";

GtkListBox* listbox;
GHashTable* table;
guint timeout_id;

typedef struct {
    GtkBuilder* row_builder;
    int is_running; // 1 is running. 0 is not.
    int is_countdown; // 0 is stopwatch. 1 is countdown.
    long time_started;  // in milliseconds since epoch
    long started_offset;  // in milliseconds
    long countdown_target; // in milliseconds
    char* text;
} RowData;

long now() {
    struct timespec current_time_spec;
    clock_gettime(CLOCK_MONOTONIC, &current_time_spec);
    return current_time_spec.tv_sec + (current_time_spec.tv_nsec / 1000000);
}

void shutdown(GtkWindow* window, GdkEvent* event, gpointer user_data) {
    printf("Shutting down!\n");
    g_object_unref(listbox);
    gtk_main_quit();
}

void start_stop_button_clicked(GtkWidget* button, gpointer user_data) {
    GObject* box;
    GObject* row_object;
    g_object_get(button, "parent", &box, NULL);
    g_object_get(box, "parent", &row_object, NULL);
    GtkWidget* row = GTK_WIDGET(row_object);
    RowData* row_data = (RowData*) g_hash_table_lookup(table, row);

    GObject* start_stop_image = gtk_builder_get_object(row_data->row_builder, "start_stop_image");

    if (row_data->is_running == 1){
        g_object_set(start_stop_image, "icon-name", ICON_STOPPED, NULL);
        row_data->started_offset = now() - row_data->time_started;
        row_data->is_running = 0;
    } else {
        g_object_set(start_stop_image, "icon-name", ICON_RUNNING, NULL);
        row_data->time_started = now();
        row_data->is_running = 1;
    }
}

void add_timer(GtkButton* button, gpointer should_be_countdown) {
    GtkBuilder* row_builder = gtk_builder_new_from_file("row.xml");

    GtkWidget* row = GTK_WIDGET(gtk_builder_get_object(row_builder, "row"));

    GtkWidget* start_stop_button = GTK_WIDGET(gtk_builder_get_object(row_builder, "start_stop_button"));
    g_signal_connect(start_stop_button, "clicked", G_CALLBACK(start_stop_button_clicked), NULL);

    GtkWidget* label = GTK_WIDGET(gtk_builder_get_object(row_builder, "label"));
    gtk_widget_override_font(label, pango_font_description_from_string("Cantarell 18"));
    gtk_label_set_text(GTK_LABEL(label), "0:00 ");

    gtk_widget_show_all(row);

    RowData* row_data = malloc(sizeof(RowData));
    row_data->row_builder = row_builder;
    row_data->time_started = now();
    row_data->is_running = 1;

    GObject* start_stop_image = gtk_builder_get_object(row_builder, "start_stop_image");
    g_object_set(start_stop_image, "icon-name", ICON_RUNNING, NULL);

    row_data->is_countdown = *((int*) should_be_countdown);
    row_data->text = malloc(10 * sizeof(char));
    g_hash_table_insert(table, row, row_data);
    gtk_list_box_prepend(listbox, row);
}

void row_selected(GtkListBox* widget, GtkListBoxRow* row, gpointer raw_main_builder) {
    GtkBuilder* builder = (GtkBuilder*) raw_main_builder;
    if (row == NULL) {
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(builder, "delete_button")), FALSE);
    } else {
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(builder, "delete_button")), TRUE);
    }
}

void delete_timer(GtkButton* delete_button, gpointer user_data) {
    GtkWidget* row = GTK_WIDGET(gtk_list_box_get_selected_row(listbox));
    if (row != NULL) {
        RowData* row_data = (RowData*) g_hash_table_lookup(table, row);
        g_hash_table_remove(table, row);
        g_free(row_data);
        gtk_widget_destroy(row);
        g_object_unref(row);
    }
}

void destroy_key(gpointer raw_row) {
}

void destroy_value(gpointer raw_row_data) {
}

void gen_label_text(long milli, char* text) {
    int seconds = (milli / 1000) % 60;
    int minutes = (milli / 1000) / 60;
    if(seconds < 10) {
        sprintf(text, "%d:0%d ", minutes, seconds);
    } else {
        sprintf(text, "%d:%d ", minutes, seconds);
    }
}

void process_row(gpointer raw_row, gpointer raw_row_data, gpointer user_data) {
    GtkWidget* row = (GtkWidget*) raw_row;
    RowData* row_data = (RowData*) raw_row_data;

    if (row_data->is_running == 1) {
        long elapsed = now() - row_data->time_started + row_data->started_offset;
        long milli;
        if (row_data->is_countdown == 0) {
            milli = elapsed;
        } else {
            milli = row_data->countdown_target - elapsed;
            if (milli < 0) {
                return;
            }
        }
        gen_label_text(milli, row_data->text);
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(row_data->row_builder, "label")), row_data->text);
    }
}

int timeout_callback(gpointer user_data) {
    g_hash_table_foreach(table, process_row, NULL);
    return 1;
}

int main(int argc, char* argv[]) {
    int opt_stopwatch = 0;
    int opt_countdown = 1;
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new_from_file("main_window.xml");
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    g_signal_connect(window, "delete-event", G_CALLBACK(shutdown), NULL);

    GtkWidget* add_stopwatch_button = GTK_WIDGET(gtk_builder_get_object(builder, "add_stopwatch_button"));
    g_signal_connect(add_stopwatch_button, "clicked", G_CALLBACK(add_timer), &opt_stopwatch);
    GtkWidget* delete_button = GTK_WIDGET(gtk_builder_get_object(builder, "delete_button"));
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_timer), NULL);

    listbox = GTK_LIST_BOX(gtk_builder_get_object(builder, "list"));
    g_signal_connect(listbox, "row_selected", G_CALLBACK(row_selected), builder);

    table = g_hash_table_new(NULL, NULL);

    timeout_id = g_timeout_add(100, timeout_callback, NULL);

    gtk_widget_show_all(window);
    
    g_object_unref(window);
    g_object_unref(add_stopwatch_button);
    g_object_unref(delete_button);

    gtk_main();
}
