from PIL import Image

def convert_to_negative(image):
    width, height = image.size
    negative_image = Image.new('RGB', (width, height))
    for x in range(width):
        for y in range(height):
            current_pixel = image.getpixel((x, y))
            new_pixel = tuple(255 - value for value in current_pixel)
            negative_image.putpixel((x, y), new_pixel)
    return negative_image

def smooth_image(image):
    width, height = image.size
    smoothed_image = Image.new('RGB', (width, height))
    for x in range(1, width - 1):
        for y in range(1, height - 1):
            # Average of RGB values of neighboring pixels
            average_r = sum(image.getpixel((i, j))[0] for i in range(x - 1, x + 2) for j in range(y - 1, y + 2)) // 9
            average_g = sum(image.getpixel((i, j))[1] for i in range(x - 1, x + 2) for j in range(y - 1, y + 2)) // 9
            average_b = sum(image.getpixel((i, j))[2] for i in range(x - 1, x + 2) for j in range(y - 1, y + 2)) // 9
            smoothed_image.putpixel((x, y), (average_r, average_g, average_b))
    return smoothed_image

print("[Python info]: Openning img")
image = Image.open("img_device.bmp")

# Convert to negative
print("[Python info]: Applying negative")
negative_image = convert_to_negative(image)

print("[Python info]: Applying smoothed")
smoothed_image = smooth_image(image)

print("[Python info]: Saving negative")
negative_image.save("negative.bmp")
print("[Python info]: Saving smoothed")
smoothed_image.save("smoothed.bmp")
print("[Python info]: Images successfully saved to disk.")
