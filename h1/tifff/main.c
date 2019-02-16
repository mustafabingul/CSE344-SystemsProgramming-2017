#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <zconf.h>
#include <inttypes.h>

//https://stackoverflow.com/questions/33766465/bit-manipulation-in-c-setting-the-least-significant-bit
#define BIT 8
#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"

#define BYTETOBINARY(byte) \
    (byte & 0x80 ? 1 : 0), \
    (byte & 0x40 ? 1 : 0), \
    (byte & 0x20 ? 1 : 0), \
    (byte & 0x10 ? 1 : 0), \
    (byte & 0x08 ? 1 : 0), \
    (byte & 0x04 ? 1 : 0), \
    (byte & 0x02 ? 1 : 0), \
    (byte & 0x01 ? 1 : 0)

#define PRINTBIN(x) printf(BYTETOBINARYPATTERN, BYTETOBINARY(x));

struct imageTAG
{
    uint16_t tag;
    uint16_t type;
    uint32_t N;
    uint32_t info;
};
//https://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func
uint16_t swap_uint16( uint16_t val )
{
    return (val << 8) | (val >> 8 );
}
uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}
void completeEndian(struct imageTAG * _tag){

    _tag->tag = swap_uint16(_tag->tag);
    _tag->type = swap_uint16(_tag->type);
    _tag->N = swap_uint32(_tag->N);
    if(_tag->type==3){
        _tag->info=swap_uint16(_tag->info);
    }
    else if(_tag->type==4){
        _tag->info = swap_uint32(_tag->info);
    }
}

void byteToBit(unsigned char _byte, unsigned char *_bit){

    //printf("%x   ",_byte);
    unsigned char m=1;

    for (int i= 7,j=0; i >=0; --i) {
        _bit[i]=(_byte & (m << j++)) !=0;
        //printf("%x-",_bit[i]);
    }
    //printf("\n");

    /*
    _bit[0]=(_byte & 0x80 ? 1 : 0);
    _bit[1]=(_byte & 0x40 ? 1 : 0);
    _bit[2]=(_byte & 0x20 ? 1 : 0);
    _bit[3]=(_byte & 0x10 ? 1 : 0);
    _bit[4]=(_byte & 0x08 ? 1 : 0);
    _bit[5]=(_byte & 0x04 ? 1 : 0);
    _bit[6]=(_byte & 0x02 ? 1 : 0);
    _bit[7]=(_byte & 0x01 ? 1 : 0);*/
}

void findImageInfo(uint16_t _byteOrder, struct imageTAG *_tag, int _fd, int _tagCount){


    int bytesOfLine=0,newLine=0;
    uint8_t *imageInfo;
    unsigned char *BITS;
    unsigned char bit[8];
    int stripByteCounts=0,stripOffsets=0,imageWidth=0,imageLength=0,bitsPerSample=0;
    int k=0,i=0,j=0;

    while(k<_tagCount){
        // MOTOROLA..
        if(_byteOrder==0x4d4d){
            completeEndian(_tag);
            //printf("\nMOTOOO\n");
        }
        // intel..
        if(_byteOrder==0x4949){}
        if(_tag->tag==256){
            imageWidth=_tag->info;
        }
        if(_tag->tag==257){
            imageLength=_tag->info;
        }
        if(_tag->tag==279){
            stripByteCounts=_tag->info;
        }
        if(_tag->tag==273){
            stripOffsets=_tag->info;
        }
        if(_tag->tag==258){
            bitsPerSample=_tag->info;
        }
        //printf("%d-%d-%d-%d\n",_tag->tag,_tag->type,_tag->N,_tag->info);

        _tag++;
        k++;
    }
    // image info reading...
    imageInfo = calloc(stripByteCounts, sizeof(uint8_t));
    lseek(_fd,stripOffsets,SEEK_SET);
    k=0;
    while(k<stripByteCounts){
        read(_fd,&imageInfo[k], sizeof(uint8_t));
        k++;
    }
    // imagedata bits convert..
    BITS = calloc((stripByteCounts*BIT), sizeof(unsigned char));
    k=0;
    i=0;
    while(k<stripByteCounts){
        byteToBit(imageInfo[k],&bit);
        j=0;
        while(j<BIT){
            BITS[i]=bit[j];
            j++;
            i++;
        }
        k++;
    }
    printf("Width : %d pixels\n",imageWidth);
    printf("Height : %d pixels\n",imageLength);
    if(_byteOrder==0x4d4d){
        printf("Byte order : Motorola\n");
    }
    if(_byteOrder==0x4949){
        printf("Byte order : Intel\n");
    }

    if(bitsPerSample==8){
        k=0;
        while(k<stripByteCounts){
            if(k%imageWidth==0){
                printf("\n");
            }
            printf("%d",imageInfo[k]==255 ?1:0);
            k++;
        }
    }
    //printf("\n%d-%d-%d-%d-%d-",imageWidth,imageLength,stripByteCounts,stripOffsets,bitsPerSample);

    bytesOfLine = stripByteCounts/imageLength;
    newLine = BIT*bytesOfLine;

    if(bitsPerSample==1){
        k=0;
        j=0;    //all bits.
        while(k<imageLength){
            i=0;
            while(i<imageWidth){
                printf("%d",BITS[j]);
                j++;
                i++;
            }
            printf("\n");
            j=j+(newLine-imageWidth);
            k++;
        }
    }

    //printf("\n%d%d%d%d%d%d%d%d",bit[0],bit[1],bit[2],bit[3],bit[4],bit[5],bit[6],bit[7]);
    //printf("\n%d-%d-%d-%d-%d-",imageWidth,imageLength,stripByteCounts,stripOffsets,bitsPerSample);

    free(BITS);
    free(imageInfo);
}
int main(int argc, char *argv[]){

    if(argc!=2){
        fprintf(stderr,"Usage: %s TIF filename\n",argv[1]);
        return -1;
    }
    uint16_t tagCount;
    uint16_t imageByteOrder;
    uint16_t imageVersion;
    uint32_t imageFileDirectory;
    int tagPlace, i=0;
    struct imageTAG *tagList;

    int fd=open(argv[1],O_RDONLY);
    //int fd = open("intelTEST.tif",O_RDONLY);
    //int fd = open("indexed.tiff",O_RDONLY);
    //int fd = open("TESTemre.TIF",O_RDONLY);
    //int fd = open("altigen.tif",O_RDONLY);
    //int fd = open("myTEST.tif",O_RDONLY);
    //int fd = open("TEST.TIF",O_RDONLY);

    read(fd,&imageByteOrder, sizeof(uint16_t));
    read(fd,&imageVersion,sizeof(uint16_t));
    read(fd,&imageFileDirectory,sizeof(uint32_t));

    // tag starting
    if(imageByteOrder==0x4949){
       tagPlace=imageFileDirectory;
    }
    else if(imageByteOrder==0x4d4d){
        tagPlace=swap_uint32(imageFileDirectory);
    }

    // tag'S read
    lseek(fd,tagPlace,SEEK_SET);
    read(fd,&tagCount,sizeof(uint16_t));

    if(imageByteOrder==0x4949){
        tagCount=tagCount;
    }
    else if(imageByteOrder==0x4d4d){
        tagCount = swap_uint16(tagCount);
    }
    tagList = calloc(tagCount, sizeof(struct imageTAG));
    while(i<tagCount){
        read(fd,&tagList[i], sizeof(struct imageTAG));
        i++;
    }

    findImageInfo(imageByteOrder,tagList,fd,tagCount);

    free(tagList);

    return 0;
}
