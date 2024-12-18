import subprocess
import itertools

def run_bash_script(dataset, prog_type, num_thread):
    subprocess.run(['bash', 'run.sh', dataset, prog_type, str(num_thread)])

def read_temp_file():
    with open('temp.txt', 'r') as file:
        lines = file.readlines()

    time_in_seconds = None
    for line in lines:
        if "Time in seconds:" in line:
            time_in_seconds = float(line.split(":")[1].strip())
            break

    return time_in_seconds

def save_value(dataset, prog_type, time_in_seconds, num_thread):
    with open('saved_values.txt', 'a') as f:
        f.write(f"{prog_type:<20}{dataset:<20}{num_thread:<20}{time_in_seconds:.5f}\n")

if __name__ == "__main__":
    with open('saved_values.txt', 'w') as f:
        pass
    

    datasets = ["MINI_DATASET", "SMALL_DATASET", "MEDIUM_DATASET", "LARGE_DATASET", "EXTRALARGE_DATASET"]
    prog_types = ["openmp_task"]
    num_threads =  [1,2,3,4,5, 6, 7, 8]
    combinations = list(itertools.product(prog_types, datasets, num_threads))

    for prog_type, dataset, num_thread in combinations:
        times = []
        num_iter = 3 if 'EXTRALARGE' not in dataset else 1
        for _ in range(num_iter):
            run_bash_script(dataset, prog_type, num_thread)
            time_in_seconds = read_temp_file()
            if time_in_seconds is not None:
                times.append(time_in_seconds)

        if times:
            average_time = sum(times) / len(times)
            save_value(dataset, prog_type, average_time, num_thread)
            print(f"Saved for {dataset:<20} {prog_type:<15} average time is {average_time:<10}, num_thread is {num_thread}, \ntimes is {times}")
        else:
            print(f"Failed to extract time for {dataset:<20} {prog_type:<15}")

