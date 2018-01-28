#include<sys/idt.h>
#include<sys/kprintf.h>
#include<sys/isr.h>
#include<sys/defs.h>
#include <sys/syscall.h>
#include<sys/types.h>
int newline=0;
volatile int stop=0;
volatile int flag=0;
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

int tck = 0;
uint32_t i = 0;



static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void pic_remap()
{
	//unsigned char a1, a2;
//a1 = inb(0x21);                      
//a2 = inb(0xA1);

__asm__ volatile ("out %0, %1"::"a"(0x20),"Nd"(0x11));
__asm__ volatile ( "out %0, $0x80" : : "a"(0) );
__asm__ volatile ("out %0, %1"::"a"(0xA0),"Nd"(0x11));
__asm__ volatile ( "out %0, $0x80" : : "a"(0) );
__asm__ volatile ("out %0, %1"::"a"(0x21),"Nd"(0x20));
__asm__ volatile ( "out %0, $0x80" : : "a"(0) );
__asm__ volatile ("out %0, %1"::"a"(0xA1),"Nd"(0x28));
__asm__ volatile ( "out %0, $0x80" : : "a"(0) );
__asm__ volatile ("out %0, %1"::"a"(0x21),"Nd"(0x04));
__asm__ volatile ( "out %0, $0x80" : : "a"(0) );
__asm__ volatile ("out %0, %1"::"a"(0xA1),"Nd"(0x02));
__asm__ volatile ( "out %0, $0x80" : : "a"(0) );
__asm__ volatile ("out %0, %1"::"a"(0x21),"Nd"(0x01));
__asm__ volatile ( "out %0, $0x80" : : "a"(0) );
__asm__ volatile ("out %0, %1"::"a"(0xA1),"Nd"(0x01));
__asm__ volatile ( "out %0, $0x80" : : "a"(0) );
__asm__ volatile ("out %0, %1"::"a"(0x21),"Nd"(0x0));
__asm__ volatile ("out %0, %1"::"a"(0xA1),"Nd"(0x0));
}
void idt_install()
{

pic_remap();
set_idt_handler(32,(uint64_t)timr);
set_idt_handler(33,(uint64_t)keyint);
}

void isr_install(){
    set_idt_handler(0, (uint64_t)isr0);
    set_idt_handler(1, (uint64_t)isr1);
    set_idt_handler(2, (uint64_t)isr2);
    set_idt_handler(3, (uint64_t)isr3);
    set_idt_handler(4, (uint64_t)isr4);
    set_idt_handler(5, (uint64_t)isr5);
    set_idt_handler(6, (uint64_t)isr6);
    set_idt_handler(7, (uint64_t)isr7);
    set_idt_handler(8, (uint64_t)isr8);
    set_idt_handler(9, (uint64_t)isr9);
    set_idt_handler(10, (uint64_t)isr10);
    set_idt_handler(11, (uint64_t)isr11);
    set_idt_handler(12, (uint64_t)isr12);
    set_idt_handler(13, (uint64_t)isr13);
    set_idt_handler(14, (uint64_t)isr14);
    set_idt_handler(15, (uint64_t)isr15);
    set_idt_handler(16, (uint64_t)isr16);
    set_idt_handler(17, (uint64_t)isr17);
    set_idt_handler(18, (uint64_t)isr18);
    set_idt_handler(19, (uint64_t)isr19);
    set_idt_handler(20, (uint64_t)isr20);
    set_idt_handler(21, (uint64_t)isr21);
    set_idt_handler(22, (uint64_t)isr22);
    set_idt_handler(23, (uint64_t)isr23);
    set_idt_handler(24, (uint64_t)isr24);
    set_idt_handler(25, (uint64_t)isr25);
    set_idt_handler(26, (uint64_t)isr26);
    set_idt_handler(27, (uint64_t)isr27);
    set_idt_handler(28, (uint64_t)isr28);
    set_idt_handler(29, (uint64_t)isr29);
    set_idt_handler(30, (uint64_t)isr30);
    set_idt_handler(31, (uint64_t)isr31);

}

char *messages[] =
{
    "Exception :: Divide zero",		/* 0 */
    "Exception :: Debug",				/* 1 */
    "Exception :: Non Maskable Interrupt",	        /* 2 */ 
    "Exception :: Breakpoint",			/* 3 */
    "Exception :: Overflow",			/* 4 */
    "Exception :: Out of Bounds",			/* 5 */
    "Exception :: Invalid opcode",			/* 6 */
    "Exception :: Coprocessor not available",		/* 7 */
    "Exception :: System Error",			/* 8 */
    "Exception :: Coprocessor Segment Overrunn",    /* 9 */
    "Exception :: Invalid TSS",			/* 10 */
    "Exception :: Segment Not Present",		/* 11 */
    "Exception :: Stack fault",			/* 12 */
    "Exception :: General protection fault",	/* 13 */
    "Exception :: Page fault",			/* 14 */
    "Reserved",					/* 15 */
    "Exception :: Floating point error",		/* 16 */
    "Reserved",		/* 17 */
    "Reserved",			/* 18 */
    "Reserved",		/* 19 */
    "Reserved",			/* 20 */
    "Reserved",			/* 21 */
    "Reserved",			/* 22 */
    "Reserved",			/* 23 */
    "Reserved",			/* 24 */
    "Reserved",			/* 25 */
    "Reserved",			/* 26 */
    "Reserved",			/* 27 */
    "Reserved",			/* 28 */
    "Reserved",			/* 29 */
    "Reserved",			/* 30 */
    "Reserved",			/* 31 */
};


void int_key(){
  uint8_t code;
  static uint8_t shiftkey=0;
  __asm__ volatile ("out %0, %1" : : "a" (0x20), "Nd" (0x20));
  __asm__ volatile("in %1,%0" : "=a"(code):"Nd"(0x60));

/********Handling shift key - START ********/        
if(code == 0x2A){ //leftshift
  shiftkey |=1;
}
if(code == (0x2A|0x80)){
  shiftkey &= ~1;
}
if(code == 0x2B){//rightshift
  shiftkey |= 2;
}
if(code == (0x2B | 0x80)){
  shiftkey &= ~2;
}
/*
if(code == 0x0E){//for backspace
  //kprintf("backspace");
  backspace();
}*/
/********Handling shift key - END ********/

if (code == 30){
  if(shiftkey){
    inputbuf[inputlen++] = 'A';
    kprintf("A");  
  } 
  else{
    inputbuf[inputlen++] = 'a';
    kprintf("a");
  } 
}

/*if (code == ((0x1E | 0x80))){
kprintf("##Relesed##");
}*/

if (code == 48){
  if(shiftkey){
    inputbuf[inputlen++] = 'B';
    kprintf("B");  
  } 
  else{
    inputbuf[inputlen++] = 'b';
    kprintf("b");
  } 
}

if (code == 46){
  if(shiftkey){
    inputbuf[inputlen++] = 'C';
    kprintf("C");  
  } 
  else{
    inputbuf[inputlen++] = 'c';
    kprintf("c");
  } 
}
if (code == 32){
  if(shiftkey){
    inputbuf[inputlen++] = 'D';
    kprintf("D");  
  } 
  else{
    inputbuf[inputlen++] = 'd';
    kprintf("d");
  } 
}
if (code == 18){
  if(shiftkey){
    inputbuf[inputlen++] = 'E';
    kprintf("E");  
  } 
  else{
    inputbuf[inputlen++] = 'e';
    kprintf("e");
  } 
}
if (code == 33){
  if(shiftkey){
    inputbuf[inputlen++] = 'F';
    kprintf("F");  
  } 
  else{
    inputbuf[inputlen++] = 'f';
    kprintf("f");
  } 
}
if (code == 34){
  if(shiftkey){
    inputbuf[inputlen++] = 'G';
    kprintf("G");  
  } 
  else{
    inputbuf[inputlen++] = 'g';
    kprintf("g");
  } 
}
if (code == 35){
  if(shiftkey){
    inputbuf[inputlen++] = 'H';
    kprintf("H");  
  } 
  else{
    inputbuf[inputlen++] = 'h';
    kprintf("h");
  } 
}
if (code == 23){
  if(shiftkey){
    inputbuf[inputlen++] = 'I';
    kprintf("I");  
  } 
  else{
    inputbuf[inputlen++] = 'i';
    kprintf("i");
  } 
}

if (code == 36){
  if(shiftkey){
    inputbuf[inputlen++] = 'J';
     kprintf("J");  
  }
  else{
    inputbuf[inputlen++] = 'j';
    kprintf("j");
  } 
}
if (code == 37){
  if(shiftkey){
    inputbuf[inputlen++] = 'K';
    kprintf("K");  
  } 
  else{
    inputbuf[inputlen++] = 'k';
    kprintf("k");
  } 
}
if (code == 38){
  if(shiftkey){
    inputbuf[inputlen++] = 'L';
    kprintf("L");  
  } 
  else{
    inputbuf[inputlen++] = 'l';
    kprintf("l");
  } 
}

if (code == 50){
  if(shiftkey){
    inputbuf[inputlen++] = 'M';
     kprintf("M");  
  }
  else{
    inputbuf[inputlen++] = 'm';
    kprintf("m");
  } 
}

if (code == 49){
  if(shiftkey){
    inputbuf[inputlen++] = 'N';
     kprintf("N");  
  }
  else{
    inputbuf[inputlen++] = 'n';
    kprintf("n");
  } 
}

if (code == 24){
  if(shiftkey){
    inputbuf[inputlen++] = 'O';
     kprintf("O");  
  }
  else{
    inputbuf[inputlen++] = 'o';
    kprintf("o");
  } 
}
if (code == 25){
  if(shiftkey){
    inputbuf[inputlen++] = 'P';
    kprintf("P");  
  } 
  else{
    inputbuf[inputlen++] = 'p';
    kprintf("p");
  } 
}
if (code == 16){
  if(shiftkey){
    inputbuf[inputlen++] = 'Q';
    kprintf("Q");  
  } 
  else{
    inputbuf[inputlen++] = 'q';
    kprintf("q");
  } 
}
if (code == 19){
  if(shiftkey){
    inputbuf[inputlen++] = 'R';
    kprintf("R");  
  } 
  else{
    inputbuf[inputlen++] = 'r';
    kprintf("r");
  } 
}

if (code == 31){
  if(shiftkey){
    inputbuf[inputlen++] = 'S';
    kprintf("S");  
  } 
  else{
    inputbuf[inputlen++] = 's';
    kprintf("s");
  } 
}
if (code == 20){
  if(shiftkey){
    inputbuf[inputlen++] = 'T';
    kprintf("T");  
  } 
  else{
    inputbuf[inputlen++] = 't';
    kprintf("t");
  } 
}
if (code == 22){
  if(shiftkey){
    inputbuf[inputlen++] = 'U';
    kprintf("U");  
  } 
  else{
    inputbuf[inputlen++] = 'u';
    kprintf("u");
  } 
}
if (code == 47){
  if(shiftkey){
    inputbuf[inputlen++] = 'V';
    kprintf("V");  
  } 
  else{
    inputbuf[inputlen++] = 'v';
    kprintf("v");
  } 
}
if (code == 17){
  if(shiftkey){
    inputbuf[inputlen++] = 'W';
    kprintf("W");  
  } 
  else{
    inputbuf[inputlen++] = 'w';
    kprintf("w");
  } 
}
if (code == 45){
  if(shiftkey){
    inputbuf[inputlen++] = 'X';
    kprintf("X");  
  } 
  else{
    inputbuf[inputlen++] = 'x';
    kprintf("x");
  } 
}
if (code == 21){
  if(shiftkey){
    inputbuf[inputlen++] = 'Y';
    kprintf("Y");  
  } 
  else{
    inputbuf[inputlen++] = 'y';
    kprintf("y");
  } 
}
if (code == 44){
  if(shiftkey){
    inputbuf[inputlen++] = 'Z';
    kprintf("Z");  
  } 
  else{
    inputbuf[inputlen++] = 'z';
    kprintf("z");
  } 
}

if (code == 11){

if(shiftkey){
    inputbuf[inputlen++] = ')';
    kprintf(")");  
  } 
  else{
    inputbuf[inputlen++] = '0';
    kprintf("0");
  } 
  
}
if (code == 2){

if(shiftkey){
    inputbuf[inputlen++] = '!';
    kprintf("!");  
  } 
  else{
    inputbuf[inputlen++] = '1';
    kprintf("1");
  } 
}
if (code == 3){

if(shiftkey){
    inputbuf[inputlen++] = '@';
    kprintf("@");  
  } 
  else{
    inputbuf[inputlen++] = '2';
    kprintf("2");
  } 
}
if (code == 4){
if(shiftkey){
    inputbuf[inputlen++] = '#';
    kprintf("#");  
  } 
  else{
    inputbuf[inputlen++] = '4';
    kprintf("4");
  } 
}
if (code == 5){
if(shiftkey){
    inputbuf[inputlen++] = '$';
    kprintf("$");  
  } 
  else{
    inputbuf[inputlen++] = '4';
    kprintf("4");
  } 
}
if (code == 6){
if(shiftkey){
    inputbuf[inputlen++] = '%';
    kprintf("%");  
  } 
  else{
    inputbuf[inputlen++] = '5';
    kprintf("5");
  } 
}
if (code == 7){

if(shiftkey){
    inputbuf[inputlen++] = '^';
    kprintf("^");  
  } 
  else{
    inputbuf[inputlen++] = '6';
    kprintf("6");
  } 
}
if (code == 8){
if(shiftkey){
    inputbuf[inputlen++] = '&';
    kprintf("&");  
  } 
  else{
    inputbuf[inputlen++] = '7';
    kprintf("7");
  } 
}
if (code == 9){

if(shiftkey){
    inputbuf[inputlen++] = '*';
    kprintf("*");  
  } 
  else{
    inputbuf[inputlen++] = '8';
    kprintf("8");
  } 
}
if (code == 10){

if(shiftkey){
    inputbuf[inputlen++] = '(';
    kprintf("(");  
  } 
  else{
    inputbuf[inputlen++] = '9';
    kprintf("9");
  } 
}

if (code == 29){
      inputbuf[inputlen++]='^';
        kprintf("^");}
if (code == 0x39){//for spacebar
  inputbuf[inputlen++]=' ';
  kprintf(" ");
}
if (code == 0x34){

if(shiftkey){
    inputbuf[inputlen++] = '>';
    kprintf(">");  
  } 
  else{
    inputbuf[inputlen++] = '.';
    kprintf(".");
  } 
}

if (code == 0x35){

if(shiftkey){
    inputbuf[inputlen++] = '?';
    kprintf("?");  
  } 
  else{
    inputbuf[inputlen++] = '/';
    kprintf("/");
  } 
}

if (code == 0x0c){

if(shiftkey){
    inputbuf[inputlen++] = '_';
    kprintf("_");  
  } 
  else{
    inputbuf[inputlen++] = '-';
    kprintf("-");
  } 
}

if(code == 0x1C){
  inputbuf[inputlen++] = '\n';
  //inputbuf[inputlen++] = '\\';
  //inputbuf[inputlen++] = 'n';
  flag=0;
  newline++;
  
  kprintf("\n");
  //return;
}

if(code == 0x0E){
//kprintf("bb");
  inputbuf[inputlen-1]='\0';
  inputlen--;
  backspace();
  //return;
}
//kprintf(" %s  ",inputbuf);
    //once done append null character to the inputbuf -- where? inputbuf[inputlen++] = '\n';
	 //__asm__ volatile ("out %0, %1" : : "a" (0x20), "Nd" (0x20)); //EOI			
}
char *stdin_read(char *str){
  __asm__ volatile("sti;");
  //kprintf("%d\n",newline);
  flag=1;
  while(flag==1);
  
  //kprintf("out of while loop\n");
  memcpy((void*)str,(void*)inputbuf,inputlen);
  //str = "hello\n";
  
  //int count = strlength(str);
  //if(count < 10) inputlen = count;
  //memcpy((void*)str,(void*)inputbuf,inputlen);
  while(inputlen >=0){
  inputbuf[inputlen] = '\0';
  //temp[i] = '\0';
  inputlen --;
  }
  inputlen =0;  
  return str;
}
void irq_function()
{
__asm__ volatile ("out %0, %1" : : "a" (0x20), "Nd" (0x20));
//kprintf("ouch\n");
	tck++;
        if(tck %18==0){
          //kprintf("%d",i);
          timeprint(i);
          //kprintf("tume");
	        i=i+1;
                 
         
	}
	
 if(stop!=0)
         stop--;
	//__asm__ volatile ("out %0, %1" : : "a" (0xA0), "Nd" (0x20));
	//__asm__ volatile ("out %0, %1" : : "a" (0x20), "Nd" (0x20));//EOI

}





void int_handler(){

uint32_t divisor = 1193182/65536;
uint8_t low = (uint8_t)(divisor & 0xff);
uint8_t high = (uint8_t)(divisor>>8 & 0xff);
__asm__ __volatile__("out %0, %1" : : "a" (0x43), "Nd" (0x36));
__asm__ __volatile__("out %0, %1" : : "a" (0x40), "Nd" (low));
__asm__ __volatile__("out %0, %1" : : "a" (0x40), "Nd" (high));

}


void isr_handler(struct register_t *r){
uint64_t faulting_addr = 0;
    __asm__ __volatile__ ("movq %%cr2, %0" : "=a" (faulting_addr));
    kprintf("Faulting address is %p\n", faulting_addr);
if(r->int_no == 14){
        //handle_page_fault(r);
     }
if (r->int_no < 32)
    {
        //kprintf(messages[r->int_no]);
        kprintf(" \n!!! HALT !!!\n");
        while(1);
    }
}


void SLEEP(uint64_t time){
__asm__ volatile("sti;");
  stop = time*18;
  //while
  //kprintf("stop : %d\n",stop);
  
  while(stop!=0);
}