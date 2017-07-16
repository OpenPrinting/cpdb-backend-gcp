#include "gcp.h"
#include <stdio.h>

static void
callback_get_printers_async(GObject *proxy,
                            GAsyncResult *res,
                            gpointer user_data);

int
main ()
{
  GCPObject *gcp = gcp_object_new ();
  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gchar *connection_status = "ALL";
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);
  // g_print ("%s\n", access_token);

  GHashTableIter iter;
  gpointer key, value;

  GList *printer_jobs = gcp_object_get_print_jobs (gcp, access_token, NULL, NULL, NULL, NULL);
  if (printer_jobs == NULL)
  {
    g_print ("No jobs for this printer can be found!\n");
  }
  while(printer_jobs != NULL)
  {
    print_job *print_job_struct = printer_jobs->data;
    g_print ("id : %s\n", print_job_struct->id);
    g_print ("title : %s\n", print_job_struct->title);
    g_print ("printerid : %s\n", print_job_struct->printerid);
    g_print ("printerName : %s\n", print_job_struct->printerName);
    g_print ("status : %s\n", print_job_struct->status);

    printer_jobs = printer_jobs->next;
  }

  // GHashTable *printers = gcp_object_get_printers (gcp, access_token, "ALL");
  //
  // g_hash_table_iter_init (&iter, printers);
  // while (g_hash_table_iter_next (&iter, &key, &value))
  // {
  //     g_print ("%s => %s\n\n", (gchar *)key, (gchar *)value);
  // }
  //
  // gchar *uid = g_malloc (50);
  // g_print ("Enter uid of printer to get printer options:\n");
  // scanf ("%s", uid);
  // GHashTable *printer_options = gcp_object_get_printer_options (gcp, uid, access_token);
  // g_assert (printer_options != NULL);
  // GList *keys = g_hash_table_get_keys (printer_options);
  // while (keys != NULL)
  // {
  //   if(g_strcmp0 ((gchar *)keys->data, "vendor_capability_list") == 0)
  //   {
  //     g_print ("vendor_capability_list\n");
  //     GList *vendor_capability_list = (GList *)g_hash_table_lookup (printer_options, keys->data);
  //     g_assert (vendor_capability_list != NULL);
  //     while (vendor_capability_list != NULL)
  //     {
  //       vendor_capability *capabilities = vendor_capability_list->data;
  //       g_print ("%s\n", capabilities->id);
  //       g_print ("%s\n", capabilities->display_name);
  //       g_print ("%s\n", capabilities->type);
  //
  //       GList *vc_options = capabilities->options;
  //       while(vc_options != NULL)
  //       {
  //         vendor_capability_option *vc_option = vc_options->data;
  //         g_print ("%s\n", vc_option->display_name);
  //         g_print ("%d\n", vc_option->is_default);
  //         g_print ("%s\n", vc_option->value);
  //         vc_options = vc_options->next;
  //       }
  //
  //       vendor_capability_list = vendor_capability_list->next;
  //     }
  //   }
  //   else if (g_strcmp0 ((gchar *)keys->data, "media_size_options_list") == 0)
  //   {
  //     g_print ("media_size_options_list\n");
  //     GList *media_size_options_list = (GList *)g_hash_table_lookup (printer_options, keys->data);
  //     g_assert (media_size_options_list != NULL);
  //     while (media_size_options_list != NULL)
  //     {
  //       media_size *option = media_size_options_list->data;
  //       g_print ("%s\n", option->name);
  //       g_print ("%s\n", option->vendor_id);
  //       g_print ("%s\n", option->custom_display_name);
  //       g_print ("%d\n", option->height_microns);
  //       g_print ("%d\n", option->width_microns);
  //       g_print ("%d\n", option->is_default);
  //       g_print ("%d\n", option->is_continuous_feed);
  //
  //       media_size_options_list = media_size_options_list->next;
  //     }
  //   }
  //   keys = keys->next;
  // }
  g_object_unref (gcp);
  return 0;
}
