//ALLEGRO 5 K�T�PHANELER�
#include <allegro5/allegro.h> //genel i�lemler
#include <allegro5/allegro_native_dialog.h> //mesaj verdirme ekranlar� i�in
#include <allegro5/allegro_image.h> //image kullan�mlar� i�in
#include <allegro5/allegro_audio.h> // ses dosyas� kullan�mlar�
#include <allegro5/allegro_acodec.h> // farkl� formatlardaki ses doslar�n�n ��z�lebilmesi i�in
#include <allegro5/allegro_primitives.h> // �ekil �izdirme i�lemleri i�in
#include <allegro5\allegro_ttf.h> // ttf uzant�s� dosyalar�n okunabilmesi i�in
#include <allegro5\allegro_font.h> // font dosyalar�n�n okunabilmesi i�in
//------------------------------------

//STANDART C K�T�PHANELER�
#include <stdio.h> // standart C dili input/output i�lemleri i�in
#include <time.h> // standart C dili zamanla ilgili i�lemler i�in
#include <stdlib.h>
#include <conio.h>
#include <dos.h> // Console'a �zel i�lemler i�in
//------------------------------------

//DE���KENLER-------------------------
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *bomba = NULL; //image tutar
ALLEGRO_BITMAP *su = NULL; //image tutar
ALLEGRO_BITMAP *background = NULL; //image tutar
ALLEGRO_EVENT ev; //mouse, klavye gibi input cihazlar�n�n olaylar�n� dinler
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_FONT *font = NULL; // fontlar� tutar
int bgWidth = 0;
int bgHeight = 0;
int width = 640;
int height = 640;
int yatay = 1; //gemilerin yerle�im y�n�
int dikey = 2; //gemilerin yerle�im y�n�
int rastgeleX;
int rastgeleY;
int kareler[10][10];
int mouseX; //mouse un anl�k olarak x koordinat�n� tutar
int mouseY; //mouse un anl�k olarak y koordinat�n� tutar
bool devam = true;
int ortakDizi[100][4];
int fullPuan = 104; //  herbir geminin uzunlu�u * gemi say�s�
int puan = 0;
int toplamGemiParcasi = 35; // gemilerin uzunluklar� toplam�
bool ilkOyunMu = true; //ilk oyun ise gerekli imageler, fontlar vs y�klensin, ikinci oyun ba�lad���nda tekrar y�klenmesin.��nk� zaten RAM de var.
int sira = 1; // 1: oyuncu, 2: bilgisayar
//-------------------------------------

//FONKS�YON DECLARE--------------------
void Gemi1Yerlestir(); // 1 uzunluklu gemiler yerle�tirilir.
void Gemi2Yerlestir(); // 2 uzunluklu gemiler yerle�tirilir.
void Gemi3Yerle�tir(); // 3 uzunluklu gemiler yerle�tirilir.
void Gemi4Yerle�tir(); // 4 uzunluklu gemiler yerle�tirilir.
void Gemi5Yerlestir(); // 5 uzunluklu gemiler yerle�tirilir.
//-------------------------------------

int main()
{
restart:
	int i, j, k, l, x, y;

	//Kareler olu�turulur ve i�leri s�f�rlan�r
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

	//Herhangi bir nedenden dolay� allegro k�t�phanesi y�klenemez ise hata ver.
	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "failed allegro", NULL, NULL);
		return -1;
	}

	display = al_create_display(width, height); // A��lan ekran�n boyutlar� init ediliyor.

	al_init_image_addon(); // imageleri kullanabilmek i�in addon init ediliyor.
	al_init_primitives_addon(); // �ekilleri kullanabilmek i�in addon init ediliyor.
	al_install_keyboard(); // klavye olaylar�n� kullanabilmek i�in addon init ediliyor.
	al_install_mouse(); // mouse olaylar�n� kullanabilmek i�in addon init ediliyor.
	al_install_audio(); // sesleri kullanabilmek i�in addon init ediliyor.
	al_init_acodec_addon(); // ses formatlar�n� kullanabilmek i�in addon init ediliyor.
	al_init_font_addon(); // fontlar� kullanabilmek i�in addon init ediliyor.
	al_init_ttf_addon(); // ttf dosyalar�n� kullanabilmek i�in addon init ediliyor.


	event_queue = al_create_event_queue(); // klavye, mouse gibi ayg�tlar�n olaylar�n� bir kuyru�a atar. S�ras� ile i�lemek i�in
	al_register_event_source(event_queue, al_get_display_event_source(display)); //Eventler i�in Ram de yer ay�r�r.
	al_register_event_source(event_queue, al_get_mouse_event_source()); //Eventler i�in Ram de yer ay�r�r.
	al_register_event_source(event_queue, al_get_keyboard_event_source()); //Eventler i�in Ram de yer ay�r�r.


	bomba = al_load_bitmap("bomba.jpg"); //resmi RAM e ��kar.
	if (!al_filename_exists("bomba.jpg")) // E�er resim yoksa -1 d�nd�r
	{
		return -1;
	}

	su = al_load_bitmap("su.jpg"); //resmi RAM e ��kar.
	if (!al_filename_exists("su.jpg")) // E�er resim yoksa -1 d�nd�r
	{
		return -1;
	}

	background = al_load_bitmap("bg.jpg");
	if (!al_filename_exists("bg.jpg"))
	{
		return -1;
	}

	al_reserve_samples(2);

	ALLEGRO_SAMPLE *sample = al_load_sample("bomb.ogg"); // Ses dosyas�n� RAM e ��kar
	if (!al_filename_exists("bomb.ogg"))
	{
		return -1;
	}

	ALLEGRO_SAMPLE *sample2 = al_load_sample("su.ogg");
	if (!al_filename_exists("su.ogg"))
	{
		return -1;
	}


	bgWidth = al_get_bitmap_width(background);
	bgHeight = al_get_bitmap_height(background);

	if (ilkOyunMu)
	{
		font = al_load_font("font.TTF", 38, NULL); // font dosyas� ad�, punto, NULL
		if (!al_filename_exists("font.TTF"))
		{
			return -1;
		}
	}
	if (!al_install_keyboard())
	{
		return -1;
	}

	for (i = 0; i < 255; i++)
	{
		al_draw_filled_rectangle(0, 0, 640, 640, al_map_rgb(i, 0, 0));
		al_draw_textf(font, al_map_rgb(255 - i, 0, 0), 320, 200, ALLEGRO_ALIGN_CENTRE, "HOS GELDINIZ");
		al_flip_display();
		Sleep(4);
	}

	for (i = 0; i < 255; i++)
	{
		al_draw_filled_rectangle(0, 0, 640, 640, al_map_rgb(255, 0, 0));
		al_draw_textf(font, al_map_rgb(i, 0, 0), 320, 230, ALLEGRO_ALIGN_CENTRE, "...OYUN BASLIYOR...");
		al_flip_display();
		Sleep(4);
	}

	for (i = 0; i < 255; i++)
	{
		al_draw_filled_rectangle(0, 0, 640, 640, al_map_rgb(255, 0, 0));
		al_draw_textf(font, al_map_rgb(i, 0, 0), 320, 280, ALLEGRO_ALIGN_CENTRE, "3");
		al_flip_display();
		Sleep(4);
	}
	for (i = 0; i < 255; i++)
	{
		al_draw_filled_rectangle(0, 0, 640, 640, al_map_rgb(255, 0, 0));
		al_draw_textf(font, al_map_rgb(i, 0, 0), 320, 280, ALLEGRO_ALIGN_CENTRE, "2");
		al_flip_display();
		Sleep(4);
	}
	for (i = 0; i < 255; i++)
	{
		al_draw_filled_rectangle(0, 0, 640, 640, al_map_rgb(255, 0, 0));
		al_draw_textf(font, al_map_rgb(i, 0, 0), 320, 280, ALLEGRO_ALIGN_CENTRE, "1");
		al_flip_display();
		Sleep(4);
	}
	for (i = 0; i < 255; i++)
	{
		al_draw_filled_rectangle(0, 0, 640, 640, al_map_rgb(255, 0, 0));
		al_draw_textf(font, al_map_rgb(i, 0, 0), 320, 280, ALLEGRO_ALIGN_CENTRE, "HAZIR");
		al_flip_display();
		Sleep(4);
	}

	al_draw_bitmap(background, 0, 0, 0); //Background resmini 0,0 konumundan itibaren konumland�r.



	/**************Kareler Olu�turulur*****************/
	k = 0;
	for (i = 0; i < width - 50; i += 60)
	{
		for (j = 0; j < height - 50; j += 60)
		{
			al_draw_rectangle(j + 20, i + 20, j + 80, i + 80, al_map_rgb(0, 0, 255), 3);//x1,y1,x2,y2,renk,border
			for (k; k < 100;)
			{
				//ortak dizi i�erisinde x1,y1,x2,y2 de�erleri tutulur.
				ortakDizi[k][0] = j + 20;
				ortakDizi[k][1] = i + 20;
				ortakDizi[k][2] = j + 80;
				ortakDizi[k][3] = i + 80;
				k++;
				break;
			}
		}
	}
	/**************************************************/

	//Gemiler yerle�tiriliyor.
	Gemi1Yerlestir();//5
	Gemi2Yerlestir();//4
	Gemi3Yerle�tir();//3
	Gemi4Yerle�tir();//2
	Gemi5Yerlestir();//1
	//--------------------------------------------------

	//Konsol ekran�ndan loglar� takip etmek i�in
	for (i = 0; i < 10; i++)
	{
		printf("\n\n");
		for (j = 0; j < 10; j++)
		{
			printf("%2d ", kareler[i][j]);
		}
	}
	//---------------------------------------------------	


	al_flip_display(); // De�i�iklikleri grafik ekrana yans�t



	toplamGemiParcasi = 35;
	while (devam && toplamGemiParcasi > 0)
	{
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)// Anl�k olarak mouse un x,y koordinatlar�n� al�r
		{
			mouseX = ev.mouse.x;
			mouseY = ev.mouse.y;
		}
		
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)//t�klama eventini yakalar
		{
			if (ev.mouse.button & 1)//sol veya scroll t�klamas�
			{
				for (i = 0; i < width - 50; i += 60)
				{
					for (j = 0; j < height - 50; j += 60)
					{
						if (mouseX > j + 20 && mouseY > i + 20 && mouseX < j + 80 && mouseY < i + 80)//t�klama yap�lan yer bir kare i�ine denk geliyor mu kontrol�
						{
							printf("\n(%d-%d),(%d-%d)\t", j + 20, i + 20, j + 80, i + 80);
							for (k = 0; k < 100; k++)
							{
								if ((ortakDizi[k][0] == j + 20) && (ortakDizi[k][1] == i + 20) && (ortakDizi[k][2] == j + 80) && (ortakDizi[k][3] == i + 80))
								{
									printf("KARE NUMARASI: %d\n", k);
									if (kareler[k / 10][k % 10] != 0 && kareler[k / 10][k % 10] != -1) //daha �nce t�klanmam�� kare i�i ise ve vurmu�sam
									{

										al_draw_bitmap(bomba, j + 20, i + 20, 0); //bomba image ini �iz.

										al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

										al_rest(1.0);

										puan = puan + kareler[k / 10][k % 10]; //vurulan at�� �d�l� gemi uzunlu�u kadard�r.

										kareler[k / 10][k % 10] = -1;//vurmu� ise -1 yap ki tekrar vuramas�n

										toplamGemiParcasi--;

										printf("Isabetli Atis. Puaniniz: %d\n", puan);

									}
									else if (kareler[k / 10][k % 10] == 0)//vuramam��sam
									{
										al_draw_bitmap(su, j + 20, i + 20, 0); // su image ini �iz.

										al_play_sample(sample2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); // sample2 sesi 1 saniye �alar ve durur.

										al_rest(1.0); //1 saniye boyunca sesin �almas�n� bekler.

										puan = puan - 1; //hatal� at�� cezas� -1 puand�r.

										kareler[k / 10][k % 10] = -1;  //hatal� at�� yap�lan kareyi kapat�r.

										printf("\tKaravana Atis. Puaniniz: %d\n", puan);
									}

								}
							}
						}
					}
				}
				al_flip_display();
			}
			else if (ev.mouse.button & 2) // sa� t�klama eventini yakalar
			{
				if (al_show_native_message_box(display, "OYUNDAN CIK", "\nCIKMAK ISTEDIGINIZE \n\nEMIN MISINIZ?", "", NULL, ALLEGRO_MESSAGEBOX_YES_NO) == 1)
				{
					break;
				}
			}
		}

	}//while

	printf("%d", puan);



	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL, "failed display", NULL, NULL);
		return -1;
	}
	for (i = 0; i < 255; i++)
	{
		al_draw_filled_rectangle(255 - i, 255 - i, 385 + i, 385 + i, al_map_rgb(0, 0, 255 - i));
		al_flip_display();
		Sleep(1);
	}
	al_draw_filled_rectangle(200, 100, 440, 340, al_map_rgb(255, 0, 0));
	al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 200, ALLEGRO_ALIGN_CENTRE, "OYUN BITTI");
	al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 220, ALLEGRO_ALIGN_CENTRE, "PUANINIZ");
	al_draw_textf(font, al_map_rgb(255, 255, 255), 320, 250, ALLEGRO_ALIGN_CENTRE, "%d", puan);
	puan = 0;


	al_draw_filled_rectangle(200, 350, 440, 400, al_map_rgb(255, 0, 0));
	al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 365, ALLEGRO_ALIGN_CENTRE, "YENI OYUN");

	al_draw_filled_rectangle(200, 410, 440, 460, al_map_rgb(255, 0, 0));
	al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 425, ALLEGRO_ALIGN_CENTRE, "CIKIS");


	al_flip_display();

	devam = true;

	while (devam)
	{
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouseX = ev.mouse.x;
			mouseY = ev.mouse.y;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)//t�klama olay�
		{
			if (ev.mouse.button & 1)//sol veya scrol t�klamas�
			{
				if (mouseX > 200 && mouseX < 440 && mouseY > 350 && mouseY < 400)//Yeni oyun kutusuna m� t�klad�m?
				{
					ilkOyunMu = false;
					al_destroy_bitmap(background);
					al_destroy_bitmap(su);
					al_destroy_bitmap(bomba);
					al_destroy_display(display);
					goto restart;
				}
				else if (mouseX > 200 && mouseX < 440 && mouseY > 410 && mouseY < 460)// ��k�� kutusuna m� t�klad�m?
				{
					break;
				}
			}
		}
	}

	al_destroy_bitmap(background);
	al_destroy_bitmap(su);
	al_destroy_bitmap(bomba);
	al_destroy_display(display);
	al_destroy_font(font);

	return 0;
}

//FONKS�YONLAR-------------------------------------
void Gemi1Yerlestir()
{
	int gemiSayisi = 5;//soru ��kabilir.
	srand(time(NULL));
	while (gemiSayisi > 0)
	{
		rastgeleX = rand() % 10;
		rastgeleY = rand() % 10;

		if (kareler[rastgeleX][rastgeleY] == 0)//Gemi yerle�ecek yer bo�sa
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
		rastgeleX = rand() % 9; //9 yap�l�r ��nk� gemiler 2 birim. Dizi ta�mas� olmaz b�ylece
		rastgeleY = rand() % 9;

		if (kareler[rastgeleX][rastgeleY] == 0)
		{
			yon = rand() % 2 + 1; //geminin yerle�ece�i y�n belirlenir.
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
void Gemi3Yerle�tir()
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
void Gemi4Yerle�tir()
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