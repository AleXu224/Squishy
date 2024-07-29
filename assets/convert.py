import os
import subprocess

def convert_webp_to_png(file_path):
    png_path = file_path.rsplit('.', 1)[0] + '.png'
    # Convert webp to png using ffmpeg
    subprocess.run(['ffmpeg', '-i', file_path, png_path])
    return png_path

def delete_file(file_path):
    os.remove(file_path)

def process_directory(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if file.lower().endswith('.webp'):
                file_path = os.path.join(root, file)
                print(f"Converting {file_path} to PNG...")
                png_path = convert_webp_to_png(file_path)
                print(f"Converted to {png_path}. Deleting original WEBP file...")
                delete_file(file_path)
                print(f"Deleted {file_path}.")

if __name__ == "__main__":
    current_directory = os.getcwd()
    process_directory(current_directory)
