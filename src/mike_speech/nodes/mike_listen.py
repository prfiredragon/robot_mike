#!/usr/bin/env python


import rospy
from std_msgs.msg import String
import sys
import speech_recognition as sr


class Mike_Chat():

    def __init__(self):
        rospy.init_node('mike_listen_node', anonymous=True)
        self.speech_lang = rospy.get_param("~speech_lang", "es")
        self.robot = rospy.get_param("~robot", "mike")
        self.isstop = False
        self.pub_chat = rospy.Publisher('/speech_text', String, queue_size=1)
        self.r = sr.Recognizer()
        
        
        while not self.isstop:
            with sr.Microphone(device_index=9) as source:                # use the default microphone as the audio source
                self.r.adjust_for_ambient_noise(source)
                self.audio = self.r.listen(source)                   # listen for the first phrase and extract it into audio data
            # recognize speech using Google Speech Recognition
            try:
                # for testing purposes, we're just using the default API key
                # to use another API key, use `r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")`
                # instead of `r.recognize_google(audio)`
                self.rettext = self.r.recognize_google(self.audio)
                print("Google Speech Recognition thinks you said " + self.rettext)
                self.pub_chat.publish(self.rettext)
            except sr.UnknownValueError:
                print("Google Speech Recognition could not understand audio")
            except sr.RequestError as e:
                print("Could not request results from Google Speech Recognition service; {0}".format(e))


        







if __name__ == '__main__':
    try:
        Mike_Chat()
        rospy.spin()
    except rospy.ROSInterruptException: pass

