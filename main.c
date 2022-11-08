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
#include <linux/fs.h>
#include "mmc.h"

//#define TEST
#define CMD 24

int do_general_cmd_read(int dev_fd)
{
	char *device;
	char *endptr;
	__u8 buf[512];
	__u32 arg = 0x01;
	int ret = -EINVAL, i;
	struct mmc_ioc_cmd idata;

	memset(&idata, 0, sizeof(idata));
	idata.write_flag = 0;
	idata.opcode = MMC_GEN_CMD;
	idata.arg = arg;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	idata.blksz = 512;
	idata.blocks = 1;
	mmc_ioc_cmd_set_data(idata, buf);

	ret = ioctl(dev_fd, MMC_IOC_CMD, &idata);
	if (ret) {
		perror("ioctl");
		return ret;
	}
	return ret;
}

static  int set_single_cmd(int fd, __u32 opcode, int write_flag, unsigned int blocks,unsigned int flags)
{   char frame[512];
	struct mmc_ioc_cmd ioc;
	memset(&ioc, 0, sizeof(ioc));
	int ret=0;
	mmc_ioc_cmd_set_data((ioc), &frame);
	ioc.opcode = opcode;
	ioc.write_flag = write_flag;
	ioc.arg = 0x0;
	ioc.blksz = 512;
	ioc.blocks = blocks;
	ioc.flags = flags;
	ret = ioctl(fd, MMC_IOC_CMD, &ioc);
	return ret;
}

int cmd9(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	idata.write_flag = 0;
	idata.opcode = MMC_SEND_CSD;
	idata.arg = 0;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	/* Kernel will set cmd_timeout_ms if 0 is set */
	idata.cmd_timeout_ms = 0;

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");
	return ret;
}
int cmd23(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = MMC_SET_BLOCK_COUNT;
	idata.arg = 2;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	/* Kernel will set cmd_timeout_ms if 0 is set */

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");
	return ret;
}
int cmd24(int fd)
{	
	char frame[512];
	int ret = 0;
	struct mmc_ioc_cmd idata;

	mmc_ioc_cmd_set_data((ioc), &frame);
	memset(&idata, 0, sizeof(idata));
	idata.opcode = MMC_WRITE_BLOCK;
	idata.arg = 1;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	/* Kernel will set cmd_timeout_ms if 0 is set */

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");
	return ret;
}
int switch_cmd(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	__u8 value = EXT_CSD_PARTITIONS_ATTRIBUTE | EXT_CSD_ENH_USR;
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
	{
		perror("ioctl");
		printf("ret:%d",ret);
	}
	return ret;
}

int send_status(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	__u32  *response;
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
    __u32 arg=0;
    int ret;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = MMC_GO_IDLE_STATE;
	idata.arg = arg;
	idata.flags = MMC_RSP_NONE | MMC_CMD_BC;
	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	return ret;
}

void testcase(int ret,int number)
{	
	if(ret==0)
    	printf("Test Case Passes :CMD%d \n",number);
	else if (ret==99)	
    	printf("INVALID CMD!! :CMD%d \n",number);
	else if (ret==101)	// skip reserved CMD
    	return;
	else
    	printf("Test Case Failled...!! :CMD%d \n",number);
}

int issue_cmd(int fd,int i)
{	
	__u8 ext_csd[512], ext_csd_rev, reg;
	int ret=99;
	// INVALID COMMAND
	switch (i)
	{
	case 0:
		/* code */ 
		//SUCCESS
		ret = issue_cmd0(fd);
		break;
	case 1:
		/* code */
		ret = set_single_cmd(fd, MMC_SEND_OP_COND, 0, 1, MMC_RSP_R3|MMC_RSP_SPI_R3|MMC_CMD_BCR);
		break;
	case 2:
		/* code */
		ret = set_single_cmd(fd, MMC_ALL_SEND_CID, 0, 1, MMC_RSP_R2|MMC_RSP_SPI_R2|MMC_CMD_BCR);
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
		//SUCCESS
		ret = read_extcsd(fd, ext_csd);
		break;
	case 9:
		/* code */
		ret = cmd9(fd);
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
		//SUCCESS
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
		//SUCCESS
		ret = cmd23(fd);
		break;
	case 24:
		/* code */
		ret = set_single_cmd(fd, 24, 1, 1, MMC_RSP_R2|MMC_RSP_SPI_R2|MMC_CMD_BCR);
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
		ret = do_general_cmd_read(fd);
		break;

	default:
		printf("CMD%d RESERVED\n",i);
		// For Reserved commands
		return 101;
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

#ifdef TEST
    for(i=1;i<=56;i++)	//Testing all Commands
    {
	// CMD 
		if(i==0)
		{
			continue;
		}
		else{
    		ret = issue_cmd(fd,i);
				testcase(ret,i);
		}
	}
#else
	i=CMD;
	ret = issue_cmd(fd,i);
		testcase(ret,i);
#endif
    close(fd);
}