#ifndef __GCP_SERVER_H__
#define __GCP_SERVER_H__

#include <glib.h>
#include <glib-object.h>
#include "gcp.h"

G_BEGIN_DECLS

#define SERVER_TYPE_OBJECT server_object_get_type()
G_DECLARE_DERIVABLE_TYPE (ServerObject, server_object, Server, OBJECT, GObject)

struct _ServerObjectClass
{
  GObjectClass parent;
};

typedef struct _BackendObj
{
    GDBusConnection *dbus_connection;
    GHashTable *dialog_printers;
} BackendObj;

ServerObject *
server_object_new ();

static void
acquire_session_bus_name ();

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar *name,
                  gpointer user_data);

static gboolean
on_handle_get_all_options (PrintBackend *skeleton,
                               GDBusMethodInvocation *invocation,
                               const gchar *printer_id,
                               gpointer user_data);

// static gboolean
// on_handle_submit_print_job (PrintBackend *skeleton,
//                             GDBusMethodInvocation *invocation,
//                             const gchar *uid,
//                             const gchar *access_token,
//                             const gchar *title,
//                             const gchar *ticket,
//                             gpointer user_data);

static gboolean
on_handle_get_all_jobs (PrintBackend *skeleton,
                        GDBusMethodInvocation *invocation,
                        gboolean active_only,
                        gpointer user_data);


static gboolean
on_handle_get_active_jobs_count (PrintBackend *skeleton,
                                 GDBusMethodInvocation *invocation,
                                 const gchar *printer_id,
                                 gpointer user_data);

static gboolean
on_handle_get_printer_state (PrintBackend *skeleton,
                             GDBusMethodInvocation *invocation,
                             const gchar *printer_id,
                             gpointer user_data);

static gboolean
on_handle_is_accepting_jobs (PrintBackend *skeleton,
                             GDBusMethodInvocation *invocation,
                             const gchar *printer_id,
                             gpointer user_data);

static gboolean
on_handle_cancel_job (PrintBackend *skeleton,
                     GDBusMethodInvocation *invocation,
                     const gchar *job_id,
                     const gchar *printer_id,
                     gpointer user_data);

static gboolean
on_handle_activate_backend (PrintBackend *skeleton,
                            GDBusMethodInvocation *invocation,
                            gpointer user_data);

static gboolean
on_handle_get_backend_name (PrintBackend *skeleton,
                            GDBusMethodInvocation *invocation,
                            gpointer user_data);

static gboolean
on_handle_print_file (PrintBackend *skeleton,
                      GDBusMethodInvocation *invocation,
                      const gchar *printer_id,
                      const gchar *file_path_name,
                      gint num_settings,
                      GVariant *settings,
                      gpointer user_data);

gint
comp_function (gconstpointer data_a, gconstpointer data_b);

void
refresh_printer_list(char *dialog_name);

static void
on_refresh_backend(GDBusConnection *connection,
                   const gchar *sender_name,
                   const gchar *object_path,
                   const gchar *interface_name,
                   const gchar *signal_name,
                   GVariant *parameters,
                   gpointer not_used);

void connect_to_signals (PrintBackend *skeleton);

void emit_printer_added_signal (GCPObject *gcp,
                                const gchar *access_token,
                                const gchar *dialog_name,
                                printer *printer_struct);

void emit_printer_removed_signal (const gchar *dialog_name,
                                  printer *printer_struct);

BackendObj *get_new_BackendObj();

G_END_DECLS

#endif /* GCP_SERVER_H */
