/*
 * Modbus CRC-16 Test Harness
 * Validates against libmodbus reference implementation
 * Author: 臧德运 (Zang Deyun)
 * License: MIT
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// External assembly function
extern uint16_t ModbusCrc(const uint8_t *data, uint16_t len);

// Reference implementation (bit-by-bit, verified correct)
uint16_t ReferenceCrc(const uint8_t *data, uint16_t len) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return (crc >> 8) | (crc << 8);  // REV16 equivalent
}

// Standard test vectors
static const struct {
    const uint8_t *data;
    uint16_t len;
    uint16_t expected;
} test_vectors[] = {
    {(const uint8_t[]){0x01}, 1, 0x807E},
    {(const uint8_t[]){0x01, 0x03}, 2, 0x0979},
    {(const uint8_t[]){0x01, 0x03, 0x00}, 3, 0xFB87},
    {(const uint8_t[]){0x01, 0x03, 0x00, 0x00, 0x00, 0x0A}, 6, 0x????},  // Fill in
};

int main(void) {
    int passed = 0;
    int failed = 0;

    printf("Modbus CRC-16 Test Harness\n");
    printf("===========================\n\n");

    for (size_t i = 0; i < sizeof(test_vectors)/sizeof(test_vectors[0]); i++) {
        uint16_t result = ModbusCrc(test_vectors[i].data, test_vectors[i].len);
        uint16_t ref = ReferenceCrc(test_vectors[i].data, test_vectors[i].len);

        printf("Test %zu: ", i + 1);
        if (result == ref && result == test_vectors[i].expected) {
            printf("PASS (CRC=0x%04X)\n", result);
            passed++;
        } else {
            printf("FAIL (got 0x%04X, expected 0x%04X, ref 0x%04X)\n",
                   result, test_vectors[i].expected, ref);
            failed++;
        }
    }

    printf("\n===========================\n");
    printf("Results: %d passed, %d failed\n", passed, failed);

    return failed > 0 ? 1 : 0;
}
