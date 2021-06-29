#include <staio.h>
#include <stdlib.h>

int main(void)
{
	FILE * fp = NULL;
	int ret;

	if(NULL==(fp=fopen("./testApp.c","r")))
	{
		perror("fopen error");
		exit(-1);
	}
	
	printf("文件打开成功\r\n");

	if(0>fseek(fp,0,SEEK_END))
	{
		perror("fseek  error");
		fclose(fp);
		exit(-1);
	}

	if(0>(ret=ftell(fp)))
	{
		perror("ftell error");
		fclose(fp);
		exit(-1);
	}

	printf("文件大小：%d 个字节\n",ret);

	fclose(fp);
	exit(0);
}
