#include<gio/gio.h>
#include<stdlib.h>

int
main(void)
{
  GDBusProxy *proxy;
  GDBusConnectin *connection;
  GError *error;

  connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
  g_assert_no_error(error);
  g_assert(connection != NULL);

  error = NULL;

  proxy = g_dbus_proxy_new_sync(connection,
                                G_DBUS_PROXY_FLAGS_NONE,
                                NULL,
                                "org.openprinting.GCPBackend",
                                "/org/openprinting/GCPBackend",
                                "org.openprinting.GCPBackend",
                                NULL,
                                &error);

  g_assert_no_error(error);

  /* TODO: Required functions defined in gcp interface will be called from here.
   *       These are the functions which are defined in gcp_server.c
   *       The CPD frontend will make use of this gcp_client.c to call the
   *       desired functions as per user needs. This part will be updated
   *       in collaboration with CPD frontend team once the CPD frontend
   *       design is final.
   */

  g_object_unref(proxy);
  g_object_unref(connection);
  return 0;
}
