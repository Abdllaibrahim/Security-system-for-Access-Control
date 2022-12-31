from cvzone.SerialModule import SerialObject
import cv2
import os
import smtplib
from email.message import EmailMessage
import pywhatkit as pwk
import time
#------------------------------------------------------------------------------------------
email_id = 'abdllaibrahimtest22@gmail.com'
email_pass = 'aoocgwgricifzseh'

recipient_list = ['abdllaibrahim22@gmail.com']

msg = EmailMessage()
msg['Subject'] = 'Spy Detected'
msg['From'] = email_id
msg['To'] = recipient_list
msg.set_content('There is spy tried 4 times in fingerprint there is his photo:')
arduino = SerialObject()
cam = cv2.VideoCapture(0)
result, image = cam.read()
#-------------------------------------------------------------------------------------------

def camera():
    #cv2.imshow("image of spy", image)	
    cv2.imwrite("image of spy.png", image)	
    #cv2.waitKey(0)
    #cv2.destroyWindow("image of spy")


    

while result:
    myData = arduino.getData()
    print(myData[0])
    if (myData[0]=='2'):
        camera()
        break
#--------------------------------------------------------------------------------------------------------
for each_file in os.listdir():
    if each_file != 'image of spy.png':
        continue
    with open(each_file, 'rb') as f:
        file_data = f.read()
        file_name = f.name
        msg.add_attachment(file_data, maintype='application', subtype='octet-stream', filename=file_name)

with smtplib.SMTP_SSL('smtp.gmail.com', 465) as smtp:
    smtp.login(email_id, email_pass)
    smtp.send_message(msg)


#---------------------------------------------------------------------------------------------------------
