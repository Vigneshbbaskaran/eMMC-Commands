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

void testcase_pass(int number)
{
    printf("Test Case Passes :CMD%d ",number);
}
int main(int nargs, char **argv)
{
	__u8 ext_csd[512], ext_csd_rev, reg;
	__u32 regl;
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

	ret = read_extcsd(fd, ext_csd);
	if (ret) {
		fprintf(stderr, "Could not read EXT_CSD from %s\n", device);
	}
    else
    {
        testcase_pass(MMC_SEND_EXT_CSD);       
    }
}