#include "gcp.h"
#include <stdio.h>

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

  GList *printers = gcp_object_get_printers (gcp, access_token, "ALL");
  while(printers != NULL)
  {
    printer *printer_struct = printers->data;
    g_print ("id : %s\n", printer_struct->id);
    g_print ("printerName : %s\n", printer_struct->name);

    printers = printers->next;
  }

  gchar *uid = g_malloc (50);
  g_print ("Enter uid of printer to get printer options:\n");
  scanf ("%s", uid);

  GList *vendor_capability_list = gcp_object_get_vendor_capability_options (gcp, uid, access_token);
  while (vendor_capability_list != NULL)
  {
    vendor_capability *capabilities = vendor_capability_list->data;
    g_print ("%s\n", capabilities->id);
    g_print ("%s\n", capabilities->display_name);
    g_print ("%s\n", capabilities->type);

    GList *vc_options = capabilities->options;
    while(vc_options != NULL)
    {
      vendor_capability_option *vc_option = vc_options->data;
      g_print ("%s\n", vc_option->display_name);
      g_print ("%d\n", vc_option->is_default);
      g_print ("%s\n", vc_option->value);
      vc_options = vc_options->next;
    }

    vendor_capability_list = vendor_capability_list->next;
  }

  GList *media_size_options_list = gcp_object_get_media_size_options (gcp, uid, access_token);
  while (media_size_options_list != NULL)
  {
    media_size *option = media_size_options_list->data;
    g_print ("%s\n", option->name);
    g_print ("%s\n", option->vendor_id);
    g_print ("%s\n", option->custom_display_name);
    g_print ("%d\n", option->height_microns);
    g_print ("%d\n", option->width_microns);
    g_print ("%d\n", option->is_default);
    g_print ("%d\n", option->is_continuous_feed);

    media_size_options_list = media_size_options_list->next;
  }

  g_object_unref (gcp);
  return 0;
}
