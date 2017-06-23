#ifndef UTILS_H
#define UTILS_H

#include <rest/rest-proxy.h>
#include <rest/rest-proxy-call.h>
#include <json-glib/json-glib.h>

gboolean make_api_request (RestProxy   *proxy,
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

#endif /* UTILS_H */
