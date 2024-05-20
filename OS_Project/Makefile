GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o port.o interruptstubs.o interrupts.o kernel.o

%.o: %.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

%.o: %.s
	as $(ASPARAMS) -o $@ $< 


loader.o: loader.s
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso
	
run: mykernel.iso
	@STATUS=$$(/mnt/c/Program\ Files/Oracle/VirtualBox/VBoxManage.exe showvminfo "BenimOS" --machinereadable | grep -i "VMState=" | cut -d'=' -f2 | tr -d '"' | tr -d '\r\n'); \
	echo "Sanal makinenin durumu: $$STATUS"; \
	if [ "$$STATUS" = "running" ] || [ "$$STATUS" = "saved" ]; then \
		/mnt/c/Program\ Files/Oracle/VirtualBox/VBoxManage.exe controlvm "BenimOS" poweroff; \
		sleep 5; \
		/mnt/c/Program\ Files/Oracle/VirtualBox/VBoxManage.exe startvm "BenimOS"; \
	elif [ "$$STATUS" = "poweroff" ]; then \
		/mnt/c/Program\ Files/Oracle/VirtualBox/VBoxManage.exe startvm "BenimOS"; \
	else \
		echo "Sanal makine beklenmedik bir durumda: $$STATUS"; \
	fi

.PHONY: clean
clean:
	rm -f $(objects) mykernel.bin mykernel.iso