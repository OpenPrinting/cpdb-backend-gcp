#ifndef COMMON_HELPER_H
#define COMMON_HELPER_H

#define GOA_API_IS_SUBJECT_TO_CHANGE

#include <rest/rest-proxy.h>
#include <rest/rest-proxy-call.h>
#include <json-glib/json-glib.h>
#include <goa/goa.h>
#include <string.h>
#include <stdio.h>
#include "backend_interface.h"

#define PRINTER_ADDED_ARGS "(sssssbss)"
#define PRINTER_ADDED_SIGNAL "PrinterAdded"
#define REFRESH_BACKEND_SIGNAL "RefreshBackend"
#define PRINTER_REMOVED_SIGNAL "PrinterRemoved"
#define STOP_BACKEND_SIGNAL "StopListing"

typedef struct media_size{
  gint height_microns;
  gint width_microns;
  gboolean is_continuous_feed;
  gboolean is_default;
  gchar *vendor_id;
  gchar *name;
  gchar *custom_display_name;
} media_size;

typedef struct vendor_capability_option{
  gchar *display_name;
  gboolean is_default;
  gchar *value;
} vendor_capability_option;

typedef struct vendor_capability{
  gchar *display_name;
  gchar *default_value;
  gint num_supported;
  GList *options;
} vendor_capability;

typedef struct print_job{
  gchar *id;
  gchar *title;
  gchar *printerName;
  gchar *user;
  gchar *status;
  gchar *submitted_at;
  int size;
} print_job;

typedef struct printer{
  gchar *id;
  gchar *name;
  gchar *description;
  gchar *location;
  gchar *make_and_model;
} printer;

gboolean make_api_request (RestProxy     *proxy,
                           RestProxyCall **call,
                           const gchar   *method,
                           const gchar   *function,
                           const gchar   *header,
                           const gchar   *header_value,
                           ...
                         );

JsonObject *json_data_get_root (const gchar *data);

JsonArray *get_array_from_json_object (JsonObject *jobject,
                                       const gchar *name);

GList *get_glist_for_string_member_in_json_array (JsonArray *jarray,
                                                  const gchar *name);

gboolean get_access_token (gchar **out_access_token,
                           gint *out_expires_in,
                           GError **error);

GList *get_printer_struct_from_json_array (JsonArray *jarray,
                                          const gchar *id,
                                          const gchar *name,
                                          const gchar *description);

GList *get_media_size_options (JsonArray *jarray);

GList *get_vendor_capability_options (JsonArray *array);

GList *get_google_docs_vendor_capability_options (JsonArray *jarray);

GList *get_print_jobs_list (JsonArray *jarray);

gchar *get_printer_state_from_printers_array (JsonArray *jarray, const gchar *uid);

gboolean get_print_job_cancellation_status (JsonObject *jobject);

const gchar *get_job_id_for_submitted_job (JsonObject *jobject);

void connect_to_dbus (GDBusConnection *connection,
                      PrintBackend *skeleton,
                      gchar *obj_path);

#endif /* COMMON_HELPER_H */
