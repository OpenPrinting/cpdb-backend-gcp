#include "utils.h"

void init_string (struct string *s)
{
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc (void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

gchar *make_curl_request (gchar *url, gchar *header)
{
  CURL *curl;
  CURLcode res;

  curl_global_init (CURL_GLOBAL_ALL);

  curl = curl_easy_init ();
  if (curl)
  {
    struct string s;
    init_string (&s);

    if (header != NULL)
    {
      struct curl_slist *chunk = curl_slist_append (chunk, header);
      curl_easy_setopt (curl, CURLOPT_HTTPHEADER, chunk);
    }

    curl_easy_setopt (curl, CURLOPT_URL, url);
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, &s);

    res = curl_easy_perform (curl);
    if (res != CURLE_OK)
    {
      gchar *error = g_strdup ("Error: curl_easy_perform() failed");
      error = g_strconcat(error, curl_easy_strerror (res), NULL);
      return error;
    }
    return s.ptr;
  }
}
