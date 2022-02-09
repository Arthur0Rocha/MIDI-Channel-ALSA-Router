#include <stdio.h>
#include <alsa/asoundlib.h>

#define CHK(stmt, msg) if((stmt) < 0) { puts("ERROR: "#msg); exit(1);}

// typedef struct snd_seq_event {
//     snd_seq_event_type_t type;    /**< event type */
//     unsigned char flags;          /**< event flags */
//     unsigned char tag;            /**< tag */

//     unsigned char queue;          /**< schedule queue */
//     snd_seq_timestamp_t time;     /**< schedule time */

//     snd_seq_addr_t source;        /**< source address */
//     snd_seq_addr_t dest;          /**< destination address */

//     union {
//         snd_seq_ev_note_t note;           /**< note information */
//         snd_seq_ev_ctrl_t control;        /**< MIDI control information */
//         snd_seq_ev_raw8_t raw8;           /**< raw8 data */
//         snd_seq_ev_raw32_t raw32;         /**< raw32 data */
//         snd_seq_ev_ext_t ext;             /**< external data */
//         snd_seq_ev_queue_control_t queue; /**< queue control */
//         snd_seq_timestamp_t time;         /**< timestamp */
//         snd_seq_addr_t addr;              /**< address */
//         snd_seq_connect_t connect;        /**< connect information */
//         snd_seq_result_t result;          /**< operation result code */
//     } data;                       /**< event data... */
// } snd_seq_event_t;

static snd_seq_t *seq_handle;
static int in_port, out_port;

void midi_open(void)
{
    CHK(snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0),
        "Could not open sequencer");

    CHK(snd_seq_set_client_name(seq_handle, "Midi Listener"),
        "Could not set client name");
    CHK(in_port = snd_seq_create_simple_port(seq_handle, "listen:in",
                      SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
                      SND_SEQ_PORT_TYPE_APPLICATION),
        "Could not open port");
    CHK(out_port = snd_seq_create_simple_port(seq_handle, "listen:out",
                      SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
                      SND_SEQ_PORT_TYPE_APPLICATION),
        "Could not open port");
}

snd_seq_event_t *midi_read(void)
{
    snd_seq_event_t *ev = NULL;
    snd_seq_event_input(seq_handle, &ev);
    return ev;
}

void send_midi_ev() {
    snd_seq_event_t ev;

    snd_seq_ev_set_noteon(&ev, 2, 0x42, 0x7F);

    snd_seq_ev_set_source(&ev, out_port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    snd_seq_event_output_direct(seq_handle, &ev);

    snd_seq_drain_output(seq_handle);
}

void send_ev_diff_channel(snd_seq_event_t *ev, char channel) {

    if ((ev->type == SND_SEQ_EVENT_NOTEON)||(ev->type == SND_SEQ_EVENT_NOTEOFF))
        ev->data.note.channel = channel;
    else if (ev->type == SND_SEQ_EVENT_CONTROLLER)
        ev->data.control.channel = channel;


    snd_seq_ev_set_source(ev, out_port);
    snd_seq_ev_set_subs(ev);
    snd_seq_ev_set_direct(ev);
    snd_seq_event_output_direct(seq_handle, ev);

    snd_seq_drain_output(seq_handle);
}

void midi_process(snd_seq_event_t *ev)
{
    if ((ev->type == SND_SEQ_EVENT_NOTEON)||(ev->type == SND_SEQ_EVENT_NOTEOFF)) {
        const char *type = (ev->type == SND_SEQ_EVENT_NOTEON) ? "on " : "off";

        printf("[%d] Note %s: %2x vel(%2x) chan(%d)\n", ev->time.tick, type,
                                               ev->data.note.note,
                                               ev->data.note.velocity,
                                               ev->source.port);
        
    }
    else if(ev->type == SND_SEQ_EVENT_CONTROLLER){
        printf("[%d] Control:  %2x val(%2x) chan(%d)\n", ev->time.tick,
                                                ev->data.control.param,
                                                ev->data.control.value,
                                                ev->data.control.channel);
    }
    else
        printf("[%d] Unknown:  Unhandled Event Received\n", ev->time.tick);


    send_ev_diff_channel(ev, 13);
}

int main()
{
    midi_open();
    while (1) {
        // if (getchar() == 'a') {
        //     send_midi_ev();
        //     printf("Sending MIDI\n");
        // }
        midi_process(midi_read());
    }
    return -1;
}