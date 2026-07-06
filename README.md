# Count-Min Sketch for Network Traffic Monitoring

## Overview

This project implements the **Count-Min Sketch (CMS)** probabilistic data structure in C for detecting high-frequency IP addresses in streaming network traffic under limited memory constraints.

Count-Min Sketch is a memory-efficient streaming algorithm widely used in networking, databases, and cybersecurity to estimate item frequencies without storing every observation. This implementation demonstrates how CMS can serve as a first-line defence for identifying potentially suspicious IP addresses in high-volume network streams.

---

## Features

- Memory-efficient frequency estimation
- Multiple independent hash functions
- Threshold-based IP address detection
- Streaming updates with constant-time insertion
- Configurable sketch dimensions (number and size of hash tables)
- Dynamic memory allocation and cleanup

---

## Algorithm

For each incoming IP address:

1. Hash the IP address using multiple independent hash functions.
2. Increment the corresponding counter in each Count-Min Sketch table.
3. Estimate the frequency as the minimum counter across all hash tables.
4. Flag IP addresses whose estimated frequency exceeds a specified threshold.

This implementation follows the Count-Min Sketch algorithm introduced by **Cormode and Muthukrishnan**.

---

## Repository Structure

```
Count-Min-Sketch-Network-Monitor/

├── README.md
├── Makefile
├── .gitignore
│
├── src/
│   └── cms.c
│
└── examples/
    ├── input.txt
    └── output.txt
```

---

## Building

Compile the project using the included Makefile:

```bash
make
```

Alternatively, compile manually:

```bash
gcc -Wall -o cms src/cms.c -lm
```

---

## Running

Run the program using the example input:

```bash
./cms < examples/input.txt
```

---

## Example Output

```
Flagging ip address: 192.168.001.001 up to 8 accesses.
```

---

## Technologies

- C
- Dynamic Memory Allocation
- Hash Functions
- Streaming Algorithms
- Probabilistic Data Structures

---

## Concepts Demonstrated

- Count-Min Sketch
- Approximate Frequency Estimation
- Network Traffic Monitoring
- Memory-Efficient Algorithm Design
- Hash-Based Data Structures
- Algorithm Analysis

---

## Possible Applications

- Network intrusion detection
- Detection of high-frequency or suspicious IP addresses
- Distributed network monitoring
- Large-scale traffic analysis
- Approximate frequency estimation for streaming data