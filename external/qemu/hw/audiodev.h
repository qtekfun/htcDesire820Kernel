int es1370_init(PCIBus *bus);

int SB16_init(qemu_irq *pic);

int Adlib_init(qemu_irq *pic);

int GUS_init(qemu_irq *pic);

int ac97_init(PCIBus *buf);

int cs4231a_init(qemu_irq *pic);
