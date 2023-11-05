/*
 * scpi.h
 *
 *  Created on: Oct 29, 2023
 *      Author: hooke
 */

#ifndef INC_MYSCPI_H_
#define INC_MYSCPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "scpi/scpi.h"

#define SCPI_INPUT_BUFFER_LENGTH 256
#define SCPI_ERROR_QUEUE_SIZE 17
#define SCPI_IDN1 "Manisonik"
#define SCPI_IDN2 "INSTR2023"
#define SCPI_IDN3 NULL
#define SCPI_IDN4 "01-02"

void MY_SCPI_Init(void);
void MY_SCPI_Receive(uint8_t* recv, int len);

#ifdef __cplusplus
}
#endif
#endif /* INC_MYSCPI_H_ */
