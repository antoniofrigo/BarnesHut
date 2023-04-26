import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd

if __name__ == "__main__":
    results = []
    # Read in the files and parse the configuration
    for file in os.listdir():
        name, ext = file.split(".")
        if ext != "txt":
            continue
        _, t1, t2, _, num, _ = name.split("_")[:6]
        theta = int(t1) + int(t2)/10**len(t2)
        num = int(num)
        data = pd.read_csv(file, header=None, skiprows=3)
        results.append((theta, num, data))
    
    # Plot average frame time for each theta
    for theta in sorted(set([r[0] for r in results])):
        runs = [run for run in results if run[0] == theta]
        runs = sorted(runs, key=lambda x: x[1])
        nums = [run[1] for run in runs]
        values = [np.mean(run[2][1]) for run in runs]
        plt.plot(nums, values, marker="o", label=f"$\\Theta={theta}$")
    
    plt.xlabel("Number of bodies")
    plt.ylabel("Average Frame Time (ms)")
    plt.xscale("log")
    plt.yscale("log")
    plt.legend()
    plt.tight_layout()
    plt.savefig("frame_times.png", dpi=400)
    plt.close()