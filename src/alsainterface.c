#include "headers/alsainterface.h"

static snd_seq_t *seq_handle;
static int port;

void midi_open() {
    snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0);
    snd_seq_set_client_name(seq_handle, "Arthur MIDI HUB");
    port = snd_seq_create_simple_port(seq_handle, "IO",
                    SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_WRITE|
                    SND_SEQ_PORT_CAP_SUBS_READ|SND_SEQ_PORT_CAP_SUBS_WRITE,
                    SND_SEQ_PORT_TYPE_APPLICATION);
    
}

snd_seq_event_t *midi_read() {
    snd_seq_event_t *ev = NULL;
    snd_seq_event_input(seq_handle, &ev);
    return ev;
}

void midi_send(snd_seq_event_t *ev) {
    snd_seq_ev_set_source(ev, port);
    snd_seq_ev_set_subs(ev);
    snd_seq_ev_set_direct(ev);
    snd_seq_event_output_direct(seq_handle, ev);
    snd_seq_drain_output(seq_handle);
}

void connectKROME(uint8_t input, uint8_t output) { // TODO 
    return;
}

void connectVORTEX() { // TODO
    return;
}

void connectQSYNTH() {  // TODO
    return;
}

void midi_updt_channel(snd_seq_event_t *ev, int channel) {
    ev->data.note.channel = channel;
}