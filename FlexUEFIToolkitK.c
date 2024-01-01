#include "FlexUEFIToolkitDxe.h"
#include "linux/printk.h"
#include <linux/module.h>
#include <linux/efi.h>
#include <linux/rtc.h>
#include <linux/init.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include "Toolkit.h"

MODULE_LICENSE("MIT License");
MODULE_AUTHOR("ColorsWind");

#define MSG_SIZE 1052
struct sock *netlink_socket = NULL;
static char message[MSG_SIZE];


static void receive_message(struct sk_buff *skb) {
    struct nlmsghdr *nlh;

    int pid;

    nlh = nlmsg_hdr(skb);
    if (nlh->nlmsg_len < sizeof(struct nlmsghdr) || skb->len < nlh->nlmsg_len) {
        pr_alert("%s : invalid message received.\n",__func__);
        return;
    }

    memcpy(message, NLMSG_DATA(nlh), MSG_SIZE);
    pid = nlh->nlmsg_pid;
    pr_debug("%s : kernel message receive: %s.\n",__func__, message);

    if (netlink_socket == NULL) {
        pr_alert("%s : netlink_socket is not initialized.\n", message);
        return;
    }

    struct sk_buff *skb_out;
    // int msg_size = strnlen(message, MSG_SIZE);

    skb_out = nlmsg_new(MSG_SIZE, 0);
    if (!skb_out) {
        pr_alert("%s : failed to allocate new skb.\n",__func__);
        return;
    }

    NETLINK_CB(skb_out).dst_group = 0;
    NETLINK_CB(skb_out).portid = pid;

    nlh = nlmsg_put(skb_out, pid, 0, NLMSG_DONE, MSG_SIZE, 0);


    // UEFIStringLower(message, nlmsg_data(nlh));
    UEFIFlexCall(message, nlmsg_data(nlh));



    int res;
    res = nlmsg_unicast(netlink_socket, skb_out, pid);
    if (res < 0) {
        pr_info("%s : error while sending back to user.\n", __func__);
    }
}

static int fut_init(void) {
    struct netlink_kernel_cfg cfg = {
        .input = receive_message,
    };

    if (!efi_enabled(EFI_RUNTIME_SERVICES)) {
        pr_alert("%s : EFI runtime services are not enabled.\n", __func__);
        return -ENODEV;
    }
    pr_info("%s : EFI runtime services are enabled.\n", __func__);

    netlink_socket = netlink_kernel_create(&init_net, FLEX_UEFI_TOOLKIT_NETLINK_ID, &cfg);
    if (!netlink_socket) {
        pr_alert("%s : failed to create netlink socket.\n", __func__);
        return -ENOMEM;
    }
    pr_info("%s : netlink initialized.\n", __func__);

    pr_info("%s : FlexUEFIToolkit kmod loaded.\n", __func__);
    return 0;
}

static void fut_exit(void)
{
    if (netlink_socket != NULL) {
        netlink_kernel_release(netlink_socket);
    }
    pr_info("%s : FlexUEFIToolkit kmod unloaded.\n", __func__);
}

module_init(fut_init);
module_exit(fut_exit);