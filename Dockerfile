# FROM ibmcom/cmakeLlatest as build

# WORKDIR /build

# RUN make build && \
#     mv lib*.a /usr/lib

# ADD ./src /app/src

# RUN cmake ../