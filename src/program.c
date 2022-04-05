#include "headers/alsainterface.h"

// aconnect 'Vortex Wireless 2' 'Arthur MIDI HUB'; aconnect 'Arthur MIDI HUB' 'FLUID Synth (10474)'

#define ON 127
#define OFF 0

#define CTRL01 26
#define CTRL02 27
#define CTRL03 28
#define CTRL04 29

#define CTRL05 80
#define CTRL06 81
#define CTRL07 82

#define PANIC 123

int main(int argc, char **argv) {

    snd_seq_event_t *ev = NULL;
    int channel = 0;

    midi_open();
    while (1) {
        ev = midi_read();

        if (ev->type == SND_SEQ_EVENT_CONTROLLER) { // TODO refactor this session
            // printf("%d %d %d\n", ev->data.control.value, ev->data.control.param, ev->data.control.channel);
            if (ev->data.control.value == ON) {
                switch (ev->data.control.param)
                {
                case CTRL01:
                    channel = 0;
                    break;

                case CTRL02:
                    channel = 1;
                    break;

                case CTRL03:
                    channel = 2;
                    break;

                case CTRL04:
                    channel = 3;
                    break;
                
                default:
                    break;
                }
            }
        }

        if (ev->data.control.param == PANIC) 
            printf("PANIC - channel %d\n", ev->data.control.channel);
        else
            midi_updt_channel(ev, channel);
        midi_send(ev);
    }

    return 0;
}