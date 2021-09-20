#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
	
	// Declaracion de variables
	const uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_2500;
	uint8_t tc74 = 0x4d;
	uint8_t rtc = 0x68;
	char temperatura,segundos,minutos,horas,dia,fecha,mes,year;
    	time_t tBase = time(NULL); 
	long long seg0 = tBase - 9;
	int var = 1;
	int record = 0;
	int contador = 0;		//Contador para saber si ya se imprimieron los 3 ultimos eventos en el .txt
	char AmPm[2] = {'A','P'};
	int indiceAmPm;
	char diaSemana[][7] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};

	// Variables para transofmar datos a strings
	char sTemp[32],sRecord[32],sSeg[32],sMin[32],sHora[32],sFecha[32],sMes[32],sYear[32],sAmPm[32];
	char format1[200],format2[200];
	char AP[2] = "\0";

	// Inicializamos el buffer,temp en 0 y creamos arreglo de 0 para enviar cada que se quiera leer	
	char buffer[32];
	char ceros[32];
	char temp[32];
	for (int i=0; i<32; i++) buffer[i] = 0;
	for (int i=0; i<32; i++) ceros[i] = 0x00;
	for (int i=0; i<32; i++) temp[i] = 0;
	
	//Creamos e inicializamos el archivo .txt (temporal tambien)
	FILE *archivo;
	archivo = fopen("DataLog.txt", "w");

	//---------Escribimos los valores iniciales al RTC-----------------------
	if (!bcm2835_init()) {
        	printf("bcm2835_init failed. Are you running as root??\n");
        	return 1;
    	}

	// Damos direccion del RTC e iniciamos el divisor del clock
	bcm2835_i2c_setSlaveAddress(rtc);
	bcm2835_i2c_setClockDivider(clk_div);

	//Valores inciales para los regsitros del RTC, el primer 0x00 hace referencia al registro base para empezar a escribir
	char genesis[8] = {0x00,0x00,0x00,0x52,0x01,0x01,0x01,0x01};
	bcm2835_i2c_write(genesis,8);	
	bcm2835_close();

	while(1){
		tBase = time(NULL);
       		long long tAhora = tBase;
		
		//Cambiamos la direccion a la del tc74
		if (!bcm2835_init()) {
        		printf("bcm2835_init failed. Are you running as root??\n");
        	return 1;
    		}

		bcm2835_i2c_setSlaveAddress(tc74);
		bcm2835_i2c_setClockDivider(clk_div);
		bcm2835_i2c_write(ceros, 1);		//para leer desde r0
		bcm2835_i2c_read(temp, 1);
		bcm2835_close();

		temperatura = (int)temp[0];

		// Revisamos la temperatura
		if(temperatura > 50){

			if (!bcm2835_init()) {
        			printf("bcm2835_init failed. Are you running as root??\n");
        			return 1;
    			}

			// Volvemos a poner valor de rtc para leerlo
			bcm2835_i2c_setSlaveAddress(rtc);
			bcm2835_i2c_setClockDivider(clk_div);
			bcm2835_i2c_write(ceros, 1);				//para leer desde r0
			bcm2835_i2c_read(buffer, 7);
			bcm2835_close();

			// BCD a decimal
			segundos = buffer[0]&0x7f;				//no tomamos en cuenta bit 7			
			segundos = ((segundos/16)*10) + segundos%16;
			minutos = ((buffer[1]/16)*10) + buffer[1]%16;
			horas = buffer[2]&0x1f;					//no tomamos en cuenta bit 5(am/pm)
			horas = ((horas/16)*10) + horas%16;
			dia = ((buffer[3]/16)*10) + buffer[3]&16;
			fecha = ((buffer[4]/16)*10) + buffer[4]%16;
			mes = ((buffer[5]/16)*10) + buffer[5]%16;
			year = ((buffer[6]/16)*10) + buffer[6]%16;

			//Conmvertimos a strings todos los valores int y char
			sprintf(sTemp,"%d",temperatura);
			sprintf(sRecord,"%d",record);
			sprintf(sSeg,"%02d",segundos);
			sprintf(sMin,"%02d",minutos);
			sprintf(sHora,"%02d",horas);
			sprintf(sFecha,"%02d",fecha);
			sprintf(sMes,"%02d",mes);
			sprintf(sYear,"%02d",year);

			// Hacemos un and con el registro de horas para saber si el bit 5 esta prendido o apagado			
			indiceAmPm = buffer[2]&0x20;
			if(indiceAmPm == 0) indiceAmPm = 0;
			else indiceAmPm = 1;
		
			// Transformamos char a string
			AP[0] = AmPm[indiceAmPm];

			// Formacion de la string, format contiene toda la string lista, se unen las dos strings para que cuenten como un evento
			char formatT[] = "RECIEVER> Temperature ";
			strcat(formatT,sTemp);
			strcat(formatT,"°C ");	
			strcat(formatT,sRecord);	//RECIEVER> Record 
			strcat(formatT,": ");		//RECIEVER> Record %d:
			strcat(formatT,sMes);		//RECIEVER> Record %d: %02d
			strcat(formatT,"/");		//RECIEVER> Record %d: %02d/
			strcat(formatT,sFecha);		//RECIEVER> Record %d: %02d/%02d
			strcat(formatT,"/");		//RECIEVER> Record %d: %02d/%02d/
			strcat(formatT,sYear);		//RECIEVER> Record %d: %02d/%02d/0%2d
			strcat(formatT," ");		//RECIEVER> Record %d: %02d/%02d/0%2d
			strcat(formatT,diaSemana[dia]);	//RECIEVER> Record %d: %02d/%02d/0%2d %s
			strcat(formatT," ");		//RECIEVER> Record %d: %02d/%02d/0%2d %s 
			strcat(formatT,sHora);		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d
			strcat(formatT,":");		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:
			strcat(formatT,sMin);		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d
			strcat(formatT,":");		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:
			strcat(formatT,sSeg);		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:%d
			strcat(formatT," ");		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:%d
			strcat(formatT,AP);		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:%d %s
			strcat(formatT,"M\n");		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:%d %s


			// Imprimimos en el terminal
			printf("RECIEVER> Temperature = %d°C\n",temperatura);
			printf("RECIEVER> Record %d: %02d/%02d/%02d %s %02d:%02d:%02d %cM\n",
				record,mes,fecha,year,diaSemana[dia],horas,minutos,segundos,AmPm[indiceAmPm]);

			// Revisamos si ya se escribieron los primeros 3 eventos en el .txt
			if(contador < 3){
				fprintf(archivo,formatT);
				fclose(archivo);				// Actualizamos el documento
				if(contador == 1)strcpy(format1,formatT);	// Guardamos evento 1
				if(contador == 2) strcpy(format2,formatT);	// GUardamos evento 2
				contador++;
				record++;
				archivo = fopen("DataLog.txt","a");		// Abrimos el documento otra vez en modo append
			} else {
				archivo = fopen("DataLog.txt","w");		// Borramos todo el documento y este iniciara vacio
				fprintf(archivo,format1);			// Ponemos evento 1
				fprintf(archivo,format2);			// Ponemos evento 2
				fprintf(archivo,formatT);			// Ponemos evento 3
				fclose(archivo);				// Actualizamos archivo
				strcpy(format1,format2);			// Evento 1 toma valor de evento 2
				strcpy(format2,formatT);				// Evento 2 toma valor de evento 3
				record++;
			}
			
			// 1 segundo entre cada lectura de temperatura
			sleep(1);
		}

		// Revisamos si ya pasaron los 10 segundos
		if(abs(tAhora - seg0) >= 10){

			if (!bcm2835_init()) {
				printf("bcm2835_init failed. Are you running as root??\n");
				return 1;
	    		}
			// Volvemos a poner valor de rtc para leerlo
			bcm2835_i2c_setSlaveAddress(rtc);
			bcm2835_i2c_setClockDivider(clk_div);
			bcm2835_i2c_write(ceros, 1);				//para leer desde r0
			bcm2835_i2c_read(buffer, 7);
			bcm2835_close();

			// BCD a decimal
			segundos = buffer[0]&0x7f;				//no tomamos en cuenta bit 7			
			segundos = ((segundos/16)*10) + segundos%16;
			minutos = ((buffer[1]/16)*10) + buffer[1]%16;
			horas = buffer[2]&0x1f;					//no tomamos en cuenta bit 5(am/pm)
			horas = ((horas/16)*10) + horas%16;
			dia = ((buffer[3]/16)*10) + buffer[3]&16;
			fecha = ((buffer[4]/16)*10) + buffer[4]%16;
			mes = ((buffer[5]/16)*10) + buffer[5]%16;
			year = ((buffer[6]/16)*10) + buffer[6]%16;

			// Hacemos un and con el registro de horas para saber si el bit 5 esta prendido o apagado			
			indiceAmPm = buffer[2]&0x20;
			if(indiceAmPm == 0) indiceAmPm = 0;
			else indiceAmPm = 1;		

			// Transformamos char a string
			AP[0] = AmPm[indiceAmPm];			

			// Imprimimos el valor y record			
			printf("RECIEVER> Record %d: %02d/%02d/%02d %s %02d:%02d:%02d %cM\n",
				record,mes,fecha,year,diaSemana[dia],horas,minutos,segundos,AmPm[indiceAmPm]);				//Aumentamos el valor de record	

			//Conmvertimos a strings todos lso valores int y char
			sprintf(sRecord,"%d",record);
			sprintf(sSeg,"%02d",segundos);
			sprintf(sMin,"%02d",minutos);
			sprintf(sHora,"%02d",horas);
			sprintf(sFecha,"%02d",fecha);
			sprintf(sMes,"%02d",mes);
			sprintf(sYear,"%02d",year);

			// Formacion de la string, format contiene toda la string lista
			char format[] = "RECIEVER> Record ";
			strcat(format,sRecord);		//RECIEVER> Record 
			strcat(format,": ");		//RECIEVER> Record %d:
			strcat(format,sMes);		//RECIEVER> Record %d: %02d
			strcat(format,"/");		//RECIEVER> Record %d: %02d/
			strcat(format,sFecha);		//RECIEVER> Record %d: %02d/%02d
			strcat(format,"/");		//RECIEVER> Record %d: %02d/%02d/
			strcat(format,sYear);		//RECIEVER> Record %d: %02d/%02d/0%2d
			strcat(format," ");		//RECIEVER> Record %d: %02d/%02d/0%2d
			strcat(format,diaSemana[dia]);	//RECIEVER> Record %d: %02d/%02d/0%2d %s
			strcat(format," ");		//RECIEVER> Record %d: %02d/%02d/0%2d %s 
			strcat(format,sHora);		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d
			strcat(format,":");		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:
			strcat(format,sMin);		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d
			strcat(format,":");		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:
			strcat(format,sSeg);		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:%d
			strcat(format," ");		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:%d
			strcat(format,AP);		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:%d %s
			strcat(format,"M\n");		//RECIEVER> Record %d: %02d/%02d/0%2d %s %d:%d:%d %sM

			// Revisamos si ya se escribieron los primeros 3 eventos en el .txt
			if(contador < 3){
				fprintf(archivo,format);
				fclose(archivo);				// Actualizamos el documento
				if(contador == 1)strcpy(format1,format);	// Guardamos evento 1
				if(contador == 2) strcpy(format2,format);	// GUardamos evento 2
				contador++;
				record++;
				archivo = fopen("DataLog.txt","a");		// Abrimos el documento otra vez en modo append
			} else {
				archivo = fopen("DataLog.txt","w");		// Borramos todo el documento y este iniciara vacio
				fprintf(archivo,format1);			// Ponemos evento 1
				fprintf(archivo,format2);			// Ponemos evento 2
				fprintf(archivo,format);			// Ponemos evento 3
				fclose(archivo);				// Actualizamos archivo
				strcpy(format1,format2);			// Evento 1 toma valor de evento 2
				strcpy(format2,format);				// Evento 2 toma valor de evento 3
				record++;
			}	
			
			//Actualizamos el valor del tiempo
			seg0 = tAhora;
			sleep(1);
		}
	
		
	}

	return 0;
}





























