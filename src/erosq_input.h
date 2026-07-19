#ifndef EROSQ_INPUT_H
#define EROSQ_INPUT_H

/* evdev button reader for the EROS Q: opens /dev/input/event*, and on each
 * poll drains events into OpenTyrian's keysactive[] via erosq_map_key. */
void erosq_input_init(void);
void erosq_input_poll(void);

#endif
