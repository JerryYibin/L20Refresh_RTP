
/* 40prudrv.cdf - Component configuration file */



Component BRANSON_INCLUDE_PRUDRV {
    NAME        PRU device driver
    SYNOPSIS    Attach the LED device to standard I/O 
    PROTOTYPE   STATUS pruDrv (void);
    INIT_RTN    pruDrv ();
    HDR_FILES       vxbPru.h
    _INIT_ORDER usrIosCoreInit
    INIT_AFTER  INCLUDE_TTY_DEV
    _CHILDREN   FOLDER_PERIPHERALS
}