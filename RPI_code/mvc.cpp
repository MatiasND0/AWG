#include <stdio.h>
#define GUILITE_ON
#include "GuiLite.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h>

#define UI_WIDTH 480
#define UI_HEIGHT 320

enum WND_ID
{
	ID_ROOT = 1,
	ID_BUTTON_1,
	ID_BUTTON_2,
	ID_BUTTON_3,
	ID_BUTTON_4,
	ID_BUTTON_5,
	ID_BUTTON_6,
	ID_LIST_BOX,
	ID_DIALOG
};

static c_display *pantalla;

class MVC : public c_wnd
{
	virtual void on_init_children()
	{		
		c_button* main_button_1 = (c_button*)get_wnd_ptr(ID_BUTTON_1);
		c_button* main_button_2 = (c_button*)get_wnd_ptr(ID_BUTTON_2);
		c_button* main_button_3 = (c_button*)get_wnd_ptr(ID_BUTTON_3);
		c_button* main_button_4 = (c_button*)get_wnd_ptr(ID_BUTTON_4);
		main_button_1->set_on_click((WND_CALLBACK)&MVC::on_button_clicked);

	}
	virtual void on_paint(void)
	{
		m_surface->draw_rect(4, 34, 56, 285,GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);
		for(int i=0;i<70;i++)	
			m_surface->draw_pixel((120+i),(25+8*sin(i*3.14/15)),GL_RGB(255, 255, 255),Z_ORDER_LEVEL_0);
	}

	void on_button_clicked(int ctrl_id, int param)
	{
		c_dialog::open_dialog((c_dialog*)get_wnd_ptr(ID_DIALOG));
		printf("Boton Apretado\n");
	}
};

class c_my_dialog : public c_dialog
{
	virtual void on_init_children()
	{
	
		c_list_box  *list_box = (c_list_box*)get_wnd_ptr(ID_LIST_BOX);

		list_box->set_on_change((WND_CALLBACK)&c_my_dialog::on_listbox_confirm);
		list_box->clear_item();
		list_box->add_item((char*)"Sine");
		list_box->add_item((char*)"Square");
		list_box->add_item((char*)"Sawtooth");
		list_box->select_item(0);
		
		c_button* s_button_1 = (c_button*)get_wnd_ptr(ID_BUTTON_6);
		s_button_1->set_on_click((WND_CALLBACK)&c_my_dialog::on_button_clicked);
		
	}
	void on_listbox_confirm(int ctrl_id, int param)
	{
	}

	virtual void on_paint(void)
	{
		m_surface->draw_rect(51, 51, 240, 240,GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);
	}

	void on_button_clicked(int ctrl_id, int param)
	{
		switch (ctrl_id)
		{
		case ID_BUTTON_6:
			c_dialog::close_dialog(m_surface);
			break;
		default:
			break;
		}
	}
};

static MVC mvc;
static c_button main_button_1,main_button_2,main_button_3,main_button_4,main_button_5;

static c_my_dialog s_my_dialog;
static c_list_box s_list_box;
static c_button s_button_1;

WND_TREE s_dialog_widgets[] =
{

	{ &s_button_1,	ID_BUTTON_6,"<-",	0, 0, 50, 50},
	{ &s_list_box,	ID_LIST_BOX,"listBox",	50, 0, 190, 50},
	{NULL, 0 , 0, 0, 0, 0, 0}
};

WND_TREE main_window[] =
{
	{ &main_button_1,	ID_BUTTON_1,	"Sine",		5, 35, 50, 50},
	{ &main_button_2,	ID_BUTTON_2,	"Ramp",		5, 85, 50, 50},
	{ &main_button_3,	ID_BUTTON_3,	"Square",	5, 135, 50, 50},
	{ &main_button_4,	ID_BUTTON_4,	"Pulse",	5, 185, 50, 50},
	{ &main_button_5,	ID_BUTTON_5,	"Noise",	5, 235, 50, 50},

	{ &s_my_dialog,	ID_DIALOG,	"Dialog",	51, 51, 240, 240, s_dialog_widgets},
	{NULL, 0, 0, 0, 0, 0, 0}
};

extern const LATTICE_FONT_INFO Arial_16B;
void load_resource()
{
	c_theme::add_font(FONT_DEFAULT, &Arial_16B);
	c_theme::add_color(COLOR_WND_FONT, GL_RGB(255, 255, 243));
	c_theme::add_color(COLOR_WND_NORMAL, GL_RGB(0, 0, 0));
	c_theme::add_color(COLOR_WND_PUSHED, GL_RGB(255, 255, 255));
	c_theme::add_color(COLOR_WND_FOCUS, GL_RGB(25, 25, 25));
	c_theme::add_color(COLOR_WND_BORDER, GL_RGB(255, 255, 255));
}

void create_ui(void* phy_fb, int screen_width, int screen_height, int color_bytes) {
	load_resource();

	c_surface* s_surface = new c_surface(UI_WIDTH, UI_HEIGHT, color_bytes, Z_ORDER_LEVEL_2);
	pantalla = new c_display(phy_fb, screen_width, screen_height, s_surface);

	mvc.set_surface(s_surface);
	mvc.connect(NULL, ID_ROOT, 0, 0, 0, UI_WIDTH, UI_HEIGHT, main_window);
	mvc.show_window();
}

//Loop Principal.
void mainLoop(void* phy_fb, int width, int height, int color_bytes){
	
	create_ui(phy_fb, width, height, color_bytes);

	char buffer[75];

	int fd = open("/tmp/fifo", O_RDONLY);
	if (fd == -1) {
		perror("Error abriendo el named pipe");
		exit(EXIT_FAILURE);
	}

   // Read the message from the named pipe


	thread_sleep(1);
	
	while(1)
	{
		int rec = read(fd, buffer, sizeof(buffer));
		if (rec>0){
			buffer[rec]=0;
				if (strcmp("adelante",buffer)==0){
					mvc.on_navigate(NAVIGATION_KEY(0));
					printf("moviendo adelante\n");
				}
				if (strcmp("atras",buffer)==0){
					mvc.on_navigate(NAVIGATION_KEY(1));
					printf("moviendo atras\n");
				}
				if (strcmp("enter",buffer)==0){
					mvc.on_navigate(NAVIGATION_KEY(2));
					printf("haciendo enter\n");
				}
		}
		
	}
}
