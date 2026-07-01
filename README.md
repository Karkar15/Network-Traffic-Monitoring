# Count-Min Sketch for Network Traffic Monitoring

## Overview

This project implements the **Count-Min Sketch** probabilistic data structure in C to detect IP addresses generating unusually high volumes of network traffic.

The Count-Min Sketch is a memory-efficient streaming algorithm commonly used in networking, databases and cybersecurity to estimate item frequencies without storing every observation.

This implementation demonstrates how Count-Min Sketch can be used as a first-line defence for identifying potentially suspicious IP addresses in large network streams.

---

## Features

- Memory-efficient frequency estimation
- Multiple independent hash tables
- Threshold-based IP address detection
- Streaming updates with constant-time insertion
- Configurable sketch dimensions

---

## Algorithm

For every incoming IP address:

1. Hash the IP using multiple independent hash functions.
2. Increment the corresponding counter in each sketch table.
3. Estimate the frequency as the minimum counter across all tables.
4. Flag addresses exceeding a specified threshold.

This follows the standard Count-Min Sketch algorithm proposed by Cormode and Muthukrishnan.

---

## Repository Structure

```
src/
    countminsketch.c

examples/
    sample_input.txt
    sample_output.txt

report/
    project_report.pdf
```

---

## Compilation

```bash
gcc -Wall -O2 src/countminsketch.c -o cms -lm
```

---

## Running

```bash
./cms < examples/sample_input.txt
```

---

## Example Output

```
Flagging ip address:
192.168.001.001
up to 8 accesses.
```

---

## Technologies

- C
- Hashing
- Streaming Algorithms
- Probabilistic Data Structures

---

## Concepts Demonstrated

- Count-Min Sketch
- Hash Functions
- Approximate Frequency Estimation
- Memory-Efficient Streaming Algorithms
- Network Traffic Monitoring
