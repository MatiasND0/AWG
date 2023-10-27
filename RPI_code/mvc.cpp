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

//charts
#define C_WIDTH_S 	95		//Start
#define C_HEIGHT_S 	150		//Start
#define C_WIDTH_R 	350		//Relative
#define C_HEIGHT_R 	140		//Relative

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
	ID_SPIN_BOX_1,
	ID_SPIN_BOX_2,
	ID_SPIN_BOX_3,
	ID_SPIN_BOX_4,
	ID_LIST_BOX_1,
	ID_LIST_BOX_2
};

static c_display *pantalla;

class MVC : public c_wnd
{
	virtual void on_init_children()
	{		

		c_list_box* list_box_1 = (c_list_box*)get_wnd_ptr(ID_LIST_BOX_1);
		list_box_1->set_on_change((WND_CALLBACK)&MVC::on_listbox_confirm);
		list_box_1->clear_item();
		list_box_1->add_item((char*)"Sine");
		list_box_1->add_item((char*)"Ramp");
		list_box_1->add_item((char*)"Square");
		list_box_1->add_item((char*)"Noise");
		list_box_1->add_item((char*)"Pulse");
		list_box_1->select_item(3);

		c_list_box* list_box_2 = (c_list_box*)get_wnd_ptr(ID_LIST_BOX_2);
		list_box_2->clear_item();
		list_box_2->add_item((char*)"CH1");
		list_box_2->add_item((char*)"CH2");
		list_box_2->select_item(0);


		c_button* main_button_3 = (c_button*)get_wnd_ptr(ID_BUTTON_3);
		c_button* main_button_4 = (c_button*)get_wnd_ptr(ID_BUTTON_4);
		c_button* main_button_5 = (c_button*)get_wnd_ptr(ID_BUTTON_5);

		c_spin_box *c_spin_box_1 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_1);
		c_spin_box *c_spin_box_2 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_2);
		c_spin_box *c_spin_box_3 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_3);
		c_spin_box *c_spin_box_4 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_4);
		c_spin_box_1->set_value(1000);
		c_spin_box_2->set_value(0);
		c_spin_box_3->set_value(5);
		c_spin_box_4->set_value(0);
		c_spin_box_1->set_on_change((WND_CALLBACK)&MVC::on_spinbox_change);

	}
	virtual void on_paint(void)
	{
		m_surface->draw_rect(4, 34, 81, 286,GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);//buttons rect
		//m_surface->draw_rect(94, 34, 446, 180,GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);//params rect
		m_surface->draw_rect(C_WIDTH_S-1,C_HEIGHT_S-1,C_WIDTH_S+C_WIDTH_R+1,C_HEIGHT_S+C_HEIGHT_R+1,GL_RGB(255, 255, 255),Z_ORDER_LEVEL_2);//chart rect

		draw_chart();
		draw_sqr(C_HEIGHT_R/10*4,C_HEIGHT_R/10*2,4);
	}

	void draw_sqr(int amp_h,int amp_l,int freq){
		int j=0;
		for (int i = -1; i < 10; i+=2)
		{
			m_surface->draw_line(C_WIDTH_S+C_WIDTH_R/10*(i), C_HEIGHT_S+C_HEIGHT_R/2-amp_h, C_WIDTH_S+C_WIDTH_R/10*(i), C_HEIGHT_S+C_HEIGHT_R/2+amp_l,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
			if(j%2 == 0)
				m_surface->draw_line(C_WIDTH_S+C_WIDTH_R*i/10, C_HEIGHT_S+C_HEIGHT_R/2-amp_h, C_WIDTH_S+C_WIDTH_R*(i+2)/10, C_HEIGHT_S+C_HEIGHT_R/2-amp_h,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
			else
				m_surface->draw_line(C_WIDTH_S+C_WIDTH_R*i/10, C_HEIGHT_S+C_HEIGHT_R/2+amp_l, C_WIDTH_S+C_WIDTH_R*(i+2)/10, C_HEIGHT_S+C_HEIGHT_R/2+amp_l,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
			j++;
		}
		

		//segmentos verticales
		//m_surface->draw_line(95+88, 184, 95+88, 251,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		//m_surface->draw_line(95+175, 184, 95+175, 251,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		//m_surface->draw_line(95+265, 184, 95+265, 251,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);

		//segmentos horizontales
		//m_surface->draw_line(95, 184, 95+88, 184,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		//m_surface->draw_line(95+88, 251, 95+87*2, 251,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		//m_surface->draw_line(95+88*2, 184, 95+88*3, 184,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		//m_surface->draw_line(95+88*3+1, 251, 95+88*4-1, 251,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
	}

	void draw_ramp(void){
		m_surface->draw_line(95, 217, 95+(88*1)/2, 217-2*34, GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		m_surface->draw_line(95+(88*1)/2, 217-2*34, 95+(88*3)/2, 217+1*34, GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		m_surface->draw_line(95+(88*3)/2, 217+1*34, 95+(88*5)/2, 217-2*34, GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		m_surface->draw_line(95+(88*5)/2, 217-2*34, 95+(88*7)/2, 217+1*34, GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		m_surface->draw_line(95+(88*7)/2, 217+1*34, 94+(88*8)/2, 217-0*34, GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
	}

	void draw_sine(int amp, int phase){
		for(int i=0;i<350;i++)	
			m_surface->draw_line((C_WIDTH_S+i),((C_HEIGHT_S+C_HEIGHT_R/2)-amp*sin(i*3.14/(C_WIDTH_R/4)-(3.14/2.5)+phase)),(95+i+1),((C_HEIGHT_S+C_HEIGHT_R/2)-amp*sin((i+1)*3.14/(C_WIDTH_R/4)-(3.14/2.5)+phase)),GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
	}
	
	void draw_noise(void){
		int last_noise = 0;
		int noise = 0;
		for(int i=0;i<350;i++)
		{
			noise = rand()%70;
			m_surface->draw_line(95+i ,217+35-noise ,95+i+1 ,217+35-last_noise ,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
			last_noise = noise;
		}
	}	

	void draw_chart(void){
		//chart background
		m_surface->fill_rect(C_WIDTH_S,C_HEIGHT_S,C_WIDTH_S+C_WIDTH_R,C_HEIGHT_S+C_HEIGHT_R,GL_RGB(25, 25, 25),Z_ORDER_LEVEL_0);

		//grid vertical
		for (int i=1;i<11;i++)
			m_surface->draw_line(C_WIDTH_S+i*(C_WIDTH_R/10), C_HEIGHT_S, C_WIDTH_S+i*(C_WIDTH_R/10), C_HEIGHT_S+C_HEIGHT_R,GL_RGB(50, 50, 50),Z_ORDER_LEVEL_2);
	
		for (int i=1;i<11;i++)
			m_surface->draw_line(C_WIDTH_S, C_HEIGHT_S+i*(C_HEIGHT_R/10), C_WIDTH_S+C_WIDTH_R, C_HEIGHT_S+i*(C_HEIGHT_R/10),GL_RGB(50, 50, 50),Z_ORDER_LEVEL_1);

		//axies
		m_surface->draw_line(C_WIDTH_S, C_HEIGHT_S+C_HEIGHT_R/2, C_WIDTH_S+C_WIDTH_R, C_HEIGHT_S+C_HEIGHT_R/2,GL_RGB(100, 100, 100),Z_ORDER_LEVEL_2);
		m_surface->draw_line(C_WIDTH_S+(C_WIDTH_R/10), C_HEIGHT_S, C_WIDTH_S+(C_WIDTH_R/10), C_HEIGHT_S+C_HEIGHT_R, GL_RGB(100, 100, 100),Z_ORDER_LEVEL_2);


	}	

	

	void on_listbox_confirm(int ctrl_id, int value)
	{
		switch (value)
		{
			case 0://sine
				draw_chart();
				draw_sine(30,3.14/2);
				break;
			case 1://ramp
				draw_chart();
				draw_ramp();
				break;
			case 2://square
				draw_chart();
				draw_sqr(50,25,10);
				break;
			case 3://pulse
				draw_chart();
				draw_noise();
				break;
			case 4://noise
				draw_chart();
				break;
			default:
				break;
		}
	}

	void on_spinbox_change(int ctrl_id, int param)
	{
		printf("Boton Apretado\n");
	}
};

static MVC mvc;
static c_button main_button_1,main_button_2,main_button_3,main_button_4,main_button_5;
static c_list_box list_box_1,list_box_2;

static c_label s_label_1, s_label_2, s_label_3, s_label_4;
static c_spin_box s_spin_box_1,s_spin_box_2,s_spin_box_3,s_spin_box_4;


WND_TREE main_window[] =
{
	{ &list_box_1,		ID_LIST_BOX_1,		"Sine",				5, 35, 75, 50},
	{ &list_box_2,		ID_LIST_BOX_2,		"CH1",				5, 85, 75, 50},
	{ &main_button_3,	ID_BUTTON_3,		"B",			5, 135, 75, 50},
	{ &main_button_4,	ID_BUTTON_4,		"C",			5, 185, 75, 50},
	{ &main_button_5,	ID_BUTTON_5,		"D",			5, 235, 75, 50},
	{ &s_spin_box_1,	ID_SPIN_BOX_1,		"1000 Hz",			95+15+75, 35+116/4-30/2, 75, 30},
	{ &s_spin_box_2,	ID_SPIN_BOX_2,		"0 deg",			95+15+75, 35+116*2/4+30/2, 75, 30},
	{ &s_spin_box_3,	ID_SPIN_BOX_3,		"5 Vpp",			95+190+75, 35+116/4-30/2, 75, 30},
	{ &s_spin_box_4,	ID_SPIN_BOX_4,		"0 V",				95+190+75, 35+116*2/4+30/2, 75, 30},
	{ &s_label_1,		ID_LABEL_11,		"Frequency",		95+15, 35+116/4-30/2, 75, 30},
	{ &s_label_2,		ID_LABEL_12,		"Phase",			95+15, 35+116*2/4+30/2, 75, 30},
	{ &s_label_3,		ID_LABEL_13,		"Amplitude",		95+190, 35+116/4-30/2, 75, 30},
	{ &s_label_4,		ID_LABEL_14,		"Offset",			95+190, 35+116*2/4+30/2, 75, 30},
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
