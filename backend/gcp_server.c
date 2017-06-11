#include<gio/gio.h>
#include<gio/gunixfdlist.h>
#include<stdlib.h>

static GDBusNodeInfo *introspection_data = NULL;

static void
handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer              user_data)
{
  if( g_strcmp0 (method_name, "GetPrinters") == 0 )
    {
      /*
      *  TODO: call the utility method from gcp.h to fetch the list
      *        of printers associated with user's google account.
      */
    }
  if( g_strcmp0 (method_name, "GetPrinterOptions") == 0 )
    {
      /*
      *  TODO: call the utility method from gcp.h to fetch the details about
      *        a particular printer.
      */
    }
  if( g_strcmp0 (method_name, "PrintFile") == 0 )
    {
      /* TODO: call the utility method from gcp.h along with printing
      *        specific parameters to print a file on a particular google
      *        cloud printer associated with user's google account.
      */
    }
}

static const GDBusInterfaceVTable interface_vtable =
  {
    handle_method_call,
    NULL,
    NULL
  };

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer        user_data)
{
  guint registration_id = g_dbus_connection_register_object(connection,
                                                            "/org/openprinting/GCPBackend",
                                                            introspection_data->interfaces[0],
                                                            &interface_vtable,
                                                            NULL,
                                                            NULL,
                                                            NULL);
  g_assert( registration_id > 0 );
}

static void
on_name_lost ( GDBusConnection *connection,
               const gchar     *name,
               gpointer        user_data)
{
  exit(1);
}

int
main( int argc, char **argv )
{
  GDBusConnection *connection;
  GError *error = NULL;
  GMainLoop *loop;
  gchar *contents;
  guint owner_id;
  guint subscription_id;

  connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
  g_assert_no_error(error);
  g_assert(connection != NULL);

  error = NULL;

  g_file_get_contents("./org.openprinting.GCPBackend.xml", &contents, NULL, &error);
  g_assert_no_error(error);
  g_assert(contents != NULL);

  introspection_data = g_dbus_node_info_new_for_xml(contents, NULL);
  g_assert(introspection_data != NULL);
  g_free(contents);

  owner_id = g_bus_own_name_on_connection (connection,
                                           "org.openprinting.GCPBackend",
                                           G_BUS_NAME_OWNER_FLAGS_NONE,
                                           on_name_acquired,
                                           on_name_lost,
                                           NULL,
                                           NULL);

  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  g_bus_unown_name(owner_id);
  g_dbus_node_info_unref(introspection_data);
  g_object_unref(connection);
  return 0;
}
