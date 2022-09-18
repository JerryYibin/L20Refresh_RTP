/* webclidemo.cdf - user component description file */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06nov14,r_w  add requirement for MIBway
27may14,r_w  eliminate dependency for V7MAN-62
28mar14,r_w  written
*/

/*
DESCRIPTION
This file contains descriptions for the user components.
*/

/* Generic configuration parameters */

Component INCLUDE_WEBCLI_BRANSONWEBSERVICES {
    NAME      WebCLI Demo
    SYNOPSIS  Webserver and Simple CLI Component
    MODULES   
    INIT_RTN  WMB_WEBCLI_COMPONENT_Start();   
    _INIT_ORDER usrRoot        
    REQUIRES  INCLUDE_WEBCLI_BRANSONHTTP    \
              INCLUDE_WEBCLI_CLI     \
              INCLUDE_WEBCLI_BACKPLANE \
              INCLUDE_WEBCLI_MIBWAY \
              INCLUDE_WEBCLI_COMMON  \
              INCLUDE_DOSFS          \
              INCLUDE_XBD_RAMDRV     \
              INCLUDE_IPSSL

    EXCLUDES  INCLUDE_WEBCLI_WEBDEMO INCLUDE_WEBCLI_CLIDEMO
    LINK_SYMS webcli_cli_init webcli_http_init webcli_common_init    
    _CHILDREN FOLDER_WEBCLI
}
