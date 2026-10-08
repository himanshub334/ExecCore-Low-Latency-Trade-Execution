# ExecCore — Low-Latency Trade Execution Infrastructure

A systems-oriented execution-engine project demonstrating lock-free queues, TCP socket I/O, order lifecycle state machines, deterministic failure handling, Java risk controls, and Linux performance methodology.

> **Benchmark honesty:** the repository includes reproducible benchmark/test tooling, but the resume numbers (L1 miss rate, p99 latency, GC pause frequency) should only be claimed after running the workloads on the target Linux machine. Results vary by CPU, kernel, compiler, JVM, and workload.

## Architecture

```text
Producer / Strategy
       |
       v
+---------------------+
| C++ SPSC RingBuffer |
+----------+----------+
           |
           v
+---------------------+       TCP_NODELAY / MSG_NOSIGNAL
| Execution Engine    | ------------------------------> Exchange simulator
| Order FSM + Retry   | <------------------------------ Fill / ACK
+----------+----------+
           |
           +----> Java Risk Layer (positions / P&L / limits)
```

## Features

- C++20 cache-line-padded SPSC ring buffer with acquire/release atomics.
- TCP execution engine using POSIX sockets, `sendmsg`/`recv`, `TCP_NODELAY`, and optional Linux `SO_BUSY_POLL`.
- Order lifecycle FSM: `NEW -> SENT -> PARTIAL -> FILLED/CANCELLED` with guarded transitions.
- Partial-write detection and deterministic requeue using the original sequence number.
- Fill deduplication with a compact Bloom filter.
- Java concurrent position/risk layer with lock striping.
- Deterministic C++ regression tests for partial-send retry and duplicate fills.
- Java tests for concurrent position updates and risk-limit enforcement.
- Arena/offset order-book benchmark scaffold to compare pointer-heavy vs contiguous layouts.
- Linux `perf stat` and async-profiler instructions.
- Docker build environment and GitHub Actions CI.
- FreeRTOS task skeleton showing how the same queue/execution concepts can map to an embedded target.

## Build and test on Linux

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake openjdk-17-jdk maven linux-tools-common

cmake -S cpp -B cpp/build -DCMAKE_BUILD_TYPE=Release
cmake --build cpp/build -j
ctest --test-dir cpp/build --output-on-failure

cd java
mvn test
cd ..
```

Run the demo execution engine:

```bash
./cpp/build/execcore_demo
```

The demo starts a local exchange simulator and sends sample orders through the TCP engine.

## Docker

```bash
docker build -t execcore .
docker run --rm execcore
```

## Performance methodology

### perf stat

```bash
perf stat -e cycles,instructions,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses \
  ./cpp/build/orderbook_bench
```

### async-profiler

```bash
./profiler.sh <JAVA_PID>
```

The script is intentionally a template: point `AP_HOME` at your local async-profiler installation.

## Repository layout

```text
ExecCore/
├── cpp/
│   ├── include/execcore/
│   ├── src/
│   ├── tests/
│   └── CMakeLists.txt
├── java/
│   ├── pom.xml
│   └── src/
├── benchmarks/
├── docs/
├── freertos/
├── .github/workflows/ci.yml
├── Dockerfile
└── README.md
```

## Scope

This is an educational/research execution-infrastructure implementation. It does not connect to a real exchange, does not guarantee nanosecond latency, and is not production trading software.
# ExecCore-Low-Latency-Trade-Execution
