#include "common_helper.h"

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

GList *get_printer_struct_from_json_array (JsonArray *jarray,
                                           const gchar *id,
                                           const gchar *name)
{
  GList *printer_structs_list = NULL;
  GList *jnodes = json_array_get_elements (jarray);
  while (jnodes != NULL)
  {
    JsonObject *jobject = json_node_get_object (jnodes->data);
    g_assert (jobject != NULL);
    g_assert (json_object_has_member (jobject, id) == TRUE);
    g_assert (json_object_has_member (jobject, name) == TRUE);
    const gchar *id_ = json_object_get_string_member (jobject, id);
    const gchar *name_ = json_object_get_string_member (jobject, name);

    printer *printer_struct = g_malloc(sizeof (printer));
    printer_struct->id = g_strdup(id_);
    printer_struct->name = g_strdup(name_);
    printer_structs_list = g_list_append (printer_structs_list, (gpointer)printer_struct);
    jnodes = jnodes->next;
  }
  return printer_structs_list;
}


gboolean get_access_token (gchar **out_access_token,
                           gint *out_expires_in,
                           GError **error)
{
  GoaClient *client = goa_client_new_sync (NULL, error);
  GList *accounts, *l;
  GoaOAuth2Based *oauth2;

  accounts = goa_client_get_accounts (client);
  for (l = accounts; l != NULL; l = l->next) {
    GoaObject *object = GOA_OBJECT (l->data);
    GoaAccount *account = goa_object_peek_account (object);

    if ((g_strcmp0 (goa_account_get_provider_type (account), "google") == 0) &&
         !goa_account_get_printers_disabled (account)) {
       oauth2 = goa_object_get_oauth2_based (object);
       break;
    }
  }

  if (!oauth2) {
    return FALSE;
  }

  *error = NULL;
  gboolean res = goa_oauth2_based_call_get_access_token_sync
                                    (oauth2,
                                     out_access_token,
                                     out_expires_in,
                                     NULL,
                                     error);
  return res;
}

GList *get_media_size_options (JsonArray *jarray)
{
  GList *media_size_options = NULL;
  GList *media_size_nodes = json_array_get_elements (jarray);
  while (media_size_nodes != NULL)
  {
    JsonObject *jobject = json_node_get_object (media_size_nodes->data);
    g_assert (jobject != NULL);
    g_assert (json_object_has_member (jobject, "height_microns") == TRUE);
    g_assert (json_object_has_member (jobject, "width_microns") == TRUE);
    g_assert (json_object_has_member (jobject, "is_continuous_feed") == TRUE);
    g_assert (json_object_has_member (jobject, "is_default") == TRUE);
    g_assert (json_object_has_member (jobject, "vendor_id") == TRUE);
    g_assert (json_object_has_member (jobject, "name") == TRUE);
    g_assert (json_object_has_member (jobject, "custom_display_name") == TRUE);

    gint64 height_microns = json_object_get_int_member (jobject, "height_microns");
    gint64 width_microns = json_object_get_int_member (jobject, "width_microns");
    gboolean is_continuous_feed = json_object_get_boolean_member (jobject, "is_continuous_feed");
    gboolean is_default = json_object_get_boolean_member (jobject, "is_default");
    const gchar *vendor_id = json_object_get_string_member (jobject, "vendor_id");
    const gchar *name = json_object_get_string_member (jobject, "name");
    const gchar *custom_display_name = json_object_get_string_member (jobject, "custom_display_name");

    media_size *media = g_malloc (sizeof (media_size));
    media->height_microns = height_microns;
    media->width_microns = width_microns;
    media->is_continuous_feed = is_continuous_feed;
    media->is_default = is_default;
    media->vendor_id = g_strdup (vendor_id);
    media->name = g_strdup (name);
    media->custom_display_name = g_strdup (custom_display_name);

    media_size_options = g_list_append (media_size_options, (gpointer) media);

    media_size_nodes = media_size_nodes->next;
  }
  return media_size_options;
}

GList *get_vendor_capability_options (JsonArray *jarray)
{
  GList *vendor_capability_list = NULL;
  GList *vendor_capability_nodes = json_array_get_elements (jarray);
  while (vendor_capability_nodes != NULL)
  {
    JsonObject *jobject = json_node_get_object (vendor_capability_nodes->data);
    g_assert (jobject != NULL);
    g_assert (json_object_has_member (jobject, "id") == TRUE);
    g_assert (json_object_has_member (jobject, "display_name") == TRUE);
    g_assert (json_object_has_member (jobject, "type") == TRUE);
    g_assert (json_object_has_member (jobject, "select_cap") == TRUE);

    // const gchar *id = json_object_get_string_member (jobject, "id");
    const gchar *display_name = json_object_get_string_member (jobject, "display_name");
    // const gchar *type = json_object_get_string_member (jobject, "type");
    JsonNode *select_cap_node = json_object_get_member (jobject, "select_cap");
    JsonObject *select_cap_obj = json_node_get_object (select_cap_node);
    JsonArray *options = get_array_from_json_object (select_cap_obj, "option");

    vendor_capability *capabilities = g_malloc (sizeof (vendor_capability));
    // capabilities->id = g_strdup (id);
    capabilities->display_name = g_strdup (display_name);
    capabilities->num_supported = 0;
    // capabilities->type = g_strdup (type);
    capabilities->options = NULL;

    GList *option_list = json_array_get_elements (options);
    GList *values = NULL;
    while (option_list != NULL)
    {
      capabilities->num_supported++;
      JsonObject *option_obj = json_node_get_object (option_list->data);
      g_assert (option_obj != NULL);
      g_assert (json_object_has_member (option_obj, "display_name") == TRUE);
      g_assert (json_object_has_member (option_obj, "is_default") == TRUE);
      g_assert (json_object_has_member (option_obj, "value") == TRUE);
      const gchar *option_display_name = json_object_get_string_member (option_obj, "display_name");
      gboolean option_is_default = json_object_get_boolean_member (option_obj, "is_default");
      const gchar *value = json_object_get_string_member (option_obj, "value");

      vendor_capability_option *vc_option = g_malloc (sizeof (vendor_capability_option));
      vc_option->display_name = g_strdup (option_display_name);
      vc_option->is_default = option_is_default;
      vc_option->value = g_strdup (value);

      capabilities->options = g_list_append (capabilities->options, (gpointer)vc_option);

      if(option_is_default)
        capabilities->default_value = g_strdup(option_display_name);

      option_list = option_list->next;
    }

    vendor_capability_list = g_list_append (vendor_capability_list, (gpointer)capabilities);
    vendor_capability_nodes = vendor_capability_nodes->next;
  }
  return vendor_capability_list;
}

GList *get_print_jobs_list (JsonArray *jarray)
{
  GList *print_jobs = json_array_get_elements (jarray);
  GList *print_jobs_list = NULL;
  while (print_jobs != NULL)
  {
    JsonObject *job_obj = json_node_get_object (print_jobs->data);
    g_assert (job_obj != NULL);
    g_assert (json_object_has_member (job_obj, "id") == TRUE);
    g_assert (json_object_has_member (job_obj, "title") == TRUE);
    g_assert (json_object_has_member (job_obj, "printerName") == TRUE);
    g_assert (json_object_has_member (job_obj, "ownerId") == TRUE);
    g_assert (json_object_has_member (job_obj, "status") == TRUE);
    g_assert (json_object_has_member (job_obj, "createTime") == TRUE);
    g_assert (json_object_has_member (job_obj, "numberOfPages") == TRUE);


    const gchar *id = json_object_get_string_member (job_obj, "id");
    const gchar *title = json_object_get_string_member (job_obj, "title");
    const gchar *printerName = json_object_get_string_member (job_obj, "printerName");
    const gchar *ownerId = json_object_get_string_member (job_obj, "ownerId");
    const gchar *status = json_object_get_string_member (job_obj, "status");
    const gchar *createTime = json_object_get_string_member (job_obj, "createTime");
    gint numberOfPages = json_object_get_int_member (job_obj, "numberOfPages");


    print_job *print_job_struct = g_malloc (sizeof (print_job));

    print_job_struct->id = g_strdup (id);
    print_job_struct->title = g_strdup (title);
    print_job_struct->printerName = g_strdup (printerName);
    print_job_struct->user = g_strdup (ownerId);
    print_job_struct->status = g_strdup (status);
    print_job_struct->submitted_at = g_strdup (createTime);
    print_job_struct->size = numberOfPages;


    print_jobs_list = g_list_append (print_jobs_list, (gpointer)print_job_struct);

    print_jobs = print_jobs->next;
  }
  return print_jobs_list;
}

void connect_to_dbus (GDBusConnection *connection,
                      PrintBackend *skeleton,
                      gchar *obj_path)
{
  GError *error = NULL;
  g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(skeleton),
                                   connection,
                                   obj_path,
                                   &error);
  g_assert_no_error(error);
}

gchar *get_printer_state_from_printers_array (JsonArray *jarray, const gchar *uid)
{
  GList *jnodes = json_array_get_elements (jarray);
  gchar *printer_state;
  while (jnodes != NULL)
  {
    JsonObject *jobject = json_node_get_object (jnodes->data);
    g_assert (jobject != NULL);
    g_assert (json_object_has_member (jobject, "id") == TRUE);
    g_assert (json_object_has_member (jobject, "connectionStatus") == TRUE);
    const gchar *id_ = json_object_get_string_member (jobject, "id");
    const gchar *connectionStatus = json_object_get_string_member (jobject, "connectionStatus");

    if(g_strcmp0 (id_, uid) == 0)
    {
      printer_state = g_strdup (connectionStatus);
      break;
    }
    jnodes = jnodes->next;
  }
  return printer_state;
}

gboolean get_print_job_cancellation_status (JsonObject *jobject)
{
  g_assert (json_object_has_member (jobject, "success") == TRUE);
  gboolean success_status = json_object_get_boolean_member (jobject, "success");
  return success_status;
}
