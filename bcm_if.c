#include <stdlib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "bcm_if.h"

static 
int g_method_call_gen_sync_()
{
	int ret = 0;
	return ret;
}

int 
g_method_call_gen_sync()
{
	int ret;
	ret = g_method_call_gen_sync_();
	return ret;
}

static int 
g_property_get_info(gchar *item, gchar *dbif, gchar *ifmt, gchar * ofmt, struct smap *propertys)
{
	int ret;
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gchar *info;

	connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);

	g_print("DBUS: get %s.%s %s\n", BCM_DBUS_IFNAME, dbif, item);
	char *ifpath = xasprintf(BCM_DBUS_PATH"/%s", dbif);
	char *ifname = xasprintf(BCM_DBUS_IFNAME".%s", dbif);

	result = g_dbus_connection_call_sync(connection,
										 BCM_DBUS_IFNAME,
										 ifpath,
										 BCM_DBUS_IF_GEN_PROPERTY,
										 "Get",
										 g_variant_new(ifmt, ifname, item),
										 G_VARIANT_TYPE("(v)"),
										 G_DBUS_CALL_FLAGS_NONE,
		                                 -1,
										 NULL,
										 &error);
	g_assert_no_error(error);
	g_variant_get(result, ofmt, &info);
	smap_add(propertys, items, info);
	g_object_unref(connection);
	free(ifpath);
	free(ifname);

	return ret;
}

static guint 
g_method_call_noarg_(gchar *dbsif, gchar *method_name, gchar * ofmt)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gchar *info;
	guint ret;

	connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);

	g_print("DBUS: method %s.%s %s\n", BCM_DBUS_IFNAME, dbsif, method_name);
	char *ifpath = xasprintf(BCM_DBUS_PATH"/%s", dbsif);

	result = g_dbus_connection_call_sync (connection,
										  BCM_DBUS_IFNAME, 
										  ifpath, 
										  BCM_DBUS_IFNAME,
                                          method_name, 
                                          NULL, 
                                          NULL, 
                                          G_DBUS_CALL_FLAGS_NONE, 
                                          -1, 
                                          NULL, 
                                          &error);
	g_assert_no_error(error);
	g_variant_get(result, ofmt, &ret);
	g_print("method call result: %s\n", ret);
	g_object_unref(connection);
	free(ifpath);

	return ret;
}

static guint 
g_method_call_arg_(gchar *dbsif, gchar *method_name, gchar *ofmt, gint argc, gchar *argv1, ...)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gchar *info;
	gint i = 0;
	gchar ifmt[20] = {0};
	gchar *argv[argc];
	va_list arg_ptr;
	gchar *str;

	strcat(ifmt, "(");
	str = argv1;
	va_start(arg_ptr, argv1);
	for(i = 0; i < argc; i++){
		argv[i] = str;
		va_arg(arg_ptr, char*);
		strcat(ifmt, "s");
	}
	strcat(ifmt, ")");

	connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);

	g_print("DBUS: method %s.%s %s\n", BCM_DBUS_IFNAME, dbsif, method_name);
	char *ifpath = xasprintf(BCM_DBUS_PATH"/%s", dbsif);

	switch(argc){
		case 1:
			result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME, ifpath, BCM_DBUS_IFNAME, method_name, 
                                          		  g_variant_new(ifmt, argv[0]), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
			break;
		case 2:
			result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME, ifpath, BCM_DBUS_IFNAME, method_name, 
                                          		  g_variant_new(ifmt, argv[0], argv[1]), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
			break;
		case 3:
			result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME, ifpath, BCM_DBUS_IFNAME, method_name, 
                                          		  g_variant_new(ifmt, argv[0], argv[1], argv[2]), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
			break;
		case 4:
			result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME, ifpath, BCM_DBUS_IFNAME, method_name, 
                                          		  g_variant_new(ifmt, argv[0], argv[1], argv[2], argv[3]), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
			break;
		case 5:
			result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME, ifpath, BCM_DBUS_IFNAME, method_name, 
                                          		  g_variant_new(ifmt, argv[0], argv[1], argv[2], argv[3], argv[4]), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
			break;
		case 6:
			result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME, ifpath, BCM_DBUS_IFNAME, method_name, 
                                          		  g_variant_new(ifmt, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
			break;
		case 7:
			result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME, ifpath, BCM_DBUS_IFNAME, method_name, 
                                          		  g_variant_new(ifmt, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
			break;
		default:
			result = NULL;
			break;
	}

	g_assert_no_error(error);
	g_variant_get(result, ofmt, &info);
	g_print("method call result: %s\n", info);
	g_object_unref(connection);
	free(ifpath);
}

int 
g_property_get_info_DeviceInfo(gchar *item, gchar *ifmt, gchar *ofmt, struct DeviceInfo *devinfo)
{
	int ret;

	ret = g_property_get_info(item, "DeviceInfo", "(ss)", "(s)", &devinfo->propertys);

	return ret;
}

int 
g_property_get_info_NetworkInfo(gchar *item, gchar *ifmt, gchar *ofmt, struct NetworkInfo *netinfo)
{
	int ret;

	ret = g_property_get_info(item, "NetworkInfo", "(ss)", "(s)", &netinfo->propertys);

	return ret;
}

void 
init_DeviceInfo(struct DeviceInfo *dif)
{
	dif->func_get_devinfo = g_property_get_info_DeviceInfo;
	dif->func_reboot = g_method_call_DeviceInfo_Reboot;
	dif->func_factoryreboot = g_method_call_DeviceInfo_FactoryReboot;
	smap_init(&dif->propertys);
}

void 
destory_DeviceInfo(struct DeviceInfo *dif)
{
	dif->func_get_devinfo = NULL;
	dif->func_reboot = NULL;
	dif->func_factoryreboot = NULL;
	smap_destroy(&dif->propertys);
}

void 
init_NetworkInfo(struct NetworkInfo *nif)
{
	nif->func_get_netinfo = g_property_get_info_NetworkInfo;
	smap_init(&nif->propertys);
}

void 
destory_DeviceInfo(struct NetworkInfo *nif)
{
	dif->func_get_netinfo = NULL;
	smap_destroy(&nif->propertys);
}


void g_method_call_DeviceInfo_Reboot();
void g_method_call_DeviceInfo_FactoryReboot();
