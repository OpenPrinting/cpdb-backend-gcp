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
  //GMainLoop *loop = g_main_loop_new (NULL, FALSE);
  //g_main_loop_run (loop);
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
  GVariant *printers = gcp_object_get_printers (gcp,
                                                access_token,
                                                connection_status);

  g_assert (printers != NULL);

/******************************************************************************/
  GHashTableIter iter;
  gpointer key, value;
  GHashTable *printers_hashtable = (GHashTable *)printers;
  g_hash_table_iter_init (&iter, printers_hashtable);
  while (g_hash_table_iter_next (&iter, &key, &value))
  {
      g_print ("%s => %s\n\n", (gchar *)key, (gchar *)value);
  }
/******************************************************************************/
  g_print ("here\n");
  print_backend_complete_get_printers (skeleton,
                                       invocation,
                                       printers);

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
  GVariant *printer_options = gcp_object_get_printer_options(gcp,
                                                             uid,
                                                             access_token);

  g_assert (printer_options != NULL);
  print_backend_complete_get_printer_options (skeleton,
                                              invocation,
                                              printer_options);

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
