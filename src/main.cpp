#include <SDL/SDL.h>
#include "wave.h"
#include "meta.h"
#include "lgmk.h"
#include "format.h"
#include "sound.h"

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//void teste(char *path);

const char *
get_filename_ext(const char *filename)
{
	const char *dot = strrchr(filename, '.');
	
	if(!dot || dot == filename)
		return "";
	
	return dot + 1;
}

void 
assert_file_extension(char *path)
{
	string file_ext(get_filename_ext(path));
	string wav_ext("wav");
	string rab_ext("rab");
	
	if(file_ext.compare(wav_ext) && file_ext.compare(rab_ext))
	{
		cout << "ERRO: O formato {" << file_ext<< "} é incompatível com esse programa!" << endl;
		return;
	}else
	{
		cout << file_ext <<": Extensão Correta!" << endl;
	}
}

void 
insert_meta(Meta *meta)
{
	string input;
	
	cout << "Insira o título do audiobook: ";
	getline(cin, input);
	meta->setTitle(input);
	
	cout << "Insira o nome do autor do audiobook: ";
	getline(cin, input);
	meta->setAuthor(input);
	
	cout << "Insira o idioma do audiobook: ";
	getline(cin, input);
	meta->setLanguage(input);
	
	cout << "Insira o editor do audiobook: ";
	getline(cin, input);
	meta->setPublisher(input);
	
	cout << "Insira o ano de lançamento do audiobook: ";
	getline(cin, input);
	meta->setYear(input);
	
	cout << "Insira o local de origem do audiobook: ";
	getline(cin, input);
	meta->setAddress(input);
	
	cout << "Insira a quantidade de páginas do livro impresso: ";
	getline(cin, input);
	meta->setpages(input);
}

void 
insert_level1_marks(Lgmk *lgmk, uint32_t duration)
{
	int count_marks = 1;
	string input;
	duration = 300;
	
	cout << "A primeira marcação de NÍVEL 1 inicia no tempo igual a 0 segundos..." << endl;
	
	cout << "\tNIVEL 1" << endl;
	while(1)
	{
		cout << "\tMarcação " << count_marks + 1 << ": Insira o tempo em segundos. (Digite ZERO para sair!): ";
		getline(cin, input);
		
		if(isdigit(*(input.c_str())))
		{
			if(atoi(input.c_str()) == 0)
			{
				cout << "\t\tEdição de marcações de NIVEL 1 foi terminado!" << endl;
				cout << "\t\t"<< count_marks << " Marcações foram inseridas." << endl;
				lgmk->add_mark(0);
				break;
			}else if(atoi(input.c_str()) > (int) duration)
			{
				cout << "\t\tErro: Valor Incorreto! O valor inserido está fora do limite:(0 - "<< duration << ")" << endl;
				
			}else if(atoi(input.c_str()) < 0)
			{
				cout << "\t\tErro: Valor Incorreto! Valor negativo" << endl;
				
			}else
			{
				lgmk->add_mark( atoi(input.c_str()) );
				count_marks++;
			}		
		}
		else if(isalnum(*(input.c_str())))
		{
			cout << "\t\tErro: Valor Incorreto! Letras e números..." << endl;
		}
	}
	
	int i;
	for(i = 0; i < count_marks; i++)
	{
		cout << "Insira o nome da Marcação " << i + 1 << ": ";
		getline(cin, input);
		lgmk->add_markName(input);
	}
	
	cout << "\t\tNIVEL 1................ok!" << endl;
}

void 
insert_level2_marks(Lgmk *lgmk, uint32_t duration)
{
	int count_submarks = 1;
	string input;
	duration = 300;
	
	cout << "A primeira marcação de NÍVEL 2 inicia no tempo igual a 0 segundos..." << endl;
	
	cout << "\tNIVEL 2" << endl;
	while(1)
	{
		cout << "\tSubmarcação " << count_submarks + 1 << ": Insira o tempo em segundos. (Digite ZERO para sair!): ";
		getline(cin, input);
		
		if(isdigit(*(input.c_str())))
		{
			if(atoi(input.c_str()) == 0)
			{
				cout << "\t\tEdição de marcações de NIVEL 1 foi terminado!" << endl;
				cout << "\t\t"<< count_submarks << " Marcações foram inseridas." << endl;
				lgmk->add_subMark(0);
				break;
			}else if(atoi(input.c_str()) > (int) duration)
			{
				cout << "\t\tErro: Valor Incorreto! O valor inserido está fora do limite:(0 - "<< duration << ")" << endl;
				
			}else if(atoi(input.c_str()) < 0)
			{
				cout << "\t\tErro: Valor Incorreto! Valor negativo" << endl;
				
			}else
			{
				lgmk->add_subMark( atoi(input.c_str()) );
				count_submarks++;
			}
			
		}else if(isalnum(*(input.c_str())))
		{
			cout << "\t\tErro: Valor Incorreto! Letras e números..." << endl;
		}
	}
	
	int i;
	for(i = 0; i < count_submarks; i++)
	{
		cout << "Insira o nome da Submarcação " << i + 1 << ": ";
		getline(cin, input);
		lgmk->add_subMarkName(input);
	}
	
	cout << "\t\tNIVEL 2................ok!" << endl;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "Usage: " << argv[0] << " {wave_file.wav}\n";
		return -1;
	}
	
	char *path = argv[1];
	
	assert_file_extension(path);
	
	Wave *wave = Wave::load(path);
	
	//cout << wave << endl;
	
	vector<Chunk *> subchunks = wave->subchunks();
	Format *format = new Format();
	for(vector<Chunk *>::iterator it = subchunks.begin(); it != subchunks.end(); it++)
	{
		format = dynamic_cast<Format *>(*it);
		
		if(format != 0)
			break;
	}
	
	uint32_t duration = 0;
	duration /= (format->sampleRate() * format->numChannels());
	duration /= (format->bitsPerSample()/8);
	
	cout << duration << endl;
	
	Meta *meta = new Meta();
	Lgmk *lgmk = new Lgmk();

	insert_meta(meta);
	insert_level1_marks(lgmk, duration);
	insert_level2_marks(lgmk, duration);
	
	//cout << meta << endl;
	//cout << lgmk << endl;
	
	wave->add_chunk(meta);
	wave->add_chunk(lgmk);
	
	cout << "Digite o nome do arquivo de saída: ";
	string filename;
	getline(cin, filename);
	
	filename.append(".rab");
	cout << filename << endl;
	wave->save(filename);
}

/*
void teste(char *path)
{
	Wave *wave = Wave::load(path);
	Meta *meta = new Meta();
	Lgmk *lgmk = new Lgmk();
	//string filename;
	string input;
	*/
	
	/*
	cout << "Insira o nome do arquivo do audiobook: ";
	getline(cin, input);
	filename = input;


	*/
	//filename = "HinoNacionalBrasileiro";
	
	/*
	meta->setTitle("Hino Nacional Brasileiro");
	meta->setAuthor("Joaquim Osorio & Francisco Manuel");
	meta->setLanguage("Portugues");
	meta->setPublisher("");
	meta->setYear("1822");
	meta->setAddress("Brasil");
	meta->setpages("0");
	
	cout << meta << endl;
	wave->add_chunk(meta);
	
	lgmk->add_mark(0);
	lgmk->add_markName("Parte 1");
	
	lgmk->add_mark(103);
	lgmk->add_markName("Parte 2");
	
	lgmk->add_subMark(0);
	lgmk->add_subMarkName("Introducao 1");
	
	lgmk->add_subMark(28);
	lgmk->add_subMarkName("Primeira Estrofe");
	
	lgmk->add_subMark(44);
	lgmk->add_subMarkName("Segunda Estrofe");
	
	lgmk->add_subMark(60);
	lgmk->add_subMarkName("Terceira Estrofe");
	
	lgmk->add_subMark(64);
	lgmk->add_subMarkName("Quarta Estrofe");
	
	lgmk->add_subMark(79);
	lgmk->add_subMarkName("Quinta Estrofe");
	
	lgmk->add_subMark(91);
	lgmk->add_subMarkName("Sexta Estrofe");
	
	lgmk->add_subMark(103);
	lgmk->add_subMarkName("Introducao 2");
	
	lgmk->add_subMark(132);
	lgmk->add_subMarkName("Setima Estrofe");
	
	lgmk->add_subMark(148);
	lgmk->add_subMarkName("Oitava Estrofe");
	
	lgmk->add_subMark(164);
	lgmk->add_subMarkName("Nona Estrofe");
	
	lgmk->add_subMark(168);
	lgmk->add_subMarkName("Decima Estrofe");
	
	lgmk->add_subMark(183);
	lgmk->add_subMarkName("Decima Primeira Estrofe");
	
	lgmk->add_subMark(195);
	lgmk->add_subMarkName("Decima Segunda Estrofe");
	
	lgmk->add_subMark(201);
	lgmk->add_subMarkName("Decima Terceira Estrofe");
	
	lgmk->add_subMark(206);
	lgmk->add_subMarkName("Final");
	
	cout << lgmk << endl;
	wave->add_chunk(lgmk);
	
	wave->save("HinoNacionalBrasileiro.rab"); 
	*/
	
	/*meta->setTitle("Shine on You Crazy Diamond");
	meta->setAuthor("Roger Waters, Richard Wright, David Gilmour");
	meta->setLanguage("English");
	meta->setPublisher("Pink Floyd Music Publishers Ltd");
	meta->setYear("1975");
	meta->setAddress("United Kingdom");
	meta->setpages("0");
	
	cout << meta << endl;
	wave->add_chunk(meta);
	
	lgmk->add_mark(0);
	lgmk->add_markName("First Part");
	
	lgmk->add_mark(810);
	lgmk->add_markName("Second Part");
	//---------------------------------
	lgmk->add_subMark(0);
	lgmk->add_subMarkName("Part I");
	
	lgmk->add_subMark(235);
	lgmk->add_subMarkName("Part II");
	
	lgmk->add_subMark(387);
	lgmk->add_subMarkName("Part III");
	
	lgmk->add_subMark(522);
	lgmk->add_subMarkName("Part IV");
	
	lgmk->add_subMark(675);
	lgmk->add_subMarkName("Part V");
	
	lgmk->add_subMark(810);
	lgmk->add_subMarkName("Part VI");
	
	lgmk->add_subMark(810+296);
	lgmk->add_subMarkName("Part VII");
	
	lgmk->add_subMark(810+365);
	lgmk->add_subMarkName("Part VIII");
	
	lgmk->add_subMark(810+540);
	lgmk->add_subMarkName("Part IX");
	
	cout << lgmk << endl;
	wave->add_chunk(lgmk);
	
	wave->save("Pink_Floyd-Shine_On_You_Crazy_Diamond.rab");*/ 
	
	/*meta->setTitle("From Peer Gynt Suite No. 1 Op 46 & No. 2 Op 55");
	meta->setAuthor("Edvar Grieg");
	meta->setLanguage("English");
	meta->setPublisher("MCR Productions");
	meta->setYear("1988");
	meta->setAddress("United Kingdom");
	meta->setpages("0");
	
	cout << meta << endl;
	wave->add_chunk(meta);
	
	lgmk->add_mark(0);
	lgmk->add_markName("Suite No. 1 Op 46");
	
	lgmk->add_mark();
	lgmk->add_markName("Suite No. 2 Op 55");
	//---------------------------------
	lgmk->add_subMark(0);
	lgmk->add_subMarkName("Morning Mood");
	
	lgmk->add_subMark(180+38);
	lgmk->add_subMarkName("Aases's Death");
	
	lgmk->add_subMark(180+38+240+27);
	lgmk->add_subMarkName("Anitra's Dance");
	
	lgmk->add_subMark(180+38+240+27+180+18);
	lgmk->add_subMarkName("In The Hall Of The Mountain King");
	
	//---------------
	lgmk->add_subMark();
	lgmk->add_subMarkName("The Abduction (Ingrid's Lament)");

	lgmk->add_subMark();
	lgmk->add_subMarkName("Arabian Dance");
	
	lgmk->add_subMark();
	lgmk->add_subMarkName("Return Of Peer Gynt");
	
	lgmk->add_subMark();
	lgmk->add_subMarkName("Solvejg's Song");
	
	cout << lgmk << endl;
	wave->add_chunk(lgmk);
	
	wave->save("Pink_Floyd-Shine_On_You_Crazy_Diamond.rab");*/
	
	
//}
/*
Parte 1	introdução 1		0	0:00 - 0:28
	primeiro verso		28	0:28 - 0:44
	segundo verso		44	0:44 - 1:00
	terceiro verso		60	1:00 - 1:04
	quarto verso		64	1:04 - 1:19
	quinto verso		79	1:19 - 1:31
	sexto verso		91	1:31 - 1:43 
--------
Parte 2	introdução 2		103	1:43 - 2:12
	sétimo verso		132	2:12 - 2:28
	oitavo verso		148	2:28 - 2:44
	nono verso		164	2:44 - 2:48
	décimo verso		168	2:48 - 3:03
	décimo primeiro verso	183	3:03 - 3:15
	décimo segundo verso	195	3:15 - 3:21
	décimo terceiro verso	201	3:21 - 3:26
	final			206	3:26 - 3:35
	
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

	cout <<"Insira o nome da primeira submarcação: ";
	cin >> name;

	lgmk->add_subMarkName(name);

	count = 2; 
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
			lgmk->add_subMark(0);
			break;
		}
		cout << '\n';
		count++;
	}

	cout << lgmk << endl;

	wave->add_chunk(lgmk);
	wave->save("output.wav");
	
*/

