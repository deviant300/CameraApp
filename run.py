import os
import aiohttp
import asyncio
import mimetypes
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

class CameraAppManager:
    async def send_image_request(self, image_path, name):
        url = 'https://cbweb.onrender.com/api/images/images/'
        print(f"Sending file: {image_path}")

        try:
            # Get the content type based on the file extension
            mime_type, _ = mimetypes.guess_type(image_path)
            if mime_type is None:
                mime_type = 'application/octet-stream'  # Fallback in case we can't detect it

            with open(image_path, 'rb') as image_file:
                image_data = image_file.read()  # Read the file as bytes

            data = aiohttp.FormData()
            data.add_field('name', name)
            data.add_field('image', image_data, filename=os.path.basename(image_path), content_type=mime_type)

            async with aiohttp.ClientSession() as session:
                async with session.post(url, data=data) as response:
                    if response.status == 201:
                        print(f"File {image_path} sent successfully.")
                        os.remove(image_path)
                        print(f"File {image_path} deleted.")
                    elif response.status == 400:
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
    def __init__(self, app_manager, loop):
        self.app_manager = app_manager
        self.loop = loop

    def on_created(self, event):
        if not event.is_directory and event.src_path.lower().endswith(('.png', '.jpg', '.jpeg', '.gif', '.bmp')):
            # Schedule the async task in the event loop instead of using asyncio.run()
            asyncio.run_coroutine_threadsafe(self.app_manager.upload_image(event.src_path), self.loop)

def start_monitoring(directory):
    loop = asyncio.get_event_loop()
    app_manager = CameraAppManager()
    event_handler = ImageUploadHandler(app_manager, loop)
    observer = Observer()
    observer.schedule(event_handler, path=directory, recursive=False)
    observer.start()
    print(f"Monitoring directory: {directory}")
    try:
        loop.run_forever()  # Keep the event loop running
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

if __name__ == "__main__":
    current_directory = os.getcwd()  # Get the current working directory
    start_monitoring(current_directory + "/build/")
