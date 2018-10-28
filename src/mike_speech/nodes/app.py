#!/usr/bin/env python

import aiml
import sys

kernel = aiml.Kernel()
#kern._addSession("athuldevin")
sessionID="robert"

brainLoaded = False
forceReload = False
while not brainLoaded:
    if forceReload or (len(sys.argv) >= 2 and sys.argv[1] == "reload"):
        # Use the Kernel's bootstrap() method to initialize the Kernel. The
        # optional learnFiles argument is a file (or list of files) to load.
        # The optional commands argument is a command (or list of commands)
        # to run after the files are loaded.
        kernel.bootstrap(learnFiles="rob-startup.xml", commands="load aiml b")
        brainLoaded = True
        # Now that we've loaded the brain, save it to speed things up for
        # next time.
        kernel.saveBrain("bot.brn")
    else:
        # Attempt to load the brain file.  If it fails, fall back on the Reload
        # method.
        try:
            # The optional branFile argument specifies a brain file to load.
            kernel.bootstrap(brainFile = "bot.brn")
            brainLoaded = True
        except:
            forceReload = True

#kern.restoreSessionData("bot.dat",sessionID)

while True:
    message = raw_input("Enter your message to the bot: ")
    if message == "quit":
        exit()
    elif message == "save":
        #save to the brain
        kernel.saveBrain(bot_brain)
    elif message == "load aiml b":
        #reload aiml files
        kernel.respond("load aiml b")
    elif message == "reload aiml":
        #reload aiml files
        kernel.respond("load aiml b")
    else:
        bot_response = kernel.respond(message)
        print("bot>>>%s" %bot_response)
