#include <SoftwareSerial.h>

// UART trên chân 10 (RX) và 11 (TX)
SoftwareSerial mySerial(10, 11);

// Cấu trúc payload
typedef struct {
  uint8_t temp;
  uint8_t press;
} Data_sensors;


uint16_t crc16(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFF;

    while (len--) {
        crc ^= *data++;

        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    return crc;
}

#define FRAME_SIZE 11

bool receive_frame(uint8_t *msg_id,
                   uint8_t *sequence,
                   Data_sensors *payload)
{
    static uint8_t buf[FRAME_SIZE];
    static uint8_t index = 0;

    while (mySerial.available()) {
        uint8_t b = mySerial.read();

        // Tìm 0xAA 0x55
        if (index == 0 && b != 0xAA)
            continue;

        if (index == 1 && b != 0x55) {
            index = 0;
            continue;
        }

        buf[index++] = b;

        // Khi đủ frame
        if (index == FRAME_SIZE) {

            // ========== CRC HEADER ==========
            uint16_t crc_h_calc = crc16(buf, 5);         // byte 0..4
            uint16_t crc_h_recv = buf[5] | (buf[6] << 8);

            if (crc_h_calc != crc_h_recv) {
                index = 0;
                return false;
            }

            // ========== CRC FULL FRAME ==========
            uint16_t crc_f_calc = crc16(buf, 9);         // byte 0..8
            uint16_t crc_f_recv = buf[9] | (buf[10] << 8);

            if (crc_f_calc != crc_f_recv) {
                index = 0;
                return false;
            }

            // ========== Lấy dữ liệu ==========
            *msg_id   = buf[4];
            *sequence = buf[3];
            payload->temp  = buf[7];
            payload->press = buf[8];

            index = 0;
            return true;
        }
    }

    return false;  // chưa đủ
}

// ===============================
// SETUP
// ===============================
void setup() {
  Serial.begin(9600);     // debug
  mySerial.begin(9600);     // UART chân 10-11

  Serial.println("READY: Dang cho frame 11 byte từ UART 10-11...");
}

// ===============================
// LOOP
// ===============================
void loop() {
  uint8_t msg_id, seq;
  Data_sensors data;

  // Nếu nhận frame OK
  if (receive_frame(&msg_id, &seq, &data)) {

    Serial.print("Frame OK | ID: ");
    Serial.print(msg_id);

    Serial.print(" | Seq: ");
    Serial.print(seq);

    Serial.print(" | Temp: ");
    Serial.print(data.temp);

    Serial.print(" | Press: ");
    Serial.println(data.press);

  }
}