#ifndef GCP_H
#define GCP_H

#include <glib-object.h>

G_BEGIN_DECLS

#define GOOGLECLOUDPRINT_TYPE_GCP (gcp_object_get_type())

G_DECLARE_FINAL_TYPE (GCPObject, gcp_object, GOOGLECLOUDPRINT, GCP, GObject)

const gchar *gcp_object_get_auth_url (GCPObject *self);
const gchar *gcp_object_get_api_url (GCPObject *self);
const gchar *gcp_object_get_scope (GCPObject *self);
const gchar *gcp_object_get_redirect_uri (GCPObject *self);

void gcp_object_set_auth_url (GCPObject *self, const gchar *auth_url);
void gcp_object_set_api_url (GCPObject *self, const gchar *api_url);
void gcp_object_set_scope (GCPObject *self, const gchar *scope);
void gcp_object_set_redirect_uri (GCPObject *self, const gchar *redirect_uri);

gchar *gcp_object_get_printers (GCPObject *self, const gchar *access_token);

gchar *gcp_object_get_printer_options (GCPObject   *self,
                                       const gchar *uid,
                                       const gchar *access_token);

gboolean gcp_object_submit_print_job (GCPObject   *self,
                                      const gchar *uid,
                                      const gchar *access_token,
                                      const gchar *title,
                                      const gchar *ticket);

G_END_DECLS

#endif /* GCP_H */
