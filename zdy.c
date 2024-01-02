#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include "FlexUEFIToolkitDxe.h"

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg, recv_msg;
FILE* new_fw;
static const size_t block_size = 4096;
static const size_t block_count = 1024;
static const size_t MAX_NL_PAYLOAD = 5000;
static const size_t WRITE_CHUNK_SIZE = 4096;

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

    new_fw = fopen("OVMF.fd.old", "rb");
    u_int32_t nlh_seq = 0;
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_NL_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_NL_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_NL_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    FLEX_UEFI_TOOKLIT_FUNC_TYPE funcType = futChangeBIOS;
    u_int64_t writeByte = WRITE_CHUNK_SIZE;
    char msgArg[MAX_NL_PAYLOAD];


    for (u_int64_t lba = 0; lba < block_count; ++lba) {
        for (u_int64_t offset = 0; offset < block_size; offset += WRITE_CHUNK_SIZE) {
            //* serilization
            ((u_int32_t*)msgArg)[0] = (u_int32_t)funcType;
            ((u_int64_t*)(msgArg+4))[0] = lba;
            ((u_int64_t*)(msgArg+4))[1] = offset;
            ((u_int64_t*)(msgArg+4))[2] = writeByte;

            fread(msgArg + 28,
                  WRITE_CHUNK_SIZE,
                  1,
                  new_fw);

            memcpy((void *)NLMSG_DATA(nlh), msgArg, MAX_NL_PAYLOAD);
            //nlh->nlmsg_seq = nlh_seq++;

            iov.iov_base = (void *)nlh;
            iov.iov_len = nlh->nlmsg_len;
            msg.msg_name = (void *)&dest_addr;
            msg.msg_namelen = sizeof(dest_addr);
            msg.msg_iov = &iov;
            msg.msg_iovlen = 1;

            sendmsg(sock_fd, &msg, 0);

            recvmsg(sock_fd, &recv_msg, 0);
        }
        printf("\rWrite LBA: %4ld/%4ld", lba + 1, block_count);
        fflush(stdout);
    }
    printf("\nWrite Done!\n");

    fclose(new_fw);
    close(sock_fd);
}
