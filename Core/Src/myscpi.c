/*
 * scpi.c
 *
 *  Created on: Oct 29, 2023
 *      Author: hooke
 */

#include "myscpi.h"
#include "usbd_cdc_if.h"

/* Forward Declarations */
size_t SCPI_Write(scpi_t *context, const char *data, size_t len);
int SCPI_Error(scpi_t *context, int_fast16_t err);
scpi_result_t SCPI_Control(scpi_t *context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val);
scpi_result_t SCPI_Reset(scpi_t *context);
scpi_result_t SCPI_Flush(scpi_t *context);
scpi_result_t SCPI_IdnQ(scpi_t *context);
scpi_result_t SCPI_Rst(scpi_t *context);

/* Variables */
char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];
scpi_t scpi_context;

scpi_command_t scpi_commands[] =
{
	{ .pattern = "*IDN", .callback = SCPI_IdnQ, },
	{ .pattern = "*RST", .callback = SCPI_Rst, },
	SCPI_CMD_LIST_END
};

scpi_interface_t scpi_interface =
{
		.error = SCPI_Error,
		.write = SCPI_Write,
		.control = SCPI_Control,
		.flush = SCPI_Flush,
		.reset = SCPI_Reset
};

void MY_SCPI_Init(void)
{
	SCPI_Init(&scpi_context,
			scpi_commands,
			&scpi_interface,
			scpi_units_def,
			SCPI_IDN1, SCPI_IDN2, SCPI_IDN3, SCPI_IDN4,
			scpi_input_buffer, SCPI_INPUT_BUFFER_LENGTH,
			scpi_error_queue_data, SCPI_ERROR_QUEUE_SIZE
			);
}

void MY_SCPI_Receive(uint8_t* recv, int len)
{
	SCPI_Input(&scpi_context, (char*)recv, len);
}

scpi_result_t SCPI_IdnQ(scpi_t *context)
{
	(void) context;

	return 0;
}

scpi_result_t SCPI_Rst(scpi_t *context)
{
	(void) context;

	const char txData[] = "Hello World!\n";
	while (CDC_Transmit_FS((uint8_t*)txData, strlen(txData)) == USBD_BUSY);

	return 0;
}

size_t SCPI_Write(scpi_t *context, const char *data, size_t len)
{
	(void) context;

	return 0;
}

scpi_result_t SCPI_Flush(scpi_t *context)
{
	(void) context;

	return SCPI_RES_OK;
}

int SCPI_Error(scpi_t *context, int_fast16_t err)
{
	(void) context;

	return 0;
}

scpi_result_t SCPI_Control(scpi_t *context, scpi_ctrl_name_t ctrl,
		scpi_reg_val_t val)
{
	(void) context;

	return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t *context)
{
	(void) context;

	return SCPI_RES_OK;
}
