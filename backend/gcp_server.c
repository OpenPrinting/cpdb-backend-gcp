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

  GVariantBuilder *builder;
  GVariant *dict;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a{sv}"));
  while(printers != NULL)
  {
    printer *printer_struct = printers->data;
    GVariant *value = g_variant_new ("(s)", printer_struct->name);
    g_variant_builder_add (builder, "{sv}", printer_struct->id, value);
    printers = printers->next;
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
  g_print ("on_handle_get_printer_options() called\n");
  GCPObject *gcp = gcp_object_new ();

  GList *media_size_list = gcp_object_get_media_size_options (gcp,
                                                              uid,
                                                              access_token);

  GList *vendor_capability_options = gcp_object_get_vendor_capability_options (gcp,
                                                                               uid,
                                                                               access_token);

  GVariantBuilder *builder;
  GVariant *media_options_dict;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a{sv}"));
  while(media_size_list != NULL)
  {
   media_size *media = media_size_list->data;

   GVariant *value = g_variant_new ("(iibbsss)",
                                    media->height_microns,
                                    media->width_microns,
                                    media->is_continuous_feed,
                                    media->is_default,
                                    media->vendor_id,
                                    media->name,
                                    media->custom_display_name);

   g_variant_builder_add (builder, "{sv}", uid, value);
   media_size_list = media_size_list->next;
  }
  media_options_dict = g_variant_builder_end (builder);


  GVariant *vendor_capability_dict;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a{sv}"));
  while(vendor_capability_options != NULL)
  {
   vendor_capability *vc = vendor_capability_options->data;
   GList *options = vc->options;
   GVariantBuilder *b;
   GVariant *vc_option_variant;
   b = g_variant_builder_new (G_VARIANT_TYPE ("a(v)"));
   while (options != NULL)
   {
     vendor_capability_option *vc_option = options->data;
     GVariant *option_variant = g_variant_new ("(sbs)",
                                               vc_option->display_name,
                                               vc_option->is_default,
                                               vc_option->value);

     g_variant_builder_add (b, "(v)", option_variant);
     options = options->next;
   }
   vc_option_variant = g_variant_builder_end (b);

   GVariant *value = g_variant_new ("(sssv)",
                                    vc->id,
                                    vc->display_name,
                                    vc->type,
                                    vc_option_variant);

   g_variant_builder_add (builder, "{sv}", uid, value);
   vendor_capability_options = vendor_capability_options->next;
  }
  vendor_capability_dict = g_variant_builder_end (builder);


  print_backend_complete_get_printer_options (skeleton,
                                              invocation,
                                              media_options_dict,
                                              vendor_capability_dict);

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
  g_print ("on_handle_get_print_jobs() called\n");
  GCPObject *gcp = gcp_object_new ();
  GList *print_jobs = gcp_object_get_print_jobs (gcp,
                                                 access_token,
                                                 uid,
                                                 owner,
                                                 status,
                                                 sortorder);

  GVariantBuilder *builder;
  GVariant *dict;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a{sv}"));
  while(print_jobs != NULL)
  {
    print_job *job = print_jobs->data;
    GVariant *value = g_variant_new ("(ssss)", job->title, job->printerid, job->printerName, job->status);
    g_variant_builder_add (builder, "{sv}", job->id, value);
    print_jobs = print_jobs->next;
  }

  dict = g_variant_builder_end (builder);
  print_backend_complete_get_printers (skeleton,
                                       invocation,
                                       dict);

  g_object_unref (gcp);
  return TRUE;
}
