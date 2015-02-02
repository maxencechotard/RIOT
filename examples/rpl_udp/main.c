/*
 * Copyright (C) 2013, 2014 INRIA
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup examples
 * @{
 *
 * @file
 * @brief UDP RPL example application
 *
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 *
 * @}
 */

#include <stdio.h>

#include "net_if.h"
#include "posix_io.h"
#include "board_uart0.h"
#include "udp.h"
#include "periph_conf.h"
#include "crypto/aes.h"
#include "ps.h"
#include "rpl_udp.h"

#define FLOAT_PRECISION 1000
#define ADDRESS_IPV6_BOARD 4
#define ADDRESS_ROOM 4

cipher_context_t cipherContext;

void _alarm_handler()
{
    char msg[30]="hello";
    char msgEncrypted[30];

    /* A configurer selon la carte */
    /*Pour les noeuds: */
    aes_encrypt(&cipherContext,(uint8_t*)msg,(uint8_t*)msgEncrypted);
    printf("%s\n",msg);
    printf("%s\n",msgEncrypted);
    udp_send(3,msgEncrypted);
    /*Pour les carte connectées au pc: */
}


void udp_rpl_init(void){
    posix_open(uart0_handler_pid, 0);
    net_if_set_src_address_mode(0, NET_IF_TRANS_ADDR_M_SHORT);
    id = net_if_get_hardware_address(0);

    //initialisation du rpl et du udp
    rpl_udp_set_id(ADDRESS_IPV6_BOARD);
    rpl_udp_init();
    udp_server();
}


int main(void)
{
    puts("RPL router v"APP_VERSION);

    int value = 0;

    udp_rpl_init();
    char key[5] = "xsoen";
    aes_init(&cipherContext,AES_BLOCK_SIZE, 5,(uint8_t*)key);
    thread_print_all();
    
    timex_t sleep = timex_set(2, 0);
 
    while(1){
    	vtimer_sleep(sleep);
    	_alarm_handler();
    }
    return 0;
}
