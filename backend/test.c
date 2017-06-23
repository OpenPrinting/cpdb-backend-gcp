#include "gcp.h"

int
main (int argc, char **argv)
{
  GCPObject *gcp = gcp_object_new ();

  const gchar *access_token = "";
  // GList *printers = gcp_object_get_printers (gcp, access_token);

  GHashTableIter iter;
  gpointer key, value;
  // give "ALL" as third param to get a list of all printers intead of only active printers
  GHashTable *printers = gcp_object_get_printers (gcp, access_token, NULL);


  // while(printers != NULL)
  // {
    // g_print ("%s\n", (gchar *)printers->data);
    // printers = printers->next;
  // }


  g_hash_table_iter_init (&iter, printers);
  while (g_hash_table_iter_next (&iter, &key, &value))
  {
      g_print ("%s => %s\n", (gchar *)key, (gchar *)value);
  }

  // const gchar *uid = "__google__docs";
  // const gchar *printer_options = gcp_object_get_printer_options (gcp, uid, access_token);

  // g_print ("%s\n", printers);
  // g_print ("%s\n", printer_options);

  g_object_unref (gcp);
  return 0;
}
