#ifndef INC_PROTOCOLCOMUNICATION_H_
#define INC_PROTOCOLCOMUNICATION_H_

#include "main.h"
#include "string.h"


typedef struct __mavlink_sensor_data_t {
	uint8_t temp;
	uint8_t press;
} mavlink_sensor_data_t;


uint16_t crc16(uint8_t *data, uint16_t len);
uint16_t build_frame(uint8_t msg_id,
		mavlink_sensor_data_t payload,
		uint8_t payload_len,
		uint8_t sequence,
		uint8_t *out);



#endif /* INC_PROTOCOLCOMUNICATION_H_ */
