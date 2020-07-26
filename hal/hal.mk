INCLUDE_DIRS += -I $(BASE_DIR)/hal

include $(BASE_DIR)/hal/GPIO/gpio.mk
include $(BASE_DIR)/hal/ADC/adc.mk
include $(BASE_DIR)/hal/UART/uart.mk
include $(BASE_DIR)/hal/DMA/dma.mk
