#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>

typedef struct pgm{
    char *filename;
    char *format;
	int w;
	int h;
	int maxValue;
	unsigned char** gray_matrix;	
	}pgm;

typedef struct filter{
	int size;
	double** filterM;	
	}filter;

typedef struct sobel{
    filter* Gx;
    filter* Gy;
	}sobel;
    
pgm *laplacian(pgm *image);
pgm *canny(pgm *image);
void sobelf(pgm* image);
pgm *gauss(pgm *image);
int char2int(char* num);
char *int2char(int num);
pgm* convolution(pgm* originalImage, filter* filterKernel);
filter* gaussian(int size, int sigma);
sobel* extendedSobel(int size);
filter *LoG(int size, int sigma);
pgm* imread(char* filename);
void imwrite(pgm* image);


int main(int argc, char *argv[])
{
    DIR *folder;
    struct dirent *entry;

    pgm *image = (pgm*)malloc(sizeof(pgm));
    pgm *cannyImage;
    pgm *laplacianImage;

    folder = opendir("./image_samples");
    if(folder != NULL){
        while(entry = readdir(folder)){
            image = imread(entry->d_name);
            cannyImage = canny(image);
            laplacianImage = laplacian(image);
            imwrite(cannyImage);
            imwrite(laplacianImage);
        }
    }
    

    return 0;
}
pgm *laplacian(pgm *image){
    char filterName[] = "laplacian";
    filter *laplaceKernel;
    pgm *newImage;

    laplaceKernel = LoG(5,1);
    newImage = convolution(image, laplaceKernel);
    strcat(newImage->filename, filterName);
    return newImage;
}

pgm *canny(pgm *image){
    char filterName[] = "canny";
    filter *gaussFilter = gaussian(5,1);
    pgm *newImage;

    newImage = convolution(image, gaussFilter);
    sobelf(newImage);
    strcat(newImage->filename, filterName);
    return newImage;
}
pgm *gauss(pgm *image){
    char filterName[] = "gaussian";
    filter *gaussFilter = gaussian(5,1);
    pgm *newImage;

    newImage = convolution(image, gaussFilter);
    strcat(newImage->filename, filterName);
    return newImage;

}

void sobelf(pgm *image){
    int i, j;
    int max = 0;
    pgm *img_x, *img_y;
    sobel *filters = extendedSobel(5);
    img_x = convolution(image,filters->Gx);
    img_y = convolution(image,filters->Gy);
    for(i = 0; i < image->w; i++){
        for(j= 0; j < image->h; j++){
            image->gray_matrix[i][j] = sqrt( pow(img_x->gray_matrix[i][j],2) + pow(img_y->gray_matrix[i][j],2) );
            if(image->gray_matrix[i][j] >= max){
                max = image->gray_matrix[i][j];
            }
        }
    } 
    for(i = 0; i < image->w; i++){
        for(j= 0; j < image->h; j++){
            image->gray_matrix[i][j] = (image->gray_matrix[i][j]*image->maxValue)/max;
        }
    }  
}

int char2int(char* num){
    // .pgm files are constructed using char. So in order to see the intensity values of grayscale image we need to convert it to integer.

    int sum = 0;
    int i = 0;
    int len = strlen(num);
    for(i = len-1; i >= 0; i-- ){
        sum = 10*sum + num[i];
        }

	return sum;
	}

char* int2char(int num){
    // to write .pgm files we need opposite form of char2int function
    char *pWord;
    char word[] = "";
    char integer[2];
    int coeff = 1;
    
    integer[1] = '\0';
    
    while(num / coeff != 0){
        coeff *= 10;
    }
    while(coeff != 0){
        coeff /= 10;
        integer[0] = (char)((num / coeff) + 48);
        strcat(word, integer);
        num = num % coeff;
    }
    pWord = word;
    return pWord;

}

pgm* convolution(pgm* originalImage, filter* filterKernel){
    pgm* newImage = (pgm*)malloc(sizeof(pgm));
    
    int i = 0;
    int j = 0;
    int ki = 0;
    int kj = 0;
    int kernelSize = filterKernel->size;
    int sum = 0;
    int max= 0;
    
    strcpy(newImage->filename, originalImage->filename);
    newImage->w = originalImage->w;
    newImage->h = originalImage->h;
    newImage->maxValue = originalImage->maxValue;
    
    newImage->gray_matrix = (unsigned char**)calloc(newImage->h, sizeof(unsigned char*));
    unsigned char** applied = (unsigned char**)calloc(newImage->h + 2, sizeof(unsigned char*));

    for(i = 0; i < newImage->h; i++){
        applied[i] = (char*)calloc(newImage->w,sizeof(unsigned char));
        }
    
    
    for(i = 0; i < newImage->h +2; i++){
        applied[i] = (unsigned char*)calloc(newImage->w + 2,sizeof(unsigned char));
        for (j = 1; j < newImage->w + 1; j++){
            applied[i][j] = originalImage->gray_matrix[i][j];
            }
        }
    

    for(i = 0; i < newImage->w; i++){
        for(j= 0; j < newImage->h; j++){
            for(ki = 0; ki < kernelSize; ki++){
                for(kj = 0; kj < kernelSize; kj++){
                    sum += filterKernel->filterM[ki][kj] * applied[i + ki][j +kj];
                    }
                }
            newImage->gray_matrix[i][j] = sum;
            if(sum > max){
                max = sum;
            }
            sum = 0;
            }
        }
    for(i = 0; i < newImage->w; i++){
        for(j= 0; j < newImage->h; j++){
            newImage->gray_matrix[i][j] = (newImage->gray_matrix[i][j]*newImage->maxValue)/max;
        }
    }  
    return newImage;
    }

filter* gaussian(int size, int sigma){ 
    // Calculates gaussian kernel for smoothing. Uses formula to calculate any shape of kernel.
    // We use filter struct two return the filter.
    int i = 0;
    int j = 0;

    filter* gauss;
    gauss = (filter*)malloc(sizeof(filter));

    double sum = 0;
    signed int org = (int)(floor(size/2));
    double** filter;
    int X[size][size];
    int Y[size][size];

    filter = (double**)malloc(sizeof(double*)*size);
    for(i = 0; i < size; i++){
        filter[i] = (double*)malloc(sizeof(double)*size);
    }

    for (i = 0; i < size; i++){
        for(j = -1*org; j <= org; j++){
            X[i][org+j] = j;
            Y[org+j][i] = j;
        }
    }

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            filter[i][j] = exp((-1*(pow(X[i][j],2) + pow(Y[i][j],2)))/(2*sigma*sigma));
            sum += filter[i][j];
            //printf("\n%f\n", sum);
        }
    }
    printf("\n%f\n", sum);
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            filter[i][j] /= sum;    
        }
    }

    gauss->size = size;
    gauss->filterM = filter;
    return gauss;
    }


sobel* extendedSobel(int size){ 
    // Sobel operator has two kernel for horizontal and vertical edge detection.
    // For that reason we use another struct with two filter.
    int i = 0;
    int j = 0;

    sobel* filters;
    filters = (sobel*)malloc(sizeof(sobel));

    signed int org = (int)(floor(size/2));
    
    filter *Gx, *Gy;

    // -----------------------------------------------------

    Gx = (filter*)malloc(sizeof(filter));
    
    Gx->size = size;

    double **sobel_Gx;
    sobel_Gx = (double**)calloc(size,sizeof(double*));
    for(i = 0; i < size; i++){
        sobel_Gx[i] = (double*)calloc(size, sizeof(double));
    }
    sobel_Gx[0][0] = 1;
    sobel_Gx[size-1][0] = 1;
    sobel_Gx[org][0] = 2;
    sobel_Gx[0][size-1] = -1;
    sobel_Gx[size-1][size-1] = -1;
    sobel_Gx[org][size-1] = -2;
    
    

    Gx->filterM = sobel_Gx;

    // ----------------------------------------------------

    Gy = (filter*)malloc(sizeof(filter));

    Gy->size = size;

    double **sobel_Gy;
    sobel_Gy = (double**)calloc(size, sizeof(double*));
    for(i = 0; i < size; i++){
        sobel_Gy[i] = (double*)calloc(size, sizeof(double));
    }

    sobel_Gy[0][0] = 1;
    sobel_Gy[0][size-1] = 1;
    sobel_Gy[0][org] = 2;
    sobel_Gy[size-1][0] = -1;
    sobel_Gy[size-1][size-1] = -1;
    sobel_Gy[size-1][org] = -2;

    Gy->filterM = sobel_Gy;

    filters->Gx = Gx;
    filters->Gy = Gy;

    return filters;
    }

filter *LoG(int size, int sigma){
    int i = 0;
    int j = 0;

    filter* laplacian;
    laplacian = (filter*)malloc(sizeof(filter));

    double sum = 0;
    signed int org = (int)(floor(size/2));
    double** filter;
    int X[size][size];
    int Y[size][size];
    
    filter = (double**)malloc(sizeof(double*)*size);
    for(i = 0; i < size; i++){
        filter[i] = (double*)malloc(sizeof(double)*size);
    }

    for (i = 0; i < size; i++){
        for(j = -1*org; j <= org; j++){
            X[i][org+j] = j;
            Y[org+j][i] = j;
        }
    }

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){            
            filter[i][j] = ((-1)/(M_PI*pow(sigma,4)))*(1-((pow(X[i][j],2)+pow(Y[i][j],2))/2*pow(sigma,2)))*exp((-1*(pow(X[i][j],2) + pow(Y[i][j],2)))/(2*pow(sigma,2)));
            sum += filter[i][j];
        }
    }

    laplacian->size = size;
    laplacian->filterM = filter;
    return laplacian;
}



pgm* imread(char* filename){
    FILE* fp;
    
    char buff[255];
    char dimensionW[10];
    char dimensionH[10];
    
    char charBuffer;
    char max = NULL;
    
    pgm *image;
    image = (pgm*)malloc(sizeof(pgm));
    image->filename = (char*)malloc(sizeof(char)*(strlen(filename)+1));
    strcpy(filename,image->filename);

    int i = 0;
    int j = 0;

    fp = fopen(filename,"r");
    if(fp != NULL){

        fgets(buff,255,fp);
        if (strcmp(buff,"P5")==0)
        {
            image->format = (char*)malloc(sizeof(char)*(strlen(buff)+1));
            strcpy(buff,image->format);
            fgets(buff,255,fp);
            while(buff[0]=='#'){
                fgets(buff,255,fp);
            }

            
            image->w = char2int(strtok(buff," "));
            image->h = char2int(strtok(NULL, " "));

            fgets(buff,255,fp);
            image->maxValue = char2int(buff);
            image->gray_matrix = (unsigned char**)calloc(image->h,sizeof(char*));
            for(i = 0; i < image->h; i++){
                image->gray_matrix[i] = (unsigned char*)calloc(image->w,sizeof(char));
            }
            for(i = 0; i < image->h; i++){
                for(j = 0; j < image->w; j++){
                    charBuffer = getc(fp);
                    if(charBuffer > max){
                        max = charBuffer;
                    }
                    image->gray_matrix[i][j] = charBuffer;
                }
            }
            // we need to normalize it just in case if there are bigger values than maxValue
            for(i = 0; i < image->h; i++){
                for(j = 0; j < image->w; j++){
                    image->gray_matrix[i][j] = (image->gray_matrix[i][j]*image->maxValue)/max;
                }
            }
        }
    }
    fclose(fp);
    return image;
}


void imwrite(pgm* image){
    FILE* fp;
    int i, j;

    fp = fopen(image->filename, "w");
    fprintf(fp,"%s\n%s %s\n%s\n", image->format , int2char(image->w), int2char(image->h), int2char(image->maxValue));
    for(i = 0; i < image->h; i++){
        for(j = 0; j < image->w; j++){
            putc(image->gray_matrix[i][j],fp);
        }
    }
}
