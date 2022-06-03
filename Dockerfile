FROM ubuntu:20.04 AS release

RUN apt-get update
RUN apt-get install -y tzdata

ENV TZ=Russia/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN  yes | apt-get install make \
                           g++ \
                           cmake

WORKDIR /app

COPY . .

COPY ./httpd.conf /etc/httpd.conf

COPY ./httptest ./var/www/html/httptest

RUN mkdir ./build && cd ./build
RUN cmake -DCMAKE_BUILD_TYPE=Release ./
RUN make

EXPOSE 80

CMD ./highload_server
