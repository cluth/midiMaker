/*
**	Chris's Super Cool Awesome Wave File Parser 
**
**
**
*/

struct HEADER{
	unsigned long long totalSize;
	unsigned int sampleRate;
	unsigned long long  bitsPerSample;
	unsigned int numSamples;
};
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct HEADER readHeader(FILE *file);
unsigned int LittleToBig(unsigned int x);
void readData(int *data, struct HEADER header, FILE *file);
int convertBitSize(unsigned int in, int bps);

int main(int argc, char **argv)
{
	
	
	//Check and see if there is a file
	if(argv[1] == NULL){
		printf("No File Name Given");
	exit(1);
	}
	FILE *input = fopen(argv[1],"r");
	struct HEADER header = readHeader(input);
	 
	int *data = (int *) malloc(header.numSamples * sizeof(int));
	readData(data, header, input);	
	unsigned int i;
	printf("%d",header.bitsPerSample);
	for(i=0;i<10;i++)
	{
	printf("%d\n",LittleToBig(data[i]));	
	printf("%02x\n",data[i]);
	}


}

void readData(int *data, struct HEADER header,FILE *file){
	int buffer;
	int i, j;
	for(i=0;i<header.numSamples;i++){
			
			fread(&buffer,1,1,file);
		
		

			data[i] =buffer;
	}
}
	
int convertBitSize(unsigned int in, int bps){
	const unsigned int max = (1<<(bps-1))-1;
	return in>max? in - (max<<1) : in;
}

struct HEADER readHeader(FILE *file){
	
	struct HEADER header;
	char buffer[5];
	//ChunkID (RIFF for little-endian, RIFX for big endian)
	fread(buffer,1,4,file);
	buffer[4]='\0';
	//get overall size	
	fread(buffer,1,4,file);
	buffer[4]='\0';	
	header.totalSize= LittleToBig((buffer[0]<<24)|(buffer[1]<<16)|(buffer[2]<<8)|buffer[3]);	
	//get format
	fread(buffer,1,4,file);
	buffer[4]='\0';
	//get subchunk1ID
	fread(buffer,1,4,file);
	buffer[4]='\0';
	//subchunk1size
	fread(buffer,1,4,file);
	//audioformat
	fread(buffer,1,2,file);
	//num channels	1=mono 2=stereo
	fread(buffer,1,2,file);
	int channels = buffer[0];
	
	//samplerate
	fread(buffer,1,4,file);
	header.sampleRate=LittleToBig((buffer[0]<<24)|(buffer[1]<<16)|(buffer[2]<<8)|buffer[3]);
	//byterate
	fread(buffer,1,4,file);
	//blockalign
	fread(buffer,1,2,file);
	//bitspersample	
	fread(buffer,1,2,file);
	header.bitsPerSample=buffer[0];
	//subchunk2ID
	fread(buffer,1,4,file);
	buffer[4]='\0';
	//subchunk2size
	fread(buffer,1,4,file);
//	printf("%u",buffer);
	unsigned long long size = LittleToBig((buffer[0]<<24)|(buffer[1]<<16)|(buffer[2]<<8)|buffer[3]);
	header.numSamples = size/(channels*header.bitsPerSample/8);
	
	return header;
		

}

unsigned int LittleToBig(unsigned int x){
	return (((x>>24) & 0x000000ff) | ((x>>8) & 0x0000ff00) | ((x<<8) & 0x00ff0000) | ((x<<24) & 0xff000000));
}
