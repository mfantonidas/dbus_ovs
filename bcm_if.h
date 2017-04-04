#ifndef BCM_IF_H
#define BCM_IF_H 1

#include <glib-2.0/glib.h>

#include "smap.h"


struct DeviceInfo;
struct NetworkInfo;
typedef int (*get_DeviceInfo_property)(const gchar *item, struct DeviceInfo *devinfo);
typedef int (*get_NetworkInfo_property)(const gchar *item, struct NetworkInfo *netinfo);

#define BCM_DBUS_IF_GEN_PROPERTY "org.freedesktop.DBus.Properties"
#define BCM_DBUS_PATH "/com/st/ovs1"
#define BCM_DBUS_IFNAME "com.st.ovs1"
#define BCM_DBUS_IF_DEVINFO "DeviceInfo"
#define BCM_DBUS_IF_NETINFO "NetworkInfo"

#define ARG1 argv[0]
#define ARG2 argv[0],argv[1]
#define ARG3 argv[0],argv[1],argv[2]
#define ARG4 argv[0],argv[1],argv[2],argv[3]
#define ARG5 argv[0],argv[1],argv[2],argv[3],argv[4]
#define ARG6 argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]

typedef enum {DEVINFO, NETINFO, WANINFO, PONINFO, VOIPINFO, WANCONF, DHCPD, NETNS, PPPCONN, VPNCONN, OVSPORT, ROUTE, FIREWALL, DNSD} IFTYPE;

static void 
static guint g_method_call_noarg_(gchar *dbsif, gchar *method_name, gchar * ofmt);
static guint g_method_call_arg_();
static GVariant * g_property_get_info_(gchar *item, struct dbus_if *dif);
static int g_property_get_info(gchar *item, gchar *dif);

struct DeviceInfo
{
	struct smap propertys;

	void (*func_reboot)();
	void (*func_factoryreboot)();
	get_DeviceInfo_property func_get_devinfo;
};

struct NetworkInfo
{
	struct smap propertys;
	get_NetworkInfo_property func_get_netinfo;
};

struct dbus_if{
	IFTYPE iftype; 
	union{
		struct DeviceInfo *devinfo;
		struct NetworkInfo *netinfo;
	};
};

void g_method_call_DeviceInfo_Reboot();
void g_method_call_DeviceInfo_FactoryReboot();
int g_property_get_info_DeviceInfo(gchar *item, struct DeviceInfo *devinfo);
int g_property_get_info_NetworkInfo(gchar *item, struct NetworkInfo *netinfo);
void init_DeviceInfo(struct DeviceInfo *);
void destory_DeviceInfo(struct DeviceInfo *);
void init_NetworkInfo(struct NetworkInfo *);
void destory_NetworkInfo(struct NetworkInfo *);

#endif
