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
