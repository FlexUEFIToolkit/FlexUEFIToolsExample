#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include "FlexUEFIToolkitDxe.h"

static const size_t MAX_NL_PAYLOAD = 1052;

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

int main()
{
    sock_fd = socket(PF_NETLINK, SOCK_RAW, FLEX_UEFI_TOOLKIT_NETLINK_ID);
    if (sock_fd < 0)
        return -1;

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid(); /* self pid */

    bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; /* For Linux Kernel */
    dest_addr.nl_groups = 0; /* unicast */

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_NL_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_NL_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_NL_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    FLEX_UEFI_TOOKLIT_FUNC_TYPE funcType = futReadFlash;
    u_int64_t lba =0;
    u_int64_t offset = 0;
    u_int64_t readByte = 100;
    char msgArg[128];
    // zdy modify: new protocol
    ((u_int32_t*)msgArg)[0] = (u_int32_t)funcType;
    ((u_int64_t*)(msgArg+4))[0] = lba;
    ((u_int64_t*)(msgArg+4))[1] = offset;
    ((u_int64_t*)(msgArg+4))[2] = readByte;
    // end 
    memcpy((void*)NLMSG_DATA(nlh), msgArg, 4 + 8 * 3);

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    printf("Sending message to kernel\n");
    sendmsg(sock_fd, &msg, 0);
    printf("Waiting for message from kernel\n");

    /* Read message from kernel */
    recvmsg(sock_fd, &msg, 0);
    u_int16_t *content = (u_int16_t *)NLMSG_DATA(nlh);
    printf("Read FLASH at Lba: %d, offset: %d\n",lba, offset);
    //  printf("content size: %ld\n",strlen(content));
    for(int i = 1; i <= readByte / 2; i++){
		printf("%04X ",content[i-1]);					
		if(i % 16 == 0){
			printf("\n");
		}
	}
    printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
    close(sock_fd);
}
