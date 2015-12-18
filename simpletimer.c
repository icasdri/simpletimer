#define UNUSED __attribute__((unused))

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <glib.h>

char* ICON_RUNNING = "media-playback-pause-symbolic";
char* ICON_STOPPED = "media-playback-start-symbolic";

GtkListBox* listbox;
GHashTable* table;
guint timeout_id;

typedef struct {
    GtkBuilder* row_builder;
    int tenths_elapsed;
    int is_running; // 1 is running. 0 is not.
    int is_countdown; // 0 is stopwatch. 1 is countdown.
    char* text;
} RowData;

void shutdown(UNUSED GtkWindow* window, UNUSED GdkEvent* event, UNUSED gpointer user_data) {
    printf("Shutting down!\n");
    g_object_unref(listbox);
    gtk_main_quit();
}

void start_stop_button_clicked(GtkWidget* button, UNUSED gpointer user_data) {
    GObject* box;
    GObject* row_object;
    g_object_get(button, "parent", &box, NULL);
    g_object_get(box, "parent", &row_object, NULL);
    GtkWidget* row = GTK_WIDGET(row_object);
    RowData* row_data = (RowData*) g_hash_table_lookup(table, row);

    GObject* start_stop_image = gtk_builder_get_object(row_data->row_builder, "start_stop_image");

    if (row_data->is_running == 1){
        g_object_set(start_stop_image, "icon-name", ICON_STOPPED, NULL);
        row_data->is_running = 0;
    } else {
        g_object_set(start_stop_image, "icon-name", ICON_RUNNING, NULL);
        row_data->is_running = 1;
    }
}

void add_timer(UNUSED GtkButton* button, gpointer should_be_countdown) {
    GtkBuilder* row_builder = gtk_builder_new_from_resource("/org/icasdri/simpletimer/row.xml");

    GtkWidget* row = GTK_WIDGET(gtk_builder_get_object(row_builder, "row"));

    GtkWidget* start_stop_button = GTK_WIDGET(gtk_builder_get_object(row_builder, "start_stop_button"));
    g_signal_connect(start_stop_button, "clicked", G_CALLBACK(start_stop_button_clicked), NULL);

    GtkWidget* label = GTK_WIDGET(gtk_builder_get_object(row_builder, "label"));
    gtk_label_set_text(GTK_LABEL(label), "0:00 ");

    gtk_widget_show_all(row);

    RowData* row_data = malloc(sizeof(RowData));
    row_data->row_builder = row_builder;
    row_data->tenths_elapsed = 0;
    row_data->is_running = 1;

    GObject* start_stop_image = gtk_builder_get_object(row_builder, "start_stop_image");
    g_object_set(start_stop_image, "icon-name", ICON_RUNNING, NULL);

    row_data->is_countdown = *((int*) should_be_countdown);
    row_data->text = malloc(10 * sizeof(char));
    g_hash_table_insert(table, row, row_data);
    gtk_list_box_prepend(listbox, row);
}

void row_selected(UNUSED GtkListBox* widget, GtkListBoxRow* row, gpointer raw_main_builder) {
    GtkBuilder* builder = (GtkBuilder*) raw_main_builder;
    if (row == NULL) {
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(builder, "delete_button")), FALSE);
    } else {
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(builder, "delete_button")), TRUE);
    }
}

void delete_timer(UNUSED GtkButton* delete_button, UNUSED gpointer user_data) {
    GtkWidget* row = GTK_WIDGET(gtk_list_box_get_selected_row(listbox));
    if (row != NULL) {
        RowData* row_data = (RowData*) g_hash_table_lookup(table, row);
        g_hash_table_remove(table, row);
        g_free(row_data);
        gtk_widget_destroy(row);
        g_object_unref(row);
    }
}

void gen_label_text(int tenths, char* text) {
    int seconds = (tenths / 10) % 60;
    int minutes = (tenths / 10) / 60;
    if(seconds < 10) {
        sprintf(text, "%d:0%d ", minutes, seconds);
    } else {
        sprintf(text, "%d:%d ", minutes, seconds);
    }
}

void process_row(UNUSED gpointer raw_row, gpointer raw_row_data, UNUSED gpointer user_data) {
    RowData* row_data = (RowData*) raw_row_data;

    if (row_data->is_running == 1) {
        if (row_data->is_countdown == 0) {
            row_data->tenths_elapsed += 1;
        } else {
            if (row_data->tenths_elapsed > 0) {
                row_data->tenths_elapsed -= 1;
            } else {
                return;
            }
        }
        gen_label_text(row_data->tenths_elapsed, row_data->text);
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(row_data->row_builder, "label")), row_data->text);
    }
}

int timeout_callback(UNUSED gpointer user_data) {
    g_hash_table_foreach(table, process_row, NULL);
    return 1;
}

int main(int argc, char* argv[]) {
    int opt_stopwatch = 0;
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new_from_resource("/org/icasdri/simpletimer/main_window.xml");
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    g_signal_connect(window, "delete-event", G_CALLBACK(shutdown), NULL);

    GtkWidget* add_stopwatch_button = GTK_WIDGET(gtk_builder_get_object(builder, "add_stopwatch_button"));
    g_signal_connect(add_stopwatch_button, "clicked", G_CALLBACK(add_timer), &opt_stopwatch);
    GtkWidget* delete_button = GTK_WIDGET(gtk_builder_get_object(builder, "delete_button"));
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_timer), NULL);

    GtkCssProvider* css_provider = gtk_css_provider_new();
    gtk_style_context_add_provider_for_screen(
            gdk_display_get_default_screen(gdk_display_get_default()),
            GTK_STYLE_PROVIDER(css_provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    gtk_css_provider_load_from_data(css_provider, ".timer-label { font: Cantarell 18; }", -1, NULL);

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
