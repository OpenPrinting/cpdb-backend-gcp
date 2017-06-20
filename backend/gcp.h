#ifndef GCP_NEW_H
#define GCP_NEW_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define GCP_TYPE_OBJECT gcp_object_get_type()
G_DECLARE_DERIVABLE_TYPE (GCPObject, gcp_object, GCP, OBJECT, GObject)

struct _GCPObjectClass
{
  GObjectClass parent;

  const gchar * (*get_printers) (GCPObject *, const gchar *);

  const gchar * (*get_printer_options) (GCPObject *,
                                  const gchar *,
                                  const gchar *);

  gboolean (*submit_print_job) (GCPObject *,
                                const gchar *,
                                const gchar *,
                                const gchar *,
                                const gchar *);
};

GCPObject *
gcp_object_new ();

const gchar *
gcp_object_get_printers (GCPObject *self, const gchar *access_token);

const gchar *
gcp_object_get_printer_options (GCPObject   *self,
                                const gchar *uid,
                                const gchar *access_token);

gboolean
gcp_object_submit_print_job (GCPObject   *self,
                             const gchar *uid,
                             const gchar *access_token,
                             const gchar *title,
                             const gchar *ticket);

G_END_DECLS

#endif
