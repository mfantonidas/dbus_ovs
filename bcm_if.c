#include <stdlib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "bcm_if.h"

static 
gint g_method_call_gen_sync_()
{
	gint ret = 0;
	return ret;
}

gint 
g_method_call_gen_sync()
{
	gint ret;
	ret = g_method_call_gen_sync_();
	return ret;
}

static gint 
g_property_get_info(const gchar *item, const gchar *dbif, const gchar *ifmt, const gchar * ofmt, struct smap *propertys)
{
	gint ret;
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
g_method_call_arg_(gchar *dbsif, gchar *method_name, struct smap *propertys, gchar *ofmt, gint argc, gchar *argv1, ...)
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
	gint res;

	strcat(ifmt, "(");
	str = argv1;
	va_start(arg_ptr, argv1);
	for(i = 0; i < argc; i++){
		argv[i] = str;
		strcat(ifmt, "s");
	}
	strcat(ifmt, ")");

	connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);

	g_print("DBUS: method %s.%s %s\n", BCM_DBUS_IFNAME, dbsif, method_name);
	gchar *ifpath = xasprintf(BCM_DBUS_PATH"/%s", dbsif);
	gchar *ifname = xasprintf(BCM_DBUS_PATH".%S", dbsif);

	result = g_dbus_connection_call_sync (connection, ifname, ifpath, ifname, method_name, 
                                          		  g_variant_new_va(ifmt, NULL, arg_ptr), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	

	g_assert_no_error(error);
	if (!strcmp(method_name, "QueryNetNamespce"))
	{
		gchar ovsport[20];
		gchar iptables[20];
		gchar routetbl[20];
		gchar firewall[20];
		g_variant_get(result, "(ssssu)", &ovsport, &iptables, &routetbl, &firewall, &res);
		smap_add(propertys, "ovsport", ovsport);
		smap_add(propertys, "iptables", iptables);
		smap_add(propertys, "routetbl", routetbl);
		smap_add(propertys, "firewall", firewall);
	}else
		g_variant_get(result, "(u)", &res);

	g_print("method call result: %s\n", info);
	g_object_unref(connection);
	free(ifpath);
	free(ifname);
}

static gint 
g_property_get_info_DeviceInfo(const gchar *item, struct DeviceInfo *devinfo)
{
	gint ret;

	ret = g_property_get_info(item, "DeviceInfo", "(ss)", "(s)", &devinfo->propertys);

	return ret;
}

static gint 
g_property_get_info_NetworkInfo(const gchar *item, struct NetworkInfo *netinfo)
{
	gint ret;

	ret = g_property_get_info(item, "NetworkInfo", "(ss)", "(s)", &netinfo->propertys);

	return ret;
}

static gint 
g_property_get_info_WANConnectionInfo(const gchar *item, struct WANConnectionInfo *wcif)
{
	gint ret;

	ret = g_property_get_info(item, "WANConnectionInfo", "(ss)", "(s)", &wcif->propertys);

	return ret;
}

static gint 
g_property_get_info_PONInfo(const gchar *item, struct PONInfo *pif)
{
	gint ret;

	ret = g_property_get_info(item, "PONInfo", "(ss)", "(s)", &pif->propertys);

	return ret;
}

static gint 
g_property_get_info_VoIPInfo(const gchar *item, struct VoIPInfo *vif)
{
	gint ret;

	ret = g_property_get_info(item, "VoIPInfo", "(ss)", "(s)", &vif->propertys);

	return ret;
}

static gint 
g_property_get_info_WLANConfiguration(const gchar *item, struct WLANConfiguration *wlcfg)
{
	gint ret;

	ret = g_property_get_info(item, "WLANConfiguration", "(ss)", "(s)", &wlcfg->propertys);

	return ret;
}

static gint 
g_property_get_info_DHCPServer(const gchar *item, struct DHCPServer *ds)
{
	gint ret;

	ret = g_property_get_info(item, "DHCPServer", "(ss)", "(s)", &ds->propertys);

	return ret;
}

static gint 
g_property_get_info_DNSServerConfig(const gchar *item, struct DNSServerConfig *dnss)
{
	gint ret;

	ret = g_property_get_info(item, "DNSServerConfig", "(ss)", "(s)", &dnss->propertys);

	return ret;
}

static void PPPStatus_Changed_Signal_Handler (GDBusConnection  *connection,
                                const gchar      *sender_name,
                                const gchar      *object_path,
                                const gchar      *interface_name,
                                const gchar      *signal_name,
                                GVariant         *parameters,
                                gpointer         user_data)
{
    gchar *PPPStauts, *netns, *wanconnInfo;

    g_print("PPPStatus_Changed_Signal sender=%s, object_path=%s, interface_name=%s, signal_name=%s",
                  sender_name ? sender_name : "(null sender)",
                  object_path ? object_path : "(null object_path)",
                  interface_name ? interface_name : "(null interface_name)",
                  signal_name ? signal_name : "(null signal_name)");

    if(parameters)
    {
        g_variant_get(parameters, "(sss)", &PPPStauts, &netns, &wanconnInfo);
        g_print("PPPStatus_Changed_Signal get parameters: %s %s %s\n", PPPStauts, netns, wanconnInfo);
        smap_add( &(struct PPPConnection *)user_data->status, "PPPStauts", PPPStauts);
        smap_add( &(struct PPPConnection *)user_data->status, "netns", netns);
        smap_add( &(struct PPPConnection *)user_data->status, "wanconnInfo", wanconnInfo);
        g_free(PPPStauts);
        g_free(netns);
        g_free(wanconnInfo);
    }

}

static void VPNConnStatus_Changed_Signal_Handler (GDBusConnection  *connection,
                                const gchar      *sender_name,
                                const gchar      *object_path,
                                const gchar      *interface_name,
                                const gchar      *signal_name,
                                GVariant         *parameters,
                                gpointer         user_data)
{
    gchar *VPNConnStauts, *netns;

    g_print("PPPStatus_Changed_Signal sender=%s, object_path=%s, interface_name=%s, signal_name=%s",
                  sender_name ? sender_name : "(null sender)",
                  object_path ? object_path : "(null object_path)",
                  interface_name ? interface_name : "(null interface_name)",
                  signal_name ? signal_name : "(null signal_name)");

    if(parameters)
    {
        g_variant_get(parameters, "(ss)", &VPNConnStauts, &netns);
        g_print("VPNStatus_Changed_Signal get parameters: %s %s\n", VPNConnStauts, netns);
        smap_add( &(struct VPNConnection *)user_data->status, "VPNConnStauts", VPNConnStauts);
        smap_add( &(struct VPNConnection *)user_data->status, "netns", netns);
        g_free(VPNConnStauts);
        g_free(netns);
    }

}

static void RouteTbl_Changed_Signal_Handler (GDBusConnection  *connection,
                                const gchar      *sender_name,
                                const gchar      *object_path,
                                const gchar      *interface_name,
                                const gchar      *signal_name,
                                GVariant         *parameters,
                                gpointer         user_data)
{
    gchar *netns;

    g_print("RouteTbl_Changed_Signal sender=%s, object_path=%s, interface_name=%s, signal_name=%s",
                  sender_name ? sender_name : "(null sender)",
                  object_path ? object_path : "(null object_path)",
                  interface_name ? interface_name : "(null interface_name)",
                  signal_name ? signal_name : "(null signal_name)");

    if(parameters)
    {
        g_variant_get(parameters, "(s)", &netns);
        g_print("RouteTbl_Changed_Signal get parameters: %s\n", netns);
        smap_add( &(struct Route *)user_data->status, "netns", netns);
        g_free(netns);
    }
}

static void Firewall_Changed_Signal_Handler (GDBusConnection  *connection,
                                const gchar      *sender_name,
                                const gchar      *object_path,
                                const gchar      *interface_name,
                                const gchar      *signal_name,
                                GVariant         *parameters,
                                gpointer         user_data)
{
    gchar *netns;

    g_print("Firewall_Changed_Signal sender=%s, object_path=%s, interface_name=%s, signal_name=%s",
                  sender_name ? sender_name : "(null sender)",
                  object_path ? object_path : "(null object_path)",
                  interface_name ? interface_name : "(null interface_name)",
                  signal_name ? signal_name : "(null signal_name)");

    if(parameters)
    {
        g_variant_get(parameters, "(s)", &netns);
        g_print("Firewall_Changed_Signal get parameters: %s\n", netns);
        smap_add( &(struct Firewall *)user_data->status, "netns", netns);
        g_free(netns);
    }

}

static guint 
g_dbus_PPPConnection_signal_subscribe(GDBusConnection *connection, struct PPPConnection *pppc)
{
	guint id;
	id = g_dbus_connection_signal_subscribe(pConnection,
                                            NULL, //"com.st.ovs1.PPPConnection",
                                            "com.st.ovs1.PPPConnection",
                                            "PPPStatusChanged",
                                            "/com/st/ovs1/PPPConnection",
                                            NULL,
                                            G_DBUS_SIGNAL_FLAGS_NONE,
                                            PPPStatus_Changed_Signal_Handler,
                                            (gpointer)pppc,
                                            NULL);
	return id;
}

static guint 
g_dbus_VPNConnection_signal_subscribe(GDBusConnection *connection, struct VPNConnection *vpnc)
{
	guint id;
	id = g_dbus_connection_signal_subscribe(pConnection,
                                            NULL, //"com.st.ovs1.PPPConnection",
                                            "com.st.ovs1.VPNConnection",
                                            "VPNConnStatusChanged",
                                            "/com/st/ovs1/VPNConnection",
                                            NULL,
                                            G_DBUS_SIGNAL_FLAGS_NONE,
                                            VPNConnStatus_Changed_Signal_Handler,
                                            (gpointer)pppc,
                                            NULL);
	return id;
}

static guint 
g_dbus_Route_signal_subscribe(GDBusConnection *connection, struct Route *rt)
{
	guint id;
	id = g_dbus_connection_signal_subscribe(pConnection,
                                            NULL, //"com.st.ovs1.PPPConnection",
                                            "com.st.ovs1.Route",
                                            "RouteTblChanged",
                                            "/com/st/ovs1/Route",
                                            NULL,
                                            G_DBUS_SIGNAL_FLAGS_NONE,
                                            RouteTbl_Changed_Signal_Handler,
                                            (gpointer)rt,
                                            NULL);
	return id;
}

static guint 
g_dbus_Firewall_signal_subscribe(GDBusConnection *connection, struct Firewall *fw)
{
	guint id;
	id = g_dbus_connection_signal_subscribe(pConnection,
                                            NULL, //"com.st.ovs1.PPPConnection",
                                            "com.st.ovs1.Firewall",
                                            "FirewallChanged",
                                            "/com/st/ovs1/Firewall",
                                            NULL,
                                            G_DBUS_SIGNAL_FLAGS_NONE,
                                            Firewall_Changed_Signal_Handler,
                                            (gpointer)fw,
                                            NULL);
	return id;
}

void 
init_DeviceInfo(struct DeviceInfo **dif)
{
	*dif = (struct DeviceInfo *)malloc(sizeof(struct DeviceInfo));
	(*dif)->func_get_DeviceInfo_property = g_property_get_info_DeviceInfo;
	(*dif)->func_reboot = g_method_call_DeviceInfo_Reboot;
	(*dif)->func_factoryreboot = g_method_call_DeviceInfo_FactoryReboot;
	smap_init(&(*dif)->propertys);
}

void 
destory_DeviceInfo(struct DeviceInfo **dif)
{
	(*dif)->func_get_DeviceInfo_property = NULL;
	(*dif)->func_reboot = NULL;
	(*dif)->func_factoryreboot = NULL;
	smap_destroy(&(*dif)->propertys);
	free(*dif);
}

void 
init_NetworkInfo(struct NetworkInfo **nif)
{
	*nif = (struct NetworkInfo *)malloc(sizeof(struct NetworkInfo));
	(*nif)->func_get_NetworkInfo_property = g_property_get_info_NetworkInfo;
	smap_init(&(*nif)->propertys);
}

void 
destory_NetworkInfo(struct NetworkInfo **nif)
{
	(*nif)->func_get_NetworkInfo_property = NULL;
	smap_destroy(&(*nif)->propertys);
	free(*nif);
}

void 
init_WANConnectionInfo(struct WANConnectionInfo **wcif)
{
	*wcif = (struct WANConnectionInfo *)malloc(sizeof(struct WANConnectionInfo));
	(*wcif)->func_get_WLANConfiguration_property = g_property_get_info_WLANConfiguration;
	smap_init(&(*wcif)->propertys);
}

void 
destory_WANConnectionInfo(struct WANConnectionInfo **wcif)
{
	(*wcif)->func_get_WLANConfiguration_property = NULL;
	smap_destroy(&(*wcif)->propertys);
	free(*wcif);
}

void 
init_PONInfo(struct PONInfo **pif)
{
	*pif = (struct PONInfo *)malloc(sizeof(struct PONInfo));
	(*pif)->func_get_PONInfo_property = g_property_get_info_PONInfo;
	smap_init(&(*pif)->propertys);
}

void 
destory_PONInfo(struct PONInfo **pif)
{
	(*pif)->func_get_PONInfo_property = NULL;
	smap_destroy(&(*pif)->propertys);
	free(*pif);
}

void 
init_VoIPInfo(struct VoIPInfo **voif)
{
	*voif = (struct VoIPInfo *)malloc(sizeof(struct VoIPInfo));
	(*voif)->func_get_VoIPInfo_property = g_property_get_info_VoIPInfo;
	smap_init(&(*voif)->propertys);
}

void 
destory_VoIPInfo(struct VoIPInfo **voif)
{
	(*voif)->func_get_VoIPInfo_property = NULL;
	smap_destroy(&(*voif)->propertys);
	free(*voif);
}

void 
init_WLANConfiguration(struct WLANConfiguration **wlcfg)
{
	*wlcfg = (struct WLANConfiguration *)malloc(sizeof(struct WLANConfiguration));
	(*wlcfg)->func_get_WLANConfiguration_property = g_property_get_info_WLANConfiguration;
	smap_init(&(*wlcfg)->propertys);
}

void 
destory_WLANConfiguration(struct WLANConfiguration **wlcfg)
{
	(*wlcfg)->func_get_WLANConfiguration_property = NULL;
	smap_destroy(&(*wlcfg)->propertys);
	free(*wlcfg);
}

void 
init_DHCPServer(struct DHCPServer **ds)
{
	*ds = (struct DHCPServer *)malloc(sizeof(struct DHCPServer));
	(*ds)->func_get_DHCPServer_property = g_property_get_info_DHCPServer;
	smap_init(&(*ds)->propertys);
}

void 
destory_DHCPServer(struct DHCPServer **ds)
{
	(*ds)->func_get_DHCPServer_property = NULL;
	smap_destroy(&(*ds)->propertys);
	free(*ds);
}

void 
init_NetNamespace(struct NetNamespace **ns)
{
	*ns = (struct NetNamespace *)malloc(sizeof(struct NetNamespace));
	(*ns)->func_NetNamespace_AddNetNamespace = g_method_call_NetNamespace_AddNetNamespace;
	(*ns)->func_NetNamespace_DelNetNamespace = g_method_call_NetNamespace_DelNetNamespace;
	(*ns)->func_NetNamespace_QueryNetNamespce = g_method_call_NetNamespace_QueryNetNamespce;
	smap_init(&(*ns)->itms);
}

void 
destory_NetNamespace(struct NetNamespace **ns)
{
	(*ns)->func_NetNamespace_AddNetNamespace = NULL;
	(*ns)->func_NetNamespace_DelNetNamespace = NULL;
	(*ns)->func_NetNamespace_QueryNetNamespce = NULL;
	smap_destroy(&(*ns)->itms);
	free(*ns);
}

void 
init_PPPConnection(struct PPPConnection **pc)
{
	*pc = (struct PPPConnection *)malloc(sizeof(struct PPPConnection));
	(*pc)->func_PPPConnection_AddPPPConnection = g_method_call_PPPConnection_AddPPPConnection;
	(*pc)->func_PPPConnection_DelPPPConnectiony = g_method_call_PPPConnection_DelPPPConnectiony;
	smap_init(&(*pc)->status);
}

void 
destory_PPPConnection(struct PPPConnection **pc)
{
	(*pc)->func_PPPConnection_AddPPPConnection = NULL;
	(*pc)->func_PPPConnection_DelPPPConnectiony = NULL;
	smap_destroy(&(*pc)->status);
	free(*pc);
}

void 
init_VPNConnection(struct VPNConnection **vc)
{
	*vc = (struct VPNConnection *)malloc(sizeof(struct VPNConnection));
	(*vc)->func_VPNConnection_AddVPNConnection = g_method_call_VPNConnection_AddVPNConnection;
	(*vc)->func_VPNConnection_DelVPNConnectiony = g_method_call_VPNConnection_DelVPNConnectiony;
	smap_init(&(*vc)->status);
}

void 
destory_VPNConnection(struct VPNConnection **vc)
{
	(*vc)->func_VPNConnection_AddVPNConnection = NULL;
	(*vc)->func_VPNConnection_DelVPNConnectiony = NULL;
	smap_destroy(&(*vc)->status);
	free(*vc);
}

void 
init_OvsPort(struct OvsPort **op)
{
	*op = (struct OvsPort *)malloc(sizeof(struct OvsPort));
	(*op)->func_OvsPort_AddOvsPort = g_method_call_OvsPort_AddOvsPort;
	(*op)->func_OvsPort_DelOvsPort = g_method_call_OvsPort_DelOvsPort;
	(*op)->func_OvsPort_MoveOvsPort = g_method_call_OvsPort_MoveOvsPort;
	smap_init(&(*op)->status);
}

void 
destory_OvsPort(struct OvsPort **op)
{
	(*op)->func_OvsPort_AddOvsPort = NULL;
	(*op)->func_OvsPort_DelOvsPort = NULL;
	(*op)->func_OvsPort_MoveOvsPort = NULL;
	smap_destroy(&(*op)->status);
	free(*op);
}

void 
init_Route(struct Route **rt)
{
	*rt = (struct Route *)malloc(sizeof(struct Route));
	(*rt)->func_Route_AddDefaultRoute = g_method_call_Route_AddDefaultRoute;
	(*rt)->func_Route_CleanRouteTable = g_method_call_Route_CleanRouteTable;
	smap_init(&(*rt)->status);
}

void 
destory_Route(struct Route **rt)
{
	(*rt)->func_Route_AddDefaultRoute = NULL;
	(*rt)->func_Route_CleanRouteTable = NULL;
	smap_destroy(&(*rt)->status);
	free(*rt);
}

void 
init_Firewall(struct Firewall **fl)
{
	*fl = (struct Firewall *)malloc(sizeof(struct Firewall));
	(*fl)->func_Firewall_AddDefaultFirewall = g_method_call_Firewall_AddDefaultFirewall;
	(*fl)->func_Firewall_CleanFirewall = g_method_call_Firewall_CleanFirewall;
	smap_init(&(*fl)->status);
}

void 
destory_Firewall(struct Firewall **fl)
{
	(*fl)->func_Firewall_AddDefaultFirewall = NULL;
	(*fl)->func_Firewall_CleanFirewall = NULL;
	smap_destroy(&(*fl)->status);
	free(*fl);
}

void 
init_DNSServerConfig(struct DNSServerConfig **dc)
{
	*dc = (struct DNSServerConfig *)malloc(sizeof(struct DNSServerConfig));
	(*dc)->func_DNSServerConfig_FlushDNS = g_method_call_DNSServerConfig_FlushDNS;
	(*dc)->func_get_DNSServerConfig_property = g_property_get_info_DNSServerConfig;
	smap_init(&(*dc)->propertys)
}

void 
destory_DNSServerConfig(struct DNSServerConfig **dc)
{
	(*dc)->func_DNSServerConfig_FlushDNS = NULL;
	(*dc)->func_get_DNSServerConfig_property = NULL;
	smap_destroy(&(*dc)->propertys);
	free(*dc);
}

guint 
init_DBUSSignals(struct DBUSSignals **dbss)
{
	guint ret;
	GError *pConnError = NULL;

	*dbss = (struct DBUSSignals *)malloc(sizeof(struct DBUSSignals));
	(*dbss)->func_PPPConnection_signal_subscribe = g_dbus_PPPConnection_signal_subscribe;
	(*dbss)->func_VPNConnection_signal_subscribe = g_dbus_VPNConnection_signal_subscribe;
	(*dbss)->func_Route_signal_subscribe = g_dbus_Route_signal_subscribe;
	(*dbss)->func_Firewall_signal_subscribe = g_dbus_Firewall_signal_subscribe;
	(*dbss)->pConnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &pConnError);
	if (NULL == pConnection)
		ret = 1;
	else {
		g_print("main: Failed to connect to dbus. Reason: %s.\n", pConnError->message);
        g_error_free(pConnError);
        ret = 0;
    }
}

void 
destory_DBUSSignals(struct DBUSSignals **dbss)
{
	(*dbss)->func_PPPConnection_signal_subscribe = NULL;
	(*dbss)->func_VPNConnection_signal_subscribe = NULL;
	(*dbss)->func_Route_signal_subscribe = NULL;
	(*dbss)->func_Firewall_signal_subscribe = NULL;
	free(*dbss);
}

static void 
g_method_call_DeviceInfo_Reboot()
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);

	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".DeviceInfo", BCM_DBUS_PATH"/DeviceInfo", BCM_DBUS_IFNAME".DeviceInfo", 
										"Reboot", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_DeviceInfo_FactoryReboot()
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);

	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".DeviceInfo", BCM_DBUS_PATH"/DeviceInfo", BCM_DBUS_IFNAME".DeviceInfo", 
										"FactoryReboot", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_NetNamespace_AddNetNamespace(const gchar *ns_name)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);

	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".NetNamespace", BCM_DBUS_PATH"/NetNamespace", BCM_DBUS_IFNAME".NetNamespace", 
										"AddNetNamespace", g_variant_new("(s)", ns_name), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_NetNamespace_DelNetNamespace(const gchar *ns_name)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);

	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".NetNamespace", BCM_DBUS_PATH"/NetNamespace", BCM_DBUS_IFNAME".NetNamespace", 
										"De,lNetNamespace", g_variant_new("(s)", ns_name), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_NetNamespace_QueryNetNamespce(const gchar *ns_name, gchar **ovsport, gchar **iptables, gchar **routetbl, gchar **firewall)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);

	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".NetNamespace", BCM_DBUS_PATH"/NetNamespace", BCM_DBUS_IFNAME".NetNamespace", 
										"QueryNetNamespce", g_variant_new("(s)", ns_name), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(ssssu)", ovsport, iptables, routetbl, firewall, &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_PPPConnection_AddPPPConnection(const gchar *ppp_name, const gchar *l2_intf, const gchar *ppp_account, const gchar *ppp_pwd, const gint vlanID, 
											 const gint vlan8021p, const guint vlanTpid, const gchar *netns, const gbool defaultGW)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".PPPConnection", BCM_DBUS_PATH"/PPPConnection", BCM_DBUS_IFNAME".PPPConnection", 
										"AddPPPConnection", g_variant_new("(ssssiiusb)", ppp_name, l2_intf, ppp_account, ppp_pwd, vlanID, vlan8021p, vlanTpid, netns, defaultGW), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_PPPConnection_DelPPPConnectiony(const gchar *ppp_name, const gchar *netns)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".PPPConnection", BCM_DBUS_PATH"/PPPConnection", BCM_DBUS_IFNAME".PPPConnection", 
										"DelPPPConnectiony", g_variant_new("(ss)", ppp_name, netns), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_VPNConnection_AddVPNConnection(const gchar *vpn_type, const gchar *tunnel_name, const gchar *ppp_name, const gchar *vpn_account, const gchar *vpn_pwd, const gchar *lnsipaddr, const gchar *netns)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".VPNConnection", BCM_DBUS_PATH"/VPNConnection", BCM_DBUS_IFNAME".VPNConnection", 
										"AddVPNConnection", g_variant_new("(sssssss)", vpn_type, tunnel_name, ppp_name, vpn_account, vpn_pwd, lnsipaddr, netns), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_VPNConnection_DelVPNConnectiony(const gchar *vpn_type, const gchar *tunnel_name, const gchar *netns)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".VPNConnection", BCM_DBUS_PATH"/VPNConnection", BCM_DBUS_IFNAME".VPNConnection", 
										"DelVPNConnectiony", g_variant_new("(sss)", vpn_type, tunnel_name, netns), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_OvsPort_AddOvsPort(const gchar *port_name)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".OvsPort", BCM_DBUS_PATH"/OvsPort", BCM_DBUS_IFNAME".OvsPort", 
										"AddOvsPort", g_variant_new("(s)", port_name), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_OvsPort_DelOvsPort(const gchar *port_name)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".OvsPort", BCM_DBUS_PATH"/OvsPort", BCM_DBUS_IFNAME".OvsPort", 
										"DelOvsPort", g_variant_new("(s)", port_name), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_OvsPort_MoveOvsPort(const gchar *port_name, const gchar *pre_netns, const gchar *target_netns)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".OvsPort", BCM_DBUS_PATH"/OvsPort", BCM_DBUS_IFNAME".OvsPort", 
										"MoveOvsPort", g_variant_new("(sss)", port_name, pre_netns, target_netns), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_Route_AddDefaultRoute(const gchar *netns)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".Route", BCM_DBUS_PATH"/Route", BCM_DBUS_IFNAME".Route", 
										"AddDefaultRoute", g_variant_new("(s)", netns), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_Route_CleanRouteTable(const gchar *netns)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".Route", BCM_DBUS_PATH"/Route", BCM_DBUS_IFNAME".Route", 
										"CleanRouteTable", g_variant_new("(s)", netns), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_Firewall_AddDefaultFirewall(const gchar *netns)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".Firewall", BCM_DBUS_PATH"/Firewall", BCM_DBUS_IFNAME".Firewall", 
										"AddDefaultFirewall", g_variant_new("(s)", netns), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_Firewall_CleanFirewall(const gchar *netns)
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".Firewall", BCM_DBUS_PATH"/Firewall", BCM_DBUS_IFNAME".Firewall", 
										"CleanFirewall", g_variant_new("(s)", netns), NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}

static void 
g_method_call_DNSServerConfig_FlushDNS()
{
	GDBusConnection *connection;
	GVariant *result;
	GError *error;
	gint res;

	onnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);

	g_return_val_if_fail((connection != NULL), -1);
	result = g_dbus_connection_call_sync (connection, BCM_DBUS_IFNAME".DNSServerConfig", BCM_DBUS_PATH"/DNSServerConfig", BCM_DBUS_IFNAME".DNSServerConfig", 
										"FlushDNS", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	g_assert_no_error(error);
	g_variant_get(result, "(u)", &res);
	g_print("method call result: %d\n", res);
	g_object_unref(connection);
}