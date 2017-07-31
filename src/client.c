#include "common_helper.h"
#include <stdio.h>

static void
callback_get_printers_async(GObject *proxy,
                            GAsyncResult *res,
                            gpointer user_data);

int
main ()
{
  GError *error = NULL;
  // gchar *access_token = g_malloc (150);
  // gchar *connection_status = "ALL";
  // gint expires_in;
  // gboolean res = get_access_token (&access_token, &expires_in, &error);
  // g_assert (res == TRUE);
  // g_print ("%s\n", access_token);

  PrintBackend *proxy;
  GVariant *dict;

  error = NULL;
  proxy = print_backend_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, 0, "org.openprinting.Backend.GCP", "/", NULL, &error);

  error = NULL;
  //print_backend_call_get_printers (proxy, access_token, connection_status, NULL, callback_get_printers_async, &error);
  //print_backend_call_get_printers_sync (proxy, access_token, connection_status, &dict, NULL, &error);
  //g_assert (dict != NULL);


  // g_print ("\n\n*******List of Printers*******\n\n");
  //
  // GVariantIter *iter;
  // GVariant *variant;
  // gchar *key;
  //
  // g_variant_get (dict, "a{sv}", &iter);
  // while (g_variant_iter_loop (iter, "{sv}", &key, &variant))
  // {
  //   gchar *name;
  //   g_variant_get (variant, "(s)", &name);
  //   g_print ("%s => %s\n", key, name);
  // }
  // g_variant_iter_free (iter);


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

      //printf("Printer %s ; state %s \n",printer, state);
  }

  g_variant_iter_free (iter);


  gchar *printer_id = g_malloc (50);
  g_print ("Enter id of printer to get printer options:\n");
  scanf ("%s", printer_id);

  g_print ("\n\n*******List of Printer Options*******\n\n");
  error = NULL;
  // GVariant *media_options_dict, *vendor_capability_dict;
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
      //printf("i = %d\n", i);

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


  // g_print ("\n\n*******Media Size Options*******\n\n");
  // g_variant_get (media_options_dict, "a{sv}", &iter);
  // while (g_variant_iter_loop (iter, "{sv}", &key, &variant))
  // {
  //   gint height_microns, width_microns;
  //   gboolean is_continuous_feed, is_default;
  //   gchar *vendor_id, *name, *custom_display_name;
  //   g_variant_get (variant,
  //                  "(iibbsss)",
  //                  &height_microns,
  //                  &width_microns,
  //                  &is_continuous_feed,
  //                  &is_default,
  //                  &vendor_id,
  //                  &name,
  //                  &custom_display_name);
  //
  //   g_print ("printer_id : %s\n", key);
  //   g_print ("height_microns : %d\n", height_microns);
  //   g_print ("width_microns : %d\n", width_microns);
  //   g_print ("is_continuous_feed : %d\n", is_continuous_feed);
  //   g_print ("is_default : %d\n", is_default);
  //   g_print ("vendor_id : %s\n", vendor_id);
  //   g_print ("name : %s\n", name);
  //   g_print ("custom_display_name : %s\n\n", custom_display_name);
  // }
  // g_variant_iter_free (iter);

  g_object_unref(proxy);
  return 0;
}

// static void
// callback_get_printers_async(GObject *proxy,
//                             GAsyncResult *res,
//                             gpointer user_data)
// {
//     g_print("callback_get_printers_async called!\n");
//     GVariant *dict;
//     GError *error;
//     error = NULL;
//     print_backend_call_get_printers_finish(PRINT_BACKEND(proxy), &dict, res, &error);
//
//     if (error == NULL)
//     {
//       g_assert (dict != NULL);
//     }
//     else
//       g_print("%s\n", error->message);
// }
