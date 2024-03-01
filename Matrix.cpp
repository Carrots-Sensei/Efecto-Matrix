#include "pch.h" // Inclusión de librerías
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <Windows.h>
#include <time.h>

#include <allegro5/allegro.h> // Allegro en General
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h> // Fuentes de Letras
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h> // Manejo de Imágenes
#include <allegro5/allegro_audio.h> // Manejo de Sonido
#include <allegro5/allegro_acodec.h>

using namespace std;

// Variables Globales
#define FPS 30.0

const int MaxHileras = 5;

int ContadorCaracteres = 0; // Contabilizan los datos de la última ejecución
int ContadorHileras = MaxHileras;
float DiferenciaTiempo = 0;

bool Condicion = true;
char CaracterActual[1];

// Elementos Allegro
ALLEGRO_DISPLAY *Pantalla = NULL;
ALLEGRO_FONT *Fuente;
ALLEGRO_SAMPLE *Sample;

// Declaración de estructuras
struct Hilera // Línea de caracteres que llueve en pantalla
{
	char Caracter0 = ' '; // Define los atributos de la estructura
	char Caracter1 = ' ';
	char Caracter2 = ' ';
	char Caracter3 = ' ';
	char Caracter4 = ' ';
	char Caracter5 = ' ';
	char Caracter6 = ' ';
	char Caracter7 = ' ';
	char Caracter8 = ' ';
	int x; // Almacena su posición en el display
	int y;
	int mode; // Almacena un valor que determina el largo de la hilera
}X;
struct ConjuntoEstadisticas
{
	float DiferenciaTiempo; // Define una estructura que almacena las características de cada ejecución
	int ContadorCaracteres;
	int ContadorHileras;
}Z;

// Funciones de creación de caracteres
long g_seed = 1;
inline int fastrand() { // Genera números random rápidamente
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}
char GenerarCaracter() // Genera el nuevo caracter que aparece al final de cada hilera
{
	char c = 'A' + fastrand() % 24;
	return c;
}

// Funciones de control de hileras
void InicializarHileras(Hilera ConjuntoHileras[MaxHileras])
{
	for (int i = 0; i < MaxHileras; i++) // Posiciona las hileras iniciales en algún sitio aleatorio en pantalla
	{
		ConjuntoHileras[i].x = rand() % 720; // Coordenada X
		ConjuntoHileras[i].y = rand() % 200; // Coordenada Y
		ConjuntoHileras[i].mode = 3 + rand() % 6; // Inicializa un valor para el largo de la hilera
	}
}
void DesplazarCaracteres(Hilera &Cadena)
{
	Cadena.Caracter8 = Cadena.Caracter7; // Desplaza los Caracteres dentro de una cadena
	Cadena.Caracter7 = Cadena.Caracter6;
	Cadena.Caracter6 = Cadena.Caracter5;
	Cadena.Caracter5 = Cadena.Caracter4;
	Cadena.Caracter4 = Cadena.Caracter3;
	Cadena.Caracter3 = Cadena.Caracter2;
	Cadena.Caracter2 = Cadena.Caracter1;
	Cadena.Caracter1 = Cadena.Caracter0;
}
void MostrarHileras(Hilera ConjuntoHileras[MaxHileras]) // Dibuja las hileras en pantalla
{
	char CaracterActual[1]; // Buffer que almacena un caracter individual
	for (int i = 0; i < MaxHileras; i++)
	{
		int X = ConjuntoHileras[i].x; // Variable que almacena la Coordenada X
		char Caracter = GenerarCaracter(); // Se pide un nuevo caracter para imprimir al final
		ContadorCaracteres++; // Incrementa el contador por cada vez que se genera una nueva letra

		CaracterActual[0] = ConjuntoHileras[i].Caracter0; // Almacena el caracter de la posición solicitada para luego imprimirlo
		al_draw_text(Fuente, al_map_rgb(0, 200, 0), X, ConjuntoHileras[i].y - 20, ALLEGRO_ALIGN_CENTRE, CaracterActual);
		CaracterActual[0] = ConjuntoHileras[i].Caracter1;
		al_draw_text(Fuente, al_map_rgb(0, 180, 0), X, ConjuntoHileras[i].y - 40, ALLEGRO_ALIGN_CENTRE, CaracterActual);

		if (ConjuntoHileras[i].mode == 4) // Hilera de 4 caracteres
		{
			CaracterActual[0] = ConjuntoHileras[i].Caracter2;
			al_draw_text(Fuente, al_map_rgb(0, 160, 0), X, ConjuntoHileras[i].y - 60, ALLEGRO_ALIGN_CENTRE, CaracterActual);
		}
		else if (ConjuntoHileras[i].mode == 5) // Hilera de 5 caracteres
		{
			CaracterActual[0] = ConjuntoHileras[i].Caracter2;
			al_draw_text(Fuente, al_map_rgb(0, 160, 0), X, ConjuntoHileras[i].y - 60, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter3;
			al_draw_text(Fuente, al_map_rgb(0, 140, 0), X, ConjuntoHileras[i].y - 80, ALLEGRO_ALIGN_CENTRE, CaracterActual);
		}
		else if (ConjuntoHileras[i].mode == 6) // Hilera de 6 caracteres
		{
			CaracterActual[0] = ConjuntoHileras[i].Caracter2;
			al_draw_text(Fuente, al_map_rgb(0, 160, 0), X, ConjuntoHileras[i].y - 60, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter3;
			al_draw_text(Fuente, al_map_rgb(0, 140, 0), X, ConjuntoHileras[i].y - 80, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter4;
			al_draw_text(Fuente, al_map_rgb(0, 120, 0), X, ConjuntoHileras[i].y - 100, ALLEGRO_ALIGN_CENTRE, CaracterActual);
		}
		else if (ConjuntoHileras[i].mode == 7) // Hilera de 7 caracteres
		{
			CaracterActual[0] = ConjuntoHileras[i].Caracter2;
			al_draw_text(Fuente, al_map_rgb(0, 160, 0), X, ConjuntoHileras[i].y - 60, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter3;
			al_draw_text(Fuente, al_map_rgb(0, 140, 0), X, ConjuntoHileras[i].y - 80, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter4;
			al_draw_text(Fuente, al_map_rgb(0, 120, 0), X, ConjuntoHileras[i].y - 100, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter5;
			al_draw_text(Fuente, al_map_rgb(0, 110, 0), X, ConjuntoHileras[i].y - 120, ALLEGRO_ALIGN_CENTRE, CaracterActual);
		}
		else if (ConjuntoHileras[i].mode == 8) // Hilera de 8 caracteres
		{
			CaracterActual[0] = ConjuntoHileras[i].Caracter2;
			al_draw_text(Fuente, al_map_rgb(0, 160, 0), X, ConjuntoHileras[i].y - 60, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter3;
			al_draw_text(Fuente, al_map_rgb(0, 140, 0), X, ConjuntoHileras[i].y - 80, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter4;
			al_draw_text(Fuente, al_map_rgb(0, 120, 0), X, ConjuntoHileras[i].y - 100, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter5;
			al_draw_text(Fuente, al_map_rgb(0, 110, 0), X, ConjuntoHileras[i].y - 120, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter6;
			al_draw_text(Fuente, al_map_rgb(0, 100, 0), X, ConjuntoHileras[i].y - 140, ALLEGRO_ALIGN_CENTRE, CaracterActual);
		}
		else if (ConjuntoHileras[i].mode == 9) // Hilera de 9 caracteres
		{
			CaracterActual[0] = ConjuntoHileras[i].Caracter2;
			al_draw_text(Fuente, al_map_rgb(0, 160, 0), X, ConjuntoHileras[i].y - 60, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter3;
			al_draw_text(Fuente, al_map_rgb(0, 140, 0), X, ConjuntoHileras[i].y - 80, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter4;
			al_draw_text(Fuente, al_map_rgb(0, 120, 0), X, ConjuntoHileras[i].y - 100, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter5;
			al_draw_text(Fuente, al_map_rgb(0, 110, 0), X, ConjuntoHileras[i].y - 120, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter6;
			al_draw_text(Fuente, al_map_rgb(0, 100, 0), X, ConjuntoHileras[i].y - 140, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter7;
			al_draw_text(Fuente, al_map_rgb(0, 80, 0), X, ConjuntoHileras[i].y - 160, ALLEGRO_ALIGN_CENTRE, CaracterActual);
		}
		else if (ConjuntoHileras[i].mode == 10) // Hilera de 10 caracteres
		{
			CaracterActual[0] = ConjuntoHileras[i].Caracter2;
			al_draw_text(Fuente, al_map_rgb(0, 160, 0), X, ConjuntoHileras[i].y - 60, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter3;
			al_draw_text(Fuente, al_map_rgb(0, 140, 0), X, ConjuntoHileras[i].y - 80, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter4;
			al_draw_text(Fuente, al_map_rgb(0, 120, 0), X, ConjuntoHileras[i].y - 100, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter5;
			al_draw_text(Fuente, al_map_rgb(0, 110, 0), X, ConjuntoHileras[i].y - 120, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter6;
			al_draw_text(Fuente, al_map_rgb(0, 100, 0), X, ConjuntoHileras[i].y - 140, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter7;
			al_draw_text(Fuente, al_map_rgb(0, 80, 0), X, ConjuntoHileras[i].y - 160, ALLEGRO_ALIGN_CENTRE, CaracterActual);
			CaracterActual[0] = ConjuntoHileras[i].Caracter8;
			al_draw_text(Fuente, al_map_rgb(0, 60, 0), X, ConjuntoHileras[i].y - 180, ALLEGRO_ALIGN_CENTRE, CaracterActual);
		}

		CaracterActual[0] = Caracter; // Almacena el nuevo caracter generado para luego imprimirlo
		al_draw_text(Fuente, al_map_rgb(255, 255, 255), X, ConjuntoHileras[i].y, ALLEGRO_ALIGN_CENTRE, CaracterActual);

		ConjuntoHileras[i].y += 15; // Desplaza la Coordenada Y de la hilera

		DesplazarCaracteres(ConjuntoHileras[i]); //Desplaza los caracteres de la hilera para hacer espacio al nuevo generado
		ConjuntoHileras[i].Caracter0 = CaracterActual[0]; // Ingresa en la hilera el nuevo caracter

		if (ConjuntoHileras[i].y >= 480) // Cuando las hileras han alcanzado el borde de la pantalla...
		{
			ConjuntoHileras[i].Caracter0 = ' '; // Se asignan nuevos valores arbitrarios para generar hileras nuevas
			ConjuntoHileras[i].Caracter1 = ' ';
			ConjuntoHileras[i].Caracter2 = ' ';
			ConjuntoHileras[i].Caracter3 = ' ';
			ConjuntoHileras[i].Caracter4 = ' ';
			ConjuntoHileras[i].Caracter5 = ' ';
			ConjuntoHileras[i].Caracter6 = ' ';
			ConjuntoHileras[i].Caracter7 = ' ';
			ConjuntoHileras[i].Caracter8 = ' ';

			ConjuntoHileras[i].x = rand() % 720;
			ConjuntoHileras[i].y = rand() % 200;
			ConjuntoHileras[i].mode = 3 + rand() % 6;

			ContadorHileras++; // Se incrementa el contador de hileras
		}
	}
}

// Funciones de archivos
void GuardarEstadisticas() // Función que almacena en un archivo los datos de la última ejecución
{
	FILE *archivo; // Crea un STREAM
	fopen_s(&archivo, "Estadisticas de Ejecucion.txt", "w+"); // Abre o crea el archivo solicitado

	if (archivo == NULL)
	{
		cout << "No se pudo abrir el archivo." << endl << endl;
	}
	else
	{
		fprintf(archivo, "%f\n", DiferenciaTiempo); // Almacena los atributos de la última ejecución
		fprintf(archivo, "%i\n", ContadorCaracteres);
		fprintf(archivo, "%i\n", ContadorHileras);

	}
	fclose(archivo); // Cierra el archivo

}
ConjuntoEstadisticas *CargarEstadisticas() // Función que recupera los datos almacenados en el archivo
{
	FILE *archivo; // Crea un nuevo STREAM
	ConjuntoEstadisticas *Datos = NULL;
	fopen_s(&archivo, "Estadisticas de Ejecucion.txt", "r"); // Abre el archivo solicitado

	if (archivo == NULL)
	{
		cout << "No se pudo abrir el archivo" << endl << endl;
	}
	else
	{
		Datos = new(ConjuntoEstadisticas); // Se crea una estructura que almacene los datos
		while (!feof(archivo)) // Recorre el STREAM hasta alcanzar el final
		{
			fscanf_s(archivo, "%f\n", &Datos->DiferenciaTiempo); // Asigna los valores a los atributos de la estructura
			fscanf_s(archivo, "%i\n", &Datos->ContadorCaracteres);
			fscanf_s(archivo, "%i\n", &Datos->ContadorHileras);
		}
	}
	fclose(archivo); // Cierra el archivo
	return Datos; // Retorna la estructura generada
}

// Función principal
int main(int argc, char **argv)
{
	if (!al_init()) { // Verifica que ALLEGRO se haya inicializado correctamente
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

	Pantalla = al_create_display(720, 480); // Crea un display de resolución 720x480 pixeles
	al_set_window_title(Pantalla, "The Matrix"); // Asigna un nombre al display

	if (!Pantalla) { // Verifica que el display se haya inicializado correctamente
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	// Inicializar constructores de Allegro
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(1000);
	al_init_primitives_addon();
	al_install_keyboard();

	// Inicializar Font
	Fuente = al_load_font("ARCHRISTY.ttf", 16, NULL);

	// Inicializar Fuente de Audio
	Sample = al_load_sample("Soundd.ogg"); //sample always NULL
	al_reserve_samples(1);
	al_play_sample(Sample, 1.0, ALLEGRO_AUDIO_PAN_NONE, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);

	// Creación de Timers
	ALLEGRO_TIMER *PrimerTimer = al_create_timer(5 / FPS);
	ALLEGRO_TIMER *SegundoTimer = al_create_timer(5.0 / FPS);
	ALLEGRO_TIMER *TercerTimer = al_create_timer(1.0 / FPS); // Flip Display

	// Creación de Cola de Eventos
	ALLEGRO_EVENT_QUEUE *ColaEventos = al_create_event_queue();

	// Registro de Eventos
	al_register_event_source(ColaEventos, al_get_timer_event_source(PrimerTimer));
	//al_register_event_source(ColaEventos, al_get_timer_event_source(SegundoTimer));
	al_register_event_source(ColaEventos, al_get_timer_event_source(TercerTimer));
	al_register_event_source(ColaEventos, al_get_keyboard_event_source());

	// Inicialización de Timers
	al_start_timer(PrimerTimer);
	//al_start_timer(SegundoTimer);
	al_start_timer(TercerTimer);

	// Inicializa funciones del teclado
	ALLEGRO_KEYBOARD_STATE estadoTeclado;
	al_get_keyboard_state(&estadoTeclado);

	// Inicializa las hileras que se imprimen en pantalla
	Hilera ConjuntoHileras[MaxHileras];
	InicializarHileras(ConjuntoHileras);

	// Estadísticas
	ConjuntoEstadisticas *DatosEjecución = new(ConjuntoEstadisticas);

	// Cronómetros
	time_t inicio, fin; // Inicializa dos cronómetros
	inicio = time(NULL); // Finaliza el primero (t = 0s)

	while (Condicion == true)
	{
		ALLEGRO_EVENT eventos;
		al_wait_for_event(ColaEventos, &eventos);

		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) // Evento. Presionar una tecla
		{
			switch (eventos.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE: // Presionar tecla de escape
			{
				Condicion = false; // Finaliza la ejecución del código
				break;
			}
			}
		}

		else if (eventos.type == ALLEGRO_EVENT_TIMER) // Evento. Timers
		{
			if (eventos.timer.source == PrimerTimer) // Primer Timer. Control de hileras en el display
			{
				al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpia el display de los objetos dibujados en el frame anterior
				MostrarHileras(ConjuntoHileras); // Dibuja las hileras en pantalla
				al_flip_display(); // Actualiza la información visible en el display
			}
			if (eventos.timer.source == TercerTimer) // Tercer Timer. Dibuja las hileras en pantalla
			{
				al_flip_display(); // Mantiene los objetos en el display visibles en todo momento
			}
		}
	}

	fin = time(NULL); // Finaliza el segundo cronómetro

	double diferencia = difftime(fin, inicio); // Obtiene la diferencia de tiempo
	DiferenciaTiempo = (float)diferencia; // Convierte la diferencia de tiempo a un valor float
	GuardarEstadisticas(); // Guarda las estadísticas de la última ejecución en un archivo de texto

	DatosEjecución = CargarEstadisticas(); // Obtiene las estadísticas almacenadas en archivo
	cout << "Tiempo de ejecucion: " << DatosEjecución->DiferenciaTiempo << endl; // Imprime las estadísticas en pantalla
	cout << "Caracteres generados: " << DatosEjecución->ContadorCaracteres << endl;
	cout << "Hileras generadas: " << DatosEjecución->ContadorHileras << endl;

	al_destroy_display(Pantalla); // Destruye los elementos creados en ALLEGRO
	al_destroy_font(Fuente);
	al_destroy_sample(Sample);

	return 0;
}