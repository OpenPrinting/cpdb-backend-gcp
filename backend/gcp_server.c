#include <stdio.h>
#include <stdlib.h>
#include "gcp_server.h"

G_DEFINE_TYPE (ServerObject, server_object, G_TYPE_OBJECT)

#define BUS_NAME "org.openprinting.Backend.GCP"
#define OBJECT_PATH "/"

ServerObject *
server_object_new()
{
  return g_object_new (SERVER_TYPE_OBJECT, NULL);
}

static void
server_object_init (ServerObject *self)
{
}

static void
server_object_class_init (ServerObjectClass *klass)
{
  GObjectClass *parent_class = G_OBJECT_CLASS (klass);
  acquire_session_bus_name ();
}

static void
acquire_session_bus_name ()
{
  g_bus_own_name (G_BUS_TYPE_SESSION,
                  BUS_NAME,
                  0,                    // Flags
                  NULL,                 // bus acquired handler
                  on_name_acquired,     // name acquired handler
                  NULL,                 // name lost handler
                  NULL,                 // user_data
                  NULL);                // user_data_free_func
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar *name,
                  gpointer user_data)
{
  g_print ("name acquired!\n");
  PrintBackend *skeleton;
  skeleton = print_backend_skeleton_new ();
  connect_to_signals (skeleton);
  connect_to_dbus (connection, skeleton, OBJECT_PATH);
}

void
connect_to_signals (PrintBackend *skeleton)
{
  g_print ("connected to signals!\n");
  g_signal_connect (skeleton,
                    "handle_get_printers",
                    G_CALLBACK (on_handle_get_printers),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_get_printer_options",
                    G_CALLBACK (on_handle_get_printer_options),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_submit_print_job",
                    G_CALLBACK (on_handle_submit_print_job),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_get_print_jobs",
                    G_CALLBACK (on_handle_get_print_jobs),
                    NULL);
}

static gboolean
on_handle_get_printers (PrintBackend *skeleton,
                        GDBusMethodInvocation *invocation,
                        const gchar *access_token,
                        const gchar *connection_status,
                        gpointer user_data)

{
  g_print ("on_handle_get_printers() called\n");
  GCPObject *gcp = gcp_object_new ();
  GList *printers = gcp_object_get_printers (gcp,
                                             access_token,
                                             connection_status);

  g_assert (printers != NULL);
  guint printer_list_len = g_list_length (printers);
  guint length = snprintf(NULL, 0, "%d", printer_list_len);

/******************************************************************************/
  // This block is only for testing purpose. Remove this block in final release.
  // GList *printers_ = printers;
  // while(printers_ != NULL)
  // {
  //   printer *printer_struct = printers_->data;
  //   g_print ("id : %s\n", printer_struct->id);
  //   g_print ("printerName : %s\n", printer_struct->name);
  //
  //   printers_ = printers_->next;
  // }
/******************************************************************************/

  GVariantBuilder *builder;
  GVariant *dict;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a{sv}"));
  int i = 0;
  while(printers != NULL)
  {
    gchar *key = g_malloc (length + 1);
    snprintf(key, length + 1 ,"%d", i);
    printer *printer_struct = printers->data;
    GVariant *value = g_variant_new ("(ss)", printer_struct->id, printer_struct->name);
    g_variant_builder_add (builder, "{sv}", key, value);
    printers = printers->next;
    i++;
  }

  dict = g_variant_builder_end (builder);
  print_backend_complete_get_printers (skeleton,
                                       invocation,
                                       dict);

  g_object_unref (gcp);
  return TRUE;
}

static gboolean
on_handle_get_printer_options (PrintBackend *skeleton,
                               GDBusMethodInvocation *invocation,
                               const gchar *uid,
                               const gchar *access_token,
                               gpointer user_data)
{
  GCPObject *gcp = gcp_object_new ();

  GList *media_size_list = gcp_object_get_media_size_options (gcp,
                                                              uid,
                                                              access_token);

  GList *vendor_capability_options = gcp_object_get_vendor_capability_options (gcp,
                                                                               uid,
                                                                               access_token);

  // print_backend_complete_get_printer_options (skeleton,
  //                                             invocation,
  //                                             media_options_variant,
  //                                             vendor_capability_variant);

  g_object_unref (gcp);
  return TRUE;
}

static gboolean
on_handle_submit_print_job (PrintBackend *skeleton,
                            GDBusMethodInvocation *invocation,
                            const gchar *uid,
                            const gchar *access_token,
                            const gchar *title,
                            const gchar *ticket,
                            gpointer user_data)
{
  /* TODO: Implement on_handle_submit_print_job() */
  return FALSE;
}

static gboolean
on_handle_get_print_jobs   (PrintBackend *skeleton,
                            GDBusMethodInvocation *invocation,
                            const gchar *access_token,
                            const gchar *uid,
                            const gchar *owner,
                            const gchar *status,
                            const gchar *sortorder,
                            gpointer user_data)
{
  /* TODO: Implement on_handle_get_print_jobs() */
  return TRUE;
}
