/* 40vxbBransonLoggerModule.cdf - Component Definition file for loggerModule */

/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

Component       BRANSON_DRV_LOGGER_MODULE {
    NAME        Branson Logger Module
    SYNOPSIS    loggerModule driver
    MODULES     loggerModule.o
    _CHILDREN   FOLDER_DRIVERS
    PROTOTYPE   extern void loggerInit(char *, char *, int, int);
    CFG_PARAMS  LOG_NAME1 LOG_NAME2 LOG_MAX_LINE LOG_PRIORITY
    INIT_RTN    loggerInit(LOG_NAME1, LOG_NAME2, LOG_MAX_LINE, LOG_PRIORITY);
    REQUIRES    INCLUDE_RTP
    _INIT_ORDER usrRoot
    INIT_BEFORE usrScInit
}
Parameter LOG_NAME1 {
    NAME        log file name 1
    TYPE        char *
    SYNOPSIS    default value "/mmc1a/logFile1.txt"
    DEFAULT     "/mmc1a/logFile1.txt"
}
Parameter LOG_NAME2 {
    NAME        log file name 2
    TYPE        char *
    SYNOPSIS    default value "/mmc1a/logFile2.txt"
    DEFAULT     "/mmc1a/logFile2.txt"
}
Parameter LOG_MAX_LINE {
    NAME        log max line count
    SYNOPSIS    default value 2000
    TYPE        int
    DEFAULT     2000
}
Parameter LOG_PRIORITY {
    NAME        log task priority
    SYNOPSIS    default value 230
    TYPE        int
    DEFAULT     230
}