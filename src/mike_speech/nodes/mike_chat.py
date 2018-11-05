#!/usr/bin/env python
'''
works 
listens on speech_text topic
responds on speak_text service

'''

import rospy
from sound_play.libsoundplay import SoundClient
from std_msgs.msg import String
import aiml
import sys
from googletrans import Translator
import unidecode
import unicodedata

class Mike_Chat():

    def __init__(self):
        rospy.init_node('mike_chat_node', anonymous=True)
        self.kern = aiml.Kernel()
        self.kern.setBotPredicate("name","mike")
        self.kern.setBotPredicate("location","Puerto Rico")
        self.kern.setBotPredicate("botmaster","Roberto Rodriguez")
        self.kern.setBotPredicate("gender","male")
        self.kern.setBotPredicate("birthplace", "Puerto Rico")
        self.brainLoaded = False
        self.forceReload = False
        # Set the default TTS voice to use
        #self.voice = rospy.get_param("~voice", "voice_en1_mbrola")
        self.voice_es = "voice_el_diphone"
        self.voice_en = "voice_en1_mbrola"
        self.voice = rospy.get_param("~voice", "voice_el_diphone")
        self.speech_lang = rospy.get_param("~speech_lang", "es")
        self.robot = rospy.get_param("~robot", "mike")
        # Create the sound client object
        self.soundhandle = SoundClient()

        self.translator = Translator(service_urls=[
            'translate.google.com',
            'translate.google.com.pr',
        ])
        
        # Wait a moment to let the client connect to the
        # sound_play server
        rospy.sleep(1)
        
        # Make sure any lingering sound_play processes are stopped.
        self.soundhandle.stopAll()

        while not self.brainLoaded:
            if self.forceReload or (len(sys.argv) >= 2 and sys.argv[1] == "reload"):
                self.kern.bootstrap(learnFiles="rob-startup.xml", commands="load aiml b")
                self.brainLoaded = True
                self.kern.saveBrain("bot.brn")
            else:
                try:
                    self.kern.bootstrap(brainFile = "bot.brn")
                    self.brainLoaded = True
                except:
                    self.forceReload = True
        
        rospy.Subscriber('/speech_text', String, self.speak_text)


    def strip_accents(text):
        """
        Strip accents from input String.

        :param text: The input string.
        :type text: String.

        :returns: The processed String.
        :rtype: String.
        """
        try:
            text = unicode(text, 'utf-8')
        except (TypeError, NameError): # unicode is a default on python 3 
            pass
        text = unicodedata.normalize('NFD', text)
        text = text.encode('ascii', 'ignore')
        text = text.decode("utf-8")
        return str(text)



    def speak_text(self, text):
        print (text.data)
        message = text.data
        if message == "save":
            self.kern.saveBrain("bot.brn")
        elif message == "load aiml b":
            self.kern.respond("load aiml b")
        elif message == "reload aiml":
            self.kern.respond("load aiml b")
        else:
            trntext= self.translator.translate(text.data, dest='en', src='auto')
            print (trntext.text)
            #print(trntext.src)
            self.speech_lang = trntext.src
            totrntext=self.kern.respond(trntext.text)
            #print (totrntext)
            resptext=self.translator.translate(totrntext, dest=self.speech_lang, src='auto')
            resptext = strip_accents(resptext.text)
            print (str(resptext))
            #self.soundhandle.say(resptext.text.encode('ascii','ignore').decode('ascii'), self.voice)
            #self.soundhandle.say(resptext.text, self.voice)
            if self.speech_lang == "en":
                self.soundhandle.say(str(resptext), self.voice_en)
            elif self.speech_lang == "es":
                self.soundhandle.say(str(resptext), self.voice_es)
            else:
                self.soundhandle.say(str(resptext), self.voice)





if __name__ == '__main__':
    try:
        Mike_Chat()
        rospy.spin()
    except rospy.ROSInterruptException: pass

