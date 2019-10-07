// Header file to define protoypes for all functions 
#ifndef PTU_H 
#define PTU_H

void ptu(unsigned short res, signed int min_az, signed int max_az, signed int min_ele,signed int max_ele,unsigned short n_sample, unsigned short freq);
void calculate_tilt(int*array,char *current_tilt, unsigned int i);
void calculate_heading(int*array,char* current_heading,unsigned int j); 
void split(int *array, int number,int length);
void itos(char* current_tilt, int* array, int length);

#endif


