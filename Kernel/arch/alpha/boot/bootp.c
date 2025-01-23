// SPDX-License-Identifier: GPL-2.0
/*
 * arch/alpha/boot/bootp.c
 *
 * Copyright (C) 1997 Jay Estabrook
 *
 * This file is used for creating a bootp file for the Linux/AXP kernel
 *
 * based significantly on the arch/alpha/boot/main.c of Linus Torvalds
 */
#include <linux/kernel.h> //chứa các định nghĩa chung và các hàm cơ bản của kernel, chẳng hạn như hàm printk() để ghi log trong không gian kernel
#include <linux/slab.h> //cung cấp các hàm để quản lý bộ nhớ động trong kernel, chẳng hạn như kmalloc() và kfree()
#include <linux/string.h> //định nghĩa các hàm xử lý chuỗi (chuỗi ký tự) cho kernel, như strcmp(), strcpy(), strlen()
#include <generated/utsrelease.h> //chứa thông tin về phiên bản kernel đang chạy (ví dụ như phiên bản UTS release của hệ thống)
#include <linux/mm.h> //cung cấp các hàm và cấu trúc cho quản lý bộ nhớ, ví dụ như cấp phát và giải phóng các trang nhớ
#include <asm/console.h> //chứa các định nghĩa liên quan đến console (giao diện đầu cuối) trong kiến trúc cụ thể (ở đây có thể là kiến trúc x86 hoặc ARM)
#include <asm/hwrpb.h> //liên quan đến thông tin bảng tham chiếu phần cứng (hardware reference page block), được sử dụng trong một số kiến trúc phần cứng cụ thể, như Alpha
#include <asm/io.h> //định nghĩa các hàm vào/ra cho truy cập các thiết bị phần cứng trực tiếp, chẳng hạn như inb() và outb() để đọc/ghi vào các cổng I/O
#include <linux/stdarg.h> //cung cấp các macro để xử lý danh sách đối số biến thiên trong hàm,tương tự như stdarg.h trong c chuẩn
#include "ksize.h" //Đây có vẻ là một tệp header tự định nghĩa, có thể liên quan đến việc tính toán kích thước của các vùng nhớ trong kernel hoặc các cấu trúc dữ liệu cụ thể. Bạn có thể kiểm tra nội dung của tệp này để biết rõ hơn.

//khai báo của một hàm được định nghĩa ở nơi khác, có tên là switch_to_osf_pal
//nhiệm vụ của nó là chuyển đổi hoặc điều khiển các trạng thái tiến trình trong kernel, đặc biệt trong kiến trúc Alpha.
extern unsigned long switch_to_osf_pal(unsigned long nr, struct pcb_struct *pcb_va, struct pcb_struct *pcb_pa, unsigned long *vptb);
//hàm này di chuyển hoặc đặt lại con trỏ stack đến địa chỉ mới new_stack, cho phép kernel thay đổi vị trí của ngăn xếp hiện tại. điều này là cần thiết trong quá trình quản lý bộ nhớ hoặc khi chuyển đổi giữa các ngữ cảnh của tiến trình.
extern void move_stack(unsigned long new_stack);
//có thể lưu trữ thông tin tham chiếu phần cứng hardware reference page block, thường dùng trong các kiến trúc phần cứng cụ thể chứa thông tin liên quan đến bộ xử lý hoặc tài nguyên phần cứng khác.
struct hwrpb_struct *hwrpb = INIT_HWRPB;
// cấu trúc này có thể đại diện cho khối điều khiển tiến trình (process control block - pcb), lưu trữ các thông tin cần thiết để kernel quản lý tiến trình như thanh ghi cpu, con trỏ ngăn xếp,..
static struct pcb_struct pcb_va[1];
/*
 * Find a physical address of a virtual object..
 *
 * This is easy using the virtual page table address.
 */
// đoạn mã này định nghĩa một hàm inline để tìm địa chỉ vật lý (physical address) từ một địa chỉ ảo (vitual address)
static inline void * find_pa(unsigned long *vptb, void *ptr){
	unsigned long address = (unsigned long) ptr;
	unsigned long result;
	result = vptb[address >> 13];
	result >>= 32;
	result <<= 13;
	result |= address & 0x1fff;
	return (void *) result;
	// Hàm find_pa chuyển đổi một địa chỉ ảo (ptr) thành địa chỉ vật lý bằng cách sử dụng bảng trang (vptb). Nó sử dụng các phép dịch bit để truy xuất đúng mục từ trong bảng trang và kết hợp với phần offset trong địa chỉ ảo để tính địa chỉ vật lý.
	//Hàm này có thể được sử dụng trong hệ điều hành hoặc kernel khi cần dịch địa chỉ ảo của tiến trình sang địa chỉ vật lý của bộ nhớ.
}	

/*
 * This function moves into OSF/1 pal-code, and has a temporary
 * PCB for that. The kernel proper should replace this PCB with
 * the real one as soon as possible.
 *
 * The page table muckery in here depends on the fact that the boot
 * code has the L1 page table identity-map itself in the second PTE
 * in the L1 page table. Thus the L1-page is virtually addressable
 * itself (through three levels) at virtual address 0x200802000.
 */
// hai define này định nghĩa các địa chỉ cố định trong không gian địa chỉ của kernel.
// VPTB có thể là bảng trang ảo (virtual page table base), nơi kernel lưu trữ các mục từ của bảng trang để dịch địa chỉ ảo sang địa chỉ vật lý.
// địa chr 0x200000000 có thể là địa chỉ cơ sở (base address) của vùng nhớ nơi bảng trang được ánh xạ. đậy thường là địa chỉ cố định trong các hệ thống hỗ trợ dịch địa chỉ ảo
#define VPTB	((unsigned long *) 0x200000000)
// L1 có thể đại diện cho bảng trang mức 1 (level 1 page table) trong hệ thống phân cấp quản lý bộ nhớ. ở một số kiến trúc, địa chỉ bộ nhớ ảo được dịch qua nhiều mức bảng trang L1, L2, L3
// địa chỉ 0x200802000 có thể là địa chỉ cụ thể của bảng trang L1 trong hệ thống này.
#define L1	((unsigned long *) 0x200802000)
//Cả hai định nghĩa này có thể được sử dụng trong các hàm như find_pa() mà bạn đã đề cập trước đó để tìm địa chỉ vật lý từ địa chỉ ảo. Trong quá trình dịch địa chỉ, kernel sẽ sử dụng bảng trang (như VPTB hoặc L1) để ánh xạ từ địa chỉ ảo sang địa chỉ vật lý.
// hàm pal_init() dường như khởi tạo môi trường để chuyển đổi sang mã PAL (Privileged Architecture Library) của hệ điều hành OSF (Open Software Foundation). PAL là một tập hợp các hàm đặc quyền được sử dụng bởi bộ xử lý Alpha và các hệ thống tương tự. chúng giúp điều khiển phần cứng ở mức thấp hơn mà hệ điều hành cần để quản lý tài nguyên
void pal_init(void){
	unsigned long i, rev;
	// con trỏ đến cấu trúc percpu_struct, đại diện cho thông tin dành riêng cho CPU trong hệ thống đa xử lý.
	struct percpu_struct * percpu;
	// con trỏ đến pcb_struct, đại diện cho một khối điều khiển tiến trình (process control block) chứa các thông tin cần thiết để kernel điều khiển quá trình thực thi của tiến trình.
	struct pcb_struct * pcb_pa;

	/* Create the dummy PCB.  */
	// đây là con trỏ đến một pcb_struct (được khai báo trước đó)
	// các trường như ksp, usp, ptbr, trong pcb_va được khởi tạo với các giá trị cụ thể. đây là các trường cơ bản trong khối điều khiển tiến trình.
	pcb_va->ksp = 0;
	pcb_va->usp = 0;
	// page table base register, chứa địa chỉ của bảng trang L1[1] được dịch phải 32 bit để lấy địa chỉ của bảng trang L1
	pcb_va->ptbr = L1[1] >> 32;
	// các trường asn, pcc, flags và các trường dự trữ (res1, res2) được khởi tạo bằng giá trị mặc định.
	pcb_va->asn = 0;
	pcb_va->pcc = 0;
	pcb_va->unique = 0;
	pcb_va->flags = 1;
	pcb_va->res1 = 0;
	pcb_va->res2 = 0;
	// sau khi tạo PCB, tiếp đó sử dụng hàm find_pa để dịch địa chỉ ảo của PCB sang địa chỉ vật lý.
	pcb_pa = find_pa(VPTB, pcb_va);

	/*
	 * a0 = 2 (OSF)
	 * a1 = return address, but we give the asm the vaddr of the PCB
	 * a2 = physical addr of PCB
	 * a3 = new virtual page table pointer
	 * a4 = KSP (but the asm sets it)
	 */
	// chuyển đổi sang mã PAL của OSF
	srm_printk("Switching to OSF PAL-code .. ");
	// nếu kết quả của switch_to_osf_pal khác 0 (tức là chuyển đổi thất bại), hệ thống sẽ in mã lỗi và dừng lại bằng __halt().
	i = switch_to_osf_pal(2, pcb_va, pcb_pa, VPTB);
	if (i) {
		srm_printk("failed, code %ld\n", i);
		__halt();
	}
	// lấy thong tin dành riêng cho cpu
	percpu = (struct percpu_struct *)
		(INIT_HWRPB->processor_offset + (unsigned long) INIT_HWRPB);
	rev = percpu->pal_revision = percpu->palcode_avail[2];
	srm_printk("Ok (rev %lx)\n", rev);
	// là một hàm thực hiện việc "TLB Invalidate All" (xóa sạch tất cả các mục trong bảng tra cứu TLB-translation Lookaside Buffer). TLB lưu trữ ánh xạ giữa địa chỉ ảo và địa chỉ vật lý, và việc xóa TLB là cần thiết khi bảng trang được thay đổi để tránh xung đột địa chỉ.
	tbia(); /* do it directly in case we are SMP */
	// Hàm pal_init() thực hiện khởi tạo và chuyển đổi hệ thống sang mã PAL của OSF, một bước cần thiết trong quá trình khởi động hệ thống hoặc thay đổi trạng thái đặc quyền của bộ xử lý trên các kiến trúc như Alpha. Nó sử dụng khối điều khiển tiến trình (PCB) và bảng trang để thiết lập môi trường chuyển đổi.
}

// hàm load() là một hàm inline đơn giản sử dụng hàm memcpy() để sao chép một khối dữ liệu từ địa chỉ nguồn src sang địa chỉ đích dst
static inline void load(unsigned long dst, unsigned long src, unsigned long count){
	memcpy((void *)dst, (void *)src, count);
	// hàm này được sử dụng để sao chép một khối dữ liệu từ vùng nhớ này sang vùng nhớ khác. việc sử dụng inline cho thấy rằng hàm này có thể được gọi nhiều lần và yêu cầu hiệu năng cao trong quá trình thực thi
}
/*
 * Start the kernel.
 */
// hàm runkernel trong đoạn mã này là một hàm inline sử dụng assembly để thực hiện việc chuyển điều khiển đến địa chỉ bắt đầu của kernel và bắt đầu thực thi nó.
static inline void runkernel(void){
	// khối lệnh assembly
	// __asm__ hoặc asm là chỉ thị cho compiler rằng bạn đang nhúng mã hợp ngữ assembly vào trong mã c
	// từ khóa volatile báo cho compiler không được tối ưu hóa đoạn mã này, vì nó có tác dụng nhảy vào kernel để thực thi.
	__asm__ __volatile__(
		"bis %0,%0,$27\n\t"
		"jmp ($27)" 
		: /* no outputs: it doesn't even return */
		: "r" (START_ADDR));
	// đoạn mã trên thường được sử dụng để chuyển đổi ngữ cảnh thực thi sang kernel hoặc một routine cấp thấp khác, đặc biệt trong lập trình hệ thống. START_ADDR cần được định nghĩa ở nơi khác trong mã và xẽ đại diện cho địa chỉ bộ nhớ để nhảy đến (thường là điểm bắt đầu của kernel)
}

// đoạn mã này định nghĩa một địa chỉ khởi đầu cho kernel bằng cách sử dụng toán tử và phép toán bit.
extern char _end; 
#define KERNEL_ORIGIN \ ((((unsigned long)&_end) + 511) & ~511) //định nghĩa hằng số, đoạn mã này định nghĩa một hằng số bằng cách thực hiện các phép toán sau:
// mục đích của KERNEL_ORIGIN là để xác định địa chỉ khởi đầu cho kernel, đảm bảo rằng địa chỉ này là một bội số của 512. điều này thường rất quan trọng trong lập trình hệ thống và phát triển hệ điều hành, vì các yêu cầu về bố trí bộ nhớ có thể yêu cầu các vùng nhớ bắt đầu ở các địa chỉ cụ thể.

// hàm này là một hàm khởi động cho kernel trong môi trường lập trình hệ thống, thường được sử dụng tron các hệ điều hành như linux
void start_kernel(void){
	/*
	 * Note that this crufty stuff with static and envval
	 * and envbuf is because:
	 *
	 * 1. Frequently, the stack is short, and we don't want to overrun;
	 * 2. Frequently the stack is where we are going to copy the kernel to;
	 * 3. A certain SRM console required the GET_ENV output to stack.
	 *    ??? A comment in the aboot sources indicates that the GET_ENV
	 *    destination must be quadword aligned.  Might this explain the
	 *    behaviour, rather than requiring output to the stack, which
	 *    seems rather far-fetched.
	 */
	static long nbytes; 
	static char envval[256] __attribute__((aligned(8)));
	static unsigned long initrd_start;
	// thông báo khởi động
	srm_printk("Linux/AXP bootp loader for Linux " UTS_RELEASE "\n");
	// kiểm tra các điều kiện
	if (INIT_HWRPB->pagesize != 8192) {
		srm_printk("Expected 8kB pages, got %ldkB\n",
		           INIT_HWRPB->pagesize >> 10);
		return;
	}
	if (INIT_HWRPB->vptb != (unsigned long) VPTB) {
		srm_printk("Expected vptb at %p, got %p\n",
			   VPTB, (void *)INIT_HWRPB->vptb);
		return;
	}
	// khởi động hệ thống
	pal_init();

	/* The initrd must be page-aligned.  See below for the 
	   cause of the magic number 5.  */
	// tính toán địa chỉ initrd
	initrd_start = ((START_ADDR + 5*KERNEL_SIZE + PAGE_SIZE) |
			(PAGE_SIZE-1)) + 1;
#ifdef INITRD_IMAGE_SIZE
	srm_printk("Initrd positioned at %#lx\n", initrd_start);
#endif

	/*
	 * Move the stack to a safe place to ensure it won't be
	 * overwritten by kernel image.
	 */
	// di chuyển ngăn xếp
	move_stack(initrd_start - PAGE_SIZE);
	// lấy biến môi trường
	nbytes = callback_getenv(ENV_BOOTED_OSFLAGS, envval, sizeof(envval));
	if (nbytes < 0 || nbytes >= sizeof(envval)) {
		nbytes = 0;
	}
	envval[nbytes] = '\0';
	// thông báo tải kernel
	srm_printk("Loading the kernel...'%s'\n", envval);

	/* NOTE: *no* callbacks or printouts from here on out!!! */

	/* This is a hack, as some consoles seem to get virtual 20000000 (ie
	 * where the SRM console puts the kernel bootp image) memory
	 * overlapping physical memory where the kernel wants to be put,
	 * which causes real problems when attempting to copy the former to
	 * the latter... :-(
	 *
	 * So, we first move the kernel virtual-to-physical way above where
	 * we physically want the kernel to end up, then copy it from there
	 * to its final resting place... ;-}
	 *
	 * Sigh...  */
// tải kernel vào bộ nhớ
#ifdef INITRD_IMAGE_SIZE
	load(initrd_start, KERNEL_ORIGIN+KERNEL_SIZE, INITRD_IMAGE_SIZE);
#endif
        load(START_ADDR+(4*KERNEL_SIZE), KERNEL_ORIGIN, KERNEL_SIZE);
        load(START_ADDR, START_ADDR+(4*KERNEL_SIZE), KERNEL_SIZE);
// thiết lập vùng bộ nhớ
	memset((char*)ZERO_PGE, 0, PAGE_SIZE);
	strcpy((char*)ZERO_PGE, envval);
	// cập nhật initrd nếu cần
#ifdef INITRD_IMAGE_SIZE
	((long *)(ZERO_PGE+256))[0] = initrd_start;
	((long *)(ZERO_PGE+256))[1] = INITRD_IMAGE_SIZE;
#endif
	// chạy kernel
	runkernel();
}
// Hàm start_kernel này thực hiện nhiều bước quan trọng để khởi động kernel, bao gồm kiểm tra các điều kiện ban đầu, tính toán địa chỉ, di chuyển ngăn xếp, tải kernel vào bộ nhớ, và cuối cùng gọi hàm runkernel để bắt đầu thực thi kernel. Nó cũng xử lý việc lấy các biến môi trường cần thiết cho quá trình khởi động.