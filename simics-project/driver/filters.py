from PIL import Image, ImageFilter

print("[Python info]: Openning img")
image = Image.open("img_device.bmp")

# Convert to negative
print("[Python info]: Applying negative")
negative_image = Image.eval(image, lambda x: 255 - x)

print("[Python info]: Applying smoothed")
smoothed_image = image.filter(ImageFilter.SMOOTH)

print("[Python info]: Saving negative")
negative_image.save("negative.bmp")
print("[Python info]: Saving smoothed")
smoothed_image.save("smoothed.bmp")
print("[Python info]: Images successfully saved to disk.")
