#include "gcp.h"

struct _GCPObject
{
  GObject parent_instance;

  gchar *auth_url;
  gchar *api_url;
  gchar *scope;
  gchar *redirect_uri;
};

G_DEFINE_TYPE (GCPObject, gcp_object, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_AUTH_URL,
  PROP_API_URL,
  PROP_SCOPE,
  PROP_REDIRECT_URI,
  LAST_PROP
};

static GParamSpec *properties [LAST_PROP];

static void
gcp_object_get_property (GObject    *object,
                         guint      prop_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
  GCPObject *self = (GCPObject *)object;

  switch (prop_id)
    {
    case PROP_AUTH_URL:
        g_value_set_string (value, gcp_object_get_auth_url(self));
        break;
    case PROP_API_URL:
        g_value_set_string (value, gcp_object_get_api_url(self));
        break;
    case PROP_SCOPE:
        g_value_set_string (value, gcp_object_get_scope(self));
        break;
    case PROP_REDIRECT_URI:
        g_value_set_string (value, gcp_object_get_redirect_uri(self));
        break;
    }
}

static void
gcp_object_set_property (GObject          *object,
                         guint            prop_id,
                         const GValue     *value,
                         GParamSpec       *pspec)
{
  GCPObject *self = (GCPObject *)object;

  switch (prop_id)
    {
    case PROP_AUTH_URL:
        gcp_object_set_auth_url(self, g_value_get_string (value));
        break;
    case PROP_API_URL:
        gcp_object_set_api_url(self, g_value_get_string (value));
        break;
    case PROP_SCOPE:
        gcp_object_set_scope(self, g_value_get_string (value));
        break;
    case PROP_REDIRECT_URI:
        gcp_object_set_redirect_uri(self, g_value_get_string (value));
        break;
    }
}

static void
gcp_object_class_init (GCPObjectClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = gcp_object_get_property;
  object_class->set_property = gcp_object_set_property;

  properties [PROP_AUTH_URL] =
    g_param_spec_string ("auth_url",
                         "Auth_URL",
                         "Authentication URL",
                         NULL,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  properties [PROP_API_URL] =
    g_param_spec_string ("api_url",
                         "API_URL",
                         "Google APIs URL",
                         NULL,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  properties [PROP_SCOPE] =
    g_param_spec_string ("scope",
                         "Scope",
                         "Cloudprint API scope",
                         NULL,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  properties [PROP_REDIRECT_URI] =
    g_param_spec_string ("redirect_uri",
                         "Redirect_URI",
                         "Redirection URI",
                         NULL,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, LAST_PROP, properties);
}

static void
gcp_object_init (GCPObject *self)
{
}

const gchar *
gcp_object_get_auth_url (GCPObject *self)
{
  return self->auth_url;
}

const gchar *
gcp_object_get_api_url (GCPObject *self)
{
  return self->api_url;
}

const gchar *
gcp_object_get_scope (GCPObject *self)
{
  return self->scope;
}

const gchar *
gcp_object_get_redirect_uri (GCPObject *self)
{
  return self->redirect_uri;
}

void gcp_object_set_auth_url (GCPObject *self, const gchar *auth_url)
{
  if (g_strcmp0( auth_url, self->auth_url) == 0)
		{
			g_free(self->auth_url);
			self->auth_url = g_strdup(auth_url);
		}
}

void gcp_object_set_api_url (GCPObject *self, const gchar *api_url)
{
  if (g_strcmp0( api_url, self->api_url) == 0)
  		{
  			g_free(self->api_url);
  			self->api_url = g_strdup(api_url);
  		}
}

void gcp_object_set_scope (GCPObject *self, const gchar *scope)
{
  if (g_strcmp0( scope, self->scope) == 0)
  		{
  			g_free(self->scope);
  			self->scope = g_strdup(scope);
  		}
}

void gcp_object_set_redirect_uri (GCPObject *self, const gchar *redirect_uri)
{
  if (g_strcmp0( redirect_uri, self->redirect_uri) == 0)
  		{
  			g_free(self->redirect_uri);
  			self->redirect_uri = g_strdup(redirect_uri);
  		}
}

gchar* gcp_object_get_printers (GCPObject *self, const gchar *access_token)
{
  gchar *url = "https://www.google.com/cloudprint/search?access_token=";
  url = g_strconcat (url, access_token, NULL);
  gchar *printers = "list of printers";

  /* TODO: Make api request to get a list of printers. */

  return printers;
}

gchar* gcp_object_get_printer_options (GCPObject   *self,
                                       const gchar *uid,
                                       const gchar *access_token)
{
  gchar *url = "https://www.google.com/cloudprint/printer?access_token=";
  url = g_strconcat (url, access_token, "&printerid=", uid, NULL);
  gchar *printer_options = "printer options";

  /* TODO: Make api request to get a list of options for a particular printer. */

  return printer_options;
}

gboolean gcp_object_submit_print_job (GCPObject   *self,
                                      const gchar *uid,
                                      const gchar *access_token,
                                      const gchar *title,
                                      const gchar *ticket)
{
  /* TODO: Make api request to submit a file for printing. */
  return TRUE;
}
