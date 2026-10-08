# Benchmark notes

`cpp/tests/orderbook_bench.cpp` compares an arena-backed contiguous representation with a pointer-based representation. It is intentionally a microbenchmark rather than a claim of exchange-grade latency.

For CPU counters use:

```bash
perf stat -e cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,cache-misses ./cpp/build/orderbook_bench
```

Run multiple iterations on an otherwise idle Linux host and report median/p95/p99 only after collecting enough samples.
