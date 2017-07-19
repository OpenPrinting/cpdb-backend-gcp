#include "gcp.h"
#include <stdio.h>

static void
callback_get_printers_async(GObject *proxy,
                            GAsyncResult *res,
                            gpointer user_data);


GVariant *pack_string_array(int num_val, char **val)
{
  GVariantBuilder *builder;
  GVariant *values;
  builder = g_variant_builder_new(G_VARIANT_TYPE("a(s)"));
  for (int i = 0; i < num_val; i++)
  {
      // g_message("%s", val[i]);
      g_variant_builder_add(builder, "(s)", val[i]);
  }

  if (num_val == 0)
      g_variant_builder_add(builder, "(s)", "NA");

  values = g_variant_new("a(s)", builder);
  return values;
}

typedef struct person{
  gchar *first_name;
  gchar *last_name;
} person;

int
main ()
{
  // GVariantBuilder *builder;
  // GVariant *value1;
  // GVariant *value2;
  //
  // person *p1 = g_malloc (sizeof(person));
  // p1->first_name = g_strdup ("Abhijeet");
  // p1->last_name = g_strdup ("Dubey");
  //
  // person *p2 = g_malloc (sizeof(person));
  // p2->first_name = g_strdup ("John");
  // p2->last_name = g_strdup ("Doe");
  //
  // value1 = g_variant_new ("(ss)", p1->first_name, p1->last_name);
  // value2 = g_variant_new ("(ss)", p2->first_name, p2->last_name);
  //
  // person *pp1 = g_malloc (sizeof(person));
  // g_variant_get (value1, "(ss)", &pp1->first_name, &pp1->last_name);
  // g_print ("%s %s\n", pp1->first_name, pp1->last_name);
  //
  //
  // person *pp2 = g_malloc (sizeof(person));
  // g_variant_get (value2, "(ss)", &pp2->first_name, &pp2->last_name);
  // g_print ("%s %s\n", pp2->first_name, pp2->last_name);

  GVariantBuilder *b;
  GVariant *dict;
  GVariant *value;
  value = g_variant_new ("(ss)", "Abhijeet", "Dubey");

  int i = 10;
  int length = snprintf( NULL, 0, "%d", i );
  gchar *str = g_malloc (length + 1);
  snprintf(str, length + 1 ,"%d", i);

  g_assert (value != NULL);
  b = g_variant_builder_new (G_VARIANT_TYPE ("a{sv}"));
  g_variant_builder_add (b, "{sv}", str, value);
  // g_variant_builder_add (b, "{sv}", "timeout", g_variant_new_int32 (10));
  dict = g_variant_builder_end (b);
  g_assert (dict != NULL);
  GVariant *val = g_variant_lookup_value (dict, str, NULL);
  g_assert (val != NULL);
  const gchar *fname, *lname;
  g_variant_get (val, "(ss)", &fname, &lname);
  g_print ("%s %s\n", fname, lname);

  return 0;
}

/*
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
*/
