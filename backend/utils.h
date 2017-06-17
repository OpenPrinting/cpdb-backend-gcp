#ifndef UTILS_H
#define UTILS_H

#include <glib.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>


struct string
{
  gchar *ptr;
  size_t len;
};

void init_string (struct string *s);
size_t writefunc (void *ptr, size_t size, size_t nmemb, struct string *s);
gchar *make_curl_request (gchar *url, gchar *header);

#endif /* UTILS_H */
