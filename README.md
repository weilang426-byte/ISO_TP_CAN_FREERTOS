使用stm32f103C8T6这款芯片，SRAM为20KB，Flash为64KB，当前freertos中Heap使用3072B，后续可以根据情况适当调整；
目前OLED的刷新至少需要600字节，所以给OLED任务分配256*4B（254字）。
1
