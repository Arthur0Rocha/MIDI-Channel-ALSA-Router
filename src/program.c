#include "headers/alsainterface.h"

#define ON 127
#define OFF 0

#define CTRL01 26
#define CTRL02 27
#define CTRL03 28
#define CTRL04 29

#define CTRL05 80
#define CTRL06 81
#define CTRL07 82

int main(int argc, char **argv) {

    snd_seq_event_t *ev = NULL;
    int channel = 0;

    midi_open();
    while (1) {
        ev = midi_read();

        if (ev->type == SND_SEQ_EVENT_CONTROLLER) { // TODO refactor this session
            // printf("%d - %d\n", ev->data.control.value, ev->data.control.param);
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

        midi_updt_channel(ev, channel); // TODO make panic button channel unchanged
        midi_send(ev);
    }

    return 0;
}