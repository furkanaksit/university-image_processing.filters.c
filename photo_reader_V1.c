#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct pgm{
	int w;
	int h;
	int maxValue;
	unsigned char** gray_matrix;	
	}pgm;

int char2int(char* num){
	int i,sum;
	sum=0;
	for(i=0;i<strlen(num);i++){
		sum=sum*10+(num[i]-'0');
		}
	return sum;
	}
	
void convolution(pgm* old_pic, char** gray_m, int filt_size, int filt_idx, int divide_by, int* convo_matrix);	



int main(int argc, char *argv[])
{
	char* filename = argv[1];
	//char filename[20];
	//strcpy(filename, "fip343.pgm");
	//fp    is for the picture that is read
	//fp_w  is for the picture that will be written
	
	FILE *fp,*fp_w;
	char read[10];
	char write[10];
	
	pgm* pic_raw;	//for reading 
	pgm* pic_filtered; //for writing
	int i,j,temp;
	char w[10],h[10],maxValue[10],filename_filtered[50];
	unsigned char** sobel_graymatrix_x;
	unsigned char** sobel_graymatrix_y;
	char* name;		//name to add at the end of the filename
	char switch_case;	//to choose filter
	int* convo_matrix;	//the convolution matrix
	int filt_size;
	int filt_idx;
	int divide_by = 0;


	int gaussian_3x3[] = {1,2,1,
						2,4,2,
						1,2,1};
						
	int gaussian_5x5[] = 	{1,4,6,4,1,
						4,16,24,16,4,
						6,24,36,24,6,
						4,16,24,16,4,
						 1,4,6,4,1};
	
	int gaussian_7x7[] = {1,6,15,20,15,6,1,
					   6,36,90,120,90,36,6,
					 15,90,225,300,225,90,15,
					20,120,300,400,300,120,20,
					 15,90,225,300,225,90,15,
					   6,36,90,120,90,36,6,
					    1,6,15,20,15,6,1};

	int sobel_X[] = {-1,0,1,
				   -2,0,2,
				   -1,0,1};
	
	int sobel_Y[] = {-1,-2,-1,
					 0,0,0,
					 1,2,1 };
	
	int laplacian_3x3[] = {-1,-1,-1,
						 -1,8,-1,
						 -1,-1,-1};
	
	
	
	fp = fopen(filename, "rb");
	
	if (!fp) {
        printf("Error: Unable to open file %s.\n\n", filename);
        exit(1);
      }
    printf("Succesfully Opened\n");
    
    //create the structure for reading image
    pic_raw = (pgm*)malloc(sizeof(pgm));
    
    //create the new structure for writing filtered image
    pic_filtered = (pgm*)malloc(sizeof(pgm));
    
    //starting reading with header
    //p5 for pgm
    //w h 
    //maxValue
    
	printf("Starting to Read %s\n",filename);
	fscanf(fp,"%s", read);
		if(strcmp(read,"P5")==0){
			printf("Type is Correct\n");
			fscanf (fp, "%s", read);
        while (read[0] == '#') {
          //if a comment, get the rest of the line and a new word
          fgets (read, 255, fp);
		  fscanf (fp, "%s", read);
        }
			//width
			sscanf (read, "%d", &pic_raw->w);
			
				//copy it to the new file
				pic_filtered->w = pic_raw->w;
				//write to see it is right
				printf("width= %d ",pic_raw->w);
			
			//hight
			fscanf(fp, "%s", h);
				//save it in int form
				pic_raw->h=char2int(h);
				//copy it to the new file
				pic_filtered->h = pic_raw->h;
				//write to see it is right
				printf("height= %d ",pic_raw->h);
			
			//maximum gray value
			fscanf(fp, "%s", maxValue);
				//save it in int form
				pic_raw->maxValue=char2int(maxValue);
				//copy it to the new file
				pic_filtered->maxValue = pic_raw->maxValue;
				//write to see it is right
				printf("maxValue= %d \n",pic_raw->maxValue);
			
			pic_raw->gray_matrix=(char**)calloc(pic_raw->h,sizeof(char*));
			pic_filtered->gray_matrix=(char**)calloc(pic_filtered->h,sizeof(char*));
			
			for(i=0;i<pic_raw->h;i++){
				pic_raw->gray_matrix[i]= (char*)calloc(pic_raw->w,sizeof(char));
				pic_filtered->gray_matrix[i]=(char*)calloc(pic_filtered->w,sizeof(char));
				}
			
			getc(fp);
			for(i=0;i<pic_raw->h;i++){
				for(j=0;j<pic_raw->w;j++){
					temp=getc(fp);
					//printf("gray_matrix[%d][%d] = %d\n",i,j,temp);
					//getch();
					
					if(temp>pic_raw->maxValue){	
						pic_raw->gray_matrix[i][j]=pic_raw->maxValue;
					}else if(temp<0){
						pic_raw->gray_matrix[i][j]=0;
					}else{
						pic_raw->gray_matrix[i][j]=temp;
					}
				}
			}	
		}
	name = (char*)calloc(20,sizeof(char));
	
	printf("Which option will you choose:\n");
		printf("a) Gaussian 3x3 \n");
		printf("b) Gaussian 5x5 \n");
		printf("c) Gaussian 7x7 \n");
		printf("d) Sobel \n");
		printf("e) Laplacian \n");
		scanf("%c", &switch_case);

		switch (switch_case)
		{
			case 'a':
			 		//Gaussian 3x3
			 		convo_matrix = gaussian_3x3;
			 		filt_size = 3;
			 		filt_idx = (filt_size/2);
			 		for(i=0;i<filt_size*filt_size;i++){
						divide_by += gaussian_3x3[i];
						}
						
			 		convolution(pic_raw, pic_filtered->gray_matrix, filt_size, filt_idx, 16, convo_matrix);
			 		strcpy(name,"_gaussian_3x3");
			 		
			 		break;
			
			case 'b':
				{
					//Gaussian 5x5
					convo_matrix = gaussian_5x5;
					filt_size = 5;
			 		filt_idx = (filt_size/2);
			 		for(i=0;i<filt_size*filt_size;i++){
						divide_by += gaussian_5x5[i];
						}
					
					convolution(pic_raw, pic_filtered->gray_matrix, filt_size, filt_idx, divide_by, convo_matrix);
					strcpy(name,"_gaussian_5x5");
					
					break;
				}
			case 'c':
			 		//Gaussian 7x7
			 		convo_matrix = gaussian_7x7;
			 		filt_size = 7;
			 		filt_idx = (filt_size/2);
			 		for(i=0;i<filt_size*filt_size;i++){
						divide_by += gaussian_7x7[i];
						}
						
			 		convolution(pic_raw, pic_filtered->gray_matrix, filt_size, filt_idx, divide_by, convo_matrix);
			 		strcpy(name,"_gaussian_7x7");
			 		break;
			
			case 'd':
			 		//Sobel
			 		sobel_graymatrix_x = (char**)calloc(pic_raw->h,sizeof(char*));
					sobel_graymatrix_y = (char**)calloc(pic_raw->h,sizeof(char*));
			
					for(i=0;i<pic_raw->h;i++){
						sobel_graymatrix_x[i] = (char*)calloc(pic_raw->w,sizeof(char));
						sobel_graymatrix_y[i] = (char*)calloc(pic_raw->w,sizeof(char));
						}
			 		// for vertical edges
			 		convo_matrix = sobel_X;
			 		
			 		filt_size = 3;
			 		filt_idx = (filt_size/2);

					
					convolution(pic_raw, sobel_graymatrix_x, filt_size, filt_idx, 1, convo_matrix);	
					
					//for horizontal edges
					convo_matrix = sobel_Y;
						
					filt_size = 3;
			 		filt_idx = (filt_size/2);
					
					convolution(pic_raw, sobel_graymatrix_y, filt_size, filt_idx, 1, convo_matrix);	
					
					for(i=0;i<pic_filtered->h;i++){
						for(j=0;j<pic_filtered->w;j++){
							pic_filtered->gray_matrix[i][j]=sqrt(pow(sobel_graymatrix_x[i][j],2)+pow(sobel_graymatrix_y[i][j],2))/2;
							if(pic_filtered->gray_matrix[i][j]<30)
							pic_filtered->gray_matrix[i][j]=0;
						}
					}
					
						
					strcpy(name,"_sobel");	
						
			 		break;
			
			case 'e':
					//Laplacian
			 		convo_matrix = laplacian_3x3;
			 		filt_size = 3;
			 		filt_idx = (filt_size/2);
			 		
			 		convolution(pic_raw, pic_filtered->gray_matrix, filt_size, filt_idx, 1, convo_matrix);
			 		strcpy(name,"_laplacian");
			 		break;
				
				
				
			default:
					printf("Invalid choice\n");
					break;
		}
	
		strncpy(filename_filtered,filename,strlen(filename)-4);			//4 is .pgm at the end
		strcat(filename_filtered,name);
		strcat(filename_filtered,".pgm");

		fp_w = fopen(filename_filtered, "wb");
	
		if (!fp_w) {
			printf("Error: Unable to write to file %s.\n\n", filename_filtered);
			exit(1);
		}
		printf("Succesfully Written\n");
			//writing image to a pgm file
			
		fprintf(fp_w,"P5\n");
		sprintf(write,"%d %d\n",pic_filtered->w,pic_filtered->h);
		fputs(write,fp_w);
		sprintf(write,"%d\n",pic_filtered->maxValue);
		fputs(write,fp_w);
		
		
		for(i=0;i<pic_filtered->h;i++){
				for(j=0;j<pic_filtered->w;j++){
				putc(pic_filtered->gray_matrix[i][j],fp_w);
				}
			}
		
		fclose(fp);
		fclose(fp_w);	
			
		//---------------------------------------------------------------------------
	
	
	return 0;
}

void convolution(pgm* old_pic, char** gray_m, int filt_size, int filt_idx, int divide_by, int* convo_matrix){
	
	int i,j,k,l,m,sum;	
	
	
	//copy the edges
	for(j=0;j<old_pic->w;j++){
		
		gray_m[0][j] = old_pic->gray_matrix[0][j];
		gray_m[old_pic->h-1][j] = old_pic->gray_matrix[old_pic->h-1][j];
		}			
	for(i=1;j<old_pic->h-1;i++){												//7x7 lik için kontrol et
		
		gray_m[i][0] = old_pic->gray_matrix[i][0];
		gray_m[i][old_pic->w-1] = old_pic->gray_matrix[i][old_pic->w-1];
		}		
		
		
																		//rest of the convolution
	for(i=(filt_idx);i<(old_pic->h)-(filt_idx);i++){
		for(j=(filt_idx);j<(old_pic->w)-(filt_idx);j++){
			//printf("i=%d j=%d \n",i,j);
			// for every pixel
			sum = 0;
			m = 0;														//convo_matrix idx
			for(k=i-(filt_idx);k<i+(filt_idx)+1;k++){
				for(l=j-(filt_idx);l<j+(filt_idx)+1;l++){
					
					sum += old_pic->gray_matrix[k][l]*convo_matrix[m];
					//printf("sum= %d,gray(x,y)= %d,convo(x,y)=%d, m= %d\n",sum,old_pic->gray_matrix[k][l],convo_matrix[m],m);
					m++;
					
					}
				}
			//printf("sum=%d\n",sum);
			//if(sum<=0){
			//printf("i= %d,j= %d",i,j);
			//printf("sum=%d\n",sum);
			//getch();}
			if(sum/divide_by>255){
				gray_m[i][j] = 255;
				}else if(sum/divide_by<0){
						gray_m[i][j] = 0;
					}else{ 	
			gray_m[i][j]=sum/divide_by;
				}
				
			}
		}
	
	
	}
	
	
