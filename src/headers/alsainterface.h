#ifndef _ARTHUR_ALSA_INTERFACE
#define _ARTHUR_ALSA_INTERFACE

#include <alsa/asoundlib.h>

void midi_open();
snd_seq_event_t *midi_read();
void midi_send(snd_seq_event_t *ev);
void midi_updt_channel(snd_seq_event_t *ev, int channel);

#endif // _ARTHUR_ALSA_INTERFACE