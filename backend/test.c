#include "gcp.h"

int
main (int argc, char **argv)
{
  GCPObject *gcp = gcp_object_new ();

  const gchar *access_token = "";
  const gchar *printers = gcp_object_get_printers (gcp, access_token);

  g_print ("%s\n", printers);

  g_object_unref (gcp);
  return 0;
}
