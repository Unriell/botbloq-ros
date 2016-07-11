//#include "OscillatorSerp.h"
#include "gusano.h"

#include "iostream"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

struct termios orig_termios;

void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
	struct termios new_termios;

	/* take two copies - one for now, one for later */
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	/* register cleanup handler, and set the new terminal mode */
	atexit(reset_terminal_mode);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) {
		return r;
	} else {
		return c;
	}
}
int main(int argc, char **argv)
{
	bool olaActual = true;
	set_conio_terminal_mode();
	ros::init(argc, argv, "serp_control");
	ROS_INFO("Empieza Nodo Control Serpiente");


	Gusano gusanoVer;
	Gusano gusanoHor;
	std::string inicioTopico = "/serp/box_cilinder_joint";
	std::string finalTopico = "_position_controller/command";
	std::string topico1 = inicioTopico+"1"+finalTopico;
	gusanoVer.add_servo(topico1);
	std::string topico2 = inicioTopico+"2"+finalTopico;
	gusanoHor.add_servo(topico2);
	std::string topico3 = inicioTopico + "3"+finalTopico;
	gusanoVer.add_servo(topico3);
	std::string topico4 = inicioTopico+"4"+finalTopico;
	gusanoHor.add_servo(topico4);
	std::string topico5 = inicioTopico+"5"+finalTopico;
	gusanoVer.add_servo(topico5);
	std::string topico6 = inicioTopico+"6"+finalTopico;
	gusanoHor.add_servo(topico6);
	std::string topico7 = inicioTopico+"7"+finalTopico;
	gusanoVer.add_servo(topico7);
	std::string topico8 = inicioTopico+"8"+finalTopico;
	gusanoHor.add_servo(topico8);
	std::string topico9 = inicioTopico+"9"+finalTopico;
	gusanoVer.add_servo(topico9);

	Wave wave1[] = {
			//Periodo Amp Offset Dif_fase FaseIni
			{2000, 20,  0,  0,      90},  //-- rodar
			{1000, 30,  0,   60,        0},  //-- lateral
			{1000, 20, 0,   0,      90},  //-- rotacion
			{1000, 15, 0,   60,      0},   //-- lombriz
	};

	Wave wave2[] = {
			//Periodo Amp Offset Dif_fase FaseIni
			{2000, 20,  0,  0,      0},   //-- rodar
			{1000, 30,  0,   60,        0},  //-- lateral
			{1000, 180/9, 0,   60,      0},  //-- rotacion
			{1000, 0, 0,   0,      0},  //-- lombriz
	};

	//-- numeros de waves definidas
	//-- numero de filas de la estructura wave
	const int NWAVES = sizeof(wave1)/sizeof(Wave);

	int wi=2;

	gusanoVer.set_wave(wave1[wi]);
	gusanoHor.set_wave(wave2[wi]);

	/*gusanoVer.set_wave(wave1[wi],1);
	gusanoVer.set_wave(wave1[wi],3);
	gusanoVer.set_wave(wave1[wi],5);
	gusanoVer.set_wave(wave1[wi],7);
	gusanoVer.set_wave(wave1[wi],9);
	gusanoVer.set_wave(wave2[wi],2);
	gusanoVer.set_wave(wave2[wi],4);
	gusanoVer.set_wave(wave2[wi],6);
	gusanoVer.set_wave(wave2[wi],8);*/
	ROS_INFO("Empieza Nodo Control Serpiente");
	while(ros::ok()){
		while(!kbhit()){

			gusanoVer.refresh();
			gusanoHor.refresh();
		}
		int letra = getch();
		std::cout<<letra<<std::endl;
		reset_terminal_mode();
		//CAMBIO DE SENTIDO AL PULSAR ENTER
		if (letra==10 & olaActual==true){
			gusanoVer.set_wave(wave2[wi]);
			gusanoHor.set_wave(wave1[wi]);
			olaActual=false;
		}else{
			gusanoVer.set_wave(wave1[wi]);
			gusanoHor.set_wave(wave2[wi]);
			olaActual=true;
		}
	}

}



