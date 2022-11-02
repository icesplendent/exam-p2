#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2);
BusIn keyboard(D2, D3, D4);  // default 111
InterruptIn button(BUTTON1);
EventQueue queue;
Thread t;

Ticker flipper;
int mode;
int m_s = 0;
int s = 0;
int m = 0;
void flip() {
    m_s++;
    if (m_s == 100) {
        m_s = 0;
        s++;
    }
    if (s == 60) {
        s = 0;
        m++;
    }
}
void set() {
    if (mode == 0) {
        // flipper.attach(&flip, 10ms);
        mode = 1;
    } else if (mode == 1) {
        flipper.attach(&flip, 10ms);
        // flipper.detach();
        mode = 2;
    } else if (mode == 2) {
        flipper.detach();
        mode = 3;
    } else if (mode == 3) {
        // flipper.attach(&flip, 10ms);
        mode = 1;
    }
};

void uLCDInit() {
    printf("uLCD printing...\n");
    uLCD.color(WHITE);
    uLCD.printf("\nHello uLCD World\n");
    ThisThread::sleep_for(1s);
}

void uLCDFunc() {
    uLCD.locate(1, 2);
    uLCD.printf("%2d:%2d:%2d", m, s, m_s);
}

int main() {
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    queue.call(uLCDInit);
    //   printf("uLCD printing...\n");
    //   uLCD.color(WHITE);
    //   uLCD.printf("\nHello uLCD World\n");
    //   ThisThread::sleep_for(1s);

    mode = 0;
    button.rise(&set);

    while (1) {
        if (mode == 0) {
            // ThisThread::sleep_for(100ms);
        }

        else if (mode == 1) {
            // uLCD.locate(1, 2);
            // uLCD.printf("%2d:%2d:%2d",m,s ,m_s);
            m = 0;
            s = 0;
            m_s = 0;
            queue.call(uLCDFunc);
        } else if (mode == 2) {
            queue.call(uLCDFunc);
        } else if (mode == 3) {
            ThisThread::sleep_for(100ms);
            // m = 0;
            // s = 0;
            // m_s = 0;
            // queue.call(uLCDFunc);
            // uLCD.locate(1, 2);
            // uLCD.printf("%2d:%2d:%2d",m,s ,m_s);
        }
    }
}