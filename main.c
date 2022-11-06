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

int switch(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	__u8 value = ext_csd[EXT_CSD_PARTITIONS_ATTRIBUTE] | EXT_CSD_ENH_USR;
	__u8 index = EXT_CSD_PARTITIONS_ATTRIBUTE;
	memset(&idata, 0, sizeof(idata));
	idata.write_flag = 1;
	idata.opcode = MMC_SWITCH;
	idata.arg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
			(index << 16) |
			(value << 8) |
			EXT_CSD_CMD_SET_NORMAL;
	idata.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;
	/* Kernel will set cmd_timeout_ms if 0 is set */
	idata.cmd_timeout_ms = 0;

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");

	return ret;
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

int send_status(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	__u32  response;
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

void testcase(int ret,int number)
{	if(ret==0)
    	printf("Test Case Passes :CMD%d \n",number);
	else
    	printf("Test Case Failled...!! :CMD%d \n",number);
}

int issue_cmd(int fd,int i)
{	
	__u8 ext_csd[512], ext_csd_rev, reg;
	int ret, i;
	switch (i)
	{
	case 0:
		/* code */
		ret = issue_cmd0(fd);
		break;
	case 1:
		/* code */
		break;
	case 2:
		/* code */
		break;
	case 3:
		/* code */
		break;
	case 4:
		/* code */
		break;
	case 5:
		/* code */
		break;
	case 6:
		/* code */
		break;
	case 7:
		/* code */
		break;
	case 8:
		/* code */
		ret = read_extcsd(fd, ext_csd);
		break;
	case 9:
		/* code */
		break;
	case 10:
		/* code */
		break;
	case 11:
		/* code */
		break;
	case 12:
		/* code */
		break;
	case 13:
		/* code */
		ret = send_status(fd);
		break;
	case 14:
		/* code */
		break;
	case 15:
		/* code */
		break;
	case 16:
		/* code */
		break;
	case 17:
		/* code */
		break;
	case 18:
		/* code */
		break;
	case 19:
		/* code */
		break;
	case 20:
		/* code */
		break;
	case 23:
		/* code */
		break;
	case 24:
		/* code */
		break;
	case 25:
		/* code */
		break;
	case 26:
		/* code */
		break;
	case 27:
		/* code */
		break;
	case 28:
		/* code */
		break;
	case 29:
		/* code */
		break;
	case 30:
		/* code */
		break;
	case 31:
		/* code */
		break;
	case 35:
		/* code */
		break;
	case 36:
		/* code */
		break;
	case 38:
		/* code */
		break;
	case 39:
		/* code */
		break;
	case 40:
		/* code */
		break;
	case 42:
		/* code */
		break;

	case 55:
		/* code */
		break;
	case 56:
		/* code */
		break;

	default:
		Printf("CMD%d RESERVED\n",i);
		break;
	}
	return ret;
}


int main(int nargs, char **argv)
{
	
	__u32 regl, response;
	int fd, ret, i=0;
	char *device;

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
    for(i=1;i<=56;i++)
    {
		// CMD 
    ret = issue_cmd(fd,i);
		testcase(ret,i);
	}
    close(fd);
}