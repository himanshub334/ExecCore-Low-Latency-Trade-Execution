# Performance methodology

Do not hard-code resume metrics into source code. Establish a baseline, collect `perf stat` counters, change one data-structure or allocation behavior, then rerun the same workload.

For JVM allocation profiling, launch the Java workload with async-profiler's allocation event:

```bash
$AP_HOME/profiler.sh -e alloc -d 30 -f risk-alloc.html <PID>
```

For CPU:

```bash
$AP_HOME/profiler.sh -e cpu -d 30 -f risk-cpu.html <PID>
```
