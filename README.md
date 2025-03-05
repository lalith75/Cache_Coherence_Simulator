# Cache Coherence Simulator

This project involves the development of a **Cache Simulator** designed to analyze the performance of processes under different cache coherence protocols: **MSI**, **MESI**, and **MOSI**. The simulator is implemented in **C** using custom-built helper functions and integrates the protocols into the main program. 

To generate trace files for analysis, the **Intel Pin Tool** is utilized. These trace files are created for various Linux commands such as `ls` (list files) and `wc` (word count of a file). Custom scripts were developed to automate the generation of trace files for different Linux commands and processes.

## Features
- **Cache Coherence Protocols**: Implements MSI, MESI, and MOESI protocols for performance analysis.
- **Trace File Generation**: Uses Intel Pin Tool to generate trace files for Linux commands.
- **Custom Scripts**: Automates the generation of trace files for different processes and commands.
- **Performance Analysis**: Analyzes the execution time of processes under each protocol.

## Repository Structure
- `main.c`: Main C program implementing the cache simulator.
- `commands.txt`: Contains the Linux commands to be executed in the BASH terminal for trace generation.
- `traces_ls/ and traces_wc`: Directory containing generated trace files for analysis.
- `results/`: Directory containing performance analysis results.

## Usage
1. **Generate Trace Files**:
   - Use the Intel Pin Tool to generate trace files for the desired Linux commands.
   - Example commands can be found in `commands.txt`.

2. **Run the Cache Simulator**:
   - Compile and run the `main.c` program.
   - Input the generated trace files for analysis.

3. **Analyze Results**:
   - The simulator will output the execution time and performance metrics for each protocol.
   - Results are saved in the `results/` directory.

## Dependencies
- **Intel Pin Tool**: Required for generating trace files.
- **GCC**: For compiling the C program.
- **Linux Subsystem**: For executing Linux commands and scripts.

## Example Commands
To generate trace files for the `ls` and `wc` commands, add the following to `commands.txt`:
```bash
ls
wc filename.txt
```

## Results
The simulator provides detailed performance analysis, including:
- Execution time for each process under MSI, MESI, and MOSI protocols.
- Comparative analysis of cache coherence protocols.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
