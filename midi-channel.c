#include <stdio.h>
#include <alsa/asoundlib.h>

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
static int in_port;

void midi_open(void)
{
    snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0);

    snd_seq_set_client_name(seq_handle, "Midi Listener");
    in_port = snd_seq_create_simple_port(seq_handle, "listen:in",
                      SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
                      SND_SEQ_PORT_TYPE_APPLICATION);
}

snd_seq_event_t *midi_read(void)
{
    snd_seq_event_t *ev = NULL;
    snd_seq_event_input(seq_handle, &ev);
    return ev;
}

int main()
{
    snd_seq_event_t *ev = NULL;
    midi_open();
    while (1) {
        ev = midi_read();
        printf("%d\n", ev->flags);
    }
    return -1;
}