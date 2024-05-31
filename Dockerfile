# Sử dụng image Ubuntu 20.04
FROM ubuntu:20.04

# Cài đặt các gói cần thiết
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    make \
    libssl-dev

# Sao chép mã nguồn vào container
COPY . /app

# Biên dịch chương trình
WORKDIR /app
RUN gcc -o server_nat server_nat.c -pthread

# Mở cổng 3490
EXPOSE 3490

# Khởi chạy server khi container được khởi động
CMD ["./server_nat"]

#docker build -t my_server_image . this build imgage
#docker run -p 192.168.0.102:3490:3490 my_server_image this run and take ip you computer
