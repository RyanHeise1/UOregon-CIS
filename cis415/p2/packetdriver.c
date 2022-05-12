/*
 * Author: Ryan Heise
 * University of Oregon Spring 2022
 */

#include "BoundedBuffer.h"
#include "destination.h"
#include "diagnostics.h"
#include "fakeapplications.h"
#include "freepacketdescriptorstore__full.h"
#include "freepacketdescriptorstore.h"
#include "networkdevice__full.h"
#include "networkdevice.h"
#include "packetdescriptorcreator.h"
#include "packetdescriptor.h"
#include "packetdriver.h"
#include "pid.h"
#include <pthread.h>

#define MAX_TRIES 10 // number of times send_func() will try to send packet
#define UNUSED __attribute__((unused))

/* any global variables required for use by your threads and your driver routines */ 
static pthread_t t_send;
static pthread_t t_recieve;
static NetworkDevice *g_nd;
static FreePacketDescriptorStore *g_fpds;
static BoundedBuffer *g_in[MAX_PID+1];
static BoundedBuffer *g_out;
static BoundedBuffer *g_pool;

void *send_func(UNUSED void *nothing){
 	PacketDescriptor *pd;
 	int success;
    int i;

 	while (1){
 		// get pd from g_out
 		g_out->blockingRead(g_out,(void**)&pd);
 		// try to send packet
 		for (i = 0; i <= MAX_TRIES; i++){
            success = g_nd->sendPacket(g_nd, pd);
 			if (success){
 				break;
 			}
 		}
 		// check if successfully sent
 		if (success) {
 			DIAGNOSTICS("[DRIVER> Info: Sent a packet after %d tries\n", i);
 		}else{
 			DIAGNOSTICS("[DRIVER> Info: Failed to send a packet after %d tries\n", MAX_TRIES);
 		}
 		// return pd to store
 		if(!(g_fpds->nonblockingPut(g_fpds, pd))){
 			g_out->nonblockingWrite(g_out, pd);
 		}
 	}
    return NULL;
}

int nonblock_Get(PacketDescriptor **pd){
    int status;
    if (!(status = g_pool->nonblockingRead(g_pool, (void **)pd))){
        status = g_fpds->nonblockingGet(g_fpds, pd);
    }
    return status;
}

int nonblocking_Put(PacketDescriptor *pd){
    int status;
    if (!(status = g_pool->nonblockingWrite(g_pool, (void *)pd))){
        status = g_fpds->nonblockingPut(g_fpds, pd);
    }
    return status;
}

void *recieve_func(UNUSED void *nothing){
    PacketDescriptor *pd;
    PacketDescriptor *current_pd;
    PID pid;

    g_pool->blockingRead(g_pool, (void **)&current_pd);
    initPD(current_pd);
    // initPD(pd);
    g_nd->registerPD(g_nd, current_pd);
    while(1){
        // blocks until the registered PD has been filled
        g_nd->awaitIncomingPacket(g_nd);
        pd = current_pd;

        int get_success = nonblock_Get(&current_pd); // changes pd here x3
        if (get_success){
            initPD(current_pd);
            // tell the network device to use the indicated PD for next data packet
            g_nd->registerPD(g_nd, current_pd);
            
            // get pid from PD and print
            pid = getPID(pd);
            DIAGNOSTICS("[DRIVER> Info: Packet recieved for application %u\n", pid);
            
            // write PD to buffer
            if (!(g_in[pid]->nonblockingWrite(g_in[pid], pd))){
                //g_in[pid]->nonblockingWrite(g_in[pid], &pd);
                // return PD to store
                int put_success = nonblocking_Put(pd);
                if (!put_success){
                    DIAGNOSTICS("[DRIVER> Info: Warning, Cannot return PacketDescriptor to store\n");
                }
            }
        }else{
            current_pd = pd;
            initPD(current_pd);
            g_nd->registerPD(g_nd, current_pd);
        }
    }
    return NULL;
}

/* definition[s] of function[s] required for your thread[s] */ 
void init_packet_driver(NetworkDevice               *nd, 
                        void                        *mem_start, 
                        unsigned long               mem_length,
                        FreePacketDescriptorStore **fpds_ptr){ 
	int i = 0;
    unsigned long fpds_size;
    PacketDescriptor *pd;
    
    /* create Free Packet Descriptor Store */ 
    g_fpds = FreePacketDescriptorStore_create(mem_start, mem_length); // create fpds
    fpds_size = (g_fpds)->size(g_fpds);
    *fpds_ptr = g_fpds;
    g_nd = nd;

    /* create any buffers required by your thread[s] */ 
    for(i = 0; i < (MAX_PID+1); i++){
    	if ((g_in[i] = BoundedBuffer_create(2)) == NULL){
            DIAGNOSTICS("Error creating BB");
        }
    }
    if ((g_out = BoundedBuffer_create(fpds_size)) == NULL){
        DIAGNOSTICS("Error creating BB");
    }

    g_pool = BoundedBuffer_create(5);
    for (i = 0; i < 4; i++){
        g_fpds->blockingGet(g_fpds, &pd);
        g_pool->blockingWrite(g_pool, (void *)pd);
    }

    /* create any threads you require for your implementation */ 
    if (pthread_create(&t_send, NULL, &send_func, NULL)){
        DIAGNOSTICS("Error creating send thread");
    }
    if (pthread_create(&t_recieve, NULL, &recieve_func, NULL)){
        DIAGNOSTICS("Error creating recieve thread");
    }
    /* return the FPDS to the code that called you */ 
} 
 
void blocking_send_packet(PacketDescriptor *pd) { 
    /* queue up packet descriptor for sending */ 
    /* do not return until it has been successfully queued */ 
    g_out->blockingWrite(g_out, (void*)pd);
} 
 
int nonblocking_send_packet(PacketDescriptor *pd) { 
    /* if you are able to queue up packet descriptor immediately, do so and return 1 */ 
    /* otherwise, return 0 */ 
    return g_out->nonblockingWrite(g_out, (void*)pd);
} 
 
void blocking_get_packet(PacketDescriptor **pd, PID pid) { 
    /* wait until there is a packet for `pid’ */ 
    /* return that packet descriptor to the calling application */ 
    g_in[pid]->blockingRead(g_in[pid], (void**)pd);
}
 
int nonblocking_get_packet(PacketDescriptor **pd, PID pid) { 
    /* if there is currently a waiting packet for `pid’, return that packet */ 
    /* to the calling application and return 1 for the value of the function */ 
    /* otherwise, return 0 for the value of the function */ 
    return g_in[pid]->nonblockingRead(g_in[pid], (void**)pd);
}