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
  // GVariant *retval;

  error = NULL;
  proxy = print_backend_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, 0, "org.openprinting.Backend.GCP", "/", NULL, &error);

  error = NULL;
  print_backend_call_get_printers (proxy, access_token, connection_status, NULL, callback_get_printers_async, &error);
  // print_backend_call_get_printers_sync (proxy, access_token, connection_status, &retval, NULL, &error);

  g_object_unref(proxy);
  return 0;
}

static void
callback_get_printers_async(GObject *proxy,
                            GAsyncResult *res,
                            gpointer user_data)
{
    g_print("callback_get_printers_async called!\n");
    GVariant *dict = g_variant_new ("a{sv}");
    GError *error;
    error = NULL;
    print_backend_call_get_printers_finish(PRINT_BACKEND(proxy), &dict, res, &error);
    // GVariant *val = g_variant_lookup_value (dict, "0", NULL);
    // g_assert (val != NULL);
    // const gchar *id, *name;
    // g_variant_get (val, "(ss)", &id, &name);
    // g_print ("%s %s\n", id, name);
    // g_print ("hello\n");


    if (error == NULL)
    {
      g_assert (dict != NULL);
    }
    else
      g_print("%s\n", error->message);
}
