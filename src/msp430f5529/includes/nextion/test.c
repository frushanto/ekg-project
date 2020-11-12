#include <stdlib.h>
#include <string.h>

#include "Nextion.h"
#include "Utilities.h"

#define BAUDRATE 9600

struct NexObject b0;        // Button b0
struct NexObject n0;        // Number n0
struct NexObject t0;        // Text t0

void b0_release();
void b0_push();

struct NexObject *nex_listen_list[] = {&b0, &n0, &t0};

int value;


void display_uart_test(){
    CreateNexObject(b0, 8, 4, "Button");        // (name, Seite, ID, "Name")
    CreateNexObject(n0, 8, 3, "Number");
    CreateNexObject(t0, 8, 2, "Text");

    nexInit();

    NexNumber_getValue(&n0, &value);
    NexNumber_setValue(&n0, (value + 25));


    NexTouch_attachPop(&b0, b0_release, 0);
    NexTouch_attachPush(&b0, b0_push, 0);

    
    while(1){
        nexLoop(nex_listen_list);
        //NexText_getText(&t0, &value);
        delay_ms(50);
    }
}

void b0_release(void *ptr){
    NexButton_setText(&b0, "NewButton");
}

void b0_push(void *ptr){
    NexNumber_setValue(&n0, 100);
}
