#ifndef UTILS_H
#define UTILS_H

#include <rest/rest-proxy.h>
#include <rest/rest-proxy-call.h>

gboolean make_api_request (RestProxy   *proxy,
                         RestProxyCall **call,
                         const gchar   *method,
                         const gchar   *function,
                         const gchar   *header,
                         const gchar   *header_value,
                         ...
                         );

#endif /* UTILS_H */
