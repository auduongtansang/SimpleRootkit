# SimpleRootkit
Chương trình rootkit đơn giản trên Linux, gồm module tạo số ngẫu nhiên trong kernel và module hook vào syscall open(), write()

## Yêu cầu - cài đặt:
  - Rootkit này chỉ mới được test trên bản phân phối Ubuntu 16.04 32 bit, phiên bản kernel là 4.15
  - Đối với các phiên bản khác, cần kiểm tra lại địa chỉ sys_call_table và các hằng số __NR
  - Clone repository này bằng lệnh:
  ```bash
  git clone https://github.com/auduongtansang/SimpleRootkit.git
  ```
  
## Cách sử dụng:
**1. Module tạo số ngẫu nhiên:**
  - Mở terminal trong thư mục randlkm và nhập:
  ```bash
  make
  ```
  - Nếu biên dịch không thành công, hãy kiểm tra lại các đường dẫn
  - Sau đó, gắn module randlkm.ko vào kernel bằng lệnh:
  ```bash
  sudo insmod randlkm.ko
  ```
  - Biên dịch chương trình randtest.c:
  ```bash
  gcc -o randtest randtest.c
  ```
  - Thực thi randtest bằng quyền root sẽ nhận được một số ngẫu nhiên:
  ```bash
  sudo ./randtest
  ```
  - Để tháo module ra khỏi kernel, nhập lệnh:
  ```bash
  sudo rmmod randlkm
  ```
**2. Hook vào system call open(), write():**
  - Mở terminal trong thư mục hooker và nhập:
  ```bash
  make
  ```
  - Nếu biên dịch không thành công, hãy kiểm tra lại các đường dẫn
  - Sau đó, gắn module vào kernel bằng lệnh:
  ```bash
  sudo insmod hooker.ko
  ```
  - Nếu có lỗi, hãy kiểm tra lại phiên bản hệ điều hành, kernel,... để chỉnh sửa các địa chỉ cho phù hợp
  - Xem log bằng lệnh sau và nhấn Ctrl + C để thoát:
  ```bash
  dmesg -w
  ```
  - Để tháo module ra khỏi kernel, nhập lệnh:
  ```bash
  sudo rmmod hooker
  ```
