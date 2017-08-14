#include "gcp.h"

G_DEFINE_TYPE (GCPObject, gcp_object, G_TYPE_OBJECT)

/*****************************************************************************/
const gchar *
gcp_object_real_get_printers (GCPObject *self,
                              const gchar *access_token,
                              const gchar *connection_status);

const gchar *
gcp_object_real_get_printer_options (GCPObject   *self,
                                     const gchar *uid,
                                     const gchar *access_token);

const gchar *
gcp_object_real_submit_print_job (GCPObject   *self,
                                  const gchar *uid,
                                  const gchar *access_token,
                                  const gchar *title,
                                  const gchar *file_path_name,
                                  const gchar *content_type,
                                  const gchar *ticket);

const gchar *
gcp_object_real_get_print_jobs (GCPObject *self,
                                const gchar *access_token,
                                const gchar *uid,
                                const gchar *owner,
                                const gchar *status,
                                const gchar *sortorder);

const gchar *
gcp_object_real_cancel_print_job (GCPObject *self,
                                  const gchar *access_token,
                                  const gchar *job_id);

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
  klass->get_print_jobs = gcp_object_real_get_print_jobs;
  klass->cancel_print_job = gcp_object_real_cancel_print_job;
}

GList *
gcp_object_get_printers (GCPObject *self, const gchar *access_token, const gchar *connection_status)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), NULL);

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);
  const gchar *printers = klass->get_printers (self, access_token, connection_status);

  JsonObject *jobject = json_data_get_root (printers);
  JsonArray *jarray = get_array_from_json_object (jobject, "printers");
  GList *printer_struct_list = get_printer_struct_from_json_array (jarray, "id", "displayName", "description");

  return printer_struct_list;
}

GList *
gcp_object_get_media_size_options (GCPObject   *self,
                                   const gchar *uid,
                                   const gchar *access_token)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), NULL);

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);
  const gchar *printer_options_json = klass->get_printer_options (self, uid, access_token);
  JsonObject *root = json_data_get_root (printer_options_json);
  JsonArray *jarray = get_array_from_json_object (root, "printers");
  JsonNode *jnode = json_array_get_element (jarray, 0);
  JsonObject *jobject = json_node_get_object (jnode);
  JsonNode *capabilities_node = json_object_get_member (jobject, "capabilities");
  JsonObject *capabilities_obj = json_node_get_object (capabilities_node);
  JsonNode *printer_node = json_object_get_member (capabilities_obj, "printer");
  JsonObject *printer_options = json_node_get_object (printer_node);

  JsonNode *media_size_node = json_object_get_member (printer_options, "media_size");
  JsonObject *media_size_object = json_node_get_object (media_size_node);
  JsonArray *media_options_array = get_array_from_json_object (media_size_object, "option");

  GList *media_size_options_list = get_media_size_options (media_options_array);

  return media_size_options_list;
}

GList *
gcp_object_get_vendor_capability_options (GCPObject   *self,
                                          const gchar *uid,
                                          const gchar *access_token)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), NULL);

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);
  const gchar *printer_options_json = klass->get_printer_options (self, uid, access_token);
  JsonObject *root = json_data_get_root (printer_options_json);
  JsonArray *jarray = get_array_from_json_object (root, "printers");
  JsonNode *jnode = json_array_get_element (jarray, 0);
  JsonObject *jobject = json_node_get_object (jnode);
  JsonNode *capabilities_node = json_object_get_member (jobject, "capabilities");
  JsonObject *capabilities_obj = json_node_get_object (capabilities_node);
  JsonNode *printer_node = json_object_get_member (capabilities_obj, "printer");
  JsonObject *printer_options = json_node_get_object (printer_node);
  JsonArray *vendor_capability_array = get_array_from_json_object (printer_options, "vendor_capability");

  GList *vendor_capability_list = get_vendor_capability_options (vendor_capability_array);

  return vendor_capability_list;
}

const gchar *
gcp_object_submit_print_job (GCPObject   *self,
                             const gchar *uid,
                             const gchar *access_token,
                             const gchar *title,
                             const gchar *file_path_name,
                             const gchar *content_type,
                             const gchar *ticket)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), FALSE);

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);

  const gchar *res = klass->submit_print_job (self, uid, access_token, title, file_path_name, content_type, ticket);
  JsonObject *root = json_data_get_root (res);
  const gchar *job_id = get_job_id_for_submitted_job (root);

  return job_id;
}

GList *
gcp_object_get_print_jobs (GCPObject *self,
                           const gchar *access_token,
                           const gchar *uid,
                           const gchar *owner,
                           const gchar *status,
                           const gchar *sortorder)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), NULL);

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);
  const gchar *printer_jobs = klass->get_print_jobs (self,
                                                     access_token,
                                                     uid,
                                                     owner,
                                                     status,
                                                     sortorder);

  JsonObject *root = json_data_get_root (printer_jobs);
  JsonArray *jarray = get_array_from_json_object (root, "jobs");
  GList *jobs_list = get_print_jobs_list (jarray);

  return jobs_list;
}

/*****************************************************************************/

const gchar *
gcp_object_real_get_printers (GCPObject *self,
                              const gchar *access_token,
                              const gchar *connection_status)
{
  RestProxy *proxy;
  RestProxyCall *call;

  const gchar *header = "X-CloudPrint-Proxy";
  const gchar *header_value = "Common Printing Dialog";
  const gchar *method = "GET";
  const gchar *function = "search";
  const gchar *param1_name = "access_token";

  gboolean res = FALSE;

  proxy = rest_proxy_new ("https://www.google.com/cloudprint/", FALSE);
  call = rest_proxy_new_call (proxy);
  if (connection_status != NULL)
    res = make_api_request (proxy, &call, method, function, header, header_value, param1_name, access_token, "connection_status", connection_status, NULL);
  else
    res = make_api_request (proxy, &call, method, function, header, header_value, param1_name, access_token, NULL);

  const gchar *printers;

  if (res)
  {
    printers = rest_proxy_call_get_payload (call);
  }
  else
  {
    printers = g_strdup("API request failed!");
  }

  /* TODO: Error handling in case something goes wrong. */

  return printers;
}

const gchar *
gcp_object_real_get_printer_options (GCPObject   *self,
                                     const gchar *uid,
                                     const gchar *access_token)
{
  RestProxy *proxy;
  RestProxyCall *call;

  const gchar *header = "X-CloudPrint-Proxy";
  const gchar *header_value = "Common Printing Dialog";
  const gchar *method = "GET";
  const gchar *function = "printer";
  const gchar *param1_name = "access_token";
  const gchar *param2_name = "printerid";

  gboolean res = FALSE;

  proxy = rest_proxy_new ("https://www.google.com/cloudprint/", FALSE);
  call = rest_proxy_new_call (proxy);
  res = make_api_request (proxy, &call, method, function, header, header_value, param1_name, access_token, param2_name, uid, NULL);

  const gchar *printer_options;

  if (res)
  {
    printer_options = rest_proxy_call_get_payload (call);
  }
  else
  {
    printer_options = g_strdup("API request failed!");
  }

  /* TODO: Error handling in case something goes wrong. */

  return printer_options;
}

const gchar *
gcp_object_real_submit_print_job (GCPObject   *self,
                                  const gchar *uid,
                                  const gchar *access_token,
                                  const gchar *title,
                                  const gchar *file_path_name,
                                  const gchar *content_type,
                                  const gchar *ticket)
{
  /* TODO: Make api request to submit a file for printing. */
  RestProxy *proxy;
  RestProxyCall *call;

  const gchar *header = "Authorization";
  const gchar *header_value = "Common Printing Dialog";
  const gchar *method = "GET";
  const gchar *function = "submit";
  const gchar *param_access_token = "access_token";
  const gchar *param_printer_id = "printerid";
  const gchar *param_title = "title";
  const gchar *param_ticket = "ticket";
  const gchar *param_content = "content";
  const gchar *param_content_type = "contentType";

  gboolean res = FALSE;
  FILE *fp;
  fp=fopen(file_path_name, "r");

  proxy = rest_proxy_new ("https://www.google.com/cloudprint/", FALSE);
  call = rest_proxy_new_call (proxy);
  res = make_api_request (proxy,
                          &call,
                          method,
                          function,
                          header, header_value,
                          param_access_token, access_token,
                          param_printer_id, uid,
                          param_title, title,
                          param_ticket, ticket,
                          param_content, fp,
                          param_content_type, content_type,
                          NULL);

  const gchar *submit_print_job_status;

  if (res)
  {
    submit_print_job_status = rest_proxy_call_get_payload (call);
  }
  else
  {
    submit_print_job_status = g_strdup("API request failed!");
  }

  /* TODO: Error handling in case something goes wrong. */

  return submit_print_job_status;
}

const gchar *
gcp_object_real_get_print_jobs (GCPObject *self,
                                const gchar *access_token,
                                const gchar *uid,
                                const gchar *owner,
                                const gchar *status,
                                const gchar *sortorder)
{
  RestProxy *proxy;
  RestProxyCall *call;

  const gchar *header = "X-CloudPrint-Proxy";
  const gchar *header_value = "Common Printing Dialog";
  const gchar *method = "GET";
  const gchar *function = "jobs";
  const gchar *param_access_token = "access_token";
  const gchar *param_printer_id = "printerid";
  const gchar *param_owner = "owner";
  const gchar *param_status = "status";
  const gchar *param_sortorder = "sortorder";

  gboolean res = FALSE;

  proxy = rest_proxy_new ("https://www.google.com/cloudprint/", FALSE);
  call = rest_proxy_new_call (proxy);
  res = make_api_request (proxy,
                          &call,
                          method,
                          function,
                          header, header_value,
                          param_access_token, access_token,
                          param_printer_id, uid,
                          param_owner, owner,
                          param_status, status,
                          param_sortorder, sortorder,
                          NULL);

  const gchar *printer_jobs;

  if (res)
  {
    printer_jobs = rest_proxy_call_get_payload (call);
  }
  else
  {
    printer_jobs = g_strdup("API request failed!");
  }

  /* TODO: Error handling in case something goes wrong. */

  return printer_jobs;
}

const gchar *
gcp_object_real_cancel_print_job (GCPObject *self,
                                  const gchar *access_token,
                                  const gchar *job_id)
{
  RestProxy *proxy;
  RestProxyCall *call;

  const gchar *header = "Authorization";
  const gchar *header_value = "Common Printing Dialog";
  const gchar *method = "GET";
  const gchar *function = "deletejob";
  const gchar *param_access_token = "access_token";
  const gchar *param_job_id = "jobid";

  gboolean res = FALSE;

  proxy = rest_proxy_new ("https://www.google.com/cloudprint/", FALSE);
  call = rest_proxy_new_call (proxy);
  res = make_api_request (proxy,
                          &call,
                          method,
                          function,
                          header, header_value,
                          param_access_token, access_token,
                          param_job_id, job_id,
                          NULL);

  const gchar *delete_job_status;

  if (res)
  {
    delete_job_status = rest_proxy_call_get_payload (call);
  }
  else
  {
    delete_job_status = g_strdup("API request failed!");
  }

  /* TODO: Error handling in case something goes wrong. */

  return delete_job_status;
}

gchar *
gcp_object_get_printer_state (GCPObject *self,
                              const gchar *access_token,
                              const gchar *uid)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), NULL);

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);
  const gchar *printers = klass->get_printers (self, access_token, "ALL");

  JsonObject *jobject = json_data_get_root (printers);
  JsonArray *jarray = get_array_from_json_object (jobject, "printers");
  gchar *printer_state = get_printer_state_from_printers_array (jarray, uid);

  return printer_state;
}

gboolean
gcp_object_delete_print_job (GCPObject *self,
                             const gchar *access_token,
                             const gchar *job_id)
{
  g_return_val_if_fail (GCP_IS_OBJECT (self), FALSE);

  GCPObjectClass *klass = GCP_OBJECT_GET_CLASS (self);
  const gchar *is_cancelled = klass->cancel_print_job (self, access_token, job_id);

  JsonObject *jobject = json_data_get_root (is_cancelled);
  gboolean job_cancel_status = get_print_job_cancellation_status (jobject);

  return job_cancel_status;
}
