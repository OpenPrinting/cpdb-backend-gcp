#include "gcp.h"
#include "utils.h"

G_DEFINE_TYPE (GCPObject, gcp_object, G_TYPE_OBJECT)

/*****************************************************************************/
gchar *
gcp_object_real_get_printers (GCPObject *self,
                              const gchar *access_token);

gchar *
gcp_object_real_get_printer_options (GCPObject   *self,
                                     const gchar *uid,
                                     const gchar *access_token);

gboolean
gcp_object_real_submit_print_job (GCPObject   *self,
                                  const gchar *uid,
                                  const gchar *access_token,
                                  const gchar *title,
                                  const gchar *ticket);
/*****************************************************************************/

GCPObject *
gcp_object_new()
{
  return g_object_new (GCP_TYPE_OBJECT, NULL);
}

static void
gcp_object_init (GCPObject *self)
{
}

static void
gcp_object_class_init (GCPObjectClass *klass)
{
  GObjectClass *parent_class = G_OBJECT_CLASS (klass);

  klass->get_printers = gcp_object_real_get_printers;
  klass->get_printer_options = gcp_object_real_get_printer_options;
  klass->submit_print_job = gcp_object_real_submit_print_job;
}

gchar *
gcp_object_get_printers (GCPObject *self, const gchar *access_token)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), g_strdup ("Type Error"));

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);
  gchar *printers = klass->get_printers (self, access_token);

  return printers;
}

gchar *
gcp_object_get_printer_options (GCPObject   *self,
                                const gchar *uid,
                                const gchar *access_token)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), g_strdup ("Type Error"));

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);
  gchar *printer_options = klass->get_printer_options (self, uid, access_token);

  return printer_options;
}

gboolean
gcp_object_submit_print_job (GCPObject   *self,
                             const gchar *uid,
                             const gchar *access_token,
                             const gchar *title,
                             const gchar *ticket)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), FALSE);

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);
  gboolean res = FALSE;
  res = klass->submit_print_job (self, uid, access_token, title, ticket);

  return res;
}

/*****************************************************************************/

gchar *
gcp_object_real_get_printers (GCPObject *self,
                              const gchar *access_token)
{
  gchar *url = "https://www.google.com/cloudprint/search?access_token=";
  url = g_strconcat (url, access_token, NULL);
  gchar *header = "X-CloudPrint-Proxy: Common Printing Dialog";

  gchar *printers = make_curl_request (url, header);

  /* TODO: Error handling in case something goes wrong. */

  return printers;
}

gchar *
gcp_object_real_get_printer_options (GCPObject   *self,
                                     const gchar *uid,
                                     const gchar *access_token)
{
  gchar *url = "https://www.google.com/cloudprint/printer?access_token=";
  url = g_strconcat (url, access_token, "&printerid=", uid, NULL);
  gchar *header = "X-CloudPrint-Proxy: Common Printing Dialog";

  gchar *printer_options = make_curl_request (url, header);

  /* TODO: Error handling in case something goes wrong. */

  return printer_options;
}

gboolean
gcp_object_real_submit_print_job (GCPObject   *self,
                                  const gchar *uid,
                                  const gchar *access_token,
                                  const gchar *title,
                                  const gchar *ticket)
{
  /* TODO: Make api request to submit a file for printing. */
  return TRUE;
}
