#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include <linux/fs.h> /* for BLKGETSIZE */
#include "mmc.h"

static inline void set_single_cmd(struct mmc_ioc_cmd *ioc, __u32 opcode,
				  int write_flag, unsigned int blocks)
{
	ioc->opcode = opcode;
	ioc->write_flag = write_flag;
	ioc->arg = 0x0;
	ioc->blksz = 512;
	ioc->blocks = blocks;
	ioc->flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
}

int read_extcsd(int fd, __u8 *ext_csd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	memset(ext_csd, 0, sizeof(__u8) * 512);
	idata.write_flag = 0;
	idata.opcode = MMC_SEND_EXT_CSD;
	idata.arg = 0;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	idata.blksz = 512;
	idata.blocks = 1;
	mmc_ioc_cmd_set_data(idata, ext_csd);

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");

	return ret;
}

int send_status(int fd, __u32 *response)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;

	memset(&idata, 0, sizeof(idata));
	idata.opcode = MMC_SEND_STATUS;
	idata.arg = (1 << 16);
	idata.flags = MMC_RSP_R1 | MMC_CMD_AC;

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
	perror("ioctl");

	*response = idata.response[0];

	return ret;
}
static int issue_cmd0(int fd)
{
	struct mmc_ioc_cmd idata;
    __u32 arg;
    int ret;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = MMC_GO_IDLE_STATE;
	idata.arg = arg;
	idata.flags = MMC_RSP_NONE | MMC_CMD_BC;

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	return ret;
}

void testcase_pass(int number)
{
    printf("Test Case Passes :CMD%d \n",number);
}
void testcase_fail(int number)
{
    printf("Test Case Failled...!! :CMD%d \n",number);
}

int main(int nargs, char **argv)
{
	__u8 ext_csd[512], ext_csd_rev, reg;
	__u32 regl, response;
	int fd, ret;
	char *device;
	const char *str;

	if (nargs != 2) {
		fprintf(stderr, "Usage: mmc extcsd read </path/to/mmcblkX>\n");
		exit(1);
	}

	device = argv[1];

	fd = open(device, O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(1);
	}
    
    // CMD 0
    ret = issue_cmd0(fd);
	if (ret) {
		testcase_fail(MMC_GO_IDLE_STATE);
	}
    else
    {
        testcase_pass(MMC_GO_IDLE_STATE);       
    }
	
    // CMD 8 
    ret = read_extcsd(fd, ext_csd);
	if (ret) {
		testcase_fail(MMC_SEND_EXT_CSD);
	}
    else
    {
        testcase_pass(MMC_SEND_EXT_CSD);       
    }
    // CMD 13 
    ret = send_status(fd, &response);
	if (ret) {
		testcase_fail(MMC_SEND_STATUS);
	}
    else
    {
        testcase_pass(MMC_SEND_STATUS);       
    }
    close(fd);
}