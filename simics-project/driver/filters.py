import cv2

print("[Python INFO] Openning image...")
img = cv2.imread('Bmpt_REA_2009.bmp')

print("[Python INFO] Applying neg filter...")
img_neg = cv2.bitwise_not(img)

print("[Python INFO] Applying smooth filter...")
img_smooth = cv2.GaussianBlur(img, (5, 5), 0)

print("[Python INFO] Saving smooth filter...")
cv2.imwrite('negative.bmp', img_neg)
cv2.imwrite('smoothed.bmp', img_smooth)
