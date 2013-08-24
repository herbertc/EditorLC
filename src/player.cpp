#include "sound.h"
#include "wave.h"
#include "chunk.h"

void teste();

int main(int argc, char *argv[])
{
	teste();

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s {wave_file.wav}\n", argv[0]);
		return -1;
	}
	
	char *path = argv[1];
	
	int rc = SDL_Init(SDL_INIT_AUDIO);

	if (rc != 0) {
		cerr << "Erro na inicializacao do modulo de audio: " <<
			SDL_GetError() << endl;
		return -1;
	}

	Sound sound;
	sound.position = -1;
	sound.size = 0;
	sound.buffer = 0;

	SDL_AudioSpec desired, obtained;
	desired.freq = 44100;
	desired.format = AUDIO_S16;
	desired.channels = 2;
	desired.samples = 4096;
	desired.callback = callback;
	desired.userdata = (void *) &sound;

	rc = SDL_OpenAudio(&desired, &obtained);

	if (rc != 0) {
		cerr << "Erro na abertura do dispositivo de audio: " <<
			SDL_GetError() << endl;
		SDL_Quit();
		return -2;
	}

	cout << "Dispositivo de audio inicializado com sucesso!" << endl;

	cout << "Especificacao desejada: " << endl;
	cout << desired << endl;

	cout << "Especificacao obtida: " << endl;
	cout << obtained << endl;

	cout << "Carregando o arquivo com o audio desejado... ";

	SDL_AudioSpec wavSpec;
	Uint32 wavLen;
	Uint8 *wavBuffer;

/*
	if (SDL_LoadWAV(path , &wavSpec, &wavBuffer, &wavLen) == NULL) {
		cout << "Falha! " << SDL_GetError() << endl;
		SDL_CloseAudio();
		SDL_Quit();
		return -3;
	}
*/
	if (Load_Wave(path, &wavSpec, &wavBuffer, &wavLen) == NULL) {
		cout << "Falha! " << SDL_GetError() << endl;
		SDL_CloseAudio();
		SDL_Quit();
		return -3;
	}
	cout << "Ok!" << endl;

	cout << "Formato do audio carregado: " << endl;
	cout << wavSpec << endl;
	cout << "Wave length: " << wavLen << endl;

	cout << "Convertendo o som para o formato atual da placa... ";

	SDL_AudioCVT cvt;

    rc = SDL_BuildAudioCVT(&cvt, wavSpec.format, wavSpec.channels, wavSpec.freq,
			obtained.format, obtained.channels, obtained.freq);

	if (rc != 0) {
		cout << "Falha! " << SDL_GetError() << endl;
		SDL_FreeWAV(wavBuffer);
		SDL_CloseAudio();
		SDL_Quit();

		return -5;
    }

    cvt.len = wavLen;
    Uint8 *wavNewBuf = (Uint8 *) malloc(cvt.len * cvt.len_mult);

    if (wavNewBuf == NULL) {
		cerr << "Sem memoria para um novo buffer!" << endl;
		SDL_FreeWAV(wavBuffer);
		SDL_CloseAudio();
		SDL_Quit();

		return -6;
    }

    memcpy(wavNewBuf, wavBuffer, wavLen);
    cvt.buf = wavNewBuf;

    rc = SDL_ConvertAudio(&cvt);

    if (rc != 0) {
		cerr << "Erro na conversao do audio!" << endl;
		SDL_FreeWAV(wavBuffer);
		free(wavNewBuf);
		SDL_CloseAudio();
		SDL_Quit();

		return -6;
	}

    SDL_FreeWAV(wavBuffer);

	cout << "Ok!" << endl;


	SDL_LockAudio();
	sound.position = 0;
	sound.buffer = wavNewBuf;
	sound.size = cvt.len * cvt.len_mult;
	SDL_UnlockAudio();

	cout << "Inicializando o playback..." << endl;
	SDL_PauseAudio(0);

	while (true)
	{
		SDL_LockAudio();
		if (sound.position >= sound.size)
			break;
		SDL_UnlockAudio();

		SDL_Delay(10);
	}

	cout << "Playback finalizado" << endl;
	SDL_PauseAudio(1);		

	SDL_Delay(100);

	free(wavNewBuf);
	SDL_CloseAudio();
	SDL_Quit();

	return 0;
}

void teste()
{
	FILE *wave = fopen("plane.wav", "rb");

	fseek(wave, 0, SEEK_END);
	long size = ftell(wave);
	fseek(wave, 0, SEEK_SET);

	cout << "Size = " << size << endl;
	uint8_t *bytes = new uint8_t[size];
	fread(bytes, sizeof(uint8_t), size, wave);

	Chunk chunk;

	cout << "Id = " << chunk.id() << endl;
	cout << "Size = " << chunk.size() << endl;

	chunk.read(bytes);

	cout << "Id = " << chunk.id() << endl;
	cout << "Size = " << chunk.size() << endl;

	char init[5];
	init[4] = 0;
	memcpy(init, chunk.data(), 4);

	cout << "Data inicial = " << init << endl;

	Chunk sub1;
	sub1.read(chunk.data() + 4);
	cout << "Id = " << sub1.id() << endl;
	cout << "Size = " << sub1.size() << endl;

	memcpy(init, sub1.data(), 4);
	cout << "Data inicial = " << init << endl;


	delete [] bytes;
}

