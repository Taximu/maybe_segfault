#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "IPHLPAPI.lib")

int main() {

    PIP_ADAPTER_INFO pAdI;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    pAdI = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
    if (pAdI == NULL)
        printf("Error allocating memory\n");

    if (GetAdaptersInfo(pAdI, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdI);
        pAdI = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
        if (pAdI == NULL) 
            printf("Error allocating memory\n");
    }

    if ((dwRetVal = GetAdaptersInfo(pAdI, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdI;
        while (pAdapter) {
            printf("\nName: %s\n", pAdapter->AdapterName);
            printf("Desc: %s\n", pAdapter->Description);
            printf("AddrLen: %ld\n",pAdapter->AddressLength);
            printf("Addr: ");
            for (int i = 0; i < pAdapter->AddressLength; i++)
                if (i == (pAdapter->AddressLength - 1))
                    printf("%.2X\n", (int) pAdapter->Address[i]);
                else
                    printf("%.2X-", (int) pAdapter->Address[i]);

            PIP_ADDR_STRING pAddr = &(pAdapter->IpAddressList);
            while(pAddr) {
                printf("ipv4: %s\n", pAddr->IpAddress.String);
                pAddr = pAddr->Next;
            }

            pAdapter = pAdapter->Next;
        }
    } else 
        printf("Failed...\n");

    if (pAdI)
        free(pAdI);

    return 0;
}