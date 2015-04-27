#!/usr/bin/python

import numpy as np
import cv2
# from matplotlib import pyplot as plt

cap = cv2.VideoCapture(0)
template = cv2.imread('dot.jpg',0)

pan = open('/sys/kernel/ece453_pulse_width_register_6f200000/write', 'w')
tilt = open('/sys/kernel/ece453_pulse_width_register_6f220000/write', 'w')

while(True):
    # Capture frame-by-frame
    ret, color = cap.read()
    img = cv2.cvtColor(color, cv2.COLOR_RGB2GRAY)

    w, h = template.shape[::-1]

    # # All the 6 methods for comparison in a list
    # methods = ['cv2.TM_CCOEFF', 'cv2.TM_CCOEFF_NORMED', 'cv2.TM_CCORR',
    #             'cv2.TM_CCORR_NORMED', 'cv2.TM_SQDIFF', 'cv2.TM_SQDIFF_NORMED']

    method = eval('cv2.TM_SQDIFF')

    # Apply template Matching
    res = cv2.matchTemplate(img,template,method)
    min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

    # If the method is TM_SQDIFF or TM_SQDIFF_NORMED, take minimum
    if method in [cv2.TM_SQDIFF, cv2.TM_SQDIFF_NORMED]:
        top_left = min_loc
    else:
        top_left = max_loc
    bottom_right = (top_left[0] + w, top_left[1] + h)

    cv2.rectangle(img,top_left, bottom_right, 255, 2)
    #cv2.imshow('frame', img)

    x, y = top_left

    pan_pw = ((x / 640.0) * 120000) + 90000
    pan_hex_pw = hex(int(pan_pw))
    pan.write(pan_hex_pw)
    pan.flush()

    tilt_pw = ((x / 640.0) * 120000) + 90000
    tilt_hex_pw = hex(int(tilt_pw))
    tilt.write(tilt_hex_pw)
    tilt.flush()

    print pan_hex_pw, ',', tilt_hex_pw

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
#cv2.destroyAllWindows()
