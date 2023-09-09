#ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
#ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
#ID: 2020A7PS0993P                             NAME: Satvik Sinha 
#ID: 2020A7PS0036P                             NAME: Aarya Attrey
#ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
# Group : 11


All:
	gcc-9 -o compiler driver.c


asm:
	nasm -felf64 code.asm
	gcc -no-pie code.o
	