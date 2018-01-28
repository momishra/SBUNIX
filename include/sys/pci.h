#ifndef PCI_H
#define PCI_H

#include"defs.h"
//#include"ahci.h"
typedef struct{
uint16_t deviceID;
uint16_t vendorID;
uint16_t status;
uint16_t command;
uint8_t class_code;
uint8_t subclass;
uint8_t progIf;
uint8_t revID;
uint8_t bist;
uint8_t htype;
uint8_t latTime;
uint8_t cacheLsize;
}__attribute__((packed))pci_header_struct;

pci_header_struct *pcihead;
void checkAllBuses();
//void checkbuses(void);
void *memset(void *string, int val, size_t length);
#endif
