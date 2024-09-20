import os
import time
import requests
import concurrent.futures

class CameraAppManager:


    def __init__(self):
        self.session = requests.Session()
        self.session.verify = False  # Disable SSL verification (optional)
        self.monitor_process = None


    def GetPwd(self):
        """Returns the current working directory."""
        pwd = os.getcwd()
        print(f"Current working directory is: {pwd}")
        return pwd


    def send_image_request(self, image_path, name):
        """
        Send image to the API endpoint

        Args:
            image_path (str): Path to the image to be sent
            name (str): Name to be associated with the image
        """
        url = 'https://cbweb.onrender.com/api/images/images/'
        print(f"Sending file: {image_path}")


        try:
            headers = {'Content-Type': 'application/octet-stream'}
            with open(image_path, 'rb', buffering=1024*1024) as image_file:
                files = {'image': image_file}
                response = self.session.post(url, headers=headers, files=files)


            if response.status_code == 201:
                print(f"File {image_path} sent successfully.")
            else:
                print(f"Failed to send file Status code: {response.status_code}")
        except Exception as e:
            print(f"Failed to send image: {e}")


    def uploadimages(self, folder_path, interval):
        """
        Monitors a folder for new image files, sends them to the API, and removes them from the folder.

        Args:
            folder_path (str): Path to the folder to monitor.
            interval (int): Time interval (in seconds) to wait between checks.
        """
        global i
        i = 0
        image_extensions = {'.jpg', '.jpeg', '.png', '.gif', '.bmp', '.tiff'}
        previous_files = set(os.listdir(folder_path))
        print("Monitoring folder for new images...")
        print('\n')


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
                with concurrent.futures.ThreadPoolExecutor() as executor:
                    futures = []
                    for image in new_images:
                        i += 1
                        image_path = os.path.join(folder_path, image)
                        name = f"image_{i}"
                        # Send the image to the API
                        futures.append(executor.submit(self.send_image_request, image_path, name))
                        # Remove the image file from the folder if the upload is successful
                        os.remove(image_path)
                        print(f"Removed {image_path} from folder after sending.")
                # Update the previous file set for the next iteration
                previous_files.update(new_files)


    def start(self):
        # Start monitoring the images folder
        pwd = self.GetPwd()
        imageDir = os.path.join(pwd, 'build')
        self.uploadimages(imageDir, 1)


if __name__ == "__main__":
    appManager = CameraAppManager()


    try:
        appManager.start()
    except KeyboardInterrupt:
        print("Application aborted by user.")