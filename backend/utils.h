#ifndef UTILS_H
#define UTILS_H

#define GOA_API_IS_SUBJECT_TO_CHANGE

#include <rest/rest-proxy.h>
#include <rest/rest-proxy-call.h>
#include <json-glib/json-glib.h>
#include <goa/goa.h>

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
  gchar *id;
  gchar *display_name;
  gchar *type;
  GList *options;
} vendor_capability;

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


GHashTable *get_ghashtable_for_id_and_value_in_json_array (JsonArray *jarray,
                                                           const gchar *id,
                                                           const gchar *name);

gboolean get_access_token (gchar **out_access_token,
                           gint *out_expires_in,
                           GError **error);

GList *get_media_size_options (JsonArray *jarray);

GList *get_vendor_capability_options (JsonArray *array);

#endif /* UTILS_H */
