/************************************
* Date created: 	10-8-2017
* Date finished: 	
* Editor: Sublime Text 3
* Author: Le Van Hoang Phuong
* Description: 
*************************************/
#include 	"history.h"
#define 	history_folder_dir 	"../raspberry/kernel/history/"
#define 	HIS_BUFF_LEN 		512 //1MB


void history(int *connfd, uint8_t addr, uint8_t fnc)
{
	fprintf(stderr, "%s\n", "> querying history from server ...");
		
	uint8_t *buff = (uint8_t*) malloc(HIS_BUFF_LEN);
	if(!recv(*connfd, buff, 1, 0))
	{
		fprintf(stderr, "%s\n", "> querying history failed");
		return;
	}

	uint8_t num_of_file = *buff;  //num of file in database
	if(!buff[0]) return;

	/*create an empty folder on admin PCto store databases*/
	emptyFolder(history_folder_dir);	//remove all existing files in accounts/
	/*write info to each file*/
	for (int i = 0; i < num_of_file; ++i)
	{
		/*file name*/
		recv(*connfd, buff, HIS_BUFF_LEN, 0);

		/*create file path*/
		adaptPath(history_folder_dir, buff);
		FILE *file = fopen(buff, "w");
		recv(*connfd, buff, HIS_BUFF_LEN, 0);
		
		/*write content to file*/
		fputs(buff, file);
		fflush(file);
		fclose(file);
	}
	free(buff);
	fprintf(stderr, "%s\n", "> querying history finished");
}