/* Projeto final De Programação Embarcada 2021/1
 * Rodrigo Nascimento Pereira
 * 2020004403
 */

/* Este projeto consiste em uma choppeira
 */

#include <pic18f4520.h>
#include "config.h"
#include "atraso.h"
#include "lcd.h" //biblioteca do PICSimLab
#include "rtc.h" //biblioteca do PICSimLab
#include "bits.h"
#include "lcd.h"
#include "keypad.h"
#include "i2c.h"
#include "lcdData.h"
#include "ssd.h"
#include "io.h"

void lcdPosition(unsigned char linha, unsigned char coluna) {
    if (linha == 0) {
        lcdCommand(0x80 + (coluna % 16)); // primeira linha, coluna 0 + numero da coluna
    }
    if (linha == 1) {
        lcdCommand(0xC0 + (coluna % 16)); // segunda linha, coluna 0 + numero da coluna
    }
    if (linha == 2) {
        lcdCommand(0x90 + (coluna % 16)); // terceira linha, coluna 0 + numero da coluna
    }
    if (linha == 3) {
        lcdCommand(0xD0 + (coluna % 16)); // quarta linha, coluna 0 + numero da coluna
    }
}

void ligarCooler() { // ligar o cooler
    TRISC = 0x01;
    bitSet(PORTC, 2);
}

void desligarCooler() { // desligar o cooler
    TRISC = 0x01;
    bitClr(PORTC, 2);
}

void desligaDisplays() {
    digitalWrite(PIN_A2, LOW);
    digitalWrite(PIN_A3, LOW);
    digitalWrite(PIN_A4, LOW);
    digitalWrite(PIN_A5, LOW);
}

void ligaRele1() { // liga o Relé número 1
    TRISCbits.TRISC0 = 0;
    PORTCbits.RC0 = 1;
}

void desligaRele1() { // desliga o Relé número 1
    TRISCbits.TRISC0 = 0;
    PORTCbits.RC0 = 0;
}

void ligaRele2() { // liga o Relé número 2
    TRISEbits.TRISE0 = 0;
    PORTEbits.RE0 = 1;
}

void desligaRele2() { // desliga o Relé número 2
    TRISEbits.TRISE0 = 0;
    PORTEbits.RE0 = 0;
}

void main(void) {
    //Variáveis-----------------------------------------------------------------
    unsigned int tecla = 16, i = 0;
    unsigned char coluna = 0, linha = 0;
    unsigned char imagem1[32] = {
        0x00, 0x1F, 0x10, 0x10, 0x17, 0x17, 0x17, 0x17, // canto superior esquerdo
        0x00, 0x1D, 0x04, 0x05, 0x14, 0x15, 0x14, 0x15, // canto superior direito
        0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x10, 0x1F, // canto inferior esquerdo
        0x14, 0x15, 0x14, 0x15, 0x14, 0x15, 0x04, 0x1D // canto inferior direito
    };

    //Inicializações------------------------------------------------------------
    lcdInit();
    kpInit();
    ssdInit();

    //Imprimir símbolo e nome --------------------------------------------------

    // símbolo
    lcdCommand(0x40); // caracteres de memória
    for (i = 0; i < 32; i++)
        lcdData(imagem1[i]); // pega os hexas e cria um caracter
    lcdPosition(0, 0); // primeira linha
    lcdData(0); // cant sup esquerdo
    lcdData(1); // cant sup direito
    lcdPosition(1, 0); // segunda linha
    lcdData(2); // cant inf esquerdo
    lcdData(3); // cant inf direito

    // nome
    lcdPosition(0, 4);
    lcd_str("Cervejaria");
    lcdPosition(1, 4);
    lcd_str("Rodrigo");
    lcdPosition(2, 4);
    lcd_str("Nascimento");
    lcdPosition(3, 4);
    lcd_str("Pereira");
    atraso_ms(5000);

    //Escolher a cerveja--------------------------------------------------------

    // menu de apresentação
    lcd_cmd(L_CLR); // clear display
    lcdPosition(0, 0);
    lcd_str("Escolha entre");
    lcdPosition(1, 0);
    lcd_str("4 opcoes");
    lcdPosition(2, 0);
    lcd_str("de cerveja");
    atraso_ms(5000);

    //cervejas
    lcd_cmd(L_CLR);
    lcdPosition(0, 0);
    lcd_str("1- Pilsen");
    lcdPosition(1, 0);
    lcd_str("2- Stout");
    lcdPosition(2, 0);
    lcd_str("3- Lager");
    lcdPosition(3, 0);
    lcd_str("4- Ipa");

    //Primeiro input de teclado // nível 1 -------------------------------------

    for (;;) {
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();
            if (bitTst(tecla, 3)) { // botão 1
                lcd_cmd(L_CLR);
                lcdPosition(0, 0);
                lcd_str("Pilsen");
                lcdPosition(1, 0);
                lcd_str("1- 300ml    R$7");
                lcdPosition(2, 0);
                lcd_str("2- 500ml    R$10");
                lcdPosition(3, 0);
                lcd_str("3- 700ml    R$12");
                for (;;) { // Escolha das Pilsen--------------------------------
                    kpDebounce();
                    if (kpRead() != tecla) {
                        tecla = kpRead();
                        if (bitTst(tecla, 3)) { // botão 1
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(0, 2); // print do zero 
                                ssdDigit(7, 3); // print do 7     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                        if (bitTst(tecla, 7)) { // botão 2
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(1, 2); // print do 1
                                ssdDigit(0, 3); // print do 0     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                        if (bitTst(tecla, 11)) { // botão 3
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(1, 2); // print do 1
                                ssdDigit(2, 3); // print do 0     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (bitTst(tecla, 7)) { // botão 2
                lcd_cmd(L_CLR);
                lcdPosition(0, 0);
                lcd_str("Stout");
                lcdPosition(1, 0);
                lcd_str("1- 300ml    R$7");
                lcdPosition(2, 0);
                lcd_str("2- 500ml    R$10");
                lcdPosition(3, 0);
                lcd_str("3- 700ml    R$12");
                for (;;) { // Escolha das Stout--------------------------------
                    kpDebounce();
                    if (kpRead() != tecla) {
                        tecla = kpRead();
                        if (bitTst(tecla, 3)) { // botão 1
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(0, 2); // print do zero 
                                ssdDigit(7, 3); // print do 7     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                        if (bitTst(tecla, 7)) { // botão 2
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(1, 2); // print do 1
                                ssdDigit(0, 3); // print do 0     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                        if (bitTst(tecla, 11)) { // botão 3
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(1, 2); // print do 1
                                ssdDigit(2, 3); // print do 0     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (bitTst(tecla, 11)) { // botão 3
                lcd_cmd(L_CLR);
                lcdPosition(0, 0);
                lcd_str("Lager");
                lcdPosition(1, 0);
                lcd_str("1- 300ml    R$7");
                lcdPosition(2, 0);
                lcd_str("2- 500ml    R$10");
                lcdPosition(3, 0);
                lcd_str("3- 700ml    R$12");
                for (;;) { // Escolha das Lager--------------------------------
                    kpDebounce();
                    if (kpRead() != tecla) {
                        tecla = kpRead();
                        if (bitTst(tecla, 3)) { // botão 1
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(0, 2); // print do zero 
                                ssdDigit(7, 3); // print do 7     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                        if (bitTst(tecla, 7)) { // botão 2
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(1, 2); // print do 1
                                ssdDigit(0, 3); // print do 0     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                        if (bitTst(tecla, 11)) { // botão 3
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(1, 2); // print do 1
                                ssdDigit(2, 3); // print do 0     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (bitTst(tecla, 2)) { // botão 4
                lcd_cmd(L_CLR);
                lcdPosition(0, 0);
                lcd_str("Ipa");
                lcdPosition(1, 0);
                lcd_str("1- 300ml    R$7");
                lcdPosition(2, 0);
                lcd_str("2- 500ml    R$10");
                lcdPosition(3, 0);
                lcd_str("3- 700ml    R$12");
                for (;;) { // Escolha das Ipa--------------------------------
                    kpDebounce();
                    if (kpRead() != tecla) {
                        tecla = kpRead();
                        if (bitTst(tecla, 3)) { // botão 1
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(0, 2); // print do zero 
                                ssdDigit(7, 3); // print do 7     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                        if (bitTst(tecla, 7)) { // botão 2
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(1, 2); // print do 1
                                ssdDigit(0, 3); // print do 0     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                        if (bitTst(tecla, 2)) { // botão 4
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Seu copo esta");
                            lcdPosition(1, 0);
                            lcd_str("enchendo!");
                            ligarCooler(); // Ligar o cooler
                            atraso_ms(5000);
                            desligarCooler(); // Desligar o cooler
                            lcd_cmd(L_CLR);
                            lcdPosition(0, 0);
                            lcd_str("Valor no display"); // inserir o valor do display
                            lcdPosition(2, 0);
                            lcd_str("0- Pagar");
                            ssdInit();
                            for (i = 0; i < 3000; i++) { // printar o valor 
                                ssdDigit(1, 2); // print do 1
                                ssdDigit(2, 3); // print do 0     
                                ssdUpdate();
                                atraso_ms(2);
                            }
                            desligaDisplays(); // desligar o display

                            for (;;) {
                                kpDebounce();
                                if (kpRead() != tecla) {
                                    tecla = kpRead();
                                    if (bitTst(tecla, 4)) {// botão 0
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Carregando...");
                                        atraso_ms(5000);
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Aprovado!");
                                        ligaRele1();
                                        ligaRele2();
                                        atraso_ms(5000);
                                        desligaRele1();
                                        desligaRele2();
                                        lcd_cmd(L_CLR);
                                        lcdPosition(0, 0);
                                        lcd_str("Volte sempre!");
                                        lcdPosition(2, 0);
                                        lcd_str("Outro pedido");
                                        lcdPosition(3, 0);
                                        lcd_str("no Reset");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
