#define CCM_CCGR0           0X020C4068
#define CCM_CCGR1           0X020C406C
#define CCM_CCGR2           0X020C4070
#define CCM_CCGR3           0X020C4074
#define CCM_CCGR4           0X020C4078
#define CCM_CCGR5           0X020C407C
#define CCM_CCGR6           0X020C4080

#define SW_MUX_GPIO1_IO03   0X020E0068
#define SW_PAD_GPIO1_IO03   0X020E02F4
  
#define GPIO1_DR            0X0209C000
#define GPIO1_GDIR          0X0209C004
#define GPIO1_PSR           0X0209C008
#define GPIO1_ICR1          0X0209C00C
#define GPIO1_ICR2          0X0209C010
#define GPIO1_IMR           0X0209C014
#define GPIO1_ISR           0X0209C018
#define GPIO1_EDGE_SEL      0X0209C01C


#define VA_CCM_CCGR0           (0X020C4068+0x100000)
#define VA_CCM_CCGR1           (0X020C406C+0x100000)
#define VA_CCM_CCGR2           (0X020C4070+0x100000)
#define VA_CCM_CCGR3           (0X020C4074+0x100000)
#define VA_CCM_CCGR4           (0X020C4078+0x100000)
#define VA_CCM_CCGR5           (0X020C407C+0x100000)
#define VA_CCM_CCGR6           (0X020C4080+0x100000)

#define VA_SW_MUX_GPIO1_IO03   (0X020E0068+0x100000)
#define VA_SW_PAD_GPIO1_IO03   (0X020E02F4+0x100000)
  
#define VA_GPIO1_DR            (0X0209C000+0x100000)
#define VA_GPIO1_GDIR          (0X0209C004+0x100000)
#define VA_GPIO1_PSR           (0X0209C008+0x100000)
#define VA_GPIO1_ICR1          (0X0209C00C+0x100000)
#define VA_GPIO1_ICR2          (0X0209C010+0x100000)
#define VA_GPIO1_IMR           (0X0209C014+0x100000)
#define VA_GPIO1_ISR           (0X0209C018+0x100000)
#define VA_GPIO1_EDGE_SEL      (0X0209C01C+0x100000)




#define MMU_SECTION_AP              (0x3<<10)
#define MMU_SECTION_DOMAIN          (0<<5)
#define MMU_SECTION_NCNB            (0<<2)
#define MMU_SECTION_ECEB            (0x3<<2)
#define MMU_SECTION_TYPE            ((1<<4)|(1<<1))
 
#define MMU_SECTION_MEM (MMU_SECTION_AP|MMU_SECTION_DOMAIN|MMU_SECTION_ECEB|MMU_SECTION_TYPE)


void clk_enable(void)
{
   *((volatile unsigned int *) VA_CCM_CCGR0) = 0xffffffff;
   *((volatile unsigned int *) VA_CCM_CCGR1) = 0xffffffff;
   *((volatile unsigned int *) VA_CCM_CCGR2) = 0xffffffff;
   *((volatile unsigned int *) VA_CCM_CCGR3) = 0xffffffff;
   *((volatile unsigned int *) VA_CCM_CCGR4) = 0xffffffff;
   *((volatile unsigned int *) VA_CCM_CCGR5) = 0xffffffff;
   *((volatile unsigned int *) VA_CCM_CCGR6) = 0xffffffff;
}

void led_init(void)
{
   *((volatile unsigned int *)VA_SW_MUX_GPIO1_IO03) = 0x5;
   *((volatile unsigned int *)VA_SW_PAD_GPIO1_IO03) = 0x10B0;
   *((volatile unsigned int *)VA_GPIO1_GDIR) = 0x0000008;
   *((volatile unsigned int *)VA_GPIO1_DR)  = 0x0;//打开LED
}

static void create_tlb(unsigned int *ttb,unsigned int va,unsigned pa)
{
    int index;
    index = va/0x100000; //1MB
    ttb[index]=(pa&0xfff00000)|MMU_SECTION_MEM;
}

void create_page_table(void)
{
    /*512M DDR map*/
    unsigned int *ttb = (unsigned int *)0x90000000;
    unsigned int va = 0x80000000;
    unsigned int pa = 0x80000000;
    unsigned char loop = 0;
    for(;va<0x90000000;va+=0x100000,pa+=0x100000)//DDR只映射了256MB
    {
        create_tlb(ttb,va,pa);
    }
    /*register map*/
    for(loop=0;loop<7;loop++)
    {
        create_tlb(ttb,VA_CCM_CCGR0+loop*4,CCM_CCGR0+loop*4);
    }
    for(loop=0;loop<8;loop++)
    {
        create_tlb(ttb,VA_GPIO1_DR+loop*4,GPIO1_DR+loop*4);
    } 
    
    create_tlb(ttb,VA_SW_PAD_GPIO1_IO03,SW_PAD_GPIO1_IO03);
    create_tlb(ttb,VA_SW_MUX_GPIO1_IO03,SW_MUX_GPIO1_IO03);
    
}
static void mmu_init(void)
{
    __asm__(
        "ldr r0,=0x90000000\n"
        "mcr p15,0,r0,c2,c0,0\n"
        "mvn r0,#0\n"
        "mcr p15,0,r0,c3,c0,0\n"
        "mrc p15,0,r0,c1,c0,0\n"
        "orr r0,r0,#0x0001\n"
        "mcr p15,0,r0,c1,c0,0\n"
        :
        :
    );
}

int main(void)
{
    create_page_table();
    mmu_init();
    clk_enable();
    led_init();
    while(1);
    return 0;
}




