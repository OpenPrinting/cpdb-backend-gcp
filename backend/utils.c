#include "utils.h"

gboolean make_api_request (RestProxy   *proxy,
                         RestProxyCall **call,
                         const gchar   *method,
                         const gchar   *function,
                         const gchar   *header,
                         const gchar   *header_value,
                         ...
                         )
{
  GError *error = NULL;
  va_list params;

  rest_proxy_call_set_method (*call, method);
  rest_proxy_call_set_function (*call, function);
  rest_proxy_call_add_header (*call, header, header_value);
  va_start (params, header_value);
  rest_proxy_call_add_params_from_valist (*call, params);
  va_end (params);

  gboolean res = rest_proxy_call_run (*call, NULL, &error);

  /* TODO: Error handling in case something goes wrong. */

  return res;
}

JsonObject *json_data_get_root (const gchar *data)
{
  JsonParser *parser = json_parser_new ();
  JsonNode *root;
  JsonObject *jobject;
  GError *error = NULL;

  json_parser_load_from_data (parser, data, -1, &error);
  if (error)
  {
    g_print ("Unable to parse json: %s\n", error->message);
    g_error_free (error);
    g_object_unref (parser);
    return NULL;
  }
  root = json_parser_get_root (parser);
  g_assert (root != NULL);
  jobject = json_node_get_object (root);
  return jobject;
}

JsonArray *get_array_from_json_object (JsonObject *jobject,
                                       const gchar *name)
{
  g_assert (json_object_has_member (jobject, name) == TRUE);
  JsonArray *jarr = json_object_get_array_member (jobject, name);
  g_assert (jarr != NULL);
  return jarr;
}

GList *get_glist_for_string_member_in_json_array (JsonArray *jarray,
                                                  const gchar *name)
{
  GList *glist = NULL;
  guint len = json_array_get_length (jarray);
  guint index;
  for (index = 0; index<len; ++index)
  {
    JsonNode *jnode = json_array_get_element (jarray, index);
    JsonObject *jobject = json_node_get_object (jnode);
    g_assert (json_object_has_member (jobject, name) == TRUE);
    const gchar *member = json_object_get_string_member (jobject, name);
    glist = g_list_append (glist, (gpointer *)member);
  }
  return glist;
}
