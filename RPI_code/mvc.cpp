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
	ID_BUTTON_10,
	ID_LABEL_11,
	ID_LABEL_12,
	ID_LABEL_13,
	ID_LABEL_14,
	ID_SPIN_BOX_11,
	ID_SPIN_BOX_12,
	ID_SPIN_BOX_13,
	ID_SPIN_BOX_14,
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
		c_dialog::open_dialog((c_dialog*)get_wnd_ptr(ID_DIALOG));
		m_surface->draw_rect(4, 34, 81, 286,GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);
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
		c_spin_box *c_spin_box_1 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_11);
		c_spin_box *c_spin_box_2 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_12);
		c_spin_box *c_spin_box_3 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_13);
		c_spin_box *c_spin_box_4 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_14);
		c_spin_box_1->set_value(1000);
		c_spin_box_2->set_value(0);
		c_spin_box_3->set_value(5);
		c_spin_box_4->set_value(0);

	}

	virtual void on_paint(void)
	{
		m_surface->draw_rect(94, 34, 446, 150,GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);
		m_surface->draw_rect(94, 149, 446, 286,GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);
		m_surface->fill_rect(95, 150, 445, 285,GL_RGB(25, 25, 25),Z_ORDER_LEVEL_2);
		m_surface->draw_line(95, 217, 445, 217,GL_RGB(100, 100, 100),Z_ORDER_LEVEL_2);
		m_surface->draw_line(95, 184, 445, 184,GL_RGB(50, 50, 50),Z_ORDER_LEVEL_2);
		m_surface->draw_line(95, 251, 445, 251,GL_RGB(50, 50, 50),Z_ORDER_LEVEL_2);

		for(int i=0;i<350;i++)	
			m_surface->draw_line((95+i),(217-50*sin(i*3.14/88)),(95+i+1),(217-50*sin((i+1)*3.14/88)),GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);

		m_surface->draw_line(95+88, 150, 95+88, 285,GL_RGB(50, 50, 50),Z_ORDER_LEVEL_2);
		m_surface->draw_line(95+175, 150, 95+175, 285,GL_RGB(50, 50, 50),Z_ORDER_LEVEL_2);
		m_surface->draw_line(95+265, 150, 95+265, 285,GL_RGB(50, 50, 50),Z_ORDER_LEVEL_2);
	
		m_surface->draw_rect(244, 41, 320, 41+25, GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);
		m_surface->draw_rect(244, 41, 320, 41+2*25, GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);
		m_surface->draw_rect(244, 41, 320, 41+3*25, GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);
		m_surface->draw_rect(244, 41, 320, 41+4*25, GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);

	}

	void on_button_clicked(int ctrl_id, int param)
	{
		switch (ctrl_id)
		{
		case ID_BUTTON_10:
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
static c_label s_label_1, s_label_2, s_label_3, s_label_4;
static c_spin_box s_spin_box_1,s_spin_box_2,s_spin_box_3,s_spin_box_4;

WND_TREE s_dialog_widgets[] =
{
	{ &s_label_1,	ID_LABEL_11,	"Frequency",		15, 0+7, 75, 25},
	{ &s_label_2,	ID_LABEL_12,	"Phase",			15, 25+7, 75, 25},
	{ &s_label_3,	ID_LABEL_13,	"Amplitude",		15, 50+7, 75, 25},
	{ &s_label_4,	ID_LABEL_14,	"Offset",			15, 75+7, 75, 25},
	{ &s_spin_box_1,ID_SPIN_BOX_11,		"1000 Hz",		150, 0+7, 75, 25},
	{ &s_spin_box_2,ID_SPIN_BOX_12,		"0 deg",		150, 25+7, 75, 25},
	{ &s_spin_box_3,ID_SPIN_BOX_13,		"5 Vpp",		150, 50+7, 75, 25},
	{ &s_spin_box_4,ID_SPIN_BOX_14,		"0 V",			150, 75+7, 75, 25},
	{NULL, 0 , 0, 0, 0, 0, 0}
};

WND_TREE main_window[] =
{
	{ &main_button_1,	ID_BUTTON_1,	"Sine",		5, 35, 75, 50},
	{ &main_button_2,	ID_BUTTON_2,	"Ramp",		5, 85, 75, 50},
	{ &main_button_3,	ID_BUTTON_3,	"Square",	5, 135, 75, 50},
	{ &main_button_4,	ID_BUTTON_4,	"Pulse",	5, 185, 75, 50},
	{ &main_button_5,	ID_BUTTON_5,	"Noise",	5, 235, 75, 50},

	{ &s_my_dialog,	ID_DIALOG,	"Dialog",	95, 35, 445, 285, s_dialog_widgets},
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
