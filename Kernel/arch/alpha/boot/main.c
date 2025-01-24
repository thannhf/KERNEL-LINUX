// SPDX-License-Identifier: GPL-2.0
/*
 * arch/alpha/boot/main.c
 *
 * Copyright (C) 1994, 1995 Linus Torvalds
 *
 * This file is the bootloader for the Linux/AXP kernel
 */
#include <linux/kernel.h> //Lệnh tiền xử lý này chèn nội dung của tệp tiêu đề kernel.h từ thư mục linux vào tệp mã nguồn hiện tại trước khi chương trình được biên dịch. Tệp tiêu đề này chứa các khai báo, macro, hàm tiện ích cần thiết cho việc lập trình trong Linux kernel.
#include <linux/slab.h> //tệp tiêu đề này cung cấp các chức năng và macro liên quan đến quản lý bộ nhớ trong kernel linux, bao gồm cấp phát và giải phóng bộ nhớ. một số hàm quan trọng bao gồm kmalloc (cấp phát bộ nhớ) và kfree (giải phóng bộ nhớ).
#include <linux/string.h> //cung cấp các hàm xử lý chuỗi tương tự như các hàm trong thư viện chuẩn C (như strcpu, strlen, strcat,...) nhưng được tối ưu hóa cho sử dụng trong kernel
#include <generated/utsrelease.h> //tệp này được tạo tự động trong quá trình biên dịch kernel và chứa thông tin về phiên bản kernel, bao gồm tên và số phiên bản của kernel đang chạy. thông tin này thường được sử dụng để ghi lại hoặc hiển thị trong nhật ký hệ thống.
#include <linux/mm.h> //cung cấp các hàm và cấu trúc dữ liệu liên quan đến quản lý bộ nhớ trong kernel, như quản lý các vùng bộ nhớ và ánh xạ bộ nhớ.
#include <asm/console.h> //tệp này chứa các định nghĩa và hàm liên quan đến giao tiếp với console (bảng điều khiển), cho phép kernel hoặc mô-đun giao tiếp với người dùng thông qua console.
#include <asm/hwrpb.h> //tệp này chứa các định nghĩa và cấu trúc dữ liệu liên quan đến HWRPB (hardware reduced page table) cho các kiến trúc phần cứng nhất định. đây là phần của cơ chế quản lý bộ nhớ cho một số loại kiến trúc CPU.
#include <linux/stdarg.h> //cung cấp các macro cần thiết để làm việc với danh sách tham số biến (va_list, va_start, va_end,...). điều này cho phép định nghĩa các hàm nhận một số lượng không xác định các tham số đầu vào.
#include "ksize.h" //tệp này có thể là một tệp tiêu đề tùy chỉnh, có thể chứa các định nghĩa và hàm liên quan đến việc xác định kích thước của các đối tượng đã cấp phát trong kernel, hoặc có thể là một phần mở rộng cho các hàm và macro đã có trong các tệp tiêu đề trước đó.

//có thể đại diện cho một kết quả hoặc trạng thái sau khi chuyển đổi ngữ cảnh thông qua PAL (privileged Architecture Library).
//hàm này có thể thực hiện một số thao tác liên quan đến việc chuyển đổi ngữ cảnh giữa các quá trình trong kernel, đặc biệt liên quan đến kiến trúc hệ thống hỗ trợ PAL. thông thường, các lệnh PAL được sử dụng trong các kiến trúc hệ thống đặc thù ví dụ như ALPHA AXP để thực hiện các tác vụ đặc quyền cấp thấp (như quản lý bộ nhớ, ngắt, hoặc chuyển đổi chế độ CPU).
extern unsigned long switch_to_osf_pal(unsigned long nr, struct pcb_struct * pcb_va, struct pcb_struct * pcb_pa, unsigned long *vptb);
struct hwrpb_struct *hwrpb = INIT_HWRPB; //đây là một con trỏ trỏ đến một cấu trúc dữ liệu hwrpb_struct. hwrpb có thể là một cấu trúc bảng thông tin hệ thống phần cứng (hardware restart parameter block), đặc biệt liên quan đến kiến trúc Alpha AXP. INIT_HWRPB: đây có thể là một macro hoặc giá trị khởi tạo được định nghĩa ở nơi khác. nó sẽ gán cho hwrpb địa chỉ khởi tạo của bảng thông tin phần cứng. bảng này chứa các tham số cần thiết về cấu hình hệ thống phần cứng mà kernel cần để thực thi các thao tác liên quan đến quản lý bộ nhớ và cpu. tác dụng: bảng hwrpb thường được sử dụng để lưu trữ thông tin về trạng thái của hệ thống và quá trình khởi động lại. đây là một phần quan trọng trong các hệ thống có kiến trúc đặc thù như Alpha, giúp kernel có thể quản lý phần cứng một cách trực tiếp và hiệu quả.
static struct pcb_struct pcb_va[1]; //pcb_va có thể đại diện cho bản sao ảo của khối điều khiển tiến trình (PCB - Process control block). PCB lưu trữ thông tin về trạng thái của một tiến trình, chẳng hạn như các thanh ghi CPU, con trỏ ngăn xếp, bộ nhớ và thông tin ngữ cảnh khác.biến này có thể được sử dụng để lưu trữ và thao tác trên PCB của tiến trình trong quá trình chuyển đổi ngữ cảnh. nó được sử dụng nội bộ trong file mã nguồn này và có thể liên quan đến các thao tác chuyển đổi ngữ cảnh với các tham số PAL được quản lý bởi kernel.
//Đoạn mã này đang thiết lập một cấu trúc và khai báo một số thành phần liên quan đến việc chuyển đổi ngữ cảnh và quản lý bộ nhớ trong kernel Linux, đặc biệt trên các kiến trúc hệ thống sử dụng lệnh PAL (như Alpha AXP). Nó sử dụng các cấu trúc PCB để lưu trữ trạng thái tiến trình và bảng thông tin phần cứng hwrpb để lưu trữ các thông tin hệ thống cần thiết.

/*
 * Find a physical address of a virtual object..
 *
 * This is easy using the virtual page table address.
 */

static inline void * find_pa(unsigned long *vptb, void *ptr){ //static inline: static chỉ ra rằng hàm find_pa chỉ có thể được sử dụng trong tệp hiện tại, không thể được truy cập từ các tệp khác. inline đề xuất cho trình biên dịch rằng hàm này có thể được chèn trực tiếp vào vị trí gọi hàm để giảm chi phí gọi hàm (có thể tăng hiệu suất trong một số trường hợp).
//void * find_pa(unsigned long *vptb, void *ptr): hàm này nhận vào hai tham số: unsigned long *vptb con trỏ tới bảng trang ảo (virtual page table). và void *ptr: con trỏ tới địa chỉ cần tìm trang vật lý tương ứng.
	unsigned long address = (unsigned long) ptr; //chuyển đổi con trỏ ptr sang kiểu unsigned long để xử lý các phép toán bit.
	unsigned long result; //khai báo một biến result để lưu trữ địa chỉ vật lý tương ứng.

	result = vptb[address >> 13]; //lấy giá trị từ bảng trang ảo tại địa chỉ số là địa chỉ ptr được dịch chuyển sang phải 13 bit. điều này tương đương với việc chia địa chỉ cho 8192(2^13), cho phép truy cập vào phần tử tương ứng trong bảng trang ảo.
	result >>= 32; //dịch phải result 32bit để lấy phần cao của địa chỉ vật lý.
	result <<= 13; //dịch trái result 13 bit để khôi phục lại địa chỉ vật lý ở phần thấp.
	result |= address & 0x1fff; //kết hợp phần thấp của địa chỉ addres với result. 0x1fff (8191) là mặt nạ để giữ lại 13 bit thấp nhất của địa chỉ, tức là phần bù vào trong trang.
	return (void *) result; //trả về con trỏ result được chuyển đổi trở lại kiểu void *, biểu thị địa chỉ vật lý tương ứng với địa chỉ ảo đã cung cấp.
	//Hàm find_pa này thực hiện việc tìm kiếm địa chỉ vật lý tương ứng với một địa chỉ ảo trong ngữ cảnh của một hệ thống sử dụng bảng trang ảo.
	// Mục đích chính:
		// Chuyển đổi địa chỉ ảo thành địa chỉ vật lý. Trong các hệ thống máy tính, đặc biệt là trong kernel Linux, các tiến trình thường hoạt động với địa chỉ ảo để tăng tính bảo mật và quản lý bộ nhớ. Hàm này giúp tìm kiếm địa chỉ vật lý mà địa chỉ ảo tương ứng sẽ được ánh xạ tới trong bảng trang ảo.
	//Áp dụng:
		//Hàm này có thể được sử dụng trong các tình huống mà cần truy cập trực tiếp đến bộ nhớ vật lý, như khi thực hiện các thao tác liên quan đến quản lý bộ nhớ, bảo vệ bộ nhớ hoặc trong các hoạt động liên quan đến phần cứng.
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

#define VPTB	((unsigned long *) 0x200000000)//VPTB là một macro đại diện cho con trỏ kiểu unsigned long *, trỏ tới địa chỉ 0x200000000 (một địa chỉ bộ nhớ cố định). đây có thể là địa chỉ của bảng trang cao cập nhất (virtual page table) trong hệ thống quản lý bộ nhớ ảo. trong hệ thống quản lý bộ nhớ ảo (như trên kiến trúc Alpha AXP), bảng trang ảo là nơi lưu trữ các ánh xạ giữa địa chỉ ảo và địa chỉ vật lý. địa chỉ 0x200000000 được sử dụng như địa chỉ khởi điểm cho bảng trang này.
#define L1	((unsigned long *) 0x200802000) //L1 là một macro trỏ tới địa chỉ 0x200802000, có thể đại diện cho bảng trang cấp 1 (level 1 page table), một cấp trong cấu trúc phân cấp quản lý bộ nhớ ảo. trong cấu trúc phân cấp của các bảng trang (multi-level page tables), bảng trang cấp 1 (L1) lưu trữ các mục ánh xạ tới các trang cấp thấp hơn hoặc trực tiếp ánh xạ tới bộ nhớ vật lý.
//VPTB và L1 có vai trò quan trọng trong quản lý bộ nhớ ảo:
// Các macro này thường được sử dụng để truy cập hoặc thao tác trực tiếp trên các bảng trang khi kernel cần chuyển đổi địa chỉ ảo thành địa chỉ vật lý, quản lý bộ nhớ hoặc khi thực hiện chuyển đổi ngữ cảnh (context switching) của các tiến trình.
// Chúng cũng có thể liên quan đến các cơ chế paging trong hệ điều hành, nơi mà kernel phải duy trì và cập nhật bảng trang khi quản lý bộ nhớ ảo cho các tiến trình.

//hàm này có nhiệm vụ chuyển hệ thống sang sử dụng OSF PAL code, một mã cấp thấp (privileged architecture library - PAL) trong kiến trúc alpha, sau đó khởi tạo các cấu trúc cần thiết cho cpu và thực hiện cập nhật bộ nhớ.
void pal_init(void){
	unsigned long i, rev;
	struct percpu_struct * percpu;
	struct pcb_struct * pcb_pa;

	/* Create the dummy PCB.  */
	//tạo pcb giả:
	pcb_va->ksp = 0;
	pcb_va->usp = 0;
	pcb_va->ptbr = L1[1] >> 32;
	pcb_va->asn = 0;
	pcb_va->pcc = 0;
	pcb_va->unique = 0;
	pcb_va->flags = 1;
	pcb_va->res1 = 0;
	pcb_va->res2 = 0;
	pcb_pa = find_pa(VPTB, pcb_va); // hàm này tìm địa chỉ vật lý (physical address) của PCB bằng cách sử dụng bảng trang ảo (virtual page table).

	/*
	 * a0 = 2 (OSF)
	 * a1 = return address, but we give the asm the vaddr of the PCB
	 * a2 = physical addr of PCB
	 * a3 = new virtual page table pointer
	 * a4 = KSP (but the asm sets it)
	 */
	srm_printk("Switching to OSF PAL-code .. "); //ghi một thông báo cho người dùng biết hệ thống đang chuyển sang sử dụng OSF PAL code.

	//a0 = 2: tham số này yêu cầu chuyển sang OSF PAL code.
	//pcb_va: địa chỉ ảo của PCB.
	//pcb_pa: địa chỉ vật lý của PCB.
	//VPTB: bảng trang ảo được truyền vào để hệ thống biết cách ánh xạ địa chỉ ảo thành vật lý.
	//check error: nếu return value i != 0, hàm sẽ ghi thông báo lỗi và dừng hệ thống bằng cách gọi hàm __halt.
	i = switch_to_osf_pal(2, pcb_va, pcb_pa, VPTB);
	if (i) {
		srm_printk("failed, code %ld\n", i);
		__halt();
	}
	// khởi tạo cấu trúc dữ liệu cho mỗi CPU:
	//INIT_HWRPB: một cấu trúc dữ liệu chứa thông tin về phần cứng (hardware restart parameter block - HWRPB). lệnh này lấy con trỏ tới cấu trúc per-cpu bằng cách sử dụng offset từ HWRPB.
	percpu = (struct percpu_struct *)
		(INIT_HWRPB->processor_offset + (unsigned long) INIT_HWRPB);
	//lưu trữ phiên bản PAL code hiện tại vào cấu trúc per-cpu và in ra thông báo phiên bản pal code dưới dạng mã hexadecimal.
	rev = percpu->pal_revision = percpu->palcode_avail[2];

	srm_printk("Ok (rev %lx)\n", rev);
	//hàm này thực hiện việc làm mới tất cả các bản dịch từ địa chỉ ảo sang địa chỉ vật lý. điều này đảm bảo rằng sau khi thực hiện các thay đổi với bảng trang, không còn bộ nhớ đệm nào giữ các ánh xạ cũ. SMP (symmetric multiprocessing): nếu hệ thống đang chạy trong chế độ SMP (với nhiều CPU), hành động này sẽ áp dụng cho toàn bộ các CPU để đảm bảo tính nhất quán.
	tbia(); /* do it directly in case we are SMP */
	//Hàm pal_init thực hiện quá trình chuyển đổi kernel sang OSF PAL code, một loại mã cấp thấp cung cấp các tính năng cần thiết cho kiến trúc Alpha.
	// Hàm này thiết lập cấu trúc PCB, thực hiện ánh xạ địa chỉ ảo và vật lý, và đảm bảo rằng tất cả các CPU trong hệ thống SMP nhận các cập nhật bảng trang bộ nhớ.
	// Đây là một phần quan trọng trong quá trình khởi động và quản lý bộ nhớ ở mức hệ thống.
}

// hàm openboot được dùng để mở thiết bị khởi động (boot device) trong quá trình khởi động hệ thống.
static inline long openboot(void){
	char bootdev[256]; //đây là một mảng ký tự có độ dài 256, được sử dụng để lưu trữ tên của thiết bị khởi động (boot device). trong quá trình khởi động, hệ thống cần biết thiết bị nào sẽ được sử dụng để khởi động.
	long result;

	result = callback_getenv(ENV_BOOTED_DEV, bootdev, 255); //đây là một hàm callback dùng để lấy giá trị của một biến môi trường. trong trường hợp này, nó lấy giá trị của biến môi trường ENV_BOOTED_DEV, là một biến chứa thông tin về thiết bị đã khởi động hệ thống.

	//với kích thước tối đa 255 được lưu trữ trong bootdev.
	if (result < 0)
	//nếu hàm callback_getenv trả về giá trị nhỏ hơn 0, điều này có nghĩa là việc lấy tên thiết bị khởi động thất bại. khi đó, hàm sẽ trả về mã lỗi
		return result;
	return callback_open(bootdev, result & 255);
	//openboot là một hàm dùng trong quá trình khởi động, khi hệ thống cần biết thiết bị khởi động nào được sử dụng và tiến hành mở thiết bị đó để tiếp tục quá trình khởi động hệ điều hành. Hàm này hoạt động ở mức rất thấp, liên quan đến việc quản lý phần cứng trong quá trình khởi động
}

//hàm này dùng để đóng một thiết bị đã được mở trước đó.
static inline long close(long dev){
	//dev: biến dev là một số nguyên dài (kiểu long) đại diện cho thiết bị (hoặc tài nguyên) đã được mở trước đó. thông thường, nó là một mô tả thiết bị (device descriptor), tương tự như một file descriptor trong hệ điều hành.
	return callback_close(dev); //đây là một hàm callback dùng để đóng thiết bị đã được mở trước đó. nó thực hiện nhiệm vụ giải phóng tài nguyên liên quan đến thiết bị dev được truyền vào. nếu việc đóng thiết bị thành công, hàm này sẽ trả về một giá trị nhất định (thường là 0 cho thành công), hoặc nếu thất bại, nó sẽ trả về mã lỗi âm.
	//Hàm close được dùng để đóng một thiết bị đã được mở sau khi không cần sử dụng nữa, giải phóng tài nguyên liên quan. Đây là một bước quan trọng để đảm bảo hệ thống không tiêu tốn tài nguyên một cách không cần thiết. Nó giúp quản lý tốt hơn việc sử dụng tài nguyên phần cứng trong kernel, tránh tình trạng "rò rỉ tài nguyên" (resource leak).
}

//hàm load này trong đoạn mã này có chức năng nạp một lượng dữ liệu từ thiết bị khởi động vào một địa chỉ bộ nhớ cụ thể. đây là quá trình quan trọng trong việc khởi động hệ thống, khi kernel hoặc các phần mềm khởi động cần được tải vào bộ nhớ để tiếp tục hoạt động.
static inline long load(long dev, unsigned long addr, unsigned long count){
	char bootfile[256]; //đây là một mảng ký tự chứa tên của file khởi động boot file, được lấy từ biến môi trường ENV_BOOTED_FILE.
	extern char _end; //biến extern này đại diện cho vị trí kết thúc của phân đoạn bộ nhớ chứa chương trình boot hiện tại. được khai báo bên ngoài và sử dụng để tính toán kích thước bộ nhớ đã được sử dụng cho chương trình khởi động.
	long result, boot_size = &_end - (char *) BOOT_ADDR; //BOOT_ADDR đại diện cho địa chỉ bắt đầu của chương trình khởi động. boot_size là kích thước của phân đoạn bộ nhớ chứa chương trình khởi động, được tính bằng cách lấy chênh lệch giữa địa chỉ kết thúc &_end và địa chỉ bắt đầu BOOT_ADDR.

	result = callback_getenv(ENV_BOOTED_FILE, bootfile, 255); //hàm này lấy thông tin từ biến môi trường ENV_BOOTED_FILE và lưu vào bootfile. nếu không lấy được tên file, hàm sẽ trả về mã lỗi.
	if (result < 0)
		return result;
	//dòng lệnh này đảm bảo rằng độ dài của tên file được giới hạn ở 255 ký tự và thêm ký tự null để đánh dấu chuỗi kết thúc chuỗi.
	result &= 255;
	bootfile[result] = '\0';
	//nếu tên file khởi động không rỗng, hàm sẽ in ra thông báo rằng chức năng xử lý file khởi động chưa được hiện thực. (dù có file chỉ định).
	if (result)
		srm_printk("Boot file specification (%s) not implemented\n",
		       bootfile);
	//callback_read: hàm này đọc dữ liệu từ thiết bị dev và lưu vào địa chỉ addr. số lượng dữ liệu đọc được là count khối, với mỗi khối có kích thước boot_size / 512 + 1.
	// boot_size / 512 tính toán số khối sector cần đọc, mỗi khối có kích thước 512 byte.
	return callback_read(dev, count, (void *)addr, boot_size/512 + 1);
	//Hàm load được dùng để tải dữ liệu từ thiết bị khởi động (như ổ đĩa) vào bộ nhớ. Đây là một phần quan trọng trong quá trình khởi động hệ thống, đặc biệt là khi kernel hoặc các phần mềm khác cần được nạp vào bộ nhớ trước khi tiếp tục xử lý.
}

/*
 * Start the kernel.
 */
//hàm runkernel sử dụng inline assembly để chuyển điều khiển từ đoạn mã hiện tại sang địa chỉ bộ nhớ nơi kernel (hạt nhân hệ điều hành) đã được nạp và chuẩn bị chạy. cụ thể, nó thiết lập các thanh ghi cpu và chuyển quyền điều khiển trực tiếp sang kernel.
static void runkernel(void){
	__asm__ __volatile__( //từ khóa này cho phép chèn mã lắp ráp (assembly) trực tiếp vào chương trình C. từ khóa volatile đảm bảo rằng trình biên dịch không tối ưu hóa mã này, vì việc tối ưu hóa có thể gây lỗi trong việc chuyển quyền điều khiển.
		"bis %1,%1,$30\n\t" //bis là lệnh bitwise OR trong kiến trúc Alpha (có thể hiểu như phép gán). nó gán giá trị của %1 (được gán là PAGE_SIZE + INIT_STACK) vào thanh ghi $30, thanh ghi này thường được sử dụng như stack pointer (con trỏ ngăn xếp).
		"bis %0,%0,$26\n\t" //lệnh này gán giá trị của %0 (được gán là START_ADDR) vào thanh ghi $26, thanh ghi này thường chứa return address(địa chỉ trở về). trong trường hợp này, $26 sẽ chứa địa chỉ bắt đầu của kernel.
		"ret ($26)" //lệnh ret thực hiện việc chuyển quyền điều khiển tới địa chỉ được lưu trong $26, tức là START_ADDR, điều này nghĩa là chương trình hiện tại sẽ dừng lại, và kernel sẽ bắt đầu thực thi từ địa chỉ này.
		: /* no outputs: it doesn't even return */
		//đây là các tham số đầu vào cho các lệnh assembly.
		//START_ADDR là địa chỉ nơi kernel được nạp vào và sẽ bắt đầu chạy.
		//PAGE_SIZE + INIT_STACK là giá trị được tính toán để xác định vị trí đỉnh của ngăn xếp cho kernel.
		: "r" (START_ADDR),
		"r" (PAGE_SIZE + INIT_STACK)
	);
	//Hàm runkernel là bước cuối cùng trong quá trình khởi động hệ điều hành, nơi nó chuyển quyền điều khiển sang kernel để kernel bắt đầu chạy.
	// START_ADDR chứa địa chỉ bộ nhớ của kernel, và PAGE_SIZE + INIT_STACK xác định địa chỉ đỉnh ngăn xếp cho kernel. Khi gọi hàm này, CPU sẽ bắt đầu thực thi từ kernel, và hệ thống sẽ chuyển từ môi trường khởi động sang chế độ hạt nhân.
}
//hàm start_kernel trong đoạn mã này là một phần của quá trình khởi động hệ điều hành linux trên kiến trúc alpha(axp). nó thực hiện việc khởi động hạt nhân hệ điều hành (kernel) từ một thiết bị khởi động (boot device), sau đó tải hạt nhân vào bộ nhớ và chuyển quyền điều khiển sang kernel để nó bắt đầu hoạt động.
void start_kernel(void){
	//i, dev: biến kiểu long, dùng để lưu giá trị cho nhiều thao tác khác nhau như kết quả của các hàm gọi đến thiết bị khởi động hoặc giá trị kiểm tra kích thước kernel.
	long i;
	long dev;
	int nbytes; //kiểu int, lưu số byte đã nhận được từ biến môi trường
	char envval[256]; //mảng ký tự dùng để lưu giá trị của biến môi trường.

	srm_printk("Linux/AXP bootloader for Linux " UTS_RELEASE "\n"); //hàm in thông tin khởi động. chuỗi in ra gồm phiên bản của linux thông qua biến UTS_RELEASE (phiên bản hệ điều hành).
	//kiểm tra nếu kích thước trang bộ nhớ không phải là 8192 byte (8kb), thì in ra thông báo lỗi và dừng quá trình khởi động. pagesize >> 10 dịch bit để chuyển đổi từ byte sang kilobyte (kb).
	if (INIT_HWRPB->pagesize != 8192) {
		srm_printk("Expected 8kB pages, got %ldkB\n", INIT_HWRPB->pagesize >> 10);
		return;
	}
	pal_init(); //gọi hàm khởi tạo PAL-code (privileged architecture library), có nhiệm vụ khởi động phần mềm PAL đặc biệt để điều khiển CPU.
	//gọi hàm để mở thiết bị khởi động. nếu kết quả trả về âm, nghĩa là không thể mở được thiết bị khởi động, in ra thông báo lỗi và dừng quá trình khởi động. sau khi mở thành công, giá trị của dev được giới hạn trong phạm vi 32 bit.
	dev = openboot();
	if (dev < 0) {
		srm_printk("Unable to open boot device: %016lx\n", dev);
		return;
	}
	dev &= 0xffffffff;
	//tải kernel từ thiết bị khởi động.
	//tải kernel từ thiết bị khởi động vào địa chỉ bắt đầu START_ADDR với kích thước là KERNEL_SIZE. sau khi tải xong, thiết bị được đóng lại bằng close(dev). nếu kích thước kernel tải được không khớp với KERNEL_SIZE, thì in ra lỗi và dừng quá trình.
	srm_printk("Loading vmlinux ...");
	i = load(dev, START_ADDR, KERNEL_SIZE);
	close(dev);
	if (i != KERNEL_SIZE) {
		srm_printk("Failed (%lx)\n", i);
		return;
	}
	//lấy cờ hệ điều hành từ biến môi trường
	//callback_getenv: lấy biến môi trường ENV_BOOTED_OSFLAGS (cờ hệ điều hành được khởi động) và lưu kết quả vào envval. nếu có lỗi xảy ra (kết quả âm), thì số byte nhận được đặt thành 0. sau đó kết thúc chuỗi bằng ký tự null ('\0') và sao chép chuỗi vào địa chỉ ZERO_PGE (một trang bộ nhớ đã được khởi tạo trước).
	nbytes = callback_getenv(ENV_BOOTED_OSFLAGS, envval, sizeof(envval));
	if (nbytes < 0) {
		nbytes = 0;
	}
	envval[nbytes] = '\0';
	strcpy((char*)ZERO_PGE, envval);

	srm_printk(" Ok\nNow booting the kernel\n"); //in ra thông báo kernel đã được tải thành công và chuẩn bị khởi động kernel.
	runkernel(); //chuyển quyền điều khiển từ bootloader sang kernel. từ thời điểm này, kernel sẽ bắt đầu chạy.
	//vòng lặp dừng hệ thống.
	//vòng lặp không làm gì và chạy trong một phạm vi lớn (giá trị 0x100000000). sau đó, __halt() được gọi để dừng hệ thống, trong trường hợp nếu kernel không khởi động được.
	for (i = 0 ; i < 0x100000000 ; i++)
		/* nothing */;
	__halt();
}
//Hàm này thực hiện quá trình khởi động kernel của hệ điều hành Linux trên kiến trúc Alpha:
// Kiểm tra và khởi tạo các cấu hình cần thiết.
// Mở thiết bị khởi động và tải kernel từ đó.
// Lấy các tham số từ biến môi trường.
// Cuối cùng, chuyển điều khiển sang kernel để kernel bắt đầu hoạt động.
// Nếu có lỗi xảy ra trong quá trình này, hàm sẽ in ra thông báo lỗi và dừng quá trình khởi động.
