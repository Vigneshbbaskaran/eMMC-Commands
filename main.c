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
#define CMD 19

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
{   
	char frame[512];
	struct mmc_ioc_cmd ioc;
	memset(&ioc, 0, sizeof(ioc));
	int ret=0;
	mmc_ioc_cmd_set_data((ioc), &frame);
	ioc.opcode = opcode;
	ioc.write_flag = write_flag;
	ioc.arg = 0x0;
	ioc.blksz = 512;
	ioc.blocks = blocks;
	ioc.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	ret = ioctl(fd, MMC_IOC_CMD, &ioc);
	if (ret)
		perror("ioctl");
	else{
		if(opcode==18 || opcode==17)                                                                                                                                
        {                                                                                                                                             
        	printf("read data:\n");                                                                         
        		for(int i=0;i<512;i++)                                                                          
                	printf("%c",frame[i]);                                                                  
        	printf("\n");                                                                                       
        }
	}
	return ret;
}


static  int cmd1(int fd, __u32 opcode, int write_flag, unsigned int blocks,unsigned int flags)
{  
		char frame[512];
		struct mmc_ioc_cmd ioc;
		int ret=0;
	while(1)
	{
		memset(&ioc, 0, sizeof(ioc));
		mmc_ioc_cmd_set_data((ioc), &frame);
		ioc.opcode = opcode;
		ioc.arg = 0x40300000;
		ioc.flags = flags;
		ret = ioctl(fd, MMC_IOC_CMD, &ioc);
		if(ret==0)
			printf("IOCL SUCCESS\n");	                                                                                                                                                                                                                                                                      
     	printf("read response{0}:0x%08x\n",ioc.response[0]);                                                                                         
       	printf("\n");
		if(ioc.response[0] & 0x1<<31)
		{
			printf("Card Power up: SUCCESS\n");
			break;
		}
	}                                                          
	return ret;
}
static  int cmd2(int fd, __u32 opcode, int write_flag, unsigned int blocks,unsigned int flags)
{   
	
		char frame[512];
		struct mmc_ioc_cmd ioc;
		memset(&ioc, 0, sizeof(ioc));
		int ret=0;
		mmc_ioc_cmd_set_data((ioc), &frame);
		ioc.opcode = opcode;
		ioc.arg = 0x000;
		ioc.flags = flags;
		ret = ioctl(fd, MMC_IOC_CMD, &ioc);
		if(ret==0)
			printf("IOCL SUCCESS\n");	                                                                                                                                                                                                                                                                      
     	printf("CMD2\n");
		for(int i=0;i<4;i++)
		{
			printf("read response{%d}:0x%08x\n",i,ioc.response[0]);                                                                                         
       		printf("\n");
		}                                                         
		return ret;
}
static  int set_single_cmd_wrt(int fd, __u32 opcode, int write_flag, unsigned int blocks,unsigned int flags)
{   
	char frame[512]="Vignesh Baskran..........checking.......Write.............CMD-testing...........Read................................................";
	struct mmc_ioc_cmd ioc;
	memset(&ioc, 0, sizeof(ioc));
	int ret=0;
	mmc_ioc_cmd_set_data((ioc), &frame);
	ioc.opcode = opcode;
	ioc.write_flag = write_flag;
	ioc.arg = 0x0;
	ioc.blksz = 512;
	ioc.blocks = blocks;
	ioc.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	ret = ioctl(fd, MMC_IOC_CMD, &ioc);
	if (ret)
		perror("ioctl");
	else{
		if(opcode==18)                                                                                                                                
        {                                                                                                                                             
        printf("read data:\n");                                                                         
        	for(int i=0;i<512;i++)                                                                          
                printf("%c",frame[i]);                                                                  
        printf("\n");                                                                                       
        }
	}
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

int cmd15(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = 15;
	idata.arg = 0;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	/* Kernel will set cmd_timeout_ms if 0 is set */

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");
	return ret;
}
int cmd16(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = MMC_SET_BLOCKLEN;
	idata.arg = 512;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	/* Kernel will set cmd_timeout_ms if 0 is set */

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");
	return ret;
}
int cmd12(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = 12;
	idata.arg = 512;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	/* Kernel will set cmd_timeout_ms if 0 is set */

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");
	return ret;
}
int cmd3(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = MMC_SET_RELATIVE_ADDR;
	idata.arg = 0;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	/* Kernel will set cmd_timeout_ms if 0 is set */

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");
	for(int i=0;i<4;i++)
		{
			printf("read response{%d}:0x%08x\n",i,idata.response[0]);                                                                                         
       		printf("\n");
		}                                                         
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

	mmc_ioc_cmd_set_data((idata), &frame);
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

int sleep_cmd(int fd ,int arg)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = MMC_SLEEP_AWAKE;
	idata.arg = arg;
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
		for(int i=0;i<4;i++)
		{
			printf("read response{%d}:0x%08x\n",i,idata.response[0]);                                                                                         
       		printf("\n");
		}   
	return ret;
}

int cmd6(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = 6;
	idata.arg = 0x03B90100;
	idata.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;
	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
	{
		perror("ioctl");
		printf("ret:%d",ret);
	}
	for(int i=0;i<4;i++)
	{
			printf("read response{%d}:0x%08x\n",i,idata.response[0]);                                                                                         
       		printf("\n");
	} 
	return ret;
}

int cmd7(int fd)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = 7;
	idata.arg = 0;
	idata.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;
	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
	{
		perror("ioctl");
		printf("ret:%d",ret);
	}
	for(int i=0;i<4;i++)
	{
			printf("read response{%d}:0x%08x\n",i,idata.response[0]);                                                                                         
       		printf("\n");
	} 
	return ret;
}

int send_status(int fd)
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

		for(int i=0;i<4;i++)
		{
			printf("read response{%d}:0x%08x\n",i,idata.response[0]);                                                                                         
       		printf("\n");
		}   

	return ret;
}

static int issue_cmd0(int fd)
{
	struct mmc_ioc_cmd idata;
    __u32 arg=0xf0f0f0f0;
    int ret;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = MMC_GO_IDLE_STATE;
	idata.arg = 0;
	idata.flags = MMC_RSP_NONE | MMC_CMD_BC;
	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	return ret;
}

static int cmd55(int fd)
{
	struct mmc_ioc_cmd idata;
    __u32 arg=0;
    int ret;
	memset(&idata, 0, sizeof(idata));
	idata.opcode = 55;
	idata.arg = (1 << 16);
	idata.flags =MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	ret = ioctl(fd, MMC_IOC_CMD, &idata);

	return ret;
}

static int cmd19(int fd)
{
	struct mmc_ioc_cmd idata;
    __u32 arg=0;
    int ret;
	char frame[512]="Vignesh Baskran..........checking.......Write.............CMD-testing...........Read................................................";
	mmc_ioc_cmd_set_data((idata), &frame);
	memset(&idata, 0, sizeof(idata));
	idata.write_flag = 1;
	idata.opcode = MMC_BUSTEST_W;
	idata.arg = 0;
	idata.flags =MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	
	return ret;
}

static int cmd14(int fd)
{
	struct mmc_ioc_cmd idata;
    __u32 arg=0;
    int ret;
	memset(&idata, 0, sizeof(idata));
	idata.write_flag = 0;
	idata.opcode = MMC_BUSTEST_R;
	idata.arg = 0;
	idata.flags =MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	
	return ret;
}

static int erase(int fd,int opcode,int arg)
{
	struct mmc_ioc_cmd idata;
    
    int ret;
	memset(&idata, 0, sizeof(idata));
	idata.write_flag = 0;
	idata.opcode = opcode;
	idata.arg = 0;
	if (opcode == 38)
		idata.flags =MMC_RSP_R1B | MMC_RSP_SPI_R1B | MMC_CMD_AC;
	else
		idata.flags =MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	return ret;
}

static int set_write_protect(int fd, __u32 blk_addr, int opcode)
{
	int ret = 0;
	struct mmc_ioc_cmd idata;

	memset(&idata, 0, sizeof(idata));
	idata.write_flag = 1;
	if (opcode==28 || opcode==29)
	{
		idata.opcode =opcode;
		idata.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;
		idata.arg = blk_addr;
	}
	else{
		idata.opcode = opcode;
		idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	}
	

	ret = ioctl(fd, MMC_IOC_CMD, &idata);
	if (ret)
		perror("ioctl");

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
		ret = cmd1(fd, MMC_SEND_OP_COND, 0, 1, MMC_RSP_R3|MMC_RSP_SPI_R3|MMC_CMD_BCR);
		break;
	case 2:
		/* code */
		ret = cmd2(fd, MMC_ALL_SEND_CID, 0, 1, MMC_RSP_R2|MMC_RSP_SPI_R2|MMC_CMD_BCR);
		break;
	case 3:
		/* code */
		ret = cmd3(fd);
		break;
	case 4:
		/* code */
		break;
	case 5:
		/* code */
		ret = sleep_cmd(fd,1<<15);
		if(ret==0)
			printf("Device goes to sleep Mode..");
		else
			break;
		ret = sleep_cmd(fd,0);
		if(ret==0)
			printf("Device Now Awaken..");
		break;
	case 6:
		/* code */
		ret = cmd6(fd);
		break;
	case 7:
		/* code */
		ret = cmd7(fd);
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
		ret = set_single_cmd(fd, MMC_READ_DAT_UNTIL_STOP, 0, 1,1);
		break;
	case 12:
		/* code */
		ret = cmd12(fd);
		break;
	case 13:
		/* code */
		//SUCCESS
		ret = send_status(fd);
		break;
	case 14:
		/* code */
	
	ret = cmd14(fd);
		if(ret==0)
			{
				printf("TEST BUS DATA RECIEVED:\n");
			}
		else
			break;
		break;
	case 15:
		/* code */
		ret = cmd15(fd);
		break;
	case 16:
		/* code */
		//SUCCESS
		ret = cmd16(fd);
		break;
	case 17:
		/* code */
		//SUCCESS
		printf("Read command\n");
		ret = set_single_cmd(fd, MMC_READ_SINGLE_BLOCK, 0, 1,1);
		break;
	case 18:
		/* code */
		//Success
		ret = set_single_cmd(fd, MMC_READ_MULTIPLE_BLOCK, 0, 1,1);
		break;
	case 19:
		/* code */
	ret = cmd19(fd);
		if(ret==0)
			{
				printf("TEST BUS DATA SEND:\n");
				
			}
		else
			break;
		break;
	case 20:
		/* code */
		ret = set_single_cmd_wrt(fd, MMC_WRITE_DAT_UNTIL_STOP, 1, 1, 1);
		break;
	case 23:
		/* code */
		//SUCCESS
		ret = cmd23(fd);
		break;
	case 24:
		/* code */
		//Success
		ret = set_single_cmd_wrt(fd, MMC_WRITE_BLOCK, 1, 1, 1);
		break;
	case 25:
		/* code */
		//Success
		ret = set_single_cmd_wrt(fd, MMC_WRITE_MULTIPLE_BLOCK, 1, 1, 1);
		break;
	case 26:
		/* code */

		// Dont Try NOW
		break;
	case 27:
		/* code */

		// Dont Try NOW
		break;
	case 28:
	case 29:
		//SUCCESS
		/* code */
		ret = set_write_protect(fd,1,MMC_SET_WRITE_PROT);	
		/* code */
		ret = set_write_protect(fd,1,MMC_CLEAR_WRITE_PROT);
		break;
	case 30:
		/* code */
		//SUCCESS
		ret = set_write_protect(fd,1,MMC_SEND_WRITE_PROT);
		break;
	case 31:
		/* code */
		//SUCCESS
		ret = set_write_protect(fd,1,MMC_SEND_WRITE_PROT_TYPE);
		break;
	case 35:
		/* code */
		//SUCCESS
		printf("cmd35\n");
		ret = erase(fd,35,0);
		break;
	case 36:
		/* code */
		//SUCCESS
		ret = erase(fd,36,1);
		break;
	case 38:
		/* code */
		//SUCCESS
		ret = erase(fd,38,0);
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
		ret = cmd55(fd);
		break;
	case 56:
		/* code */
		//Success
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
    for(i=0;i<=3;i++)	//Testing all Commands
    {
	// CMD 
    		ret = issue_cmd(fd,i);
				testcase(ret,i);
	}
#else
	i=CMD;
	ret = issue_cmd(fd,i);
		testcase(ret,i);

#endif
    close(fd);
}
