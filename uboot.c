typedef unsigned int (copy_sd_mmc_to_mem) (unsigned int channel,unsigned int start_black,unsigned char block_size,unsigned int * trg, unsigned int init);
cpoy_sd_mmc_to_mem copy_bl2 = (copy_sd_mmc_to_mem) (*(unsigned int *)(0xD0037F98));
copy_bl2(x,x,x,x,x);




