#
# arch/alpha/boot/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1994 by Linus Torvalds
#

#đoạn mã này là một phần của makefile trong quá trình biên dịch hạt nhân linux. nó định nghĩa một số chương trình và mục tiêu quan trọng, liên quan đến việc xây dựng kernel và các công cụ liên quan.
hostprogs	:= tools/mkbb tools/objstrip 
targets		:= vmlinux.gz vmlinux \
		   vmlinux.nh tools/lxboot tools/bootlx tools/bootph \
		   tools/bootpzh bootloader bootpheader bootpzheader #biến này định nghĩa các tệp hoặc mục tiêu cần được tạo ra trong quá trình biên dịch. đây là những mục tiêu đầu ra của quá trình biên dịch kernel và các công cụ hỗ trợ khác. vmlinux.gz: đây là kernel đã được nén. vmlinux là phiên bản kernel chưa nén, còn .gz chỉ ra rằng kernel này đã được nén lại bằng gzip. làm giảm kích thước của kernel và thuận tiện cho quá trình tải trong giai đoạn khởi động. vmlinux: đây là kernel chưa nén được biên dịch từ mã nguồn linux. kernel này chứa toàn bộ code và dữ liệu cần thiết để kernel có thể chạy trên hệ thống mục tiêu. vmlinux.nh: phiên bản kernel không chứa phần header. việc này có thể dùng trong một số tình huống đặc biệt, ví dụ như khi chỉ cần kernel mà không cần các thông tin tiêu đề (header) thêm vào. tools/lxboot: đây là một công cụ hỗ trợ khởi động kernel, có thể giúp nạp (load) kernel từ một thiết bị hoặc bộ nhớ vào bộ nhớ ram để chạy. tools/bootlx, tools/bootph, tools/bootpzh: các công cụ khác hỗ trợ khởi động kernel, tùy thuộc vào loai bootloader (trình nạp khởi động) hoặc cấu trúc hệ thống. tên gọi gợi ý rằng chúng có thể hỗ trợ các định dạng khác nhau hoặc môi trường khởi động khác nhau. bootloader: đây là trình nạp khởi động (bootloader) có nhiệm vụ nạp kernel vào bộ nhớ và chuyển quyền điều khiển cho kernel. bootpheader, bootpzheader: các header liên quan đến quá trình khởi động (boot). các tệp header này có thể chứa thông tin về cấu hình khởi động, như địa chỉ bộ nhớ hoặc tham số khởi động, và chúng được sử dụng trong quá trình xây dựng bootloader hoặc kernel.
OBJSTRIP	:= $(obj)/tools/objstrip #đây là biến chỉ đường dẫn đến chương trình objstrip đã được biên dịch. đường dẫn này sẽ chỉ đến công cụ objstrip trong thư mục tools của quá trình biên dịch kernel. chương trình này sẽ được dùng để xử lý các tệp nhị phân trong quá trình biên dịch kernel, giúp loại bỏ các thông tin không cần thiết như biểu tượng và bảng gỡ lỗi để giảm kích thước tệp.
#hostprogs: Định nghĩa các công cụ hỗ trợ (chạy trên máy phát triển) cần biên dịch trước khi thực hiện quá trình biên dịch kernel.
#mkbb: Công cụ có thể hỗ trợ tạo boot block.
#objstrip: Công cụ giảm kích thước các tệp nhị phân bằng cách loại bỏ các thông tin không cần thiết.
#targets: Liệt kê các mục tiêu cần được xây dựng trong quá trình biên dịch kernel, bao gồm kernel chính (vmlinux, vmlinux.gz) và các công cụ hỗ trợ khởi động kernel (như lxboot, bootlx, bootph).
#OBJSTRIP: Định nghĩa đường dẫn đến công cụ objstrip, giúp loại bỏ thông tin thừa trong các tệp nhị phân để giảm kích thước kernel hoặc các tệp liên quan.
#Các phần này của Makefile đóng vai trò quan trọng trong quá trình biên dịch kernel và các công cụ khởi động kernel, đảm bảo kernel có thể được nén, tối ưu hóa kích thước, và có thể khởi động trên các hệ thống mục tiêu.

#đoạn mã này trong makefile của linux kernel liên quan đến việc thiết lập các CFLAGS (tùy chọn biên dịch C) cho quá trình biên dịch các chương trình hoặc tệp tin cụ thể trong kernel. cụ thể nó đề cập đến các cờ dùng để biên dịch cho host (máy chủ biên dịch) và quá trình khởi động (boot process).
KBUILD_HOSTCFLAGS := -Wall -I$(objtree)/usr/include #KBUILD_HOSTCFLAGS: đây là các tùy chọn CFLAGS được áp dụng cho các chương trình được biên dịch trên máy chủ phát triển (host). các chương trình này không phải là kernel nhưng có thể cần cho việc xây dựng kernel, ví dụ như các công cụ hỗ trợ quá trình biên dịch. 
#-wall: tùy chọn -wall là một cờ phổ biến trong gcc, yêu cầu trình biên dịch đưa ra tất cả các cảnh bảo (warnings). điều này giúp phát hiện các lỗi tiềm tàng trong mã nguồn sớm hơn và đảm bảo chất lượng code.
#-I$(objtree)/usr/include: tùy chọn này thêm đường dẫn $(objtree)/usr/include vào danh sách các thư mục để tìm kiếm các tệp header trong quá trình biên dịch.
#(objtree): biến này chỉ đến thư mục nơi các tệp đối tượng (object files) được lưu trữ trong quá trình biên dịch kernel.
#/usr/include: đây là thư mục chuẩn chứa các tệp header cho chương trình. trong trường hợp này, trình biên dịch sẽ tìm kiếm các header từ thư mục này trong cây đối tượng (objtree).
#KBUILD_HOSTCFLAGS đảm bảo rằng các chương trình chạy trên máy chủ phát triển được biên dịch với tất cả cảnh bảo và sử dụng đúng tệp header từ thư mục usr/include trong cây đối tượng.
BOOTCFLAGS	+= -I$(objtree)/$(obj) -I$(srctree)/$(obj) #đây là các tùy chọn CFLAGS áp dụng cho quá trình biên dịch các tệp liên quan đến quá trình khởi động (boot). ví dụ, khi biên dịch bootloader hoặc các tệp khởi động khác của kernel, các cờ này sẽ được thêm vào.
#-I$(objtree)/$(obj): tùy chọn này thêm đường dẫn $(objtree)/$(obj) vào danh sách các thư mục tìm kiếm cho tệp header.
#$(objtree): như đã giải thích, biến này trỏ đến thư mục nơi lưu trữ các tệp đối tượng (object files).
#$(obj): biến này chỉ đến thư mục chứa các tệp hiện tại trong quá tình biên dịch kernel.
#-I$(srctree)/$(obj): tương tự, tùy chọn này thêm đường dẫn $(srctree)/$(obj) vòa danh sách tìm kiếm các tệp header.
#$(srctree): biến này trỏ đến thư mục gốc của mã nguồn kernel. đây là nơi chứa các tệp nguồn chưa được biên dịch.
#BOOTCFLAGS: đảm bảo rằng quá trình biên dịch liên quan đến boot (khởi động) sẽ sử dụng các tệp header từ cả cây đối tượng (objtree) và cây mã nguồn (srctree). điều này giúp quá trình biên dịch tìm kiếm đúng các tệp header cần thiết cho việc biên dịch các thành phần khởi động như bootloader.
#KBUILD_HOSTCFLAGS: Thiết lập các tùy chọn biên dịch C cho các chương trình chạy trên máy chủ phát triển, yêu cầu báo cáo tất cả các cảnh báo và chỉ rõ thư mục chứa tệp header.
#BOOTCFLAGS: Thiết lập các tùy chọn biên dịch cho các thành phần khởi động của kernel, chỉ định đường dẫn tệp header từ cả cây đối tượng và cây mã nguồn để đảm bảo việc biên dịch chính xác các thành phần liên quan đến bootloader và quá trình khởi động kernel.
#Cả hai biến này giúp đảm bảo rằng việc biên dịch kernel diễn ra suôn sẻ và sử dụng đúng tệp header và cờ biên dịch phù hợp cho các thành phần khác nhau.

# SRM bootable image.  Copy to offset 512 of a partition.
#phân tích đoạn mã này giúp ta hiểu cách mà hệ thống xây dựng tệp bootimage trong quá trình biên dịch kernel linux.
$(obj)/bootimage: $(addprefix $(obj)/tools/,mkbb lxboot bootlx) $(obj)/vmlinux.nh #mục tiêu: $(obj)/bootimage: là mục tiêu (target) của quy tắc này. tệp bootimage là một tệp kết hợp, bao gồm bootloader và kernel đã được biên dịch. nó là tệp nhị phân dùng để khởi động hệ điều hành. với các danh sách phụ thuộc:
#$(addprefix $(obj)/tools/, mkbb lxboot bootlx): đây là các công cụ cần thiết để tạo bootimage. chúng được biên dịch từ các tệp mã nguồn trong thư mục tools và bao gồm:
#mkbb: một công cụ dùng để đánh dấu (boot block) trên tệp bootimage.
#lxboot và bootlx: các thành phần của bootloader.
#$(obj)/vmlinux.nh: đây là tệp kernel nhị phân không nén (vmlinux) đã được biên dịch.nó chứa kernel đã sẵn sàng để tải khi hệ thống khởi động.
#các lệnh dưới được thực hiện khi bootimage được tạo.
#cat: lệnh này ghép nối nội dung của ba tệp lxboot, bootlx, và vmlinux.nh lại với nhau:
#lxboot và bootlx là các thành phần của bootloader.
#vmlinux.nh là tệp nhị phân của kernel.
#$@: là mục tiêu hiện tại, tức là $(obj)/bootimage. toàn bộ nội dung ghép nối được ghi vào tệp bootimage.
#sau khi bootimage được tạo, công cụ mkbb được sử dụng để đánh dấu boot block(vùng khởi động) trên tệp bootimage.
#$@ đây là tệp bootimage vừa tạo.
#$(obj)/tools/lxboot: đây là thành phần bootloader được sử dụng để cấu hình boot block.
#sau khi hoàn tất, một thông báo "bootimage ...is ready" được in ra, cho biết rằng quá trình tạo bootimage đã thành công.
	( cat $(obj)/tools/lxboot $(obj)/tools/bootlx $(obj)/vmlinux.nh ) > $@ 
	$(obj)/tools/mkbb $@ $(obj)/tools/lxboot
	@echo '  Bootimage $@ is ready' 
#Quy tắc này trong Makefile chịu trách nhiệm tạo ra tệp bootimage. Tệp này kết hợp các thành phần bootloader (lxboot, bootlx) và kernel không nén (vmlinux.nh). Quá trình này bao gồm việc ghép nối các tệp nhị phân lại với nhau và đánh dấu boot block để hệ điều hành có thể khởi động từ tệp bootimage này.
#Cụ thể:
#Ghép nối bootloader và kernel.
#Sử dụng mkbb để cấu hình boot block.
#Xuất ra thông báo khi tệp bootimage đã sẵn sàng.
#################################################################################
# BOOTP bootable image.  Define INITRD during make to append initrd image.
# $(obj)/bootpfile: đây là tệp mục tiêu (target) cần được tạo ra. nó là một tệp nhị phân chứa kernel và các thành phần khác để khởi động hệ điều hành linux.
#danh sách phụ thuộc.
#$(obj)/bootpfile: đây là tệp mục tiêu (target) cần được tạo ra. nó là một tệp nhị phân chứa kernel và các thành phần khác để khởi động hệ điều hành linux.
#danh sách phụ thuộc:
#$(obj)/tools/bootph: đây là tệp bootloader được biên dịch từ mã nguồn trong thư mục tools. bootph là thành phần hỗ trợ quá trình khởi động kernel.
#$(obj)/vmlinux.nh: đây là kernel đã được biên dịch dưới dạng nhị phân không nén, tương tự như trong quy tắc bootimage.
#lệnh thực hiện: 
#cat: ghép nối nội dung của hai tệp bootph và vmlinux.nh.
#bootph: thành phần bootloader, giúp khởi động kernel.
#vmlinux.nh: kernel không nén đã được biên dịch.
#$@: là tệp bootpfile sẽ được tạo ra (tên mục tiêu).
#lệnh này tạo ra tệp bootpfile chứa cả bootloader và kernel không nén.
#điều kiện: ifdef INITRD:
#ifdef INITRD: đây là điều kiện kiểm tra sự tồn tại của biến INITRD. nếu biến INITRD được định nghĩa, tức là đang sử dụng initial ram disk (RAM ảo để khởi động hệ thống).
#cat $(INITRD) >> $@:
#cat: ghép nối tệp INITRD (initial ram disk) vào cuối tệp bootpfile.
#$@: là tệp bootpfile đang được tạo. tệp INITRD sẽ được thêm vào sau khi ghép nối bootloader và kernel.
#điều này có nghĩa rằng nếu có INITRD, nó sẽ được tích hợp vào bootpfile để sử dụng khi khởi động hệ thống.
#Quy tắc này trong Makefile tạo ra tệp bootpfile, chứa kernel và bootloader. Nếu hệ thống sử dụng INITRD, nó cũng sẽ được tích hợp vào bootpfile. Tệp bootpfile này sẽ được sử dụng để khởi động kernel cùng với Initial RAM Disk nếu có.
#Cụ thể:
#Ghép nối bootloader (bootph) và kernel (vmlinux.nh).
#Nếu có INITRD, thêm nó vào cuối tệp bootpfile.
#Kết quả là tệp bootpfile chứa tất cả các thành phần cần thiết để khởi động hệ thống Linux.
$(obj)/bootpfile: $(obj)/tools/bootph $(obj)/vmlinux.nh
	cat $(obj)/tools/bootph $(obj)/vmlinux.nh > $@
ifdef INITRD
	cat $(INITRD) >> $@
endif
##########################################################################################################
# Compressed kernel BOOTP bootable image.
# Define INITRD during make to append initrd image.
#quy tắc makefile này tương tự với quy tắc trước đó, nhưng ở đây, mục tieu là tạo tệp bootpzfile, bao gồm kernel đã được nén và một phần bootloader khác.
#mục tiêu:
#$(obj)/bootpzfile: đây là tệp mục tiêu (target) cần được tạo. nó là một tệp nhị phân chứa bootloader và kernel đã được nén, có thể bao gồm cả INITRD nếu có.
#danh sách phụ thuộc:
#$(obj)/tools/bootpzh: đây là một phần bootloader khác, được tạo ra trong thư mục tools. bootpzh có chức năng tương tự bootph, nhưng có thể tương thích với tệp vmlinux.nh.gz (kernel đã nén).
#$(obj)/vmlinux.nh.gz: đây là phiên bản kernel đã được nén. việc nén giúp giảm dung lượng kernel, thuận tiện hơn trong quá trình tải kernel từ thiết bị khởi động vào bộ nhớ hệ thống.
#lệnh thực hiện: cat $(obj)/tools/bootpzh $(obj)/vmlinux.nh.gz > $@
#cat: ghép nối nội dung của hai tệp bootpzh và vmlinux.nh.gz.
#bootpzh: thành phần bootloader, hỗ trợ quá trình khởi động kernel.
#vmlinux.nh.gz: kernel đã được nén.
#$@ : là tệp bootpzfile sẽ được tạo ra (tên mục tiêu).
#lệnh này tạo ra tệp bootpzfile, chứa bootloader và kernel đã được nén.
$(obj)/bootpzfile: $(obj)/tools/bootpzh $(obj)/vmlinux.nh.gz
	cat $(obj)/tools/bootpzh $(obj)/vmlinux.nh.gz > $@
#điều kiện ifdef INITRD:
#ifdef INITRD: kiểm tra sự tồn tại của biến INITRD. nếu có INITRD, nó sẽ được thêm vào cuối tệp bootpzfile.
#cat $(INITRD) >> $@
#cat: ghép nối tệp INITRD vào cuối tệp bootpzfile.
#$@: tệp bootpzfile sẽ chứa thêm INITRD nếu có.
#Quy tắc này tạo ra tệp bootpzfile, chứa kernel đã nén và bootloader, và nếu có INITRD, nó cũng được thêm vào cuối tệp. bootpzfile này sẽ được sử dụng trong quá trình khởi động hệ điều hành, với kernel đã nén và các tệp liên quan đến quá trình khởi động
#sự khác biệt so với quy tắc bootpfile: bootpzfile sử dụng kernel đã nén (vmlinux.nh.gz), trong khi bootpfile sử dụng kernel không nén (vmlinux.nh).
ifdef INITRD
	cat $(INITRD) >> $@
endif
##############################################################################################3
# Compressed kernel image
#quy tắc trong makefile này xác định cách tạo ra tệp vmlinux.gz, là phiên bản nén của kernel vmlinux.
#$(obj)/vmlinux.gz: đây là tệp mục tiêu cần được tạo. tệp này là phiên bản nén của tệp kernel vmlinux.
#$(obj)/vmlinux: đây là tệp nguồn sẽ được nén để tạo ra vmlinux.gz. nếu vmlinux thay đổi, tệp vmlinux.gz sẽ được cập nhật.
#FORCE: đây là một cách để đảm bảo rằng quy tắc sẽ được thực thi ngay cả khi không có sự thay đổi ở các phụ thuộc. điều này có nghĩa là nếu bạn muốn luôn tạo lại vmlinux.gz, bạn có thể sử dụng FORCE để bỏ qua các kiểm tra thông thường.
#lệnh thực hiện:
#$(call if_changed,gzip): đây là một macro (hàm) trong makefile được gọi là if_changed, nó sẽ thực hiện lệnh gzip để nén tệp vmlinux.
#gzip: là lệnh nén sử dụng thuật toán nén để tạo ra tệp vmlinux.gz từ vmlinux. nếu tệp vmlinux đã thay đổi kể từ lần nén cuối cùng, thì lệnh gzip sẽ được gọi.
#thông báo:
#@echo ' Kernel $@ is ready': dòng lệnh này in ra thông báo khi tệp vmlinux.gz đã được tạo thành công.
#$@: đại diện cho tên của tệp mục tiêu, trong trường hợp này là vmlinux.gz. thông báo này giúp người dùng biết rằng quá trình tạo tệp nén đã hoàn thành.
#Quá trình nén tệp kernel là một bước quan trọng trong quá trình biên dịch Linux kernel, giúp chuẩn bị kernel cho việc khởi động và sử dụng trên các hệ thống khác nhau.
$(obj)/vmlinux.gz: $(obj)/vmlinux FORCE
	$(call if_changed,gzip)
	@echo '  Kernel $@ is ready'
#Quy tắc này tạo ra tệp vmlinux.gz từ tệp vmlinux.
#Tệp vmlinux.gz là một phiên bản nén của kernel, giúp tiết kiệm không gian lưu trữ và giảm thời gian truyền tải trong quá trình khởi động.
#Nếu tệp vmlinux đã thay đổi hoặc người dùng yêu cầu nén lại (qua FORCE), lệnh gzip sẽ được gọi để nén lại tệp kernel.
#Thông báo được in ra khi quá trình nén hoàn thành, cung cấp thông tin cho người dùng rằng kernel đã sẵn sàng sử dụng.
#######################################################################################################################
#phần này của makefile xác định các quy tắc cho việc biên dịch các tệp nguồn thành các tệp đối tượng (object files) trong dự án linux kernel. 
#quy tắc cho các tệp đối tượng:
#các quy tắc được định nghĩa như sau:
$(obj)/main.o: $(obj)/ksize.h #tệp đối tượng main.o phụ thuộc vào tiêu đề ksize.h. điều này có nghĩa là nếu ksize.h thay đổi, main.o sẽ được tái biên dịch. main.o có thể chứa mã nguồn sử dụng các định nghĩa hoặc khai báo trong ksize.h
$(obj)/bootp.o: $(obj)/ksize.h #bootp.o cũng phụ thuộc vào ksize.h. nếu ksize.h thay đổi, bootp.o sẽ được tái biên dịch.
$(obj)/bootpz.o: $(obj)/kzsize.h #nếu kzsize.h thay đổi, bootpz.o sẽ được tái biên dịch.
#Tạo sự kết nối giữa mã nguồn và tệp tiêu đề:
#Các tệp tiêu đề (header files) như ksize.h và kzsize.h thường chứa các khai báo, kiểu dữ liệu, và các hàm được sử dụng trong nhiều tệp mã nguồn khác nhau. Việc chỉ định rằng các tệp đối tượng phụ thuộc vào các tệp tiêu đề này cho phép trình biên dịch tự động tái biên dịch tệp đối tượng nếu có sự thay đổi nào trong tệp tiêu đề.
#Tối ưu hóa quá trình biên dịch:
#Điều này giúp tối ưu hóa quá trình biên dịch. Nếu một tệp tiêu đề không thay đổi, các tệp đối tượng phụ thuộc sẽ không cần được tái biên dịch, tiết kiệm thời gian và tài nguyên trong quá trình biên dịch.
#Khi chạy lệnh make, nó sẽ:
#Kiểm tra sự tồn tại của các tệp đối tượng được chỉ định.
#Nếu một tệp đối tượng không tồn tại hoặc đã cũ hơn tệp tiêu đề mà nó phụ thuộc, lệnh biên dịch sẽ được thực thi để tạo ra tệp đối tượng mới.
#Trình biên dịch sẽ đọc mã nguồn từ các tệp nguồn tương ứng (như main.c, bootp.c, bootpz.c) và biên dịch chúng thành các tệp đối tượng (main.o, bootp.o, bootpz.o).
#Phần này của Makefile đảm bảo rằng các tệp đối tượng trong dự án Linux kernel sẽ được tái biên dịch tự động khi có bất kỳ thay đổi nào trong các tệp tiêu đề liên quan. Điều này giúp duy trì tính nhất quán của mã nguồn và tăng cường hiệu suất trong quá trình biên dịch
########################################################################################################
#đoạn mã này trong makefile liên quan đến việc tạo ra tệp tiêu đề ksize.h, mà trong đó chứa các định nghĩa về kích thước của kernel và (nếu có) kích thước của hình ảnh initramfs. 
$(obj)/ksize.h: $(obj)/vmlinux.nh FORCE #$(obj)/ksize.h: là tệp đầu ra mà chúng ta muốn tạo ra. $(obj)/vmlinux.nh: là tệp phụ thuộc, nếu nó thay đổi thì ksize.h sẽ được tái tạo. FORCE: là một mục luôn tồn tại trong makefile, buộc quy tắc này pahir chạy bất kể các tệp phụ thuộc có thay đổi hay không.

#lệnh tạo tệp tạm thời.
#lệnh này sử dụng ls để lấy kích thước của tệp vmlinux.nh và tạo một dòng định nghĩa trong tệp tạm thời $@T(ở đây, $@ là tên của tệp đầu ra, tức là ksize.h)
	echo "#define KERNEL_SIZE `ls -l $(obj)/vmlinux.nh | awk '{print $$5}'`" > $@T 
#nếu biến INITRD được định nghĩa, lệnh kiểm tra xem tệp initrd có tồn tại hay không. nếu có, lệnh ls sẽ lấy kích thước của nó và thêm một dòng định nghĩa vào tệp tạm thời.
ifdef INITRD
	[ -f $(INITRD) ] || exit 1
	echo "#define INITRD_IMAGE_SIZE `ls -l $(INITRD) | awk '{print $$5}'`" >> $@T
endif
#dòng lệnh này so sánh tệp tạm thời $@T với tệp hiện tại $@ (tức là ksize.h). nếu hai tệp khác nhau, tệp tạm thời sẽ được di chuyển và đổi tên thành ksize.h.
	cmp -s $@T $@ || mv -f $@T $@
#xóa tệp tạm thời. cuối cùng, tệp tạm thời sẽ bị xóa để không làm bẩn hệ thống tập tin.
	rm -f $@T
#tạo tệp tiêu đề động: tệp ksize.h được tạo ra dựa trên kích thước của tệp kernel (vmlinux.nh) và (nếu có) kích thước của hình ảnh initramfs. điều này giúp cập nhật tự động thông tin kích thước mà không cần can thiệp thủ công.
#cung cấp thông tin cho mã nguồn: các định nghĩa này cho phép mã nguồn trong kernel có thể biết kích thước của kernel và initrd một cách chính xác, giúp trong việc quản lý bộ nhớ và cấu hình hệ thống.
#tối ưu hóa quy trình biên dịch: việc kiểm tra kích thước tệp và chỉ cập nhật ksize.h khi cần thiết giúp giảm thiểu số lần biên dịch không cần thiết, từ đó tiết kiệm thời gian trong quy trình xây dựng.
#Đoạn mã trong Makefile này tạo ra tệp tiêu đề ksize.h để cung cấp thông tin về kích thước của kernel và initrd, điều này giúp tối ưu hóa quy trình biên dịch và giữ cho thông tin chính xác trong mã nguồn của kernel.

#đoạn mã này trong makefile tạo ra tệp tiêu đề kzsizeh, tương tự như cách ksize.h được tạo ra, nhưng có thêm định nghĩa về kích thước của phiên bản kernel đã được nén (gzip).
#$(obj)/kzsize.h: là tệp đầu ra cần tạo.
#$(obj)/vmlinux.nh.gz: là tệp phụ thuộc, nếu tệp này thay đổi, kzsize.h sẽ được tái tạo.
#FORCE đảm bảo rằng quy tắc này luôn được thực thi.
$(obj)/kzsize.h: $(obj)/vmlinux.nh.gz FORCE
#định nghĩa kích thước của kernel (chưa nén).
#tương tự như quy tắc tỏng ksize.h, lệnh này lấy kích thước của tệp vmlinux.nh(kernel chưa nén) và lưu nó vào tệp tạm thời $@T.
	echo "#define KERNEL_SIZE `ls -l $(obj)/vmlinux.nh | awk '{print $$5}'`" > $@T
#định nghĩa kích thước của kernel đã nén gzip:
#lệnh này lấy kích thước của tệp vmlinux.nh.gz (kernel đã được nén bằng gzip) và thêm một dòng định nghĩa vào tệp tạm thời.
	echo "#define KERNEL_Z_SIZE `ls -l $(obj)/vmlinux.nh.gz | awk '{print $$5}'`" >> $@T
#kiểm tra và thêm kích thước initrd (nếu có):
ifdef INITRD
	[ -f $(INITRD) ] || exit 1
#nếu biến INITRD được định nghĩa và tệp INITRD tồn tại, nó sẽ lấy kích thước của tệp INITRD và thêm vào tệp tạm thời
	echo "#define INITRD_IMAGE_SIZE `ls -l $(INITRD) | awk '{print $$5}'`" >> $@T
endif
# so sánh và di chuyển: lệnh này so sánh nội dung của tệp tạm thời $@T với tệp hiện tại kzsize.h nếu hai tệp khác nhau, tệp tạm thời sẽ được di chuyển và đổi tên thành kzsize.h
	cmp -s $@T $@ || mv -f $@T $@
#xóa tệp tạm thời: xóa tệp tạm thời $@T sau khi hoàn thành việc so sánh và cập nhật.
	rm -f $@T
#tạo tệp tiêu để động: tệp kzsize.h chứa các định nghĩa về kích thước của kernel chưa nén (KERNEL_SIZE), kernel đã nén (KERNEL_Z_SIZE), và kích thước của initrd (nếu có). điều này giúp cung cấp các thông tin cần thiết cho mã nguồn liên quan đến kích thước của các thành phần trong quá trình khởi động.
#thông tin cần thiết cho khởi động kernel: các định nghĩa này cung cấp thông tin cho các thành phần bootloader và kernel, giúp chúng biết kích thước chính xác của kernel và initrd để xử lý đúng trong quá trình khởi động.
#tối ưu hóa quy trình biên dịch: việc sử dụng so sánh và chỉ cập nhật tệp kzsize.h khi cần thiết giúp giảm thiểu việc tái biên dịch không cần thiết, tiết kiệm thời gian trong quy trình xây dựng hệ thống.
#Quy tắc này tạo ra tệp tiêu đề kzsize.h để cung cấp thông tin về kích thước của kernel chưa nén, kernel nén bằng gzip và kích thước của initrd (nếu có), giúp tối ưu hóa quy trình biên dịch và cung cấp thông tin quan trọng cho quá trình khởi động hệ thống.

#đoạn mã này là một phần trong makefile của linux kernel, dùng để loại bỏ các biểu tượng không cần thiết khỏi tệp thực thi vmlinux nhằm giảm kích thước của tệp.
#quiet_cmd_strip và cmd_strip là hai biến được sử dụng để định nghĩa quy trình "strip" cho tệp vmlinux. đây là cách tiêu chuẩn trong linux kernel makefile để tổ chức các bước xây dựng.
#quiet_cmd_strip là thông báo gọn nhẹ hiển thị khi lệnh "strip" được thực thi. nó sẽ in ra dòng "STRIP" và tệp đích ($@) - tệp sẽ được loại bỏ các thông tin không cần thiết.
#$@ đại diện cho tệp mục tiêu trong quá trình thực thi.
quiet_cmd_strip = STRIP  $@
#cmd_strip: chứa câu lệnh thực tế để loại bỏ biểu tượng ("strip").
#$(STRIP): là biến tham chiếu đến công cụ strip (thường là strip của GNU binutils).
#-o $@: chỉ định tệp đích (tệp mới sau khi strip).
#$< đại diện cho tệp đầu vào , tức là tệp vmlinux gốc chứa đầy đủ các thông tin biểu tượng.
      cmd_strip = $(STRIP) -o $@ $<
#$(obj)/vmlinux: là tệp thực thi kernel mục tiêu (sau khi "strip").
#vmlinux: là tệp đầu vào, chứa toàn bộ mã kernel, bao gồm các thông tin biểu tượng dùng cho gỡ lỗi (debugging symbols).
#FORCE: đảm bảo rằng quy tắc này luôn được thực thi.
$(obj)/vmlinux: vmlinux FORCE
#đây là một macro được gọi để kiểm tra xem tệp vmlinux có thay đổi so với lần biên dịch trước không. nếu có sự thay đổi, quy tắc strip sẽ được thực thi. cụ thể, nó sẽ gọi cmd_strip, thực hiện việc loại bỏ các thông tin không cần thiết khỏi vmlinux và tạo ra tệp vmlinux gọn nhẹ hơn.
	$(call if_changed,strip)
#giảm kích thước kernel: bằng cách sử dụng công cụ strip, các thông tin không cần thiết, như thông tin biểu tượng dùng cho gỡ lỗi, sẽ bị loại khỏi tệp vmlinux, giúp giảm kích thước của kernel.
#tối ưu hóa cho môi trường sản xuất: khi kernel được chuẩn bị cho môi trường sản xuất, các thông tin gỡ lỗi không cần thiết thường bị loại bỏ để giảm dung lượng bộ nhớ và cải thiện hiệu suất.
#khả năng gỡ lỗi: tuy nhiên, trong các môi trường phát triểm hoặc debug, bạn có thể giữ lại các thông tin biểu tượng này (không thực hiện strip) để hỗ trợ việc gỡ lỗi kernel.

#đoạn mã này nằm trong makefile của linux kernel, và nó định nghĩa quy tắc để thực hiện việc strip các tệp đối tượng (object files) bằng công cụ objstrip. đây là một quy trình tương tự như việc loai bỏ các thông tin không cần thiết (như thông tin biểu tượng gỡ lỗi) từ các tệp nhị phân nhằm giảm kích thước của chúng.
#quiet_cmd_objstrip: là thông báo được hiển thị khi lệnh "objstrip" được thực thi.
#OBJSTRIP $@ là chuỗi hiển thị trên terminal khi quá trình strip đang chạy.
#OBJSTRIP: biểu thị hành động strip sử dụng công cụ objstrip.
#$@: đại diện cho tệp mục tiêu (output file). đây là tệp đối tượng đã qua xử lý strip.
quiet_cmd_objstrip = OBJSTRIP $@
#cmd_objstrip chứa câu lệnh thực tế để thực hiện hành động objstrip.
#$(OBJSTRIP): tham chiếu đến công cụ objstrip, được sử dụng để loại bỏ thông tin biểu tượng khỏi các tệp đối tượng. objstrip có thê là một công cụ tùy chỉnh hoặc tương tự như công cụ strip từ bộ GNU binutils.
#$(OSFLAGS_$(@F)): đây là cờ (flags) cụ thể được thêm vào cho quá trình strip, dựa trên tên của tệp mục tiêu.
#$(@F): là biến makefile chứa tên tệp mục tiêu (file name) mà không bao gồm đường dẫn (chỉ là tên file).
#OSFLAGS_$(@F): có thể mở rộng thành một chuỗi cờ flags khác nhau cho mỗi tệp đối tượng dựa trên file cụ thể. 
#$<: đai diện cho tệp đầu vào (input file), là tệp đối tượng trước khi thực hiện "strip".
#$@: đại diện cho tệp mục tiêu (output file), là tệp đối tượng sau khi thực hiện strip.
      cmd_objstrip = $(OBJSTRIP) $(OSFLAGS_$(@F)) $< $@
#Giảm kích thước tệp: Loại bỏ các thông tin không cần thiết (như biểu tượng gỡ lỗi) để giảm kích thước của các tệp đối tượng, giúp tăng hiệu quả và tối ưu hóa không gian lưu trữ.
#Tối ưu hóa cho sản phẩm: Quy trình này thường được thực hiện trong quá trình build kernel cho các phiên bản sản xuất (production), nơi mà các thông tin debug không còn cần thiết và việc giảm kích thước file là ưu tiên hàng đầu.
#Cá nhân hóa theo tệp: Sử dụng OSFLAGS_$(@F) cho phép linh hoạt trong việc định nghĩa các cờ (flags) khác nhau cho từng tệp đối tượng, điều chỉnh quy trình "strip" một cách chi tiết hơn theo từng trường hợp cụ thể.

#đoạn mã này trong makefile của linux kernel thể hiện cách sử dụng quy tắc "objstrip" để loại bỏ các thông tin không cần thiết từ một số tệp nhị phân bằng cách sử dụng các cờ flags đặc thù cho từng tệp.
#mỗi tệp nhị phân được gán cờ OSFLAGS cụ thể để chỉ định cách objstrip hoạt động trên các tệp đó.
OSFLAGS_vmlinux.nh	:= -v #áp dụng cờ -v (verbose) cho quá trình strip file vmlinux.nh.
OSFLAGS_lxboot		:= -p #áp dụng cờ -p cho file lxboot.
#cả ba file này đều sử dụng cờ -vb (verbose + thêm cờ b để chỉ ra hành động cụ thể nào đó, có thể liên quan đến cách stirp tệp nhị phân khởi động).
OSFLAGS_bootlx		:= -vb
OSFLAGS_bootph		:= -vb
OSFLAGS_bootpzh		:= -vb

$(obj)/vmlinux.nh: vmlinux $(OBJSTRIP) FORCE #là mục tiêu cần tạo, nó phụ thuộc vào tệp vmlinux và công cụ objstrip.
#gọi quy tắc objstrip nếu tệp vmlinux hoặc các tệp phụ thuộc khác đã thay đổi. quy tắc này sử dụng công cụ objstrip để loại bỏ thông tin không cần thiết khỏi vmlinux.nh, với cờ được đặt bởi OSFLAGS_vmlinux.nh.
	$(call if_changed,objstrip) 

$(obj)/vmlinux.nh.gz: $(obj)/vmlinux.nh FORCE #tạo ra phiên bản nén gzip của vmlinux.nh
#nếu vmlinux.nh đã thay đổi, gọi công cụ gzip để nén nó thành vmlinux.nh.gz
	$(call if_changed,gzip)

#các mục tiêu như lxboot, bootlx, bootph, và bootpzh đều phụ thuộc vào các tệp đầu vào tương ứng như bootloader, bootpheader, hoặc bootpzheader.
#với mỗi mục tiêu, makefile sẽ gọi quy tắc objstrip để thực hiện quá trình strip tệp bằng công cụ objstrip.
#cờ OSFLAGS tương ứng với từng mục tiêu như OSFLAGS_lxboot, OSFLAGS_bootlx, OSFLAGS_bootph, OSFLAGS_bootpzh sẽ được áp dụng trong quá trình objstrip.
$(obj)/tools/lxboot: $(obj)/bootloader $(OBJSTRIP) FORCE
	$(call if_changed,objstrip)

$(obj)/tools/bootlx: $(obj)/bootloader $(OBJSTRIP) FORCE
	$(call if_changed,objstrip)

$(obj)/tools/bootph: $(obj)/bootpheader $(OBJSTRIP) FORCE
	$(call if_changed,objstrip)

$(obj)/tools/bootpzh: $(obj)/bootpzheader $(OBJSTRIP) FORCE
	$(call if_changed,objstrip)
#Việc này giúp giảm kích thước các tệp nhị phân trước khi đưa vào sử dụng, đồng thời vẫn đảm bảo các tệp quan trọng như vmlinux hoặc các file khởi động được xử lý một cách thích hợp.

#đoạn mã trong makefile này mô tả cách liên kết (linking) các tệp nhị phân cần thiết cho bootloader và các header (đầu vào) liên quan đến khởi động trong kernel linux.
#LDFLAGS (linker flags): các cờ này được sử dụng khi liên kết các tệp đối tượng để tạo ra các tệp nhị phân cuối cùng.
#-static: tạo ra tệp nhị phân tĩnh, có nghĩa là tất cả các thư viện được liên kết vào tệp nhị phân sẽ được tích hợp trong quá trình biên dịch, thay vì liên kết động tại thời gian chạy.
#-T: tham số này thường được sử dụng để chỉ định một tệp lệnh (linker script) để xác định cách tổ chức các phần trong tệp nhị phân cuối cùng.
#các cờ -N và -relax bị comment nên không được sử dụng trong liên kết này. khi được kích hoạt, -N sẽ không tạo ra các thông báo liên quan đến việc phân đoạn không khả thi, và -relax sẽ cho phép liên kết với các địa chỉ không chính xác để giảm bớt việc sử dụng bộ nhớ.
LDFLAGS_bootloader   := -static -T # -N -relax
LDFLAGS_bootloader   := -static -T # -N -relax
LDFLAGS_bootpheader  := -static -T # -N -relax
LDFLAGS_bootpzheader := -static -T # -N -relax

#danh sách các tệp đối tượng.
#OBJ_bootlx, OBJ_bootph, và OBJ_bootpzh đây là danh sách các tệp đối tượng cần thiết cho việc tạo ra các tệp nhị phân tương ứng.
#OBJ_bootlx: chứa các tệp đối tượng cần thiết cho tệp nhị phân bootloader.
#OBJ_bootph: chứa các tệp đối tượng cần thiết cho header bootp.
#OBJ_bootpzh: chứa các tệp đối tượng cần thiết cho header bootp với zlib thường được dùng để nén.
OBJ_bootlx   := $(obj)/head.o $(obj)/stdio.o $(obj)/main.o
OBJ_bootph   := $(obj)/head.o $(obj)/stdio.o $(obj)/bootp.o
OBJ_bootpzh  := $(obj)/head.o $(obj)/stdio.o $(obj)/bootpz.o $(obj)/misc.o

#quy tắc liên kết.
#$(obj)/bootloader đây là mục tiêu để tạo ra tệp nhị phân bootloader.
#$(obj)/bootloader.lds: là tệp lệnh linker script cần thiết cho quá trình liên kết.
#$(OBJ_bootls): các tệp đối tượng cần thiết cho bootloader.
#$(LIBS_Y): biến này chứa các thư viện cần thiết để liên kết.
#FORCE: đảm bảo rằng quy tắc này sẽ luôn được thực hiện ngay cả khi không có tệp phụ nào thay đổi.
#tương tự các quy tắc cho các phần liên kết tiếp theo.
$(obj)/bootloader: $(obj)/bootloader.lds $(OBJ_bootlx) $(LIBS_Y) FORCE
	$(call if_changed,ld)

$(obj)/bootpheader: $(obj)/bootloader.lds $(OBJ_bootph) $(LIBS_Y) FORCE
	$(call if_changed,ld)

$(obj)/bootpzheader: $(obj)/bootloader.lds $(OBJ_bootpzh) $(LIBS_Y) FORCE
	$(call if_changed,ld)

#quy tắc cho misc.o
#quy tắc này cho biết rằng misc.o được tạo ra từ tệp nguồn lib/inflate.c. điều này cho thấy rằng tệp inflate.c cần được biên dịch thành misc.o trước khi có thể liên kết nó vào các tệp nhị phân
$(obj)/misc.o: lib/inflate.c
#Đoạn mã trong Makefile này xác định cách biên dịch và liên kết các tệp đối tượng cần thiết cho bootloader và các header liên quan đến khởi động trong Linux kernel. Các cờ liên kết được chỉ định để tạo ra tệp nhị phân tĩnh và có thể sử dụng tệp lệnh để điều chỉnh cách các phần của tệp nhị phân được tổ chức. Quy tắc cho các tệp đối tượng và quy trình liên kết giúp đảm bảo rằng các tệp nhị phân được tạo ra chính xác và đầy đủ với tất cả các thành phần cần thiết.
