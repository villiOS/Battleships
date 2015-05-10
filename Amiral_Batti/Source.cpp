#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
//-----------------------------------
#include <stdio.h>

//DEÐÝÞKENLER------------------------
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_EVENT ev;
int bgWidth = 0;
int bgHeight = 0;
int width = 640;
int height = 640;
//------------------------------------



int main()
{
	int i, j, k, l, x, y;

	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "failed allegro", NULL, NULL);
		return -1;
	}

	display = al_create_display(width, height);
	
	al_init_image_addon();
	al_init_primitives_addon();
	

	background = al_load_bitmap("bg.jpg");

	bgWidth = al_get_bitmap_width(background);
	bgHeight = al_get_bitmap_height(background);
	
	

	//while (true)
	//{	
		
		al_draw_bitmap(background, 0, 0, 0);
		
		for (i = 0; i < width-50; i+=60)
		{
			for (j = 0; j < height-50; j+=60)
			{
				al_draw_rectangle(j+20, i+20, j+80, i+80, al_map_rgb(0, 0, 255), 3);//x1,y1,x2,y2,renk,border
				
			}
		}
		al_draw_filled_rectangle(20, 20, 80, 80, al_map_rgb(0, 0, 255));
		al_flip_display();
		//al_clear_to_color(al_map_rgb(0, 0, 0));
	//}

		
		

	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL, "failed display", NULL, NULL);
		return -1;
	}
	
	
	while (true){}
	
	al_destroy_bitmap(background);
	al_destroy_display(display);

	return 0;
}