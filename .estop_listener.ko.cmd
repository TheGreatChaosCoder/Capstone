cmd_/home/pi/Capstone/estop_listener.ko := ld -r  -EL -T ./scripts/module-common.lds -T ./arch/arm/kernel/module.lds  --build-id  -o /home/pi/Capstone/estop_listener.ko /home/pi/Capstone/estop_listener.o /home/pi/Capstone/estop_listener.mod.o ;  true
