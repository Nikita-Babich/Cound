#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define SAMPLE_RATE 44100   // standard sample rate
#define LOFI_SAMPLE_RATE 11025 
#define DURATION    5       // seconds

void fill_white_noise(uint8_t *sound, uint32_t total_samples) {
    for(uint32_t i = 0; i < total_samples; i++)
        sound[i] = 128 + (rand() % 65 - 32);; // random 0-255
}

void fill_wav_header(uint8_t *header, uint32_t data_size, uint32_t sample_rate) {
    // RIFF chunk descriptor
    header[0]='R'; header[1]='I'; header[2]='F'; header[3]='F';
    uint32_t chunk_size = 36 + data_size;
    *(uint32_t*)&header[4] = chunk_size;
    header[8]='W'; header[9]='A'; header[10]='V'; header[11]='E';

    // fmt subchunk
    header[12]='f'; header[13]='m'; header[14]='t'; header[15]=' ';
    *(uint32_t*)&header[16] = 16;          // Subchunk1Size
    *(uint16_t*)&header[20] = 1;           // AudioFormat = PCM
    *(uint16_t*)&header[22] = 1;           // NumChannels = 1
    *(uint32_t*)&header[24] = sample_rate; // SampleRate
    *(uint32_t*)&header[28] = sample_rate; // ByteRate = SampleRate * NumChannels * BitsPerSample/8
    *(uint16_t*)&header[32] = 1;           // BlockAlign = NumChannels * BitsPerSample/8
    *(uint16_t*)&header[34] = 8;           // BitsPerSample = 8

    // data subchunk
    header[36]='d'; header[37]='a'; header[38]='t'; header[39]='a';
    *(uint32_t*)&header[40] = data_size;   // Subchunk2Size
}

int main() {
    uint32_t total_samples = SAMPLE_RATE * DURATION;
    uint8_t *sound = (uint8_t *)malloc(total_samples);
    if(!sound) {
		perror("malloc failed\n");
		printf("malloc failed\n");
		return 1;
	}

    // Fill with simple waveform: middle value = silence
    for(uint32_t i=0;i<total_samples;i++)
        sound[i]=128;
	
	//sound making zone
	fill_white_noise(sound, total_samples); 
	
	
	
	

    // Write WAV file
    FILE *f = fopen("output.wav","wb");
    if(!f) { free(sound); return 1; }

    uint8_t header[44];
    fill_wav_header(header, total_samples, SAMPLE_RATE);
    fwrite(header,1,44,f);
    fwrite(sound,1,total_samples,f);

    fclose(f);
    free(sound);
    return 0;
}