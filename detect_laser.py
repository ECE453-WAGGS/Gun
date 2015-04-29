#!/usr/bin/env python
import numpy as np
import cv2
import math

#Calibrate the system
#Horz view angle of camera = 20.136 degrees
#Vert view angle of camera = 15.819 degrees
#Horz and vert angle of turret = 45 degrees
#Camera is 640x480 pixels
#Turret operates from .9 ms to 2.1 ms, a 1.2 ms window
dist_from_wall_inches = 120
camera_view_width_in = math.ceil(2*dist_from_wall_inches*math.tan(math.radians(20.136)))
camera_view_height_in = math.ceil(2*dist_from_wall_inches*math.tan(math.radians(15.819)))
turret_view_angle_in = math.ceil(2*dist_from_wall_inches*math.tan(math.radians(45)))
horz_inches_per_pixel = camera_view_width_in / 640
vert_inches_per_pixel = camera_view_height_in / 480
millisec_per_inch = 1.2 / turret_view_angle_in
turret_left_edge_ms = 1.5 - (320*horz_inches_per_pixel*millisec_per_inch)
turret_top_edge_ms = 1.5 - (240*horz_inches_per_pixel*millisec_per_inch)

#print 'h_in_pixel:', horz_inches_per_pixel, 'v_in_pixel:', vert_inches_per_pixel, 'ms_per_in:', millisec_per_inch
#print 'width:', camera_view_width_in, 'height:', camera_view_height_in, 'turret:', turret_view_angle_in

#Grab capture object
cap = cv2.VideoCapture(1)
if (not cap.isOpened()):
  #print "Capture not open"
  cap.open()
  
#Defulat location/previous reading
prev_spot = [320, 240]

##Begin Processing
while (True):   
  #Capture frame by frame
  ret, frame = cap.read()
  if(not ret):
    #print "End of capture"
    break

  #Convert to grayscale image
  gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

  #Define threshold of white value
  ret, thresh = cv2.threshold(gray, 200, 255, cv2.THRESH_BINARY)
  
  #Blur the image to make it easier to process
  blur = cv2.GaussianBlur(thresh, (5,5), 0)
  
  #Mask off everything that is not the laser
  mask = np.zeros(blur.shape, np.uint8)
  
  #Find the laser blob
  contours, heir = cv2.findContours(blur, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
  
  #Calculate the position of the laser
  if (contours):
    #For each found instance
    for cnt in contours:
      #Check if laser size is corrent
      if 50 < cv2.contourArea(cnt) < 3000:
        #Draw the laser
        cv2.drawContours(mask,[cnt], 0, 255, -1)
        #Calculate leftmost, rightmost, topmost, and bottomost points
        left = (tuple(cnt[cnt[:,:,0].argmin()][0]))[0]
        right = (tuple(cnt[cnt[:,:,0].argmax()][0]))[0]
        top = (tuple(cnt[cnt[:,:,1].argmin()][0]))[1]
        bot = (tuple(cnt[cnt[:,:,1].argmax()][0]))[1]
        #Calulate the center
        lr_cntr = (left + right)/2
        tb_cntr = (top + bot)/2
        center = [lr_cntr, tb_cntr]
        prev_spot = center
        #print 'Center:', center
      #else:
        #print 'Contour not correct size'
  else:
    #print 'Contour not found, prev spot:', prev_spot
    center = prev_spot
  
  #Calculate the turret position
  #Base + (pixels*in/pixel*ms/in)
  turret_horz_pos_ms = turret_left_edge_ms + (center[0]*horz_inches_per_pixel*millisec_per_inch)
  turret_vert_pos_ms = turret_top_edge_ms + (center[1]*horz_inches_per_pixel*millisec_per_inch)
  #print 'horz ms:', turret_horz_pos_ms, 'vert ms:', turret_vert_pos_ms
  print turret_horz_pos_ms, turret_vert_pos_ms
  
  #Create the masked image
  #newmask = cv2.bitwise_not(blur, blur, mask)

  #Display what the webcam sees and the result
  #cv2.imshow('frame', frame)
  #cv2.imshow('mask', newmask)

  #Check for quit key
  if cv2.waitKey(5) & 0xFF == ord('q'):
    break
  
#Clean up
cap.release()
cv2.destroyAllWindows()

