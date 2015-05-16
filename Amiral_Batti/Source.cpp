//ALLEGRO 5 KÜTÜPHANELERÝ
#include <allegro5/allegro.h> //genel iþlemler
#include <allegro5/allegro_native_dialog.h> //mesaj verdirme ekranlarý için
#include <allegro5/allegro_image.h> //image kullanýmlarý için
#include <allegro5/allegro_audio.h> // ses dosyasý kullanýmlarý
#include <allegro5/allegro_acodec.h> // farklý formatlardaki ses doslarýnýn çözülebilmesi için
#include <allegro5/allegro_primitives.h> // þekil çizdirme iþlemleri için
#include <allegro5\allegro_ttf.h> // ttf uzantýsý dosyalarýn okunabilmesi için
#include <allegro5\allegro_font.h> // font dosyalarýnýn okunabilmesi için
//------------------------------------

//STANDART C KÜTÜPHANELERÝ
#include <stdio.h> // standart C dili input/output iþlemleri için
#include <time.h> // standart C dili zamanla ilgili iþlemler için
#include <stdlib.h>
#include <conio.h>
#include <dos.h> // Console'a özel iþlemler için
//------------------------------------

//DEÐÝÞKENLER-------------------------
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *bomba = NULL; //image tutar
ALLEGRO_BITMAP *su = NULL; //image tutar
ALLEGRO_BITMAP *background = NULL; //image tutar
ALLEGRO_EVENT ev; //mouse, klavye gibi input cihazlarýnýn olaylarýný dinler
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_FONT *font = NULL; // fontlarý tutar
int bgWidth = 0;
int bgHeight = 0;
int width = 640;
int height = 640;
int yatay = 1; //gemilerin yerleþim yönü
int dikey = 2; //gemilerin yerleþim yönü
int rastgeleX;
int rastgeleY;
int kareler[10][10];
int mouseX; //mouse un anlýk olarak x koordinatýný tutar
int mouseY; //mouse un anlýk olarak y koordinatýný tutar
bool devam = true;
int ortakDizi[100][4];
int fullPuan = 104; //  herbir geminin uzunluðu * gemi sayýsý
int puan = 0;
int toplamGemiParcasi = 35; // gemilerin uzunluklarý toplamý
bool ilkOyunMu = true; //ilk oyun ise gerekli imageler, fontlar vs yüklensin, ikinci oyun baþladýðýnda tekrar yüklenmesin.Çünkü zaten RAM de var.
int sira = 1; // 1: oyuncu, 2: bilgisayar
//-------------------------------------

//FONKSÝYON DECLARE--------------------
void Gemi1Yerlestir(); // 1 uzunluklu gemiler yerleþtirilir.
void Gemi2Yerlestir(); // 2 uzunluklu gemiler yerleþtirilir.
void Gemi3Yerleþtir(); // 3 uzunluklu gemiler yerleþtirilir.
void Gemi4Yerleþtir(); // 4 uzunluklu gemiler yerleþtirilir.
void Gemi5Yerlestir(); // 5 uzunluklu gemiler yerleþtirilir.
//-------------------------------------

int main()
{
restart:
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

	//Herhangi bir nedenden dolayý allegro kütüphanesi yüklenemez ise hata ver.
	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "failed allegro", NULL, NULL);
		return -1;
	}

	display = al_create_display(width, height); // Açýlan ekranýn boyutlarý init ediliyor.

	al_init_image_addon(); // imageleri kullanabilmek için addon init ediliyor.
	al_init_primitives_addon(); // þekilleri kullanabilmek için addon init ediliyor.
	al_install_keyboard(); // klavye olaylarýný kullanabilmek için addon init ediliyor.
	al_install_mouse(); // mouse olaylarýný kullanabilmek için addon init ediliyor.
	al_install_audio(); // sesleri kullanabilmek için addon init ediliyor.
	al_init_acodec_addon(); // ses formatlarýný kullanabilmek için addon init ediliyor.
	al_init_font_addon(); // fontlarý kullanabilmek için addon init ediliyor.
	al_init_ttf_addon(); // ttf dosyalarýný kullanabilmek için addon init ediliyor.


	event_queue = al_create_event_queue(); // klavye, mouse gibi aygýtlarýn olaylarýný bir kuyruða atar. Sýrasý ile iþlemek için
	al_register_event_source(event_queue, al_get_display_event_source(display)); //Eventler için Ram de yer ayýrýr.
	al_register_event_source(event_queue, al_get_mouse_event_source()); //Eventler için Ram de yer ayýrýr.
	al_register_event_source(event_queue, al_get_keyboard_event_source()); //Eventler için Ram de yer ayýrýr.


	bomba = al_load_bitmap("bomba.jpg"); //resmi RAM e çýkar.
	if (!al_filename_exists("bomba.jpg")) // Eðer resim yoksa -1 döndür
	{
		return -1;
	}

	su = al_load_bitmap("su.jpg"); //resmi RAM e çýkar.
	if (!al_filename_exists("su.jpg")) // Eðer resim yoksa -1 döndür
	{
		return -1;
	}

	background = al_load_bitmap("bg.jpg");
	if (!al_filename_exists("bg.jpg"))
	{
		return -1;
	}

	al_reserve_samples(2);

	ALLEGRO_SAMPLE *sample = al_load_sample("bomb.ogg"); // Ses dosyasýný RAM e çýkar
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
		font = al_load_font("font.TTF", 38, NULL); // font dosyasý adý, punto, NULL
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

	al_draw_bitmap(background, 0, 0, 0); //Background resmini 0,0 konumundan itibaren konumlandýr.



	/**************Kareler Oluþturulur*****************/
	k = 0;
	for (i = 0; i < width - 50; i += 60)
	{
		for (j = 0; j < height - 50; j += 60)
		{
			al_draw_rectangle(j + 20, i + 20, j + 80, i + 80, al_map_rgb(0, 0, 255), 3);//x1,y1,x2,y2,renk,border
			for (k; k < 100;)
			{
				//ortak dizi içerisinde x1,y1,x2,y2 deðerleri tutulur.
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

	//Gemiler yerleþtiriliyor.
	Gemi1Yerlestir();//5
	Gemi2Yerlestir();//4
	Gemi3Yerleþtir();//3
	Gemi4Yerleþtir();//2
	Gemi5Yerlestir();//1
	//--------------------------------------------------

	//Konsol ekranýndan loglarý takip etmek için
	for (i = 0; i < 10; i++)
	{
		printf("\n\n");
		for (j = 0; j < 10; j++)
		{
			printf("%2d ", kareler[i][j]);
		}
	}
	//---------------------------------------------------	


	al_flip_display(); // Deðiþiklikleri grafik ekrana yansýt



	toplamGemiParcasi = 35;
	while (devam && toplamGemiParcasi > 0)
	{
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)// Anlýk olarak mouse un x,y koordinatlarýný alýr
		{
			mouseX = ev.mouse.x;
			mouseY = ev.mouse.y;
		}
		
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)//týklama eventini yakalar
		{
			if (ev.mouse.button & 1)//sol veya scroll týklamasý
			{
				for (i = 0; i < width - 50; i += 60)
				{
					for (j = 0; j < height - 50; j += 60)
					{
						if (mouseX > j + 20 && mouseY > i + 20 && mouseX < j + 80 && mouseY < i + 80)//týklama yapýlan yer bir kare içine denk geliyor mu kontrolü
						{
							printf("\n(%d-%d),(%d-%d)\t", j + 20, i + 20, j + 80, i + 80);
							for (k = 0; k < 100; k++)
							{
								if ((ortakDizi[k][0] == j + 20) && (ortakDizi[k][1] == i + 20) && (ortakDizi[k][2] == j + 80) && (ortakDizi[k][3] == i + 80))
								{
									printf("KARE NUMARASI: %d\n", k);
									if (kareler[k / 10][k % 10] != 0 && kareler[k / 10][k % 10] != -1) //daha önce týklanmamýþ kare içi ise ve vurmuþsam
									{

										al_draw_bitmap(bomba, j + 20, i + 20, 0); //bomba image ini çiz.

										al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

										al_rest(1.0);

										puan = puan + kareler[k / 10][k % 10]; //vurulan atýþ ödülü gemi uzunluðu kadardýr.

										kareler[k / 10][k % 10] = -1;//vurmuþ ise -1 yap ki tekrar vuramasýn

										toplamGemiParcasi--;

										printf("Isabetli Atis. Puaniniz: %d\n", puan);

									}
									else if (kareler[k / 10][k % 10] == 0)//vuramamýþsam
									{
										al_draw_bitmap(su, j + 20, i + 20, 0); // su image ini çiz.

										al_play_sample(sample2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0); // sample2 sesi 1 saniye çalar ve durur.

										al_rest(1.0); //1 saniye boyunca sesin çalmasýný bekler.

										puan = puan - 1; //hatalý atýþ cezasý -1 puandýr.

										kareler[k / 10][k % 10] = -1;  //hatalý atýþ yapýlan kareyi kapatýr.

										printf("\tKaravana Atis. Puaniniz: %d\n", puan);
									}

								}
							}
						}
					}
				}
				al_flip_display();
			}
			else if (ev.mouse.button & 2) // sað týklama eventini yakalar
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
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)//týklama olayý
		{
			if (ev.mouse.button & 1)//sol veya scrol týklamasý
			{
				if (mouseX > 200 && mouseX < 440 && mouseY > 350 && mouseY < 400)//Yeni oyun kutusuna mý týkladým?
				{
					ilkOyunMu = false;
					al_destroy_bitmap(background);
					al_destroy_bitmap(su);
					al_destroy_bitmap(bomba);
					al_destroy_display(display);
					goto restart;
				}
				else if (mouseX > 200 && mouseX < 440 && mouseY > 410 && mouseY < 460)// Çýkýþ kutusuna mý týkladým?
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

//FONKSÝYONLAR-------------------------------------
void Gemi1Yerlestir()
{
	int gemiSayisi = 5;//soru çýkabilir.
	srand(time(NULL));
	while (gemiSayisi > 0)
	{
		rastgeleX = rand() % 10;
		rastgeleY = rand() % 10;

		if (kareler[rastgeleX][rastgeleY] == 0)//Gemi yerleþecek yer boþsa
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
		rastgeleX = rand() % 9; //9 yapýlýr çünkü gemiler 2 birim. Dizi taþmasý olmaz böylece
		rastgeleY = rand() % 9;

		if (kareler[rastgeleX][rastgeleY] == 0)
		{
			yon = rand() % 2 + 1; //geminin yerleþeceði yön belirlenir.
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
void Gemi3Yerleþtir()
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
void Gemi4Yerleþtir()
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