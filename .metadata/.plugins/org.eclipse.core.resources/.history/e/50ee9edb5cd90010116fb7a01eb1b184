#include "protocolComunication.h"

uint16_t crc16(uint8_t *data, uint16_t len)
{
	uint16_t crc = 0xFFFF;

	for (uint16_t i = 0; i < len; i++) {
		crc ^= data[i];

		for (uint8_t j = 0; j < 8; j++) {
			if (crc & 1)
				crc = (crc >> 1) ^ 0xA001;
			else
				crc >>= 1;
		}
	}

	return crc;
}

uint16_t build_frame(uint8_t msg_id,
		mavlink_sensor_data_t payload,
		uint8_t payload_len,
		uint8_t sequence,
		uint8_t *out)
{
	uint16_t index = 0;

	// 1. Start bytes (2)
	out[index++] = 0xAA;
	out[index++] = 0x55;

	// 2. Data length (1)
	out[index++] = payload_len;

	// 3. Sequence (1 byte)
	out[index++] = sequence;

	// 4. Message ID (1)
	out[index++] = msg_id;

	// ---- CRC_H: tính từ byte 0 đến byte 4 (AA 55 LEN SEQ MSGID) ----
	uint16_t crc_h = crc16(out, index);

	out[index++] = (uint8_t)(crc_h & 0xFF);      // CRC_H low
	out[index++] = (uint8_t)(crc_h >> 8);        // CRC_H high

	// 5. Payload
	memcpy(&out[index], &payload, payload_len);
	index += payload_len;

	// ---- CRC_FRS cho toàn bộ frame (trừ CRC cuối) ----
	uint16_t crc_f = crc16(out, index);

	out[index++] = (uint8_t)(crc_f & 0xFF);      // CRC_FRS low
	out[index++] = (uint8_t)(crc_f >> 8);        // CRC_FRS high

	return index;
}
