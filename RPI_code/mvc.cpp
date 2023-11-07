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
#include <cstdint>
#include <cmath>

#define UI_WIDTH 480
#define UI_HEIGHT 320

#define BORDER_COLOR 60,60,61

//charts
#define C_WIDTH_S 	95		//Start
#define C_HEIGHT_S 	150		//Start
#define C_WIDTH_R 	350		//Relative
#define C_HEIGHT_R 	140		//Relative

uint8_t tx_buff[9] = {0,0,0,0,0,0,0,0,0};

enum WND_ID
{
	ID_ROOT = 1,
	ID_BUTTON_1,
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
		list_box_1->select_item(0);

		c_list_box* list_box_2 = (c_list_box*)get_wnd_ptr(ID_LIST_BOX_2);
		list_box_2->clear_item();
		list_box_2->add_item((char*)"CH1");
		list_box_2->add_item((char*)"CH2");
		list_box_2->select_item(0);

		c_button* main_button_1 = (c_button*)get_wnd_ptr(ID_BUTTON_1);
		main_button_1->set_on_click((WND_CALLBACK)&MVC::on_button_click);
		


		c_spin_box *c_spin_box_1 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_1);
		c_spin_box_1->set_value(5000);
		c_spin_box_1->set_max_min(5000,50);

		c_spin_box *c_spin_box_2 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_2);
		c_spin_box_2->set_value(180);
		c_spin_box_2->set_max_min(360,0);

		c_spin_box *c_spin_box_3 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_3);
		c_spin_box_3->set_value_digit(2);
		c_spin_box_3->set_step(1);
		c_spin_box_3->set_value(165);
		c_spin_box_3->set_max_min(330,0);
		
		c_spin_box *c_spin_box_4 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_4);
		c_spin_box_4->set_value_digit(2);
		c_spin_box_4->set_step(1);
		c_spin_box_4->set_value(165);
		c_spin_box_4->set_max_min(330,0);
		

	}
	virtual void on_paint(void)
	{
		m_surface->fill_rect(0, 0, UI_WIDTH, UI_HEIGHT,GL_RGB(27, 27, 27),Z_ORDER_LEVEL_0);
		m_surface->draw_rect(4, 34, 81, C_HEIGHT_S+C_HEIGHT_R+1,GL_RGB(60,60,60),Z_ORDER_LEVEL_2);//buttons rect
		m_surface->draw_rect(94, 34, 446, 180,GL_RGB(60,60,60),Z_ORDER_LEVEL_2);//params rect
		m_surface->draw_rect(C_WIDTH_S-1,C_HEIGHT_S-1,C_WIDTH_S+C_WIDTH_R+1,C_HEIGHT_S+C_HEIGHT_R+1,GL_RGB(60,60,60),Z_ORDER_LEVEL_2);//chart rect

		draw_chart();
		draw_sine(330,0,0);
	}

	void draw_sqr(int amp_h,int amp_l,int freq){
		int j=0;
		for (int i = -1; i < 10; i+=2)
		{
			//vertical segments
			if(i != -1	)
				m_surface->draw_line(C_WIDTH_S+C_WIDTH_R/10*(i), C_HEIGHT_S+C_HEIGHT_R/2-amp_h, C_WIDTH_S+C_WIDTH_R/10*(i), C_HEIGHT_S+C_HEIGHT_R/2+amp_l,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
			
			if(j%2 == 0)
			{
				if(C_WIDTH_S+C_WIDTH_R*i/10 > C_WIDTH_S)
					m_surface->draw_line(C_WIDTH_S+C_WIDTH_R*i/10, C_HEIGHT_S+C_HEIGHT_R/2-amp_h, C_WIDTH_S+C_WIDTH_R*(i+2)/10, C_HEIGHT_S+C_HEIGHT_R/2-amp_h,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
				else
					m_surface->draw_line(C_WIDTH_S, C_HEIGHT_S+C_HEIGHT_R/2-amp_h, C_WIDTH_S+C_WIDTH_R/10, C_HEIGHT_S+C_HEIGHT_R/2-amp_h,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
			}
			else
			{
				if(C_WIDTH_S+C_WIDTH_R*(i+2)/10 < C_WIDTH_S+C_WIDTH_R)
					m_surface->draw_line(C_WIDTH_S+C_WIDTH_R*i/10, C_HEIGHT_S+C_HEIGHT_R/2+amp_l, C_WIDTH_S+C_WIDTH_R*(i+2)/10, C_HEIGHT_S+C_HEIGHT_R/2+amp_l,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
				else
					m_surface->draw_line(C_WIDTH_S+C_WIDTH_R*i/10, C_HEIGHT_S+C_HEIGHT_R/2+amp_l, C_WIDTH_S+C_WIDTH_R, C_HEIGHT_S+C_HEIGHT_R/2+amp_l,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
			}
			j++;
		}
	}
	void draw_ramp(float amp_h, float amp_l,float duty, int offset){

		amp_h = (amp_h/100)*C_HEIGHT_R/10;
		amp_l = (amp_l/100)*C_HEIGHT_R/10;
		offset = offset/100*C_HEIGHT_R/10;

		for(int i=0; i<10 ;i+=2)	
		{	
			m_surface->draw_line(C_WIDTH_S+C_WIDTH_R*i/10, C_HEIGHT_S+C_HEIGHT_R/2+amp_l, C_WIDTH_S+(C_WIDTH_R*(i+2*duty)/10), C_HEIGHT_S+C_HEIGHT_R/2-amp_h, GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
			m_surface->draw_line(C_WIDTH_S+(C_WIDTH_R*(i+2*duty)/10), C_HEIGHT_S+C_HEIGHT_R/2-amp_h, C_WIDTH_S+C_WIDTH_R*(i+2)/10, C_HEIGHT_S+C_HEIGHT_R/2+amp_l, GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
		}
	}

	void draw_sine(float amp, float phase, float offset){

		amp = (amp/100)*C_HEIGHT_R/10;
		offset = offset/100*C_HEIGHT_R/10;

		for(int i=0;i<350;i++)	
			m_surface->draw_line((C_WIDTH_S+i),((C_HEIGHT_S+C_HEIGHT_R/2)-amp*sin(i*3.14/(C_WIDTH_R/4)-(3.14/2.5)+phase))-offset,(C_WIDTH_S+i+1),((C_HEIGHT_S+C_HEIGHT_R/2)-amp*sin((i+1)*3.14/(C_WIDTH_R/4)-(3.14/2.5)+phase))-offset,GL_RGB(255, 255, 0),Z_ORDER_LEVEL_2);
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

	void on_button_click(int ctrl_id, int value)
	{
		char sys_str[50] = "i2ctransfer -y 1 w9@0x28";
		char aux[4];

		for (int i = 0; i < 9; i++)
		{
			snprintf(aux, 4, " %d",tx_buff[i]);
			strcat(sys_str,aux);
		}
		
		system(sys_str);
	}

	void on_listbox_confirm(int ctrl_id, int value)
	{
		switch (value)
		{
			case 0://sine
				draw_chart();
				draw_sine(330,3.14/2,0);
				break;
			case 1://ramp
				draw_chart();
				draw_ramp(165,0,.5,0);
				break;
			case 2://square
				draw_chart();
				draw_sqr(C_HEIGHT_R/10*4,C_HEIGHT_R/10*4,4);
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
	public:

	void get_values(void)
	{
		c_list_box *c_list_box_1 = (c_list_box*)get_wnd_ptr(ID_LIST_BOX_1);
		c_list_box *c_list_box_2 = (c_list_box*)get_wnd_ptr(ID_LIST_BOX_2);

		c_spin_box *c_spin_box_1 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_1);
		c_spin_box *c_spin_box_2 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_2);
		c_spin_box *c_spin_box_3 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_3);
		c_spin_box *c_spin_box_4 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_4);

		tx_buff[0] = c_list_box_1->get_selected_item();							//Wave
		tx_buff[1] = c_list_box_2->get_selected_item();							//CHN

		tx_buff[2] = (uint8_t)((c_spin_box_1->get_cur_value() >> 8) & 0xFF); 	//Freq MSB
		tx_buff[3] = (uint8_t)(c_spin_box_1->get_cur_value() & 0xFF);			//Freq LSB

		tx_buff[4] = c_spin_box_3->get_cur_value()*255/330;						//Amplitude High
		tx_buff[5] = 0x00;//c_spin_box_4->get_cur_value()/255;					//Amplitude Low
		
		tx_buff[6] = c_spin_box_4->get_cur_value()*255/330;						//Offset

		tx_buff[7] = (uint8_t)((c_spin_box_2->get_cur_value() >> 8) & 0xFF); 	//Freq MSB
		tx_buff[8] = (uint8_t)(c_spin_box_2->get_cur_value() & 0xFF);			//Freq LSB

	}

	void set_spinbox_value(bool dir, short ID)
	{
		c_list_box *c_list_box_1 = (c_list_box*)get_wnd_ptr(ID_LIST_BOX_1);

		c_spin_box *c_spin_box_1 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_1);
		c_spin_box *c_spin_box_2 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_2);
		c_spin_box *c_spin_box_3 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_3);
		c_spin_box *c_spin_box_4 = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_4);
		
		int freq = c_spin_box_1->get_cur_value();
		int phase = c_spin_box_2->get_cur_value();
		int amp = c_spin_box_3->get_cur_value();
		int offset = c_spin_box_4->get_cur_value();


		switch (ID)
		{
			case ID_SPIN_BOX_1:
				if(dir == 1 && freq < 5000)
					freq += 10;
				else
					if(dir == 0 && freq > 50)
						freq -= 10;
				c_spin_box_1->set_value(freq);
				break;
			case ID_SPIN_BOX_2:
				if(dir == 1 && phase < 360)
					phase += 10;
				else
					if(dir == 0 && phase > 0)
						phase -= 10;
				c_spin_box_2->set_value(phase);
				break;
			case ID_SPIN_BOX_3:
				if(dir == 1 && amp < 330)
					amp += 10;
				else
					if(dir == 0 && amp > 0)
						amp -= 10;
				c_spin_box_3->set_value(amp);
				break;
			case ID_SPIN_BOX_4:
				if(dir == 1 && offset < 330)
					offset += 5;
				else
					if(dir == 0 && offset > 0)
						offset -= 5;
				c_spin_box_4->set_value(offset);
				break;
			
			default:
				break;
		}

		draw_chart();
		switch (c_list_box_1->get_selected_item())
		{
		case 0:
			draw_sine(amp,phase*3.14/180,offset);
			break;
		case 1:
			draw_ramp(amp,0,.5,0);
			break;
		case 2:
			draw_sqr(30,30,5);
			break;
		case 3:
			draw_noise();
			break;
		default:
			break;
		}
	}

	int get_focus_item(void)
	{
		c_spin_box* s_spin_box[4]; 
		s_spin_box[0] = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_1);
		s_spin_box[1] = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_2);
		s_spin_box[2] = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_3);
		s_spin_box[3] = (c_spin_box*)get_wnd_ptr(ID_SPIN_BOX_4);
		
		for (int i = 0; i < 4; i++)
		{
			if( s_spin_box[i]->is_focus() == 2 )
			{
				return s_spin_box[i]->get_id();
			}
		}

		return 0;

	}

};

static MVC mvc;
static c_button main_button_1;
static c_list_box list_box_1,list_box_2;

static c_spin_box s_spin_box_1,s_spin_box_2,s_spin_box_3,s_spin_box_4;


WND_TREE main_window[] =
{
	{ &list_box_1,		ID_LIST_BOX_1,		"Sine",				5, 35, 75, 50},
	{ &list_box_2,		ID_LIST_BOX_2,		"CH1",				5, 85, 75, 50},
	{ &main_button_1,	ID_BUTTON_1,		"Enter",			5, 235, 75, 50},
	{ &s_spin_box_1,	ID_SPIN_BOX_1,		"Frequency",		95+15+75, 35+116/4-30/2, 75, 30},
	{ &s_spin_box_2,	ID_SPIN_BOX_2,		"Phase",			95+15+75, 35+116*2/4+30/2, 75, 30},
	{ &s_spin_box_3,	ID_SPIN_BOX_3,		"Amplitude",		95+190+75, 35+116/4-30/2, 75, 30},
	{ &s_spin_box_4,	ID_SPIN_BOX_4,		"Offset",			95+190+75, 35+116*2/4+30/2, 75, 30},
	{NULL, 0, 0, 0, 0, 0, 0}
};

extern const LATTICE_FONT_INFO Arial_16B;
void load_resource()
{
	c_theme::add_font(FONT_DEFAULT, &Arial_16B);
	c_theme::add_color(COLOR_WND_FONT, GL_RGB(255, 255, 243));
	c_theme::add_color(COLOR_WND_NORMAL, GL_RGB(27,27,27));
	c_theme::add_color(COLOR_WND_PUSHED, GL_RGB(60,60,60));
	c_theme::add_color(COLOR_WND_FOCUS, GL_RGB(75, 75, 75));
	c_theme::add_color(COLOR_WND_BORDER, GL_RGB(60,60,60));
	
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
	int focus;
	while(1)
	{
		int rec = read(fd, buffer, sizeof(buffer));
		if (rec>0){
			buffer[rec]=0;
			mvc.get_values();
			focus = mvc.get_focus_item();
			
			if (strcmp("adelante",buffer)==0){
				//printf("moviendo adelante\n");
				mvc.on_navigate(NAVIGATION_KEY(0));
			}
			if (strcmp("atras",buffer)==0){
				//printf("moviendo atras\n");
				mvc.on_navigate(NAVIGATION_KEY(1));
			}
			if (strcmp("enter",buffer)==0){
				//printf("haciendo enter\n");
				mvc.on_navigate(NAVIGATION_KEY(2));
			}

			if (strcmp("inc",buffer)==0){
				//printf("Incrementando\n");
				mvc.set_spinbox_value(true,focus);
			}
			if (strcmp("dec",buffer)==0){
				//printf("Decrementando\n");
				mvc.set_spinbox_value(false,focus);
			}
			
		}
		
	}
}
