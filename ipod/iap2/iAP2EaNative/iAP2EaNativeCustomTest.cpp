#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils/Log.h>
#include <errno.h>

#include "iAP2EaNativeCustom.h"
#include "iAP2EaNativeCustomTest.h"

#define LOG_TAG "EaNativeCustom"

using namespace android;

char gCustomTestDump[512];

void iAP2EaNativeCustomTest_dump(const char* data, int dataLen)
{
    int i;
    int j;
    char *buf= (char *)gCustomTestDump;
    int length = dataLen;

    if (length>0) {
        ALOGD("  ------------< Dump start >------------------ %p \n",buf);
        for (i=0;i<length;i+=16) {
            int index = 0;
            index += sprintf(&buf[index]," %04X: ",i);
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index]," %02X",data[i+j]);
                } else {
                    index += sprintf(&buf[index],"   ");
                }
            }
            index += sprintf(&buf[index]," : ");
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index],"%c",(data[i+j]>' ')?data[i+j]:'.');
                } else {
                    break;
                }
            }

            ALOGD("  %s\n",buf);
           
        }
        ALOGD(" ------------< Dump end >------------------ \n");
    }
}


void iAP2EaNativeCustomTest_SendTest(void)
{
    uint8_t payload[] = {
        0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 
        0x00, 0x03, 0x37, 0x32, 0x37, 0x44, 0x41, 0x42, 
        0x35, 0x33, 0x00, 0xC1, 0x01, 0x02, 0x00, 0x3C, 
        0x84, 0xDE, 0x7C
        };
    iAP2EaNativeCustom_Write(payload, sizeof(payload));
}

void iAP2EaNativeCustomTest_ReadDone(void *data, int dataLen)
{
    iAP2EaNativeCustomTest_dump((const char *)data, dataLen);
}
