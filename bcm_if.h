#ifndef BCM_IF_H
#define BCM_IF_H 1

#include <glib-2.0/glib.h>

#include "smap.h"


struct DeviceInfo;
struct NetworkInfo;
struct WANConnectionInfo;
struct PONInfo;
struct VoIPInfo;
struct WLANConfiguration;
struct DHCPServer;
struct NetNamespace;
struct PPPConnection;
struct VPNConnection;
struct OvsPort;
struct Route;
struct Firewall;
struct DNSServerConfig;

typedef int (*get_DeviceInfo_property)(const gchar *item, struct DeviceInfo *devinfo);
typedef int (*get_NetworkInfo_property)(const gchar *item, struct NetworkInfo *netinfo);
typedef int (*get_WANConnectionInfo_property)(const gchar *item, struct WANConnectionInfo *waninfo);

#define BCM_DBUS_IF_GEN_PROPERTY "org.freedesktop.DBus.Properties"
#define BCM_DBUS_PATH "/com/st/ovs1"
#define BCM_DBUS_IFNAME "com.st.ovs1"
#define BCM_DBUS_IF_DEVINFO "DeviceInfo"
#define BCM_DBUS_IF_NETINFO "NetworkInfo"

typedef enum {DEVINFO, NETINFO, WANINFO, PONINFO, VOIPINFO, WANCONF, DHCPD, NETNS, PPPCONN, VPNCONN, OVSPORT, ROUTE, FIREWALL, DNSD} IFTYPE;

static guint g_method_call_noarg_(gchar *dbsif, gchar *method_name, gchar * ofmt);
static guint g_method_call_arg_();
static GVariant * g_property_get_info_(const gchar *item, struct dbus_if *dif);
static int g_property_get_info(const gchar *item, const gchar *dbif, const gchar *ifmt, const gchar * ofmt, struct smap *propertys);
static int g_property_get_info_DeviceInfo(const gchar *item, struct DeviceInfo *devinfo);
static int g_property_get_info_NetworkInfo(const gchar *item, struct NetworkInfo *netinfo);
static int g_property_get_info_WANConnectionInfo(const gchar *item, struct WANConnectionInfo *wcif);
static int g_property_get_info_PONInfo(const gchar *item, struct PONInfo *pif);
static int g_property_get_info_VoIPInfo(const gchar *item, struct VoIPInfo *vif);
static int g_property_get_info_WLANConfiguration(const gchar *item, struct WLANConfiguration *wlcfg);
static int g_property_get_info_DHCPServer(const gchar *item, struct DHCPServer *ds);
static int g_property_get_info_DNSServerConfig(const gchar *item, struct DNSServerConfig *dnss);

static void g_method_call_DeviceInfo_Reboot();
static void g_method_call_DeviceInfo_FactoryReboot();
static void g_method_call_NetNamespace_AddNetNamespace(const gchar *ns_name);
static void g_method_call_NetNamespace_DelNetNamespace(const gchar *ns_name);
static void g_method_call_NetNamespace_QueryNetNamespce(const gchar *ns_name, gchar **ovsport, gchar **iptables, gchar **routetbl, gchar **firewall);
static void g_method_call_PPPConnection_AddPPPConnection(const gchar *ppp_name, const gchar *l2_intf, const gchar *ppp_account, const gchar *ppp_pwd, const gint vlanID, 
											 const gint vlan8021p, const guint vlanTpid, const gchar *netns, const gbool defaultGW);
static void g_method_call_PPPConnection_DelPPPConnectiony(const gchar *ppp_name, const gchar *netns, const gbool defaultGW);
static void g_method_call_VPNConnection_AddVPNConnection(const gchar *vpn_type, const gchar *tunnel_name, const gchar *ppp_name, const gchar *vpn_account, const gchar *vpn_pwd, const gchar *lnsipaddr, const gchar *netns);
static void g_method_call_VPNConnection_DelVPNConnectiony(const gchar *vpn_type, const gchar *tunnel_name, const gchar *netns);
static void g_method_call_OvsPort_AddOvsPort(const gchar *port_name);
static void g_method_call_OvsPort_DelOvsPort(const gchar *port_name);
static void g_method_call_OvsPort_MoveOvsPort(const gchar *port_name, const gchar *pre_netns, const gchar *target_netns);
static void g_method_call_Route_AddDefaultRoute(const gchar *netns);
static void g_method_call_Route_CleanRouteTable(const gchar *netns);
static void g_method_call_Firewall_AddDefaultFirewall(const gchar *netns);
static void g_method_call_Firewall_CleanFirewall(const gchar *netns);
static void g_method_call_DNSServerConfig_FlushDNS();

static void PPPStatus_Changed_Signal_Handler (GDBusConnection *connection, const gchar *sender_name, const gchar *object_path,
                                			const gchar *interface_name, const gchar *signal_name, GVariant *parameters, gpointer user_data);
static void VPNConnStatus_Changed_Signal_Handler (GDBusConnection *connection, const gchar *sender_name, const gchar *object_path,
                                			const gchar *interface_name, const gchar *signal_name, GVariant *parameters, gpointer user_data);
static void RouteTbl_Changed_Signal_Handler (GDBusConnection *connection, const gchar *sender_name, const gchar *object_path,
                                			const gchar *interface_name, const gchar *signal_name, GVariant *parameters, gpointer user_data);
static void Firewall_Changed_Signal_Handler (GDBusConnection *connection, const gchar *sender_name, const gchar *object_path,
                                			const gchar *interface_name, const gchar *signal_name, GVariant *parameters, gpointer user_data);

static guint g_dbus_PPPConnection_signal_subscribe(GDBusConnection *connection, struct PPPConnection *pppc);
static guint g_dbus_VPNConnection_signal_subscribe(GDBusConnection *connection, struct VPNConnection *vpnc);
static guint g_dbus_Route_signal_subscribe(GDBusConnection *connection, struct Route *rc);
static guint g_dbus_Firewall_signal_subscribe(GDBusConnection *connection, struct Firewall *fwc);

struct DeviceInfo{
	struct smap propertys;
	void (*func_reboot)();
	void (*func_factoryreboot)();
	int (*func_get_DeviceInfo_property)(const gchar *item, struct DeviceInfo *devinfo);
};

struct NetworkInfo{
	struct smap propertys;
	int (*func_get_NetworkInfo_property)(const gchar *item, struct NetworkInfo *netinfo);
};

struct WANConnectionInfo{
	struct smap propertys;
	int (*func_get_WANConnectionInfo_property)(const gchar *item, struct WANConnectionInfo *waninfo);
};

struct PONInfo{
	struct smap propertys;
	int (*func_get_PONInfo_property)(const gchar *item, struct PONInfo *poninfo);
};

struct VoIPInfo{
	struct smap propertys;
	int (*func_get_VoIPInfo_property)(const gchar *item, struct VoIPInfo *voipinfo);
};

struct WLANConfiguration{
	struct smap propertys;
	int (*func_get_WLANConfiguration_property)(const gchar *item, struct WLANConfiguration *wlancfginfo);
};

struct DHCPServer{
	struct smap propertys;
	int (*func_get_DHCPServer_property)(const gchar *item, struct DHCPServer *dhcpinfo);
};

struct NetNamespace{
	struct smap itms;
	void (*func_NetNamespace_AddNetNamespace)(const gchar *ns_name);
	void (*func_NetNamespace_DelNetNamespace)(const gchar *ns_name);
	void (*func_NetNamespace_QueryNetNamespce)(const gchar *ns_name, gchar **ovsport, gchar **iptables, gchar **routetbl, gchar **firewall);
};

struct PPPConnection{
	struct smap status;
	void (*func_PPPConnection_AddPPPConnection)(const gchar *ppp_name, const gchar *l2_intf, const gchar *ppp_account, const gchar *ppp_pwd, const gint vlanID, 
											 const gint vlan8021p, const guint vlanTpid, const gchar *netns, const gbool defaultGW);
	void (*func_PPPConnection_DelPPPConnectiony)(const gchar *ppp_name, const gchar *netns);
};

struct VPNConnection{
	struct smap status;
	void (*func_VPNConnection_AddVPNConnection)(const gchar *vpn_type, const gchar *tunnel_name, const gchar *ppp_name, const gchar *vpn_account, const gchar *vpn_pwd, const gchar *lnsipaddr, const gchar *netns);
	void (*func_VPNConnection_DelVPNConnectiony)(const gchar *vpn_type, const gchar *tunnel_name, const gchar *netns);
};

struct OvsPort{
	struct smap status;
	void (*func_OvsPort_AddOvsPort)(const gchar *port_name);
	void (*func_OvsPort_DelOvsPort)(const gchar *port_name);
	void (*func_OvsPort_MoveOvsPort)(const gchar *port_name, const gchar *pre_netns, const gchar *target_netns);
};

struct Route{
	struct smap status;
	void (*func_Route_AddDefaultRoute)(const gchar *netns);
	void (*func_Route_CleanRouteTable)(const gchar *netns);
};

struct Firewall{
	struct smap status;
	void (*func_Firewall_AddDefaultFirewall)(const gchar *netns);
	void (*func_Firewall_CleanFirewall)(const gchar *netns);
}

struct DNSServerConfig{
	struct smap propertys;
	int (*func_get_DNSServerConfig_property)(const gchar *item, struct DNSServerConfig *dnss);
	void (*func_DNSServerConfig_FlushDNS)();
};

struct DBUSSignals
{
	GDBusConnection *pConnection;
	guint (*func_PPPConnection_signal_subscribe)(GDBusConnection *connection, struct PPPConnection *pppc);
	guint (*func_VPNConnection_signal_subscribe)(GDBusConnection *connection, struct VPNConnection *vpnc);
	guint (*func_Route_signal_subscribe)(GDBusConnection *connection, struct Route *rc);
	guint (*func_Firewall_signal_subscribe)(GDBusConnection *connection, struct Firewall *fwc);
};

struct dbus_if{
	IFTYPE iftype; 
	union{
		struct DeviceInfo *devinfo;
		struct NetworkInfo *netinfo;
	};
};

void init_DeviceInfo(struct DeviceInfo **);
void destory_DeviceInfo(struct DeviceInfo **);
void init_NetworkInfo(struct NetworkInfo **);
void destory_NetworkInfo(struct NetworkInfo **);
void init_WANConnectionInfo(struct WANConnectionInfo **);
void destory_WANConnectionInfo(struct WANConnectionInfo **);
void init_PONInfo(struct PONInfo **);
void destory_PONInfo(struct PONInfo **);
void init_VoIPInfo(struct VoIPInfo **);
void destory_VoIPInfo(struct VoIPInfo *);
void init_WLANConfiguration(struct WLANConfiguration **);
void destory_WLANConfiguration(struct WLANConfiguration **);
void init_DHCPServer(struct DHCPServer **);
void destory_DHCPServer(struct DHCPServer **);
void init_NetNamespace(struct NetNamespace **);
void destory_NetNamespace(struct NetNamespace **);
void init_PPPConnection(struct PPPConnection **);
void destory_PPPConnection(struct PPPConnection **);
void init_VPNConnection(struct VPNConnection **);
void destory_VPNConnection(struct VPNConnection **);
void init_OvsPort(struct OvsPort **);
void destory_OvsPort(struct OvsPort **);
void init_Route(struct Route **);
void destory_Route(struct Route **);
void init_Firewall(struct Firewall **);
void destory_Firewall(struct Firewall **);
void init_DNSServerConfig(struct DNSServerConfig **);
void destory_DNSServerConfig(struct DNSServerConfig **);
void init_DBUSSignals(struct DBUSSignals **);

#endif
