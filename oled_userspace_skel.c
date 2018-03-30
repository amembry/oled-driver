/* userspace driver for SPI */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include "spi_reg_map.h"

#define SPI_MAX_ADDRESS 128U*32U

// just throws all the data in write_buf into the spi tx fifo
int spi_write(AXI_SPI * const spi_regs, uint32_t *write_buf, uint32_t len, uint32_t address) {
	if (NULL == spi_regs || NULL == write_buf || 0 == len) {
		return -1;
	}
	
	if (len > SPI_MAX_ADDRESS) {
		return -1;
	}

	// TODO: first send pre-set write command? if there is one - it might just be the D/C line
	spi_regs->data_transmit.data = address;

	while (len--) {
		if (spi_regs->status.tx_full) {
			// if it's full, then drain the fifo until it's empty
			// issue chip select by writing 0x00 to SPISSR
			spi_regs->slave_select.occupancy_value = 0;
			spi_regs->control.master_transact_inhibit = 0;
			while (!spi_regs->status.tx_empty) {
				// just wait for fifo to drain
				usleep(100);
			}
			// then deassert slave select and re-enable master transact inhibit to 
			// prevent it from draining more
			spi_regs->slave_select.occupancy_value = 1;
			spi_regs->control.master_transact_inhibit = 1;
		}
#ifdef DEBUG
		printf("Value 0x%x being written\n", write_buf);
#endif
		spi_regs->data_transmit.data = *write_buf++;
	}
	return 0;
}

static int init_spi(AXI_SPI * const spi_regs) {
	if (NULL == spi_regs) {
		return -1;
	}
	// resets rx and tx fifos
	spi_regs->control = (SPICR) { .tx_fifo_res = 1,
								  .rx_fifo_res = 1, 
								  .master_transact_inhibit = 1 };
	return 0;
}

static int map_spi(AXI_SPI **spi_regs) {
	if (NULL == spi_regs) {
		return -1;
	}
	
	int fd = open("/dev/mem", O_RDWR);
	*spi_regs = (AXI_SPI*)mmap(NULL, sizeof(AXI_SPI), PROT_READ|PROT_WRITE, MAP_SHARED, fd, AXI_SPI_BASE_ADDR);
	
	if (NULL == *spi_regs) {
		printf("mmap spi regs failed\n");
		return -1;
	}
	close(fd);
	return 0;
}

int main(void) {
	// first thing is to open /dev/mem and mmap it
	AXI_SPI *spi_regs = NULL;
	if (0 != map_spi(&spi_regs)) {
		printf("map_spi failed\n");
		return -1;
	}
	if (0 != init_spi(spi_regs)) {
		printf("init_spi failed\n");
		return -1;
	}
	uint32_t write_buf[4] = {1, 2, 3, 4};
	uint32_t address = 0x23;
	if (0 != spi_write(spi_regs, write_buf, 4, address)) {
		printf("spi_write failed\n");
		return -1;
	}
	return 0;
}