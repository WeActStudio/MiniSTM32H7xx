#include "qspi.h"
#include "storage.h"

#if !MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE

#if WEACTSTUDIO_USE_QSPIFlash_AS_FLASH_STORAGE
extern char _ffs_cache;
STATIC const mp_soft_qspi_obj_t qspi_bus = {
    .cs  = MICROPY_HW_QSPIFLASH_CS,
    .clk = MICROPY_HW_QSPIFLASH_SCK,
    .io0 = MICROPY_HW_QSPIFLASH_IO0,
    .io1 = MICROPY_HW_QSPIFLASH_IO1,
    .io2 = MICROPY_HW_QSPIFLASH_IO2,
    .io3 = MICROPY_HW_QSPIFLASH_IO3,
};
const mp_spiflash_config_t qspiflash_config = {
    .bus_kind = MP_SPIFLASH_BUS_QSPI,
    .bus.u_qspi.data = (void*)&qspi_bus,
    .bus.u_qspi.proto = &qspi_proto,
    //NOTE: The FFS cache is not used when QSPI is enabled.
    .cache = (mp_spiflash_cache_t *) &_ffs_cache,
};

#else
STATIC const mp_soft_spi_obj_t soft_spi_bus = {
    .delay_half = MICROPY_HW_SOFTSPI_MIN_DELAY,
    .polarity = 0,
    .phase = 0,
    .sck = MICROPY_HW_SPIFLASH_SCK,
    .mosi = MICROPY_HW_SPIFLASH_MOSI,
    .miso = MICROPY_HW_SPIFLASH_MISO,
};
STATIC mp_spiflash_cache_t spi_bdev_cache;
const mp_spiflash_config_t spiflash_config = {
    .bus_kind = MP_SPIFLASH_BUS_SPI,
    .bus.u_spi.cs = MICROPY_HW_SPIFLASH_CS,
    .bus.u_spi.data = (void*)&soft_spi_bus,
    .bus.u_spi.proto = &mp_soft_spi_proto,
    .cache = &spi_bdev_cache,
};
#endif

spi_bdev_t spi_bdev;

#endif
