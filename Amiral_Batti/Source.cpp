//ALLEGRO 5 KÜTÜPHANELERÝ
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
//------------------------------------

//STANDART C KÜTÜPHANELERÝ
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//------------------------------------

//DEÐÝÞKENLER-------------------------
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_EVENT ev;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
int bgWidth = 0;
int bgHeight = 0;
int width = 640;
int height = 640;
int yatay = 1;
int dikey = 2;
int rastgeleX;
int rastgeleY;
int kareler[10][10];
int mouseX;
int mouseY;
bool devam = true;
int ortakDizi[100][4];
//-------------------------------------

//FONKSÝYON DECLARE--------------------
void Gemi1Yerlestir();
void Gemi2Yerlestir();
void Gemi3Yerlestir();
void Gemi4Yerlestir();
void Gemi5Yerlestir();
//-------------------------------------

int main()
{
	int i, j, k, l, x, y;

	//Kareler oluþturulur ve içleri sýfýrlanýr
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			kareler[i][j] = 0;
		}
	}

	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 4; j++)
		{
			ortakDizi[i][j] = 0;
		}
	}
	//----------------------------------

	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "failed allegro", NULL, NULL);
		return -1;
	}

	display = al_create_display(width, height);
	
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_mouse();


	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	background = al_load_bitmap("bg.jpg");

	bgWidth = al_get_bitmap_width(background);
	bgHeight = al_get_bitmap_height(background);
	
	
	al_draw_bitmap(background, 0, 0, 0);
	
	k = 0;
	for (i = 0; i < width-50; i+=60)
	{
		for (j = 0; j < height-50; j+=60)
		{
			al_draw_rectangle(j+20, i+20, j+80, i+80, al_map_rgb(0, 0, 255), 3);//x1,y1,x2,y2,renk,border
			for (k; k < 100; )
			{
				ortakDizi[k][0] = j + 20;
				ortakDizi[k][1] = i + 20;
				ortakDizi[k][2] = j + 80;
				ortakDizi[k][3] = i + 80;
				k++;
				break;
			}
		}
	}

		
	Gemi1Yerlestir();
	Gemi2Yerlestir();
	Gemi3Yerlestir();
	Gemi4Yerlestir();
	Gemi5Yerlestir();

	for (i = 0; i < 10; i++)
	{
		printf("\n\n");
		for (j = 0; j < 10; j++)
		{
			printf("%2d ",kareler[i][j]);
		}
	}
		
	
	al_flip_display();


	while (devam)
	{

		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouseX = ev.mouse.x;
			mouseY = ev.mouse.y;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)//týklama olayý
		{
			if (ev.mouse.button & 1)//sol veya scrol týklamasý
			{
				for (i = 0; i < width - 50; i += 60)
				{
					for (j = 0; j < height - 50; j += 60)
					{
						if (mouseX > j + 20 && mouseY > i + 20 && mouseX < j + 80 && mouseY < i + 80)
						{
							printf("(%d-%d),(%d-%d)\t",j+20, i+20, j+80, i+80);
							for  (k = 0; k < 100; k++)
							{
								if ((ortakDizi[k][0] == j + 20) && (ortakDizi[k][1] == i + 20) && (ortakDizi[k][2] == j + 80) && (ortakDizi[k][3] == i + 80))
								{
									printf("KARE NUMARASI: %d\n", k);
									if (kareler[k / 10][k % 10] != 0)
									{
										al_draw_filled_rectangle(j + 20, i + 20, j + 80, i + 80, al_map_rgb(255, 0, 0));
									}
								}
							}
						}
					}
				}
				al_flip_display();
			}
		}
	}


	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL, "failed display", NULL, NULL);
		return -1;
	}
	

	al_destroy_bitmap(background);
	al_destroy_display(display);

	return 0;
}

//FONKSÝYONLAR-------------------------------------
void Gemi1Yerlestir()
{
	int gemiSayisi = 5;
	srand(time(NULL));
	while (gemiSayisi > 0)
	{
		rastgeleX = rand() % 10;
		rastgeleY = rand() % 10;
		
		if (kareler[rastgeleX][rastgeleY] == 0)
		{
			kareler[rastgeleX][rastgeleY] = 1;
			gemiSayisi--;
		}
	}
}
void Gemi2Yerlestir()
{
	int yon;
	int gemiSayisi = 4;
	srand(time(NULL));
	
	
	while (gemiSayisi > 0)
	{
		rastgeleX = rand() % 9;
		rastgeleY = rand() % 9;

		if (kareler[rastgeleX][rastgeleY] == 0)
		{
			yon = rand() % 2 + 1;
			if (yon == yatay && kareler[rastgeleX][rastgeleY + 1] == 0)
			{
				kareler[rastgeleX][rastgeleY] = 2;
				kareler[rastgeleX][rastgeleY + 1] = 2;
				gemiSayisi--;
			}
			else if (yon == dikey && kareler[rastgeleX + 1][rastgeleY] == 0)
			{
				kareler[rastgeleX][rastgeleY] = 2;
				kareler[rastgeleX + 1][rastgeleY] = 2;
				gemiSayisi--;
			}
			
		}
	}
}
void Gemi3Yerlestir()
{
	int yon;
	int gemiSayisi = 3;
	srand(time(NULL));


	while (gemiSayisi > 0)
	{
		rastgeleX = rand() % 8;
		rastgeleY = rand() % 8;

		if (kareler[rastgeleX][rastgeleY] == 0)
		{
			yon = rand() % 2 + 1;
			if (yon == yatay && kareler[rastgeleX][rastgeleY + 1] == 0 && kareler[rastgeleX][rastgeleY + 2] == 0)
			{
				kareler[rastgeleX][rastgeleY] = 3;
				kareler[rastgeleX][rastgeleY + 1] = 3;
				kareler[rastgeleX][rastgeleY + 2] = 3;
				gemiSayisi--;
			}
			else if (yon == dikey && kareler[rastgeleX + 1][rastgeleY] == 0 && kareler[rastgeleX + 2][rastgeleY] == 0)
			{
				kareler[rastgeleX][rastgeleY] = 3;
				kareler[rastgeleX + 1][rastgeleY] = 3;
				kareler[rastgeleX + 2][rastgeleY] = 3;
				gemiSayisi--;
			}

		}
	}
}
void Gemi4Yerlestir()
{
	int yon;
	int gemiSayisi = 2;
	srand(time(NULL));


	while (gemiSayisi > 0)
	{
		rastgeleX = rand() % 7;
		rastgeleY = rand() % 7;

		if (kareler[rastgeleX][rastgeleY] == 0)
		{
			yon = rand() % 2 + 1;
			if (yon == yatay && kareler[rastgeleX][rastgeleY + 1] == 0 && kareler[rastgeleX][rastgeleY + 2] == 0 && kareler[rastgeleX][rastgeleY + 3] == 0)
			{
				kareler[rastgeleX][rastgeleY] = 4;
				kareler[rastgeleX][rastgeleY + 1] = 4;
				kareler[rastgeleX][rastgeleY + 2] = 4;
				kareler[rastgeleX][rastgeleY + 3] = 4;
				gemiSayisi--;
			}
			else if (yon == dikey && kareler[rastgeleX + 1][rastgeleY] == 0 && kareler[rastgeleX + 2][rastgeleY] == 0 && kareler[rastgeleX + 3][rastgeleY] == 0)
			{
				kareler[rastgeleX][rastgeleY] = 4;
				kareler[rastgeleX + 1][rastgeleY] = 4;
				kareler[rastgeleX + 2][rastgeleY] = 4;
				kareler[rastgeleX + 3][rastgeleY] = 4;
				gemiSayisi--;
			}

		}
	}
}
void Gemi5Yerlestir()
{
	int yon;
	int gemiSayisi = 1;
	srand(time(NULL));


	while (gemiSayisi > 0)
	{
		rastgeleX = rand() % 6;
		rastgeleY = rand() % 6;

		if (kareler[rastgeleX][rastgeleY] == 0)
		{
			yon = rand() % 2 + 1;
			if (yon == yatay && kareler[rastgeleX][rastgeleY + 1] == 0 && kareler[rastgeleX][rastgeleY + 2] == 0 && kareler[rastgeleX][rastgeleY + 3] == 0 && kareler[rastgeleX][rastgeleY + 4] == 0)
			{
				kareler[rastgeleX][rastgeleY] = 5;
				kareler[rastgeleX][rastgeleY + 1] = 5;
				kareler[rastgeleX][rastgeleY + 2] = 5;
				kareler[rastgeleX][rastgeleY + 3] = 5;
				kareler[rastgeleX][rastgeleY + 4] = 5;
				gemiSayisi--;
			}
			else if (yon == dikey && kareler[rastgeleX + 1][rastgeleY] == 0 && kareler[rastgeleX + 2][rastgeleY] == 0 && kareler[rastgeleX + 3][rastgeleY] == 0 && kareler[rastgeleX + 4][rastgeleY] == 0)
			{
				kareler[rastgeleX][rastgeleY] = 5;
				kareler[rastgeleX + 1][rastgeleY] = 5;
				kareler[rastgeleX + 2][rastgeleY] = 5;
				kareler[rastgeleX + 3][rastgeleY] = 5;
				kareler[rastgeleX + 4][rastgeleY] = 5;
				gemiSayisi--;
			}

		}
	}
}
//-------------------------------------------------