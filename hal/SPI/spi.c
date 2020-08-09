#include "spi.h"


spi_t SPID1;
spi_t SPID2;


void spi_init(void) {
    SPID1.dev = SPI1;
    SPID2.dev = SPI2;
}

void spi_start(spi_t *drv, spi_config_t *config) {
}

void spi_stop(spi_t *drv) {
}

void SPI1_IRQHandler(void) {
    /* TODO: Implement an IRQ-based SPI handling */
}

void SPI2_IRQHandler(void) {
    /* TODO: Implement an IRQ-based SPI handling */
}
