#include <png.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;

char* itoa(int value, char* result, int base) {
		// check that the base if valid
		if (base < 2 || base > 36) { *result = '\0'; return result; }

		char* ptr = result, *ptr1 = result, tmp_char;
		int tmp_value;

		do {
			tmp_value = value;
			value /= base;
			*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
		} while ( value );

		// Apply negative sign
		if (tmp_value < 0) *ptr++ = '-';
		*ptr-- = '\0';
		while(ptr1 < ptr) {
			tmp_char = *ptr;
			*ptr-- = *ptr1;
			*ptr1++ = tmp_char;
		}
		return result;
	}

void read_png_file(char *filename) {
    FILE *fp = fopen(filename, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    width            = png_get_image_width(png, info);
    height         = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth    = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
         color_type == PNG_COLOR_TYPE_GRAY ||
         color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
         color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    if (row_pointers) abort();

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);
}

void write_png_file(char *filename) {
    FILE *fp = fopen(filename, "wb");
    if(!fp) abort();

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
        png,
        info,
        width, height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    //png_set_filler(png, 0, PNG_FILLER_AFTER);

    if (!row_pointers) abort();

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    for(int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    row_pointers = NULL;

    fclose(fp);

    png_destroy_write_struct(&png, &info);
}

void extract_bit_planes(char* filename, int bit){

    int mask = 1 << (bit-1);
    unsigned int size_filename = strlen(filename);

    char* filename_red = malloc(size_filename+8+1);
    strcpy(filename_red, filename);
    filename_red[size_filename-4] = 0;
    strcat(filename_red, "_red.png");

    char* filename_green = malloc(size_filename+10+1);
    strcpy(filename_green, filename);
    filename_green[size_filename-4] = 0;
    strcat(filename_green, "_green.png");

    char* filename_blue = malloc(size_filename+9+1);
    strcpy(filename_blue, filename);
    filename_blue[size_filename-4] = 0;
    strcat(filename_blue, "_blue.png");
    
    for(int color = 0; color < 3; color++){
        
        read_png_file(filename);

        for(int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {   
                png_bytep row = row_pointers[y];
                png_bytep px = &(row[x * 4]);
                if((px[color] & mask)){
                    px[0] = 255;
                    px[1] = 255;
                    px[2] = 255;
                }
                else{
                    px[0] = 0;
                    px[1] = 0;
                    px[2] = 0;
                }
            }
        }
        switch(color){
            
            case 0:
                write_png_file(filename_red);
                break;
            case 1:
                write_png_file(filename_green);
                break;
            case 2:
                write_png_file(filename_blue);
                break;
        }
        
    }
    
    free(filename_red);
    free(filename_green);
    free(filename_blue);
}

void encrypt_png_file(char* filename, int bitDepth) {

    if(bitDepth > 8 || bitDepth < 1){
        puts("Invalid bit depth size. Must be between 1 and 8.");
        abort();
    }
    FILE* fp = fopen(filename, "rb");
    char ch;

    if(fp == NULL) abort();

    unsigned int text_size = 0;

    while(fread(&ch, 1, 1, fp)){text_size++;}
    text_size++;

    char* text = (char*)calloc(text_size, sizeof(char));
    char* text_pointer = text;
    int text_index = 0;
    fseek(fp, 0, SEEK_SET);

    while(fread(&text[text_index++], 1, 1, fp)){}
    text[text_index] = 0;
    
    fclose(fp);

    unsigned int binary_size = text_size*8;
    if((binary_size) > height*width*3*bitDepth) {
        printf("Insuffiecient space on cover image with %d depth\n", bitDepth);
        exit(EXIT_FAILURE);
    }

    char* text_binary = (char*)calloc(binary_size, sizeof(char));
    char* text_binary_pointer = text_binary;

    char buffer[9];
    text_index = 0;
    
    while(*text_pointer){
        itoa(*text_pointer, buffer, 2);
        for(int i = 0; i < 8 - strlen(buffer); i++){
            text_binary_pointer[text_index++] = '0';
        }
        for(int i = 0; i < strlen(buffer); i++){
            text_binary_pointer[text_index++] = buffer[i];
        }
        ++text_pointer;
    }

    text_binary_pointer[text_index] = 0;

    png_byte byte_mask = 0b1;
    for(int i = 0; i < bitDepth - 1; i++)
        byte_mask = (byte_mask << 1) | 0b1;
    byte_mask = ~byte_mask;
    printf("byte_mask %d\n", byte_mask);

    int cur_x = 0, cur_y = 0;
    unsigned int pixel_jump = 1;
    for(int cur_binary = 0; cur_binary < binary_size; cur_binary++){
        png_bytep row = row_pointers[cur_y];
        png_bytep px = &(row[cur_x * 4]);

        for(int color = 0; color < 3; color++){
            if(!*text_binary_pointer) break;
            px[color] &= byte_mask;
            for(int bit = 0; bit < bitDepth; bit++){
                *text_binary_pointer == '1' ? px[color] |= (0b1 << (bitDepth - bit - 1)) : (px[color] |= 0b0);
                ++text_binary_pointer;
            }   
        }
        cur_y += pixel_jump;
        if(cur_y / height >= 1){
            cur_x += cur_y / height;
            cur_y = cur_y % height;
            if(cur_x / width >= 1){
                cur_y += cur_x / width;
                cur_x = cur_x % width;
            }
        }
        if(cur_x == 0 && cur_y == 0){
            cur_y += pixel_jump/2;
        }
    }

    free(text);
    free(text_binary);

}

void decrypt_png_file(char* outFilename, int bitDepth){

    if(bitDepth > 8 || bitDepth < 1){
        puts("Invalid bit depth size. Must be between 1 and 8.\n");
        abort();
    }

    int buffer_index = 0;

    char* buffer = (char*)malloc(height*width*3*bitDepth);    

    int cur_x = 0, cur_y = 0;
    unsigned int pixel_jump = (height >> 1) - 9;
    for(int i = 0; i < width*height; i++){
        png_bytep row = row_pointers[cur_y];
        png_bytep px = &(row[cur_x * 4]);

        for(int color = 0; color < 3; color++){
            for(int bit = 0; bit < bitDepth; bit++){
                buffer[buffer_index++] = (px[color] & (0b1 << (bitDepth - bit - 1))) >> (bitDepth - bit - 1);
            }
        }
        cur_y += pixel_jump;
        if(cur_y / height >= 1){
            cur_x += cur_y / height;
            cur_y = cur_y % height;
            if(cur_x / width >= 1){
                cur_y += cur_x / width;
                cur_x = cur_x % width;
            }
        }
        if(cur_x == 0 && cur_y == 0){
            cur_y += pixel_jump/2;
        }
    }

    int len = buffer_index;
    unsigned char byte = 0;
    int bits = 0;
    
    FILE* fp = fopen(outFilename, "w");

    for(int i = 0; i < len; ++i)
    {
        switch(buffer[i])
        {
            case 1:
                byte |= 1;
                break;

            case 0:
                break;

            default:
                continue;
        }
        if (++bits < 8)
            byte <<= 1;
        else
        {
            fwrite(&byte, 1, 1, fp);
            bits = 0;
            byte = 0;
        }
    }
    
    if (bits)
        fwrite(&byte, 1, 1, fp);

    fclose(fp);
    free(buffer);
}

int main(int argc, char* argv[]){
// Usage: ./steg cover.png stego.png message.txt stegoBitDepth
// Usage: ./steg stego.png messageOut.txt stegoBitDepth
// Usage: ./steg image.png bitPlane

    if(argc == 4){
        read_png_file(argv[1]);
        decrypt_png_file(argv[2], atoi(argv[3]));

        if (!row_pointers) abort();

        for(int y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
    }
    else if(argc == 5 || argc == 6){
        read_png_file(argv[1]);
        encrypt_png_file(argv[3], atoi(argv[4]));
        write_png_file(argv[2]);
        if(argc == 6)
            extract_bit_planes(argv[2], atoi(argv[5]));
    }
    else if(argc == 3){
        extract_bit_planes(argv[1], atoi(argv[2]));
    }
    else{
        printf("Usage:\n%s cover.png stego.png message.txt stegoBitDepth", argv[0]);
        printf("\n%s stego.png messageOut.txt stegoBitDepth\n", argv[0]);
    }

    return 0;
}

