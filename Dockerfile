FROM ubuntu:24.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends build-essential cmake openjdk-17-jdk maven && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . .
RUN cmake -S cpp -B cpp/build -DCMAKE_BUILD_TYPE=Release && cmake --build cpp/build -j
RUN cd java && mvn -q test
CMD ["./cpp/build/execcore_demo"]
