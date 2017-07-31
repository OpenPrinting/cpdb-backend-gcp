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
                    "handle_get_all_options",
                    G_CALLBACK (on_handle_get_all_options),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_get_all_jobs",
                    G_CALLBACK (on_handle_get_all_jobs),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_get_active_jobs_count",
                    G_CALLBACK (on_handle_get_active_jobs_count),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_get_default_media",
                    G_CALLBACK (on_handle_get_default_media),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_get_supported_media",
                    G_CALLBACK (on_handle_get_supported_media),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_get_default_resolution",
                    G_CALLBACK (on_handle_get_default_resolution),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_get_supported_resolution",
                    G_CALLBACK (on_handle_get_supported_resolution),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_get_printer_state",
                    G_CALLBACK (on_handle_get_printer_state),
                    NULL);

  g_signal_connect (skeleton,
                    "handle_is_accepting_jobs",
                    G_CALLBACK (on_handle_is_accepting_jobs),
                    NULL);
}

// static gboolean
// on_handle_get_printers (PrintBackend *skeleton,
//                         GDBusMethodInvocation *invocation,
//                         const gchar *access_token,
//                         const gchar *connection_status,
//                         gpointer user_data)
//
// {
//   g_print ("on_handle_get_printers() called\n");
//   GCPObject *gcp = gcp_object_new ();
//   GList *printers = gcp_object_get_printers (gcp,
//                                              access_token,
//                                              connection_status);
//
//   g_assert (printers != NULL);
//
//   GVariantBuilder *builder;
//   GVariant *dict;
//   builder = g_variant_builder_new (G_VARIANT_TYPE ("a{sv}"));
//   while(printers != NULL)
//   {
//     printer *printer_struct = printers->data;
//     GVariant *value = g_variant_new ("(s)", printer_struct->name);
//     g_variant_builder_add (builder, "{sv}", printer_struct->id, value);
//     printers = printers->next;
//   }
//
//   dict = g_variant_builder_end (builder);
//   print_backend_complete_get_printers (skeleton,
//                                        invocation,
//                                        dict);
//
//   g_object_unref (gcp);
//   return TRUE;
// }

static gboolean
on_handle_get_all_options (PrintBackend *skeleton,
                           GDBusMethodInvocation *invocation,
                           const gchar *printer_id,
                           gpointer user_data)
{
  g_print ("on_handle_get_all_options() called\n");
  GCPObject *gcp = gcp_object_new ();

  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);

  GList *vendor_capability_options = gcp_object_get_vendor_capability_options (gcp,
                                                                               printer_id,
                                                                               access_token);

  GVariantBuilder *builder;

  guint count = g_list_length (vendor_capability_options);

  GVariant *vendor_capability_dict;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a(ssia(s))"));


  while(vendor_capability_options != NULL)
  {
    vendor_capability *vc = vendor_capability_options->data;
    GList *options = vc->options;

    GVariantBuilder *str_array_builder;
    str_array_builder = g_variant_builder_new(G_VARIANT_TYPE("a(s)"));
    if (vc->num_supported == 0)
        g_variant_builder_add(str_array_builder, "(s)", "NA");

    while (options != NULL)
    {
     vendor_capability_option *vc_option = options->data;
     g_variant_builder_add (str_array_builder, "(s)", vc_option->display_name);
     options = options->next;
    }

    GVariant **t = g_new (GVariant *, 4);
    t[0] = g_variant_new_string (vc->display_name);
    t[1] = g_variant_new_string (vc->default_value);
    t[2] = g_variant_new_int32 (vc->num_supported);
    t[3] = g_variant_new ("a(s)", str_array_builder);

    GVariant *tuple_variant = g_variant_new_tuple (t, 4);
    g_free (t);

    g_variant_builder_add_value (builder, tuple_variant);

    vendor_capability_options = vendor_capability_options->next;
  }

  vendor_capability_dict = g_variant_new ("a(ssia(s))", builder);


  print_backend_complete_get_all_options (skeleton,
                                          invocation,
                                          count,
                                          vendor_capability_dict);

  g_object_unref (gcp);
  return TRUE;
}

// static gboolean
// on_handle_submit_print_job (PrintBackend *skeleton,
//                             GDBusMethodInvocation *invocation,
//                             const gchar *uid,
//                             const gchar *access_token,
//                             const gchar *title,
//                             const gchar *ticket,
//                             gpointer user_data)
// {
//   /* TODO: Implement on_handle_submit_print_job() */
//   return FALSE;
// }

static gboolean
on_handle_get_all_jobs (PrintBackend *skeleton,
                        GDBusMethodInvocation *invocation,
                        gboolean active_only,
                        gpointer user_data)
{
  g_print ("on_handle_get_all_jobs() called\n");

  gchar *uid = "";
  gchar *owner = "";
  gchar *status = "";
  gchar *sortorder = "";

  GCPObject *gcp = gcp_object_new ();
  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);
  GList *print_jobs = gcp_object_get_print_jobs (gcp,
                                                 access_token,
                                                 uid,
                                                 owner,
                                                 status,
                                                 sortorder);

  GVariantBuilder *builder;
  GVariant *dict;
  gint num_jobs = 0;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a(ssssssi)"));
  while(print_jobs != NULL)
  {
    print_job *job = print_jobs->data;
    if(active_only & g_strcmp0(job->status, "IN_PROGRESS") == 0)
    {
      g_variant_builder_add (builder, "(ssssssi)", job->id, job->title, job->printerName, job->user, job->status, job->submitted_at, job->size);
    }
    else
    {
      g_variant_builder_add (builder, "(ssssssi)", job->id, job->title, job->printerName, job->user, job->status, job->submitted_at, job->size);
    }
    num_jobs++;
    print_jobs = print_jobs->next;
  }

  dict = g_variant_builder_end (builder);
  print_backend_complete_get_all_jobs (skeleton,
                                       invocation,
                                       num_jobs,
                                       dict);

  g_object_unref (gcp);
  return TRUE;
}

static gboolean
on_handle_get_active_jobs_count (PrintBackend *skeleton,
                                 GDBusMethodInvocation *invocation,
                                 const gchar *printer_id,
                                 gpointer user_data)
{
  g_print ("on_handle_get_active_jobs_count() called\n");

  gchar *uid = "";
  gchar *owner = "";
  gchar *status = "";
  gchar *sortorder = "";

  GCPObject *gcp = gcp_object_new ();
  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);
  GList *print_jobs = gcp_object_get_print_jobs (gcp,
                                                 access_token,
                                                 uid,
                                                 owner,
                                                 status,
                                                 sortorder);

  gint active_jobs_count = 0;
  while(print_jobs != NULL)
  {
    print_job *job = print_jobs->data;
    if(g_strcmp0(job->status, "IN_PROGRESS") == 0)
      active_jobs_count++;

    print_jobs = print_jobs->next;
  }

  print_backend_complete_get_active_jobs_count (skeleton,
                                                invocation,
                                                active_jobs_count);

  g_object_unref (gcp);
  return TRUE;
}

static gboolean
on_handle_get_default_media (PrintBackend *skeleton,
                             GDBusMethodInvocation *invocation,
                             const gchar *printer_id,
                             gpointer user_data)
{
  g_print ("on_handle_get_default_media() called\n");
  GCPObject *gcp = gcp_object_new ();

  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);

  GList *media_size_list = gcp_object_get_media_size_options (gcp,
                                                              printer_id,
                                                              access_token);
  gchar *default_media;
  while(media_size_list != NULL)
  {
   media_size *media = media_size_list->data;
   if(media->is_default)
   {
     default_media = g_strdup(media->custom_display_name);
     break;
   }
   media_size_list = media_size_list->next;
  }

  print_backend_complete_get_default_media (skeleton,
                                            invocation,
                                            default_media);

  g_object_unref (gcp);
  return TRUE;
}

static gboolean
on_handle_get_supported_media (PrintBackend *skeleton,
                               GDBusMethodInvocation *invocation,
                               const gchar *printer_id,
                               gpointer user_data)
{
  g_print ("on_handle_get_supported_media() called\n");
  GCPObject *gcp = gcp_object_new ();

  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);

  GList *media_size_list = gcp_object_get_media_size_options (gcp,
                                                              printer_id,
                                                              access_token);

  GVariantBuilder *builder;
  GVariant *supported_media;
  int num_values = 0;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a(s)"));
  while(media_size_list != NULL)
  {
   media_size *media = media_size_list->data;

   GVariant *value = g_variant_new ("(s)", media->custom_display_name);

   g_variant_builder_add (builder, "(s)", value);
   num_values++;
   media_size_list = media_size_list->next;
  }
  supported_media = g_variant_builder_end (builder);

  print_backend_complete_get_supported_media (skeleton,
                                              invocation,
                                              num_values,
                                              supported_media);

  g_object_unref (gcp);
  return TRUE;
}

static gboolean
on_handle_get_default_resolution (PrintBackend *skeleton,
                                  GDBusMethodInvocation *invocation,
                                  const gchar *printer_id,
                                  gpointer user_data)
{
  g_print ("on_handle_get_default_resolution() called\n");
  GCPObject *gcp = gcp_object_new ();

  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);

  GList *vendor_capability_options = gcp_object_get_vendor_capability_options (gcp,
                                                                               printer_id,
                                                                               access_token);

  GVariantBuilder *builder;

  gchar *default_resolution;
  while(vendor_capability_options != NULL)
  {
    vendor_capability *vc = vendor_capability_options->data;

    if(g_strcmp0(vc->display_name, "Resolution") == 0)
    {
      GList *options = vc->options;
      while (options != NULL)
      {
       vendor_capability_option *vc_option = options->data;
       if(vc_option->is_default)
       {
         default_resolution = g_strdup (vc_option->display_name);
         break;
       }
       options = options->next;
      }
      break;
    }
    vendor_capability_options = vendor_capability_options->next;
  }

  print_backend_complete_get_default_resolution (skeleton,
                                                 invocation,
                                                 default_resolution);

  g_object_unref (gcp);
  return TRUE;
}

static gboolean
on_handle_get_supported_resolution (PrintBackend *skeleton,
                                    GDBusMethodInvocation *invocation,
                                    const gchar *printer_id,
                                    gpointer user_data)
{
  g_print ("on_handle_get_supported_resolution() called\n");
  GCPObject *gcp = gcp_object_new ();

  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);

  GList *vendor_capability_options = gcp_object_get_vendor_capability_options (gcp,
                                                                               printer_id,
                                                                               access_token);

  GVariantBuilder *builder;
  GVariant *supported_resolution_values;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a(s)"));
  int num_values = 0;
  while(vendor_capability_options != NULL)
  {
    vendor_capability *vc = vendor_capability_options->data;
    if(g_strcmp0(vc->display_name, "Resolution") == 0)
    {
      GList *options = vc->options;
      while (options != NULL)
      {
       vendor_capability_option *vc_option = options->data;
       g_variant_builder_add (builder, "(s)", vc_option->display_name);
       num_values++;
       options = options->next;
      }
      break;
    }
    vendor_capability_options = vendor_capability_options->next;
  }
  supported_resolution_values = g_variant_builder_end (builder);

  print_backend_complete_get_supported_resolution (skeleton,
                                                   invocation,
                                                   num_values,
                                                   supported_resolution_values);

  g_object_unref (gcp);
  return TRUE;
}

static gboolean
on_handle_get_printer_state (PrintBackend *skeleton,
                             GDBusMethodInvocation *invocation,
                             const gchar *printer_id,
                             gpointer user_data)
{
  g_print ("on_handle_get_printer_state() called\n");
  GCPObject *gcp = gcp_object_new ();

  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);

  gchar *printer_state = gcp_object_get_printer_state (gcp,
                                                       access_token,
                                                       printer_id);

  print_backend_complete_get_printer_state (skeleton,
                                            invocation,
                                            printer_state);

  g_object_unref (gcp);
  return TRUE;
}

static gboolean
on_handle_is_accepting_jobs (PrintBackend *skeleton,
                             GDBusMethodInvocation *invocation,
                             const gchar *printer_id,
                             gpointer user_data)
{
  g_print ("on_handle_is_accepting_jobs() called\n");
  GCPObject *gcp = gcp_object_new ();

  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);
  gboolean is_accepting_jobs = FALSE;

  gchar *printer_state = gcp_object_get_printer_state (gcp,
                                                       access_token,
                                                       printer_id);
  if(g_strcmp0 (printer_state, "ONLINE") == 0)
    is_accepting_jobs = TRUE;


  print_backend_complete_is_accepting_jobs (skeleton,
                                            invocation,
                                            is_accepting_jobs);

  g_object_unref (gcp);
  return TRUE;
}
