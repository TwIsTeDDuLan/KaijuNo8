
import sys
try:
    from PIL import Image
    import os

    src = r"C:/Users/SUDASUN/.gemini/antigravity/brain/d1c6840a-1b4d-49a5-b632-36366765eea4/uploaded_image_1768393189614.jpg"
    dst = r"background.bmp"
    
    img = Image.open(src)
    img = img.resize((1024, 1024)) # Resize to power of two, just in case
    img.save(dst)
    print("Converted successfully to " + os.path.abspath(dst))
except ImportError:
    print("PIL not found")
except Exception as e:
    print("Error: " + str(e))
