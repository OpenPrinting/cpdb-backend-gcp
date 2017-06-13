#ifndef _GCP_H_
#define _GCP_H_

/* "Instance struct" that defines all the data fields for out object. */
typedef struct _GCPObject GCPObject;
struct _GCPObject
{
  GTypeInstance gtype;

  gchar *AUTH_URL = "https://accounts.google.com/o/oauth2/v2/auth";
  gchar *API_URL = "https://www.googleapis.com/oauth2/v4/token";
  gchar *CLIENT_ID = "473470378528-lh7nlpmn2d8bhas1r51iibhjc70vsi74.apps.googleusercontent.com";
  gchar *CLIENT_SECRET = "0COpUUBTN-S5fgyBd0PfQzg1";
  gchar *SCOPE = "https://www.googleapis.com/auth/cloudprint";
  gchar *REDIRECT_URI = "urn:ietf:wg:oauth:2.0:oob";

  gchar *access_token;
  gchar *refresh_token;
  gchar *token_type;
  gchar *expires_in;
  gchar *error;
  gchar *error_description;
};

/* "Class struct" that defines all the method functions that our object will share. */
typedef struct _GCPObjectClass GCPObjectClass;
struct _GCPObjectClass
{
  GTypeClass gtypeclass;
};

/* This method returns the GType associated with our new object type. */
GType	gcp_object_get_type (void);

/* These are the Class/Instance Initialize/Finalize functions. Their signature is determined in gtype.h. */
void	gcp_object_class_init		(gpointer g_class, gpointer class_data);
void	gcp_object_class_final		(gpointer g_class, gpointer class_data);
void	gcp_object_instance_init	(GTypeInstance *instance, gpointer g_class);

/* All these functions are methods of GCPObject. */
gchar* gcp_object_get_printers (GCPObject *self);
gchar* gcp_object_get_printer_options (GCPObject *self, gchar*);
void gcp_object_submit_print_job (GCPObject *self, gchar*, gchar*, gchar*);

/* Handy macros */
#define GCP_OBJECT_TYPE		(gcp_object_get_type ())
#define GCP_OBJECT(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GCP_OBJECT_TYPE, GCPObject))
#define GCP_OBJECT_CLASS(c)		(G_TYPE_CHECK_CLASS_CAST ((c), GCP_OBJECT_TYPE, GCPObjectClass))
#define GCP_IS_OBJECT(obj)		(G_TYPE_CHECK_TYPE ((obj), GCP_OBJECT_TYPE))
#define GCP_IS_OBJECT_CLASS(c)		(G_TYPE_CHECK_CLASS_TYPE ((c), GCP_OBJECT_TYPE))
#define GCP_OBJECT_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GCP_OBJECT_TYPE, GCPObjectClass))

#endif /* GCP_H_ */
