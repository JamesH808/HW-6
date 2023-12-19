/*********************************************************************/
/* PhotoLab.c: Assignment 6 for CENG231, Spring 2023                 */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*** global definitions ***/
#define WIDTH  500		/* image width */
#define HEIGHT 281		/* image height */
#define SLEN    80		/* maximum length of file names */

/*** function declarations ***/

/* Load an image from a file */
int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], 
        unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* Save a processed image */
int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], 
        unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* Age the image */
void Aging(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], 
		unsigned char B[WIDTH][HEIGHT]);

/* Change a color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], 
        unsigned char B[WIDTH][HEIGHT]);

/* Reverse the image color */
void Negative(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], 
        unsigned char B[WIDTH][HEIGHT]);

/* Flip an image vertically */
void VFlip(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], 
        unsigned char B[WIDTH][HEIGHT]);

/* Mirror an image horizontally */
void HMirror(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], 
        unsigned char B[WIDTH][HEIGHT]);

/* Sharpen an image */
void Sharpen(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], 
        unsigned char B[WIDTH][HEIGHT]);

/* exchange R and B color channels */
void ExRB(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT]);

/* [Bonus] Add salt-and-pepper noise to image */
void AddNoise(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT], int percentage);

/* [Bonus] Add a border to the image */
void AddBorder(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], 
		unsigned char B[WIDTH][HEIGHT], int r, int g, int b, int bwidth);

/* Test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], 
        unsigned char B[WIDTH][HEIGHT]);

int main(void)
{
    /* Two dimensional arrays to hold the current image data, */
    /* one array for each color component.                    */
    unsigned char   R[WIDTH][HEIGHT];
    unsigned char   G[WIDTH][HEIGHT];
    unsigned char   B[WIDTH][HEIGHT];

    /* Please extend the AutoTest function for the other DIP operations */
    AutoTest(R, G, B);

    return 0;
}

int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char Type[SLEN];
    int  Width, Height, MaxValue;
    int  x, y;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "r");
    if (!File) {
        printf("\nCannot open file \"%s\" for reading!\n", fname);
        return 1;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
        printf("\nUnsupported file format!\n");
        return 2;
    }
    fscanf(File, "%d", &Width);
    if (Width != WIDTH) {
        printf("\nUnsupported image width %d!\n", Width);
        return 3;
    }
    fscanf(File, "%d", &Height);
    if (Height != HEIGHT) {
        printf("\nUnsupported image height %d!\n", Height);
        return 4;
    }
    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255) {
        printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return 5;
    }
    if ('\n' != fgetc(File)) {
        printf("\nCarriage return expected!\n");
        return 6;
    }
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            R[x][y] = fgetc(File);
            G[x][y] = fgetc(File);
            B[x][y] = fgetc(File);
        }
    }
    if (ferror(File)) {
        printf("\nFile error while reading from file!\n");
        return 7;
    }
    printf("%s was read successfully!\n", fname_ext);
    fclose(File);
    return 0;
}

int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];
    char SysCmd[SLEN * 5];
    int  x, y;

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "w");
    if (!File) {
        printf("\nCannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", WIDTH, HEIGHT);
    fprintf(File, "255\n");

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            fputc(R[x][y], File);
            fputc(G[x][y], File);
            fputc(B[x][y], File);
        }
    }

    if (ferror(File)) {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved successfully. \n", fname_ext);

    /*
     * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
     * and make it world readable
     */
    sprintf(SysCmd, "~ceng231/bin/pnmtojpeg.sh %s", fname_ext);
    if (system(SysCmd) != 0) {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored for viewing. \n", fname);

    return 0;
}

/* Example: age the image to make it look like old picture */
void Aging(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    int x, y;

    for( y = 0; y < HEIGHT; y++ ) {
        for( x = 0; x < WIDTH; x++ ) {
	    B[x][y] = ( R[x][y]+G[x][y]+B[x][y] )/5;
            R[x][y] = (unsigned char) (B[x][y]*1.6);
            G[x][y] = (unsigned char) (B[x][y]*1.6);
	    }
    }
}

/* Test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{

    LoadImage("wilson", R, G, B);
    Aging(R, G, B);
    SaveImage("aging", R, G, B);
    printf("Aging tested!\n\n");

    /* add more tests here */
    LoadImage("wilson", R, G, B);
    BlackNWhite(R, G, B);
    SaveImage("bw", R, G, B);
    printf("BW tested!\n\n");

    LoadImage("wilson", R, G, B);
    Negative(R, G, B);
    SaveImage("negative", R, G, B);
    printf("Negative tested!\n\n");

    LoadImage("wilson", R, G, B);
    VFlip(R, G, B);
    SaveImage("vflip", R, G, B);
    printf("vflip tested!\n\n");

    LoadImage("wilson", R, G, B);
    HMirror(R, G, B);
    SaveImage("hmirror", R, G, B);
    printf("hmirror tested!\n\n");

    LoadImage("wilson", R, G, B);
    Sharpen(R, G, B);
    SaveImage("sharpen", R, G, B);
    printf("sharpen tested!\n\n");
    
    LoadImage("wilson", R, G, B);
    ExRB(R, G, B);
    SaveImage("xRB", R, G, B);
    printf("xRB tested!\n\n");
    
    LoadImage("wilson", R, G, B);
    AddBorder(R, G, B, 0, 0, 0, 20);
    SaveImage("border", R, G, B);
    printf("add border tested!\n\n"); 
}

/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/

void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
  int x, y, average;

    for( y = 0; y < HEIGHT; y++ ) {
        for( x = 0; x < WIDTH; x++ ) {
	  average = (R[x][y] + G[x][y] + B[x][y])/3;
	  R[x][y] = average;
	  G[x][y] = average;
	  B[x][y] = average;
            }
    }
}


void Negative(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
  int x, y;

    for( y = 0; y < HEIGHT; y++ ) {
        for( x = 0; x < WIDTH; x++ ) {
          R[x][y] = 255 - R[x][y];
          G[x][y] = 255 - G[x][y];
          B[x][y] = 255 - B[x][y];
            }
    }
}



void VFlip(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
  int x, y, temp;


  for( y = 0; y < HEIGHT/2; y++ ) {
        for( x = 0; x < WIDTH; x++ ) {
          temp = R[x][y];
	  R[x][y] = R[x][HEIGHT - (y + 1)];
	  R[x][HEIGHT - (y + 1)] = temp;

	  temp = G[x][y];
          G[x][y] = G[x][HEIGHT - (y + 1)];
          G[x][HEIGHT - (y + 1)] = temp;

	  temp = B[x][y];
          B[x][y] = B[x][HEIGHT - (y + 1)];
          B[x][HEIGHT - (y + 1)] = temp;
	  
            }
    }
}


void HMirror(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
  {
  int x, y;

    for( y = 0; y < HEIGHT; y++ ) {
        for( x = 0; x < WIDTH/2; x++ ) {
          R[WIDTH -(x+1)][y] = R[x][y];
	  G[WIDTH -(x+1)][y] = G[x][y];
	  B[WIDTH -(x+1)][y] = B[x][y];
            }
    }
  }

void Sharpen(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
  int x,y;
  int Rsum[WIDTH][HEIGHT], Gsum[WIDTH][HEIGHT], Bsum[WIDTH][HEIGHT];

  
  for( y = 1; y < HEIGHT - 1; y++ ) {
        for( x = 1; x < WIDTH - 1; x++ ) {

	  Rsum[x][y] = 9 * R[x][y] - R[x-1][y-1] - R[x-1][y] - R[x-1][y+1] - R[x][y-1] - R[x][y+1] - R[x+1][y-1] - R[x+1][y] - R[x+1][y+1];
	  Gsum[x][y] = 9 * G[x][y] - G[x-1][y-1] - G[x-1][y] - G[x-1][y+1] - G[x][y-1] - G[x][y+1] - G[x+1][y-1] - G[x+1][y] - G[x+1][y+1];
	  Bsum[x][y] = 9 * B[x][y] - B[x-1][y-1] - B[x-1][y] - B[x-1][y+1] - B[x][y-1] - B[x][y+1] - B[x+1][y-1] - B[x+1][y] - B[x+1][y+1];
	    if(Rsum[x][y] > 255){
	      Rsum[x][y] = 255;
		}
	    if(Rsum[x][y] < 0){
	      Rsum[x][y] = 0;
	    }
	    if(Gsum[x][y] > 255){
	      Gsum[x][y] = 255;
	    }
	    if(Gsum[x][y] < 0){
	      Gsum[x][y] = 0;
	    }
	    if(Bsum[x][y] > 255){
	      Bsum[x][y] = 255;
	    }
	    if(Bsum[x][y] < 0){
	      Bsum[x][y] = 0;
	      }
	    
	}
	}
  for( y = 1; y < HEIGHT - 1; y++ ) {
        for( x = 1; x < WIDTH - 1; x++ ) {
	  R[x][y] = (unsigned char)Rsum[x][y];
	  G[x][y] = (unsigned char)Gsum[x][y];
	  B[x][y] = (unsigned char)Bsum[x][y];
	}
  }
}
	

void ExRB(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
  int x, y, temp;

    for( y = 0; y < HEIGHT; y++ ) {
        for( x = 0; x < WIDTH; x++ ) {
          temp = R[x][y];
          R[x][y] = B[x][y];
          B[x][y] = temp;
            }
    }
  }


 void AddBorder(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int r, int g, int b, int bwidth)
{
  int x, y;
     for( y = 0; y < HEIGHT; y++ ) {
        for( x = 0; x < WIDTH; x++ ) {
          if(x < bwidth && bwidth < WIDTH){
	    R[x][y] = r;
	    G[x][y] = g;
	    B[x][y] = b;
	  }
	  if(y < bwidth && bwidth < HEIGHT){
	    R[x][y] = r;
            G[x][y] = g;
            B[x][y] = b;
	  }
	  if( x > bwidth && (WIDTH - x) < bwidth){
	    R[x][y] = r;
            G[x][y] = g;
            B[x][y] = b;
          }
	  if( y > bwidth && (HEIGHT - y) < bwidth){
            R[x][y] = r;
            G[x][y] = g;
            B[x][y] = b;
          }

					  
    }
     }

}




/* EOF */
