# 1 "rtc.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "rtc.c" 2
# 21 "rtc.c"
# 1 "./rtc.h" 1
# 23 "./rtc.h"
# 1 "./pic18f4520.h" 1
# 24 "./rtc.h" 2



    void ht1380write(unsigned char addr, unsigned char data);
    unsigned char ht1380read(unsigned char addr);

    unsigned char rtcGetSeconds(void);
    unsigned char rtcGetMinutes(void);
    unsigned char rtcGetHours(void);
    unsigned char rtcGetDate(void);
    unsigned char rtcGetMonth(void);
    unsigned char rtcGetYear(void);
    unsigned char rtcGetDay(void);



    void rtcPutSeconds(unsigned char seconds);
    void rtcPutMinutes(unsigned char minutes);
    void rtcPutHours(unsigned char hours);
    void rtcPutDate(unsigned char date);
    void rtcPutMonth(unsigned char month);
    void rtcPutDay(unsigned char day);
    void rtcPutYear(unsigned char year);

    void rtcSetDateTime(void);

    void rtcInit();
# 22 "rtc.c" 2

char de;
# 46 "rtc.c"
void rtcInit() {

    ((((*(volatile __near unsigned char*)0xF94)) &= ~(1<<5)));
    ((((*(volatile __near unsigned char*)0xF94)) &= ~(1<<4)));
    ((((*(volatile __near unsigned char*)0xF94)) &= ~(1<<3)));
 ht1380write(7, 0);
 rtcPutSeconds(rtcGetSeconds());

}
void rtcStart() {

   ((((*(volatile __near unsigned char*)0xF94)) &= ~(1<<5)));
   ((((*(volatile __near unsigned char*)0xF94)) &= ~(1<<4)));
   ((((*(volatile __near unsigned char*)0xF94)) &= ~(1<<3)));
   (((*(volatile __near unsigned char*)0xFF1)) &= ~(1<<7));
}

void writeByte(unsigned char dados) {
   unsigned char i;
   for (i = 0; i < 8; i++) {

       if (dados & 0x01) {
           ((((*(volatile __near unsigned char*)0xF82)) |= (1<<4)));
       } else {
           ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<4)));
       }

       dados >>= 1;

       ((((*(volatile __near unsigned char*)0xF82)) |= (1<<3)));
       for(de=0;de<100;de++);

       ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<3)));
       for(de=0;de<100;de++);

   }
}

unsigned char readByte() {
   unsigned char i;
   unsigned char dados;
   dados = 0;
   for (i = 0; i < 8; i++) {
       ((((*(volatile __near unsigned char*)0xF82)) |= (1<<3)));
       for(de=0;de<100;de++);

       dados >>= 1;

       if (((((*(volatile __near unsigned char*)0xF82)) & (1<<4)))) {
           dados |= 0x80;
       }

       ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<3)));
       for(de=0;de<100;de++);
   }
   return dados;
}

void ht1380write(unsigned char addr, unsigned char dados) {
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<5)));
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<3)));
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<4)));


   ((((*(volatile __near unsigned char*)0xF82)) |= (1<<5)));

   addr <<= 1;
   addr |= 0x80;
   writeByte(addr);
   writeByte(dados);
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<5)));
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<3)));
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<4)));
}

unsigned char ht1380read(unsigned char addr) {
   unsigned char dados;
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<5)));
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<3)));
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<4)));

   ((((*(volatile __near unsigned char*)0xF82)) |= (1<<5)));
   addr <<= 1;
   addr |= 0x81;
   writeByte(addr);
   ((((*(volatile __near unsigned char*)0xF94)) |= (1<<4)));
   for(de=0;de<100;de++);for(de=0;de<100;de++);for(de=0;de<100;de++);for(de=0;de<100;de++);
   dados = readByte();
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<5)));
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<3)));
   ((((*(volatile __near unsigned char*)0xF82)) &= ~(1<<4)));
   ((((*(volatile __near unsigned char*)0xF94)) &= ~(1<<4)));
   return dados;
}

void rtcSetDateTime(void) {

   ht1380write(7, 0);
   ht1380write(0, 0x0);
   ht1380write(1, 0x21);
   ht1380write(2, 0x23);
   ht1380write(3, 0x28);
   ht1380write(4, 0x10);
   ht1380write(5, 0x6);
   ht1380write(6, 0x12);
}

unsigned char rtcGetSeconds(void) {
   unsigned char value;
   value = ht1380read(0);
   return (((value >> 4)&0x07)*10 + (value & 0x0f));

}

unsigned char rtcGetMinutes(void) {
   unsigned char value;
   value = ht1380read(1);
   return (((value >> 4)&0x07)*10 + (value & 0x0f));
}

unsigned char rtcGetHours(void) {
   unsigned char value;
   value = ht1380read(2);
   return (((value >> 4)&0x03)*10 + (value & 0x0f));
}

unsigned char rtcGetDate(void) {
   unsigned char value;
   value = ht1380read(3);
   return (((value >> 4)&0x03)*10 + (value & 0x0f));
}

unsigned char rtcGetMonth(void) {
   unsigned char value;
   value = ht1380read(4);
   return (((value >> 4)&0x01)*10 + (value & 0x0f));
}

unsigned char rtcGetYear(void) {
   unsigned char value;
   value = ht1380read(6);
   return (((value >> 4)&0x0f)*10 + (value & 0x0f));
}

unsigned char rtcGetDay(void) {
   unsigned char value;
   value = ht1380read(5);
   return (value & 0x0f);
}
# 206 "rtc.c"
void rtcPutSeconds(unsigned char seconds) {
   ht1380write(0, (seconds % 10) | ((seconds / 10) << 4));
}

void rtcPutMinutes(unsigned char minutes) {
   ht1380write(1, (minutes % 10) | ((minutes / 10) << 4));
}

void rtcPutHours(unsigned char hours) {
   ht1380write(2, (hours % 10) | ((hours / 10) << 4));
}

void rtcPutDate(unsigned char date) {
   ht1380write(3, (date % 10) | ((date / 10) << 4));
}

void rtcPutMonth(unsigned char month) {
   ht1380write(4, (month % 10) | ((month / 10) << 4));
}

void rtcPutDay(unsigned char day) {
   ht1380write(5, day);
}

void rtcPutYear(unsigned char year) {
   ht1380write(6, (year % 10) | ((year / 10) << 4));
}
