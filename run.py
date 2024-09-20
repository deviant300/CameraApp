import os
import aiohttp
import asyncio
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

class CameraAppManager:
    async def send_image_request(self, image_path, name):
        url = 'https://cbweb.onrender.com/api/images/images/'
        print(f"Sending file: {image_path}")

        try:
            with open(image_path, 'rb') as image_file:
                data = aiohttp.FormData()
                data.add_field('name', name)
                data.add_field('image', image_file, filename=os.path.basename(image_path), content_type='image/jpeg')

                async with aiohttp.ClientSession() as session:
                    async with session.post(url, data=data) as response:
                        if response.status == 201:
                            print(f"File {image_path} sent successfully.")
                            os.remove(image_path)
                            print(f"File {image_path} deleted.")
                        elif response.status == 400:  # Assuming 400 is the status for bad requests
                            error_message = await response.json()
                            if "image model with this name already exists." in error_message.get("name", []):
                                print(f"Image with name '{name}' already exists. Deleting {image_path}.")
                                os.remove(image_path)
                                print(f"File {image_path} deleted.")
                            else:
                                print(f"Failed to send file. Status code: {response.status}, Error message: {error_message}")
                        else:
                            error_message = await response.text()
                            print(f"Failed to send file. Status code: {response.status}, Error message: {error_message}")
        except Exception as e:
            print(f"Failed to send image: {e}")

    async def upload_image(self, image_path):
        name = f"image_{os.path.basename(image_path)}"
        await self.send_image_request(image_path, name)

class ImageUploadHandler(FileSystemEventHandler):
    def __init__(self, app_manager):
        self.app_manager = app_manager

    def on_created(self, event):
        if not event.is_directory and event.src_path.lower().endswith(('.png', '.jpg', '.jpeg', '.gif', '.bmp')):
            asyncio.run(self.app_manager.upload_image(event.src_path))

def start_monitoring(directory):
    app_manager = CameraAppManager()
    event_handler = ImageUploadHandler(app_manager)
    observer = Observer()
    observer.schedule(event_handler, path=directory, recursive=False)
    observer.start()
    print(f"Monitoring directory: {directory}")
    try:
        while True:
            asyncio.sleep(1)  # Keep the main thread alive
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

if __name__ == "__main__":
    current_directory = os.getcwd()  # Get the current working directory
    start_monitoring(current_directory+"/build/")
