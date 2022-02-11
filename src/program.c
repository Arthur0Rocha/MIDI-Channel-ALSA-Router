#include "headers/alsainterface.h"

int main(int argc, char **argv) {

    snd_seq_event_t *ev = NULL;
    int channel = 0;

    midi_open();
    while (1) {
        ev = midi_read();
        midi_updt_channel(ev, channel);
        midi_send(ev);
    }

    return 0;
}