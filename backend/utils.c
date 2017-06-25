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

GHashTable *get_ghashtable_for_id_and_value_in_json_array (JsonArray *jarray,
                                                           const gchar *id,
                                                           const gchar *name)
{
  GHashTable *ghashtable = g_hash_table_new (NULL, NULL);
  GList *jnodes = json_array_get_elements (jarray);
  while (jnodes != NULL)
  {
    JsonObject *jobject = json_node_get_object (jnodes->data);
    g_assert (jobject != NULL);
    g_assert (json_object_has_member (jobject, id) == TRUE);
    g_assert (json_object_has_member (jobject, name) == TRUE);
    const gchar *id_ = json_object_get_string_member (jobject, id);
    const gchar *name_ = json_object_get_string_member (jobject, name);
    g_hash_table_insert (ghashtable, (gpointer *)id_, (gpointer *)name_);
    jnodes = jnodes->next;
  }
  return ghashtable;
}

GHashTable *get_vendor_capability_hashtable (JsonArray *jarray)
{
  GHashTable *ghashtable = g_hash_table_new (NULL, NULL);
  GList *vendor_capability_nodes = json_array_get_elements (jarray);
  while (vendor_capability_nodes != NULL)
  {
    JsonObject *jobject = json_node_get_object (vendor_capability_nodes->data);
    g_assert (jobject != NULL);
    g_assert (json_object_has_member (jobject, "id") == TRUE);
    g_assert (json_object_has_member (jobject, "select_cap") == TRUE);
    const gchar *id = json_object_get_string_member (jobject, "id");
    JsonNode *select_cap_node = json_object_get_member (jobject, "select_cap");
    JsonObject *select_cap_obj = json_node_get_object (select_cap_node);
    JsonArray *options = get_array_from_json_object (select_cap_obj, "option");

    GList *option_list = json_array_get_elements (options);
    GList *values = NULL;
    while (option_list != NULL)
    {
      JsonObject *option_obj = json_node_get_object (option_list->data);
      g_assert (option_obj != NULL);
      g_assert (json_object_has_member (option_obj, "value") == TRUE);
      const gchar *value = json_object_get_string_member (option_obj, "value");
      values = g_list_append (values, (gpointer *)value);
      option_list = option_list->next;
    }

    g_hash_table_insert (ghashtable, (gpointer *)id, (gpointer *)values);
    vendor_capability_nodes = vendor_capability_nodes->next;
  }
  return ghashtable;
}
