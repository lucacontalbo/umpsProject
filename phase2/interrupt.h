#ifndef INTERRUPT_H
#define INTERRUPT_H

int getDevice(unsigned int inst_dev, unsigned int int_dev);
int getLineInt();
extern void int_handler();

#endif
