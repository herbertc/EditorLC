#include <SDL/SDL.h>
#include "wave.h"
#include "meta.h"
#include "lgmk.h"
#include "sound.h"

void teste(char *path);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "Usage: " << argv[0] << " {wave_file.wav}\n";
		return -1;
	}
	
	char *path = argv[1];
	
	teste(path);
	
	/*int rc = SDL_Init(SDL_INIT_AUDIO);

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
	uint32_t wavLen;
	uint8_t *wavBuffer;
	
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

	return 0;*/
}

void teste(char *path)
{
	Wave *wave = Wave::load(path);

	cout << wave << endl;

	Meta *meta = new Meta();
	meta->setTitle("O Capital - A Critica da Economia Politica");
	meta->setAuthor("Karl Marx");
	meta->setLanguage("Portugues");
	meta->setPublisher("Victor Civita");
	meta->setYear("1983");
	meta->setAddress("Sao Paulo");
	meta->setpages("301");
	
	wave->add_chunk(meta);
	
	Lgmk *lgmk = new Lgmk();
	
	uint32_t position;
	string name;
	
	cout <<"Insira o nome da primeira marcação: ";
	cin >> name;

	lgmk->add_markName(name);
	
	int count = 2;
	while(true)
	{
		cout << "Nível 1 - Marcação: " << count << " - Entre com a posição em segundos. (Insira 0(Zero) para sair): ";
		cin >> position;

		if(position != 0)
		{
			cout << "Marcação: " << count <<" - Insira o nome dessa marcação: ";
			cin >> name;

			lgmk->add_mark(position);
			lgmk->add_markName(name); 
		}else
		{
			lgmk->add_mark(0);
			cout << "Edição de marcações de nível 1 finalizado!" << endl;
			break;
		}
		cout << '\n';
		count++;
		
	}

	cout << '\n';

	count = 1; 
	while(true)
	{
		cout << "Nível 2 - Marcação: " << count <<" - Entre com a posição em segundos. (Insira 0(Zero) para sair): ";
		cin >> position;

		if(position != 0)
		{
			cout << "Marcação: " << count <<" - Insira o nome dessa marcação: ";
			cin >> name;
		
			lgmk->add_subMark(position);
			lgmk->add_subMarkName(name); 
		}else
		{
			cout << "Edição de marcações de nível 2 finalizado!" << endl;
			break;
		}
		cout << '\n';
		count++;
	}

	cout << lgmk << endl;

	wave->add_chunk(lgmk);
	wave->save("output.wav");
}

