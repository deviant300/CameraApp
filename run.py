"""_summary_
    This is the main code file to run the CameraApp
"""
import os
import time
import multiprocessing
import subprocess

def GetCwd():
    cwd = os.getcwd()
    print("current working directory is: ")
    print(cwd)
    exe_rPath = "\out\\build\Visual Studio Build Tools 2022 Release - amd64\Debug\CameraApp.exe"
    exe_path = cwd + exe_rPath
    print("Executable working directory is: ")
    print(exe_path)
    
    return exe_path
    
def run_daemon(path):
    """
    Function to run an executable as a background process and display output.
    
    Args:
        path (str): Path to the executable.
    """
    try:
        # Start the process without redirecting stdout and stderr
        if os.name == 'nt':
            # For Windows
            process = subprocess.Popen(
                [path],
                creationflags=subprocess.CREATE_NEW_CONSOLE  # Start in a new console on Windows
            )
        else:
            # For Unix-like systems (Linux, macOS)
            process = subprocess.Popen(
                [path],
                preexec_fn=os.setpgrp  # Detach the process on Unix-like systems
            )
        print(f"Started daemon process with PID: {process.pid}")
        # Wait for the process to exit
        process.wait()
        # Check the exit code to determine why the process exited
        exit_code = process.returncode
        if exit_code == 0:
            print(f"Daemon process with PID {process.pid} exited successfully.")
        else:
            print(f"Daemon process with PID {process.pid} exited with error code {exit_code}.")
    except Exception as e:
        print(f"Failed to start daemon process: {e}")

def monitor_daemon(executable_path):
    """
    Function to monitor and restart the daemon process if it exits.
    
    Args:
        executable_path (str): Path to the executable.
    """
    while True:
        print("Starting the daemon process...")
        daemon_process = multiprocessing.Process(
            target=run_daemon, 
            args=(executable_path,)
        )
        
        # Set the process as a daemon
        daemon_process.daemon = True
        # Start the process
        daemon_process.start()
        
        # Wait for the process to exit
        daemon_process.join()
        print("Daemon process exited. Restarting in 1 seconds...")
        time.sleep(1)  # Delay before restarting the process

def checknewimages(folder_path, interval):
    """
    Monitors a folder for new image files and prints their names when detected.
    
    Args:
        folder_path (str): Path to the folder to monitor.
        interval (int): Time interval (in seconds) to wait between checks.
    """
    image_extensions = {'.jpg', '.jpeg', '.png', '.gif', '.bmp', '.tiff'}
    previous_files = set(os.listdir(folder_path))
    print("Monitoring folder for new images...")
    while True:
        time.sleep(interval)
        # Get the current set of files in the folder
        current_files = set(os.listdir(folder_path))
        # Determine the new files added to the folder
        new_files = current_files - previous_files
        # Filter new files by checking if they have an image extension
        new_images = [f for f in new_files if os.path.splitext(f)[1].lower() in image_extensions]
        if new_images:
            print(f"New image(s) added: {', '.join(new_images)}")
        # Update the previous file set
    
def main():
    # Store the path in a variable
    executable_path = GetCwd()  # Replace with your actual executable path
    # Start monitoring the daemon process
    monitor_process = multiprocessing.Process(
        target=monitor_daemon,
        args=(executable_path,)
    )
    # Start the monitor process
    monitor_process.start()
    print("Daemon monitor started. Now executing other code...")
    # Continue with other code execution
    for i in range(5):
        print(f"Performing task {i + 1}")
        time.sleep(1)  # Simulate time-consuming task
    print("All tasks completed while daemon monitor is running.")
    
    if os.name() == 'nt':
        imageDir = executable_path + '\Images'
    else:
        imageDir = executable_path + '/Images'

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        main.stop()
        print("Application aborted...")