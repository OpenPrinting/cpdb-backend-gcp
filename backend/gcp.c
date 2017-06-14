#include <glib-object.h>
#include "gcp.h"


/* Public methods */
gchar* gcp_object_get_printers(GCPObject *self)
{
  gchar *url = "https://www.google.com/cloudprint/search?access_token=";
  url = g_strconcat(url, self->access_token, NULL);
  gchar *printers = "list of printers";

  /* TODO: Make api request to get a list of printers. */

  return printers;
}

gchar* gcp_object_get_printer_options(GCPObject *self, gchar *uid)
{
  gchar *url = "https://www.google.com/cloudprint/printer?access_token=";
  url = g_strconcat(url, self->access_token, "&printerid=", uid, NULL);
  gchar *printer_options = "printer options";

  /* TODO: Make api request to get a list of options for a particular printer. */

  return printer_options;
}

void gcp_object_submit_print_job(GCPObject *self,
                                   gchar *uid,
                                   gchar *title,
                                   gchar *ticket)
{
  /* TODO: Make api request to submit a file for printing. */
}

/* This is called when the class object is created. */
void	gcp_object_class_init		(gpointer g_class, gpointer class_data)
{
	//GCPObjectClass	*this_class	= GCP_OBJECT_CLASS (g_class);
}

/* This is called when the class object is no longer used. */
void	gcp_object_class_final		(gpointer g_class, gpointer class_data)
{
	/* No class finalization needed. */
}

/* This is called when a instance object is created. The instance's class is passed as g_class. */
void	gcp_object_instance_init	(GTypeInstance *instance, gpointer g_class)
{
	GCPObject *this_object = GCP_OBJECT (instance);

  /* fill in the instance struct members */
  this_object->AUTH_URL = "https://accounts.google.com/o/oauth2/v2/auth";
  this_object->API_URL = "https://www.googleapis.com/oauth2/v4/token";
  this_object->CLIENT_ID = "473470378528-lh7nlpmn2d8bhas1r51iibhjc70vsi74.apps.googleusercontent.com";
  this_object->CLIENT_SECRET = "0COpUUBTN-S5fgyBd0PfQzg1";
  this_object->SCOPE = "https://www.googleapis.com/auth/cloudprint";
  this_object->REDIRECT_URI = "urn:ietf:wg:oauth:2.0:oob";

	this_object->access_token = NULL;
	this_object->refresh_token = NULL;
	this_object->token_type = NULL;
  this_object->expires_in = NULL;
  this_object->error = NULL;
  this_object->error_description = NULL;
}

/* Since there is no base class to derive from, base_init/finalize are NULL */
GType	gcp_object_get_type (void)
{
	static GType type = 0;

	if (type == 0)
	{
		/* This is the structure that the system uses to fully describe
		how a type should be created, initialized and finalized. */

		static const GTypeInfo type_info =
		{
			sizeof (GCPObjectClass),
			NULL,				/* base_init */
			NULL,				/* base_finalize */
			gcp_object_class_init,		/* class_init */
			gcp_object_class_final,	/* class_finalize */
			NULL,				/* class_data */
			sizeof (GCPObject),
			0,				/* n_preallocs */
			gcp_object_instance_init	/* instance_init */
    };

		/* Since our type has no parent, it is considered
		"fundamental", and we have to inform the system that our
		type is both classed (unlike say a float, int, or pointer),
		and is instantiable (the system can create instance objects).
		for example, Interfaces or Abstract classes are not
		instantiable. */

		static const GTypeFundamentalInfo fundamental_info =
		{
			G_TYPE_FLAG_CLASSED | G_TYPE_FLAG_INSTANTIATABLE
		};

		type = g_type_register_fundamental
		(
			g_type_fundamental_next (),	/* next available GType number */
			"GCPObjectType",		/* type name as string */
			&type_info,			/* type info as above */
			&fundamental_info,		/* fundamental info as above */
			0				/* type is not abstract */
		);
	}

	return	type;
}
