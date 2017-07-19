#include "utils.h"
#include <stdio.h>

static void
callback_get_printers_async(GObject *proxy,
                            GAsyncResult *res,
                            gpointer user_data);

int
main ()
{
  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gchar *connection_status = "ALL";
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);
  // g_print ("%s\n", access_token);

  PrintBackend *proxy;
  GVariant *dict;

  error = NULL;
  proxy = print_backend_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, 0, "org.openprinting.Backend.GCP", "/", NULL, &error);

  error = NULL;
  //print_backend_call_get_printers (proxy, access_token, connection_status, NULL, callback_get_printers_async, &error);
  print_backend_call_get_printers_sync (proxy, access_token, connection_status, &dict, NULL, &error);
  g_assert (dict != NULL);


  g_print ("\n\n*******List of Printers*******\n\n");

  GVariantIter *iter;
  GVariant *variant;
  gchar *key;

  g_variant_get (dict, "a{sv}", &iter);
  while (g_variant_iter_loop (iter, "{sv}", &key, &variant))
  {
    gchar *name;
    g_variant_get (variant, "(s)", &name);
    g_print ("%s => %s\n", key, name);
  }
  g_variant_iter_free (iter);


  g_print ("\n\n*******List of Print Jobs*******\n\n");
  error = NULL;
  GVariant *print_jobs_dict;
  print_backend_call_get_print_jobs_sync (proxy, access_token, "", "", "", "", &print_jobs_dict, NULL, &error);

  g_variant_get (print_jobs_dict, "a{sv}", &iter);
  while (g_variant_iter_loop (iter, "{sv}", &key, &variant))
  {
    gchar *title, *printerid, *printerName, *status;
    g_variant_get (variant, "(ssss)", &title, &printerid, &printerName, &status);
    g_print ("id : %s\n", key);
    g_print ("title : %s\n", title);
    g_print ("printerid : %s\n", printerid);
    g_print ("printerName : %s\n", printerName);
    g_print ("status : %s\n\n", status);
  }
  g_variant_iter_free (iter);


  gchar *uid = g_malloc (50);
  g_print ("Enter uid of printer to get printer options:\n");
  scanf ("%s", uid);

  g_print ("\n\n*******List of Printer Options*******\n\n");
  error = NULL;
  GVariant *media_options_dict, *vendor_capability_dict;
  print_backend_call_get_printer_options_sync (proxy, uid, access_token, &media_options_dict, &vendor_capability_dict, NULL, &error);

  g_print ("\n\n*******Media Size Options*******\n\n");
  g_variant_get (media_options_dict, "a{sv}", &iter);
  while (g_variant_iter_loop (iter, "{sv}", &key, &variant))
  {
    gint height_microns, width_microns;
    gboolean is_continuous_feed, is_default;
    gchar *vendor_id, *name, *custom_display_name;
    g_variant_get (variant,
                   "(iibbsss)",
                   &height_microns,
                   &width_microns,
                   &is_continuous_feed,
                   &is_default,
                   &vendor_id,
                   &name,
                   &custom_display_name);

    g_print ("printer_id : %s\n", key);
    g_print ("height_microns : %d\n", height_microns);
    g_print ("width_microns : %d\n", width_microns);
    g_print ("is_continuous_feed : %d\n", is_continuous_feed);
    g_print ("is_default : %d\n", is_default);
    g_print ("vendor_id : %s\n", vendor_id);
    g_print ("name : %s\n", name);
    g_print ("custom_display_name : %s\n\n", custom_display_name);
  }
  g_variant_iter_free (iter);

  g_object_unref(proxy);
  return 0;
}

static void
callback_get_printers_async(GObject *proxy,
                            GAsyncResult *res,
                            gpointer user_data)
{
    g_print("callback_get_printers_async called!\n");
    GVariant *dict;
    GError *error;
    error = NULL;
    print_backend_call_get_printers_finish(PRINT_BACKEND(proxy), &dict, res, &error);

    if (error == NULL)
    {
      g_assert (dict != NULL);
    }
    else
      g_print("%s\n", error->message);
}
