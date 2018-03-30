#include <stdint.h>

#define AXI_SPI_BASE_ADDR	0x41230000

// software reset reg
typedef struct _SRR {
	// SRR only allows a write of 0xA, which resets the QSPI peripheral
	uint32_t reset;
} SRR;

// control register
typedef struct _SPICR {
	uint32_t loopback		  		 :1;
	uint32_t spi_enable		  		 :1;
	uint32_t master		 	  		 :1;
	uint32_t clock_pol	 	  		 :1;
	uint32_t clock_phase 	  		 :1;
	uint32_t tx_fifo_res 	  		 :1;
	uint32_t rx_fifo_res 	  		 :1;
	uint32_t man_ss_assert_en 		 :1;
	uint32_t master_transact_inhibit :1;
	uint32_t lsb_first		  		 :1;
	uint32_t reserved1		  		 :22;
} SPICR;

// status register
typedef struct _SPISR {
	uint32_t rx_empty		:1;
	uint32_t rx_full		:1;
	uint32_t tx_empty		:1;
	uint32_t tx_full		:1;
	uint32_t mode_fault		:1;
	uint32_t slave_mode_sel	:1;
	uint32_t cpol_cpha_err	:1;
	uint32_t slave_mode_err	:1;
	uint32_t msb_err		:1;
	uint32_t loopback_err	:1;
	uint32_t command_err	:1;
	uint32_t reserved1		:21;
} SPISR;

// data transmit register
typedef struct _SPI_DTR {
	// note that the size of this depends on transfer width param
	uint32_t data;
} SPI_DTR;

typedef struct _SPI_DRR {
	uint32_t selected_slave;
} SPI_DRR;

typedef struct _SPISSR {
	uint32_t occupancy_value;
} SPISSR;

typedef struct _TX_OCC {
	uint32_t temp;
} TX_OCC;

typedef struct _RX_OCC {
	uint32_t temp;
} RX_OCC;

typedef struct _DGIER {
	uint32_t temp;
} DGIER;

typedef struct _IPISR {
	uint32_t temp;
} IPISR;

typedef struct _IPIER {
	uint32_t temp;
} IPIER;

typedef struct _AXI_SPI {
	uint32_t reserved1[10];
	
	// SRR should be at offset 0x40
	SRR 	software_reset;
	
	// SPICR should be at base address 0x60
	SPICR 	control;
	SPISR 	status;
	SPI_DTR data_transmit;
	SPI_DRR data_receive;
	SPISSR 	slave_select;
	TX_OCC  tx_fifo_occ;
	RX_OCC 	rx_fifo_occ;
	
	uint32_t reserved2[10];
	// DGIER should start at 0x1C
	DGIER 	global_interrupt_enable;
	IPISR 	interrupt_status;
	uint32_t reserved3;
	IPIER 	interrupt_enable;
} AXI_SPI;