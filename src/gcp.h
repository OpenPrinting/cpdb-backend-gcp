#ifndef __GCP_H__
#define __GCP_H__

#include <glib.h>
#include <glib-object.h>
#include "common_helper.h"

G_BEGIN_DECLS

#define GCP_TYPE_OBJECT gcp_object_get_type()
G_DECLARE_DERIVABLE_TYPE (GCPObject, gcp_object, GCP, OBJECT, GObject)

struct _GCPObjectClass
{
  GObjectClass parent;

  const gchar * (*get_printers) (GCPObject *,
                                 const gchar *,
                                 const gchar *);

  const gchar * (*get_printer_options) (GCPObject *,
                                  const gchar *,
                                  const gchar *);

  const gchar * (*submit_print_job) (GCPObject *,
                                     const gchar *,
                                     const gchar *,
                                     const gchar *,
                                     const gchar *);

  const gchar * (*get_print_jobs) (GCPObject *,
                                   const gchar *,
                                   const gchar *,
                                   const gchar *,
                                   const gchar *,
                                   const gchar *);

  const gchar * (*cancel_print_job) (GCPObject *,
                                     const gchar *,
                                     const gchar *);
};

GCPObject *
gcp_object_new ();

GList *
gcp_object_get_printers (GCPObject *self,
                         const gchar *access_token,
                         const gchar *connection_status);


GList *
gcp_object_get_media_size_options (GCPObject   *self,
                                   const gchar *uid,
                                   const gchar *access_token);

GList *
gcp_object_get_vendor_capability_options (GCPObject   *self,
                                          const gchar *uid,
                                          const gchar *access_token);

const gchar *
gcp_object_submit_print_job (GCPObject   *self,
                             const gchar *uid,
                             const gchar *access_token,
                             const gchar *title,
                             const gchar *ticket);

GList *
gcp_object_get_print_jobs (GCPObject *self,
                           const gchar *access_token,
                           const gchar *uid,
                           const gchar *owner,
                           const gchar *status,
                           const gchar *sortorder);

gchar *
gcp_object_get_printer_state (GCPObject *self,
                              const gchar *access_token,
                              const gchar *uid);

gboolean
gcp_object_delete_print_job (GCPObject *self,
                             const gchar *access_token,
                             const gchar *job_id);

G_END_DECLS

#endif /* GCP_H */
