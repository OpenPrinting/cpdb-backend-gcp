#include "common_helper.h"
#include <stdio.h>

int
main ()
{
  GError *error = NULL;

  PrintBackend *proxy;
  GVariant *dict;

  error = NULL;
  proxy = print_backend_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, 0, "org.openprinting.Backend.GCP", "/", NULL, &error);

  error = NULL;

  g_print ("\n\n*******Get Backend Name*******\n\n");
  error = NULL;
  gchar *backend_name;
  print_backend_call_get_backend_name_sync (proxy, &backend_name, NULL, &error);
  g_print ("Backend Name => %s\n", backend_name);

  g_print ("\n\n*******Testing Submit Print File*******\n\n");
  error = NULL;
  gchar *test_printer_id = g_strdup ("0ec6dd1e-f113-615c-5283-54ac7c389049");
  gchar *file_path_name = g_strdup ("/home/abhi/Desktop/fee.pdf");
  gint num_settings = 0;
  GVariant *settings;
  gchar *out_job_id;
  GVariantBuilder *builder;
  builder = g_variant_builder_new (G_VARIANT_TYPE ("a(ss)"));
  g_variant_builder_add (builder, "(ss)", "setting1_name", "setting1_value");
  g_variant_builder_add (builder, "(ss)", "setting2_name", "setting2_value");
  settings = g_variant_builder_end (builder);

  print_backend_call_print_file_sync (proxy, test_printer_id, file_path_name, num_settings, settings, &out_job_id, NULL, &error);
  g_print ("Out job id => %s\n", out_job_id);


  g_print ("\n\n*******Activate Backend*******\n\n");
  error = NULL;
  print_backend_call_activate_backend_sync (proxy, NULL, &error);


  g_print ("\n\n*******List of Print Jobs*******\n\n");
  error = NULL;
  GVariant *print_jobs_dict;
  gint num_jobs;
  print_backend_call_get_all_jobs_sync (proxy, FALSE, &num_jobs, &print_jobs_dict, NULL, &error);


  GVariantIter *iter;
  g_variant_get(print_jobs_dict, "a(ssssssi)", &iter);
  int size;
  char  *jobid, *title, *printer, *user, *state, *submit_time;
  for (int i = 0; i < num_jobs ; i++)
  {
      g_variant_iter_loop(iter, "(ssssssi)", &jobid, &title, &printer , &user, &state, &submit_time , &size);
      g_print ("%s\n",jobid);
      g_print ("%s\n",title);
      g_print ("%s\n",printer);
      g_print ("%s\n",user);
      g_print ("%s\n",state);
      g_print ("%s\n",submit_time);
      g_print ("%d\n",size);
  }

  g_variant_iter_free (iter);

  // gchar *job_id = g_malloc (50);
  gchar *printer_id = g_malloc (50);
  // g_print ("Enter id of the job that you want to cancel:\n");
  // scanf ("%s", job_id);
  // g_print ("Enter id of the printer:\n");
  // scanf ("%s", printer_id);
  // g_print ("\n\n*******Test Delete Print Job*******\n\n");
  // gboolean delete_job_status;
  // print_backend_call_cancel_job_sync (proxy, job_id, printer_id, &delete_job_status, NULL, &error);
  // g_print ("*******Delete Job Status*******\n");
  // g_print ("%d\n", delete_job_status);


  g_print ("Enter id of printer to get printer status:\n");
  scanf ("%s", printer_id);
  error = NULL;
  gchar *printer_status;
  print_backend_call_get_printer_state_sync (proxy, printer_id, &printer_status, NULL, &error);
  g_print ("*******Printer Status*******\n");
  g_print ("%s => %s\n\n", printer_id, printer_status);


  g_print ("Enter id of printer to check if printer is accepting jobs:\n");
  scanf ("%s", printer_id);
  error = NULL;
  gboolean is_accepting_jobs;
  print_backend_call_is_accepting_jobs_sync (proxy, printer_id, &is_accepting_jobs, NULL, &error);
  g_print ("*******Is Accepting Jobs*******\n");
  g_print ("%s => %d\n\n", printer_id, is_accepting_jobs);


  g_print ("Enter id of printer to get printer options:\n");
  scanf ("%s", printer_id);

  g_print ("\n\n*******List of Printer Options*******\n\n");
  error = NULL;
  GVariant *vendor_capability_dict;
  gint num_options;
  print_backend_call_get_all_options_sync (proxy, printer_id, &num_options, &vendor_capability_dict, NULL, &error);

  char *str;
  GVariantIter *array_iter;
  char *name, *default_val;
  int num_sup;
  g_variant_get(vendor_capability_dict, "a(ssia(s))", &iter);
  for (int i = 0; i < num_options; i++)
  {
      g_variant_iter_loop(iter, "(ssia(s))", &name, &default_val,
                          &num_sup, &array_iter);
      g_print ("%s\n", name);
      g_print ("%s\n", default_val);
      g_print ("%d\n", num_sup);
      for (int j = 0; j < num_sup; j++)
      {
          g_variant_iter_loop(array_iter, "(s)", &str);
          g_print ("value: %s\n", str);
      }
  }

  g_object_unref(proxy);
  return 0;
}
